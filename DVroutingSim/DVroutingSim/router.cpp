//
//  router.cpp
//  DVroutingSim
//
//  Created by Taylor Gift on 10/15/15.
//  Copyright (c) 2015 Taylor Gift. All rights reserved.
//

#include "router.h"
#include <iostream>
#include <cstring>

using namespace std;

router::router(char& src, char& dest, char& cost, char& delay) : RTSize(0) {
    cout << "CONSTRUCTOR Source: " << &src << endl;
    cout << "CONSTRUCTOR Destination: " << &dest << endl;
    cout << "CONSTRUCTOR Cost: " << &cost << endl;
    cout << "CONSTRUCTOR Delay: " << &delay << endl;
    
    strcpy(srcRT, &src);
    strcpy(destRT, &dest);
    strcpy(costRT, &cost);
    strcpy(delayRT, &delay);
    
    cout << "strcpy complete...\n";
    
    routerName = srcRT;
    updateRT(*destRT, *costRT, *destRT);
}

router::~router() {
    
}

void router::updateRT(char& newDest, char& newCost, char& newNextHop)
{
    cout << "MADE IT\n";
    cout << "updateRT newDest: " << &newDest << endl;
    cout << "updateRT newCost: " << &newCost << endl;
    cout << "updateRT newNextHop: " << &newNextHop << endl;
    
    destination[RTSize] = &newDest;
    cost[RTSize] = &newCost;
    nextHop[RTSize] = &newNextHop;
    ++RTSize;
}

void router::printRT() const
{
    cout << "\t\t\t\t" << getRouterName() << "'s Routing Table\n";
    cout << "Destination\t\t\tCost\t\t\tNextHop\n";
    
    for (int i = 0; i < RTSize; ++i)
    {
        cout << destination[i] << "\t\t\t\t" << cost[i] << "\t\t\t" << nextHop[i] << "\n";
    }
    cout << "\n\n";
}

void router::setLink(char linkData)
{
    
}

std::string router::getRouterName() const
{
    return routerName;
}