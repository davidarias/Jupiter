// Copyright (C) 2017 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <vm/Frame.hpp>
#include <vm/VM.hpp>

#include <vm/MemoryManager.hpp>
#include <vm/Stack.hpp>
#include <vm/ConstantsTable.hpp>

#include <objects/Object.hpp>
#include <objects/CompiledMethod.hpp>

#include <primitives/primitives.hpp>

#include <misc/Exceptions.hpp>

namespace jupiter{


    Frame::Frame(VM& vm, Method& method)
        : vm(vm), globals(vm.globals), stack(vm.stack), method(method)
    {
        compiledMethod = method.compiledMethod;
        // closures block have self of the context where was created


        // LOG
        // compiledMethod->printBytecode();

        // arguments for the call should be in the stack
        auto arity = compiledMethod->arity;
        auto locals = compiledMethod->locals;
        auto stackSize = stack.size();

        localsBaseIndex = stackSize - arity;
        returnIndex = localsBaseIndex - 1; // where the receiver was

        if ( method.self ){
            this->self = method.self;
        }else{
            this->self = stack.get(returnIndex); // receiver is in the return index (locals - 1)
        }

        // resize to acomodate the locals
        stack.resize( stackSize + (locals - arity) ); // locals include arity

    }

    Frame::~Frame(){
        // return the last expresion in the stack
        // the compiler should take care of not pop this last expresion
        stack.set( returnIndex , stack.pop() );
        stack.resize( localsBaseIndex );
    }


    std::string& Frame::getStringConstant(unsigned id){
        // TODO use interned strings
        Object* obj = ConstantsTable::instance().get( id );
        String& str = static_cast<String&>(*obj);
        return str.getValue();
    }

    Object* Frame::getLocal( unsigned index ){
        return stack.get( getLocalIndex( index ) );
    }

    unsigned Frame::getLocalIndex( unsigned index ){
        return localsBaseIndex + index;
    }

    void Frame::pushLocal(unsigned id){
        stack.push( getLocal( id ) );
    }

    void Frame::pushSelf(){
        stack.push( self );
    }


    void Frame::pushGlobal(unsigned id){
        auto globalSymbol = getStringConstant( id );
        try{

            Object* global = globals.at(globalSymbol);
            stack.push( global );

        }catch(std::exception& e){
            throw "RuntimeException: Global object '"+ globalSymbol + "' not found";
        }
    }

    void Frame::pushConstant(unsigned id){
        stack.push( ConstantsTable::instance().get( id ) );
    }

    void Frame::pushClosure( unsigned id ){
        Method* closure = compiledMethod->closures[id];

        auto compiledClosure = closure->getCompiledMethod();

        Method* newClosure = make<Method>( compiledClosure );

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

    void Frame::pushUpValue( unsigned id ){
        auto upvalue = method.upvalues[ id ];
        stack.push( upvalue );
    }

    void Frame::popInto(unsigned id){
        Object* obj = stack.pop();
        stack.set( getLocalIndex( id ), obj );
    }

    void Frame::pop(){
        stack.pop();
    }

    void Frame::popNIntoArray( unsigned n ){

        unsigned size = stack.size();
        unsigned i = size - n;

        auto start = stack.begin() + i;
        auto end = stack.begin() + size;

        auto array = make<Array>(start, end);

        stack.resize( size - n );
        stack.push( array );
    }

    void Frame::dup(){
        stack.dup();
    }

    void Frame::send( uint16_t id, uint8_t receiverRelPos ){
        // the receiver position should be overwrite with the return value

        auto receiverIndex = stack.size() - receiverRelPos;
        Object* receiver = stack.get( receiverIndex );

        MethodAt methodAtVisitor(vm, id);
        receiver->accept(methodAtVisitor);
        auto nextMethod = methodAtVisitor.get();

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

            Evaluator evaluator(vm);
            nextMethod->accept( evaluator );
        }

        #else

        Evaluator evaluator(receiver, stack, globals);
        nextMethod->accept( evaluator );

        #endif

    }


    void Frame::jumpIfFalse( uint16_t id ){
        Object* obj = stack.pop();
        if ( obj == vm.cachedFalse() ){
            instructionCounter = id -1; // main loop will increment this right after this
        }
    }

    void Frame::jumpIfTrue( uint16_t id ){
        Object* obj = stack.pop();
        if ( obj == vm.cachedTrue() ){
            instructionCounter = id -1;
        }
    }

    void Frame::jump( uint16_t id ){
        instructionCounter = id;
    }

    void Frame::dispatch( Instruction instruction ){

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

    void Frame::execute(){
        std::vector<Instruction>& instructions = compiledMethod->instructions;
        auto size = instructions.size();
        instructionCounter = 0;
        for (; instructionCounter < size; instructionCounter++){
            dispatch( instructions[instructionCounter] );
        }

    }


}
