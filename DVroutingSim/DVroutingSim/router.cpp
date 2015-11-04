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

router::router(char& src, char& dest, char& cost, char& delay) : destination(), cost(), nextHop(), RTSize(0), maxRTSize(0),
                                                                neighborName(), neighborCost(), neighborDelay(), numOfNeighbors(0) {
    
    strcpy(srcRT, &src);
    strcpy(destRT, &dest);
    strcpy(costRT, &cost);
    strcpy(delayRT, &delay);
    
    routerName = srcRT;
    updateRT(*destRT, *costRT, *destRT);
    updateNeighbor(*destRT, *costRT, *delayRT);
}

router::~router() {
    
}

void router::updateRT(char& newDest, char& newCost, char& newNextHop)
{
    //no allocation needed
    if (RTSize < maxRTSize)
    {
        destination[RTSize] = &newDest;
        cost[RTSize] = &newCost;
        nextHop[RTSize] = &newNextHop;
        ++RTSize;
    }
    //allocation needed
    else
    {
        allocateRoutingTable();
        
        destination[RTSize] = &newDest;
        cost[RTSize] = &newCost;
        nextHop[RTSize] = &newNextHop;
        ++RTSize;
    }
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

string router::getRouterName() const
{
    return routerName;
}

void router::updateNeighbor(char& name, char& cost, char& delay)
{
    string sName = &name;
    string sCost = &cost;
    string sDelay = &delay;
    
    neighborName.push_back(sName);
    neighborCost.push_back(sCost);
    neighborDelay.push_back(sDelay);
    ++numOfNeighbors;
}

void router::printNeighbors() const
{
    cout << "\t\t\t\t" << getRouterName() << "'s Neighbors\n";
    cout << "Name\t\t\t\tCost\t\t\tDelay\n";
    
    for (int i = 0; i < numOfNeighbors; ++i)
    {
        cout << neighborName[i] << "\t\t\t\t" << neighborCost[i] << "\t\t\t" << neighborDelay[i] << "\n";
    }
    cout << "\n\n";
}

short router::getNumOfNeighbors()
{
    return numOfNeighbors;
}

string router::getNeighborName(int item)
{
    return neighborName[item];
}

router::DVPacket router::getDVPacket()
{
    DVPacket dvp;
    dvp.dest = destination;
    dvp.cost = cost;
    return dvp;
}

void router::allocateRoutingTable()
{
    maxRTSize += 30;
    
    string *tempDest = new string[maxRTSize];
    string *tempCost = new string[maxRTSize];
    string *tempNextHop = new string[maxRTSize];
    
    for (int i = 0; i < RTSize; ++i) {
        tempDest[i] = destination[i];
        tempCost[i] = cost[i];
        tempNextHop[i] = nextHop[i];
    }
    
    delete [] destination;
    delete [] cost;
    delete [] nextHop;
    
    destination = tempDest;
    cost = tempCost;
    nextHop = tempNextHop;
}

