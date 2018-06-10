// Copyright (C) 2017 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <vm/VM.hpp>
#include <vm/MemoryManager.hpp>
#include <vm/Stack.hpp>
#include <vm/World.hpp>
#include <vm/ConstantsTable.hpp>

#include <objects/Object.hpp>
#include <objects/CompiledMethod.hpp>

#include <primitives/primitives.hpp>

#include <misc/Exceptions.hpp>

namespace jupiter{

    VM::VM(){
        stack.push(MemoryManager<Map>::instance().get()); // to avoid stack underflow and crash
    }

    ExecutionFrame::ExecutionFrame(Stack& stack, Method& method, Object* self)
        : stack(stack), method(method), self(self)
    {
        compiledMethod = method.compiledMethod;
        // closures block have self of the context where was created
        if ( method.self ){
            this->self = method.self;
        }

        // LOG
        // compiledMethod->printBytecode();

        // arguments for the call should be in the stack
        auto arity = compiledMethod->arity;
        auto locals = compiledMethod->locals;
        auto stackSize = stack.size();

        localsBaseIndex = stackSize - arity;
        returnIndex = localsBaseIndex - 1; // where the receiver was

        // resize to acomodate the locals
        stack.resize( stackSize + (locals - arity) ); // locals include arity

    }

    ExecutionFrame::~ExecutionFrame(){
        // return the last expresion in the stack
        // the compiler should take care of not pop this last expresion
        stack.set( returnIndex , stack.pop() );
        stack.resize( localsBaseIndex );
    }


    std::string& ExecutionFrame::getStringConstant(unsigned id){
        // TODO use interned strings
        Object* obj = ConstantsTable::instance().get( id );
        String& str = static_cast<String&>(*obj);
        return str.getValue();
    }

    Object* ExecutionFrame::getLocal( unsigned index ){
        return stack.get( getLocalIndex( index ) );
    }

    unsigned ExecutionFrame::getLocalIndex( unsigned index ){
        return localsBaseIndex + index;
    }

    void ExecutionFrame::pushLocal(unsigned id){
        stack.push( getLocal( id ) );
    }

    void ExecutionFrame::pushSelf(){
        stack.push( self );
    }


    void ExecutionFrame::pushGlobal(unsigned id){
        auto globalSymbol = getStringConstant( id );
        try{

            Object* global = World::instance().globals.at(globalSymbol);
            stack.push( global );

        }catch(std::exception& e){
            throw "RuntimeException: Global object '"+ globalSymbol + "' not found";
        }
    }

    void ExecutionFrame::pushConstant(unsigned id){
        stack.push( ConstantsTable::instance().get( id ) );
    }

    void ExecutionFrame::pushClosure( unsigned id ){
        Method* closure = compiledMethod->closures[id];

        auto compiledClosure = closure->getCompiledMethod();

        Method* newClosure = MemoryManager<Method>::instance().get( compiledClosure );

        newClosure->upvalues.reserve( method.upvalues.size() + compiledClosure->upvalues.size() );
        newClosure->upvalues = method.upvalues; // copy enclosing context upvalues
        newClosure->self = self;

        // initialize enclosing context upvalues
        for (auto& pair : compiledClosure->upvalues ){
            // pair.first is the upvalue index
            // pair.second is the local index
            auto upvalue = getLocal( pair.second );
            newClosure->upvalues[pair.first] = upvalue;

        }
        stack.push( newClosure );

    }

    void ExecutionFrame::pushUpValue( unsigned id ){
        auto upvalue = method.upvalues[ id ];
        stack.push( upvalue );
    }

    void ExecutionFrame::popInto(unsigned id){
        Object* obj = stack.pop();
        stack.set( getLocalIndex( id ), obj );
    }

    void ExecutionFrame::pop(){
        stack.pop();
    }

    void ExecutionFrame::popNIntoArray( unsigned n ){

        unsigned size = stack.size();
        unsigned i = size - n;

        auto start = stack.begin() + i;
        auto end = stack.begin() + size;

        auto array = MemoryManager<Array>::instance().get(start, end);

        stack.resize( size - n );
        stack.push( array );
    }

    void ExecutionFrame::dup(){
        stack.dup();
    }

    void ExecutionFrame::send( uint16_t id, uint8_t receiverRelPos ){
        // the receiver position should be overwrite with the return value

        auto receiverIndex = stack.size() - receiverRelPos;
        Object* receiver = stack.get( receiverIndex );

        auto selector = getStringConstant( id );
        auto nextMethod = receiver->at(selector);

        #ifndef NO_TAIL_CALL
        // tail call optimization
        // since we need to search the receiver for the message
        // we cannot know at compile time
        // if we can perform tail call optimization

        // if last call is the same method we are running we can use tail call  optimization
        if ( compiledMethod->instructions.size() == instructionCounter + 1 && &method == nextMethod){
            // copy arguments to correct positions
            // the arguments had been pushed before the call

            if ( compiledMethod->arity > 0 ){
                auto firstArgumentIndex = 0;
                auto lastArgumentIndex = compiledMethod->arity -1;

                for( int i = lastArgumentIndex; i >= firstArgumentIndex; i-- ){
                    stack.set( getLocalIndex( i ), stack.pop() );
                }

            }

            // replace old receiver with new
            stack.set( returnIndex, stack.pop() );
            stack.resize( localsBaseIndex + compiledMethod->locals );

            self = receiver;
            // start again the method
            instructionCounter = -1;

        }else{

            Evaluator evaluator(receiver, stack);
            nextMethod->eval( evaluator );
        }

        #else

        Evaluator evaluator(receiver, stack);
        nextMethod->eval( evaluator );

        #endif

    }


    void ExecutionFrame::jumpIfFalse( uint16_t id ){
        Object* obj = stack.pop();
        if ( obj == World::instance().getFalse()){
            instructionCounter = id -1; // main loop will increment this rightafter this
        }
    }

    void ExecutionFrame::jumpIfTrue( uint16_t id ){
        Object* obj = stack.pop();
        if ( obj == World::instance().getTrue()){
            instructionCounter = id -1;
        }
    }

    void ExecutionFrame::jump( uint16_t id ){
        instructionCounter = id;
    }

    void ExecutionFrame::dispatch( Instruction instruction ){

        switch( instruction.bytecode ){

        case PUSH_CONSTANT:

            pushConstant( instruction.argument );
            break;

        case PUSH_LOCAL:
            pushLocal( instruction.argument );
            break;

        case PUSH_GLOBAL:

            pushGlobal( instruction.argument );
            break;

        case PUSH_SELF:
            pushSelf();
            break;

        case PUSH_CLOSURE:

            pushClosure( instruction.argument );
            break;

        case PUSH_UPVALUE:
            pushUpValue( instruction.argument );
            break;

        case POP_INTO:

            popInto( instruction.argument );
            break;

        case POP:
            pop();
            break;

        case POP_N_INTO_ARRAY:
            popNIntoArray( instruction.argument );
            break;

        case DUP:

            dup();
            break;

        case SEND:

            send( instruction.argument, instruction.shortArgument );
            break;

        case JUMP_IFFALSE:

            jumpIfFalse( instruction.argument );
            break;

        case JUMP_IFTRUE:

            jumpIfTrue( instruction.argument );
            break;

        case JUMP:

            jump( instruction.argument );
            break;

        default:
            break;
        }

    }

    void ExecutionFrame::execute(){
        std::vector<Instruction>& instructions = compiledMethod->instructions;
        auto size = instructions.size();
        instructionCounter = 0;
        for (; instructionCounter < size; instructionCounter++){
            dispatch( instructions[instructionCounter] );
        }

    }

    void VM::gc(bool full){

        if ( full ){
            for( auto it = stack.begin(); it != stack.end(); ++it){
                (*it)->mark();
            }
        }else{
            for( auto it = stack.begin(); it != stack.end(); ++it){
                if ( ! (*it)->istenured() ){
                    (*it)->mark();
                }
            }
        }

    }

    Object* VM::eval(Object* object){
        return eval(object, nullptr);
    }

    Object* VM::eval(Object* object, Object* self){
        Evaluator evaluator(self, stack);
        try{

            object->eval(evaluator);

        }catch (const char* s) {
            std::cout << s << std::endl;
        }catch (std::string s) {
            std::cout << s << std::endl;
        }catch (std::exception& e) {
            std::cout << e.what() << std::endl;
        }

        return stack.back();
    }

    Object* VM::eval(Method& method, Object* self){
        Evaluator evaluator(self, stack);
        evaluator(method);
        return stack.back();
    }

    void VM::pop(){
        stack.pop();
    }


    Evaluator::Evaluator(Object* receiver, Stack& stack) : receiver(receiver), stack(stack){}

    void Evaluator::operator()(Map& obj){
        stack.back(&obj);
    }

    void Evaluator::operator()(Number& obj){
        stack.back( &obj );
    }

    void Evaluator::operator()(String& obj ){
        stack.back( &obj );
    }

    void Evaluator::operator()(Array& obj ){
        stack.back( &obj );
    }

    void Evaluator::operator()(Method& obj ){

        ExecutionFrame newFrame(stack, obj, receiver );
        newFrame.execute();
    }

    void Evaluator::operator()(NativeMethod& method){

        auto stackSize = stack.size();

        unsigned localsBaseIndex = stackSize - method.arity;
        unsigned returnIndex = localsBaseIndex - 1; // where the receiver was

        Object** args = stack.begin() + localsBaseIndex;

            stack.set( returnIndex, method.fn( receiver, args ) );
        stack.resize( localsBaseIndex );
    }



}
