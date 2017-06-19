/*
 *  Simpleton_Engine.cpp
 *  Simpleton Engine
 *
 *  Created by Indi Kernick on 19/6/17.
 *  Copyright © 2017 Indi Kernick. All rights reserved.
 *
 */

#include <iostream>
#include "Simpleton_Engine.hpp"
#include "Simpleton_EnginePriv.hpp"

void Simpleton_Engine::HelloWorld(const char * s)
{
    Simpleton_EnginePriv *theObj = new Simpleton_EnginePriv;
    theObj->HelloWorldPriv(s);
    delete theObj;
};

void Simpleton_EnginePriv::HelloWorldPriv(const char * s) 
{
    std::cout << s << std::endl;
};

