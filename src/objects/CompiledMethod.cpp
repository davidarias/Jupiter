// Copyright (C) 2017 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <objects/Object.hpp>
#include <objects/Number.hpp>
#include <objects/CompiledMethod.hpp>

#include <vm/MemoryManager.hpp>
#include <vm/ConstantsTable.hpp>

#include <iomanip>

namespace jupiter{

    CompiledMethod::CompiledMethod()
        : locals( 0 ),
          arity(0) {}

    unsigned CompiledMethod::addClosure( Method* value ){
        unsigned index = closures.size();
        closures.push_back( value );
        return index;
    }

    void CompiledMethod::addInstruction(Bytecode code, uint16_t argument, uint8_t shortArgument){
        Instruction inst;
        inst.bytecode = code;
        inst.argument = argument;
        inst.shortArgument = shortArgument;

        instructions.push_back( inst );
    }

    void CompiledMethod::addInstruction(Bytecode code, uint16_t argument){
        Instruction inst;
        inst.bytecode = code;
        inst.argument = argument;

        instructions.push_back( inst );
    }

    void CompiledMethod::addInstruction(Bytecode code){
        Instruction inst;
        inst.bytecode = code;
        inst.argument = 0;

        instructions.push_back( inst );
    }

    void CompiledMethod::setLocals( int _locals ){
        locals = _locals;
    }

    void CompiledMethod::setArity( int _arity ){
        arity = _arity;
    }

    void CompiledMethod::addUpValue(unsigned upvalueIndex, unsigned enclosingLocalIndex ){
        // TODO optimize this search with a map
        auto it = std::find_if( upvalues.begin(), upvalues.end(),
                                [&](const std::pair<unsigned, unsigned>& element){
                                    return element.first == upvalueIndex;});
        // add upvalue if is not already in the vector
        if ( it == upvalues.end() ){
            upvalues.push_back( std::pair<unsigned, unsigned>( upvalueIndex, enclosingLocalIndex ) ) ;
        }
    }

    void CompiledMethod::printBytecode(){
        auto size = instructions.size();
        unsigned i = 0;
        for (; i < size; i++){
            auto currentInstruction = instructions[i];
            auto bytecode = currentInstruction.bytecode;
            auto argument = currentInstruction.argument;

            std::cout << std::setw(3) <<  i << " ";
            switch( bytecode ){

            case PUSH_CONSTANT:
                LOG("PUSH_CONSTANT " << argument <<
                    ": '"<< ConstantsTable::instance().get( argument )->toString() << "'"
                    );
                break;

            case PUSH_LOCAL:
                LOG("PUSH_LOCAL " << argument );
                break;

            case PUSH_GLOBAL:
                LOG("PUSH_GLOBAL " << argument  <<
                    ": '"<< ConstantsTable::instance().get( argument )->toString() << "'"
                    );
                break;

            case PUSH_SELF:
                LOG("PUSH_SELF");
                break;

            case PUSH_CLOSURE:
                LOG("PUSH_CLOSURE " << argument );
                break;

            case PUSH_UPVALUE:
                LOG("PUSH_UPVALUE " << argument);
                break;

            case POP_INTO_UPVALUE:
                LOG("POP_INTO_UPVALUE " << argument );
                break;

            case POP_INTO:
                LOG("POP_INTO " << argument );
                break;

            case POP:
                LOG("POP");
                break;

            case POP_N_INTO_ARRAY:
                LOG("POP_N " << argument);
                break;

            case DUP:
                LOG("DUP " << argument );
                break;

            case SEND:
                LOG("SEND " << argument
                    <<
                    ": '"<< ConstantsTable::instance().get( argument )->toString() << "'" <<
                    ", arity: " << (unsigned) currentInstruction.shortArgument );
                break;

            default:
                break;
            }

        }

    }
}
