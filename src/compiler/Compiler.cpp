// Copyright (C) 2017 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.


#include <objects/Object.hpp>
#include <objects/CompiledMethod.hpp>

#include <vm/VM.hpp>
#include <vm/MemoryManager.hpp>
#include <vm/ConstantsTable.hpp>

#include <primitives/primitives.hpp>

#include <compiler/Compiler.hpp>

#include <compiler/Lexer.hpp>
#include <compiler/Parser.hpp>

#include <misc/Exceptions.hpp>

namespace jupiter{

    Object* Compiler::compile(std::string& source){

        try{

            source = source;

            auto tokens = Lexer( source ).tokenize();

            auto ast = Parser( tokens ).parse();

            Compiler compiler;

            ast->accept( compiler );

            return MemoryManager<Method>::instance().get( compiler.getCompiledMethod() );

        }catch (const char* s) {
            std::cout << "CompilerException: ";
            std::cout << s << std::endl;

        }catch (std::string s) {
            std::cout << "CompilerException: ";
            std::cout << s << std::endl;

        }catch (std::exception& e) {
            std::cout << "CompilerException: ";
            std::cout << e.what() << std::endl;

        }

        return nullptr;
    }

    std::tuple<std::string, Object*> Compiler::compile(std::string& signature, std::string& source){

        auto signatureTokens = Lexer( signature ).tokenize();
        auto signatureAst = Parser( signatureTokens ).parseMethodSignature();

        auto tokens = Lexer( source ).tokenize();
        auto ast = Parser( tokens ).parse();

        std::string& name = signatureAst->selector;

        PragmaCompiler pragmaCompiler;
        ast->accept(pragmaCompiler);
        Object* primitive = pragmaCompiler.getPrimitive();

        if (primitive) return std::make_tuple(name, primitive);

        Compiler compiler( signatureAst );

        ast->accept( compiler );

        auto method = MemoryManager<Method>::instance().permanent( name, signature, source,
                                                                   compiler.getCompiledMethod() );

        return std::make_tuple(name, method);
    }

    Compiler::Compiler() : enclosingMethodCompiler(nullptr) {
        method = std::make_shared<CompiledMethod>();
        upvalues = std::make_shared<SymbolTable>();
    }

    Compiler::Compiler(std::shared_ptr<MethodSignature> signature)
        : enclosingMethodCompiler(nullptr)
    {
        method = std::make_shared<CompiledMethod>();
        addArgumentsToLocals( signature->arguments );
        upvalues = std::make_shared<SymbolTable>();
    }

    Compiler::Compiler(std::vector<std::shared_ptr<SymbolNode> >& arguments,
                       Compiler* enclosingMethodCompiler)
        : upvalues(enclosingMethodCompiler->upvalues),
          enclosingMethodCompiler(enclosingMethodCompiler)
    {
        method = std::make_shared<CompiledMethod>();
        addArgumentsToLocals( arguments );
    }

    void Compiler::addArgumentsToLocals(std::vector<std::shared_ptr<SymbolNode> >& arguments){

        method->setArity( arguments.size() );

        for ( auto symbol : arguments ){
            locals.getOrCreate( symbol->value );
        }

    }

    void Compiler::checkArgumentsLimit( unsigned n ){
        // because the second argument in SEND instruction is a uint8_t
        if ( n > 254 )
            throw "Messages are limited to 254 arguments. If you have reached this limit, you're probably doing something crazy.";
    }

    void Compiler::checkConstantsLimit( unsigned n ){

        // because first argument in send method is a uint16_t
        if ( n >= 65536 - 1 )
            throw "The number of Constants is limited to 65536 per method. If you have reached this limit, you're probably doing something crazy.";
    }

    void Compiler::checkLocalsLimit( unsigned n ){

        // because first argument in send method is a uint16_t
        if ( n >= 65536 - 1 )
            throw "The number of locals is limited to 65536 per method. If you have reached this limit, you're probably doing something crazy.";
    }

    void Compiler::checkLiteralArrayLimit( unsigned n ){

        // because first argument in send method is a uint16_t
        if ( n >= 65536 - 1 )
            throw "The number of elements in a literal array is limited to 65536. If you have reached this limit, you're probably doing something crazy.";

    }

    void Compiler::checkLiteralObjectLimit( unsigned n ){

        // because first argument in send method is a uint16_t
        if ( n >= 32768 - 1 )
            throw "The number of elements in a literal object is limited to 32768. If you have reached this limit, you're probably doing something crazy.";

    }


    int Compiler::createUpValuesRecursive( std::string& value ){
        if ( enclosingMethodCompiler == nullptr ) return -1;

        int upvalueIndex = -1;

        int localIndex = enclosingMethodCompiler->locals.find( value );

        if ( localIndex >= 0){

            upvalueIndex = upvalues->getOrCreate( value );

            // upon closure creation, this local references should be converted into
            // real upvalues  if there are nested closures, the existing upvalues
            // are copied into the new closure

            // the pair is only added if is not already in the method
            // this way we can safely call this method as many times as necesary
            method->addUpValue( upvalueIndex, localIndex );

        }else{
            // check others outher scopes
            // upvalues are shared in nested closures
            upvalueIndex = enclosingMethodCompiler->createUpValuesRecursive( value );
        }

        return upvalueIndex;
    }

    std::shared_ptr<CompiledMethod> Compiler::getCompiledMethod(){
        method->setLocals( locals.size() );
        return method;
    }

    void Compiler::visit( NumberNode& node ){
        unsigned index = ConstantsTable::instance().number( node.value );
        checkConstantsLimit(index);
        method->addInstruction( PUSH_CONSTANT, index );
    }

    void Compiler::visit( StringNode& node ){
        unsigned index = ConstantsTable::instance().string( node.value );
        checkConstantsLimit( index );
        method->addInstruction( PUSH_CONSTANT, index );
    }

    void Compiler::visit( SymbolNode& node ){

        if ( node.value == "self"){
            method->addInstruction( PUSH_SELF );
            return;
        }

        int index = locals.find( node.value );

        if ( index >= 0 ){
            method->addInstruction( PUSH_LOCAL, index );
            return;
        }

        // get or create upvalue  (if any in upper contexts )
        int upvalueIndex = createUpValuesRecursive( node.value );

        if ( upvalueIndex >= 0){
            method->addInstruction( PUSH_UPVALUE, upvalueIndex );
            return;
        }

        auto globalConstIndex = ConstantsTable::instance().string( node.value );
        method->addInstruction( PUSH_GLOBAL, globalConstIndex );

    }

    void Compiler::visit( ArrayNode& node ){

        checkLiteralArrayLimit( node.values.size() );

        for( auto node : node.values ){
            node->accept(*this);
        }

        method->addInstruction( POP_N_INTO_ARRAY, node.values.size() );
    }

    void Compiler::visit( ObjectLiteralNode& node ){

        checkLiteralObjectLimit( node.keys.size() );

        for ( unsigned i = 0; i < node.keys.size(); i++ ){

            unsigned index = ConstantsTable::instance().string( node.keys.at( i ) );
            checkConstantsLimit( index );
            method->addInstruction( PUSH_CONSTANT, index );

            node.values.at( i )->accept(*this);
        }

        method->addInstruction( POP_N_INTO_OBJECT, node.keys.size() * 2 );
     }

    void Compiler::visit( CodeBlockNode& node ){

        for (auto it = node.nodes.begin(); it != node.nodes.end(); it++){
            // to left the last evaluted expresion on the stack for the returning
            if ( std::next( it ) == node.nodes.end() ){
                isLastNode = true;
            }
            (*it)->accept(*this);
        }

    }

    void Compiler::visit( AssignmentNode& node ){

        // first compile right side, to allow create a local
        // with the same name of an upvalue and use that upvalue:
        //
        // eg. upvalue := upvalue + 1
        // this code will create a local named 'upvalue'
        // using the upvalue from enclosing context
        //
        // if we compile right side after the creation of the local
        // it will try to get the unasigned newly created local
        // and crash

        node.value->accept(*this);

        auto index = locals.createIfNotExists( node.symbol->value );

        if ( index >= 0){
            checkLocalsLimit( index );

            method->addInstruction( POP_INTO, index );

        }else{

            throw CompilerError("Cannot reassign a variable value.");
        }


    }

    void Compiler::visit( MessageExpressionNode& node ){
        node.message->accept(*this);
        // if is the last node then the last valued must be returned
        if (! isLastNode ){
            method->addInstruction( POP );
        }

    }

    void Compiler::visit( MessageNode& node ){

        checkArgumentsLimit( node.arguments.size() );

        node.receiver->accept(*this);

        for (auto node : node.arguments){
            node->accept(*this);
        }

        auto index = ConstantsTable::instance().string( node.selector );
        checkConstantsLimit( index );

        // receiver is just before the arguments
        method->addInstruction( SEND, index, node.arguments.size() + 1 );

    }

    void Compiler::visit(CascadeMessageNode& node){

        auto receiver = node.messages.front()->receiver;

        receiver->accept(*this);
        method->addInstruction( DUP );

        for(auto it = node.messages.begin(); it != node.messages.end(); it++  ){

            auto message = *it;

            checkArgumentsLimit( message->arguments.size() );

            for (auto argument : message->arguments){
                argument->accept(*this);
            }

            auto index = ConstantsTable::instance().string( message->selector );
            checkConstantsLimit( index );

            method->addInstruction( SEND, index, message->arguments.size() + 1 );

            if(std::next(it) != node.messages.end()){
                // if not last message discard discard return value and
                // dup the receiver pushed at the beguining
                method->addInstruction( POP ); // discard return value
                method->addInstruction( DUP ); // DUP receiver for the next call

            }

        }

    }

    void Compiler::visit( PragmaNode& node ){

    }

    void Compiler::visit( ClosureBlockNode& node ){
        if ( enclosingMethodCompiler == nullptr ){ // top level compiler
            // if a new closure in the top level is created, previous upvalues must be cleared
            // so we can include in the compiled method the corresponding local indexes
            upvalues->reset();
        }
        Compiler closureCompiler(node.arguments, this);

        node.code->accept( closureCompiler );
        auto compiledMethod = closureCompiler.getCompiledMethod();

        Method* methodObject = MemoryManager<Method>::instance().permanent( compiledMethod );
        auto index = method->addClosure( methodObject );

        method->addInstruction( PUSH_CLOSURE, index );
    }

    PragmaCompiler::PragmaCompiler() : primitive(nullptr) {}

    Object* PragmaCompiler::getPrimitive(){
        return primitive;
    }

    void PragmaCompiler::visit( SymbolNode& node ){
        //  get the primitive by name
        // TODO capture std::out_of_range exception
        primitive = Primitives::instance().get( node.value );
    }

    void PragmaCompiler::visit( CodeBlockNode& node ){

        for (auto it = node.nodes.begin(); it != node.nodes.end(); it++){
            auto node = *it;
            node->accept(*this);
        }
    }

    void PragmaCompiler::visit( PragmaNode& node ){
        // get first argument
        // TODO check in empty
        node.arguments[0]->accept(*this);
        // TODO check selector


    }

}
