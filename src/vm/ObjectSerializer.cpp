// Copyright (C) 2017 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <vm/ObjectSerializer.hpp>

#include <objects/Object.hpp>
#include <vm/MemoryManager.hpp>
#include <compiler/Compiler.hpp>

#include <dirent.h>

namespace jupiter{

    ObjectSerializer::ObjectSerializer(){

    }

    void ObjectSerializer::deserialize(std::string path, Map* root){
        DIR *dir;
        struct dirent *entry;


        if ( ( dir = opendir ( path.c_str() ) ) != NULL) {

            while ((entry = readdir (dir)) != NULL) {

                auto name = std::string( entry->d_name );

                if (entry->d_type == DT_DIR){

                    if ( name != "." && name != ".." ){

                        auto obj = MemoryManager::instance().permanent<Map>();

                        root->putAtMut(name, obj);
                        deserialize( path + "/" + name, obj);

                    }


                }else if (entry->d_type == DT_REG){

                    deserializeMethod( path + "/" + name, root );
                }

            }
            closedir (dir);
        } else {
            throw "could not open file or directory";
        }

    }

    void ObjectSerializer::deserializeMethod(std::string path, Map* obj){

        std::ifstream fileReader(  path  );

        std::string signature = "";
        std::string source = "";


        if ( fileReader && fileReader.good()  ){

            std::getline ( fileReader, signature );

            while ( fileReader.good() ){
                std::string line;
                std::getline ( fileReader, line );
                line += "\n";
                source += line;
            }


        }else{
            throw "could not open file: " + path;
        }

        fileReader.close();

        Object* method;
        std::string name;

        try{
            std::tie(name, method) = Compiler::compile( signature, source );
        }catch (const char* s) {
            std::cout << "CompilerException: "<< s  << std::endl << " in file: "<< path << std::endl << std::endl;

        }catch (std::string s) {
            std::cout << "CompilerException: " << s << std::endl << " in file: " << path << std::endl << std::endl;

        }catch (std::exception& e) {
            std::cout << "CompilerException: " << e.what() << std::endl << " in file: " << path << std::endl << std::endl;
        }

        obj->putAtMut( name, method );
    }

    void ObjectSerializer::serialize(std::string path){

    }
}
