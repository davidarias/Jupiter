// Copyright (C) 2017 David Arias.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.


#include "vm/World.hpp"
#include <memory/GC.hpp>

#include <string>
#include <iostream>

#define VERSION "0.2.0"

using namespace jupiter;


void intro(){
    std::cout << "Jupiter "<< VERSION << std::endl;
    std::cout << "Copyright (C) 2018 David Arias" << std::endl;
    std::cout << std::endl;

    std::cout << "This program comes with ABSOLUTELY NO WARRANTY; for details type `license warranty.'" << std::endl;
    std::cout << "This is free software, and you are welcome to redistribute it" << std::endl;
    std::cout << "under certain conditions; type `license conditions' for details." << std::endl;

    std::cout << std::endl;
}

void repl(World& world, std::string prompt){

    std::string input;


    do{
        std::cout << prompt;
        std::getline(std::cin, input, '\n');

        world.eval( input );

    } while (!std::cin.eof());

}

int main(int argc, char* argv[]){
    World world;

    // garbage colelctor needs the world isntance to trigger the mark phase
    GC::instance().setWorld(&world); //

    if ( argc > 2){
        if (std::string(argv[1]) == "-e" ){

            world.eval( argv[2] );
            return 0;
        }
    }

    intro();
    repl(world, "jupiter> ");
    return 0;
}
