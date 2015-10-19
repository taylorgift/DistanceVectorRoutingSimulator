//
//  router.cpp
//  DVroutingSim
//
//  Created by Taylor Gift on 10/15/15.
//  Copyright (c) 2015 Taylor Gift. All rights reserved.
//

#include "router.h"
#include <iostream>

using namespace std;

router::router(char& src, char& dest, char& cost, char& delay) : RTSize(1) {
    cout << "Source: " << src << endl;
    cout << "Destination: " << dest << endl;
    cout << "Cost: " << cost << endl;
    cout << "Delay: " << delay << endl;
}

router::~router() {
    
}

void router::setLink(char linkData)
{
    
}

char router::getRouterName()
{
    return routerName;
}