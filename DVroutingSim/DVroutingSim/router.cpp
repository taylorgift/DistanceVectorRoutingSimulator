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
#include <string>

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
    cout << "\n";
}

short router::getRTSize() const
{
    return RTSize;
}

string router::getRouterName() const
{
    return routerName;
}

string router::getRTDest(int item)
{
    return destination[item];
}

string router::getRTNextHop(int item)
{
    return nextHop[item];
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

string router::getNeighborCost(int item)
{
    return neighborCost[item];
}

string router::getNeighborDelay(int item)
{
    return neighborDelay[item];
}

string* router::getDVDest()
{
    string* temp = new string[RTSize];
    
    for (int i = 0; i < RTSize; ++i) {
        temp[i] = destination[i];
    }
    
    return temp;
}

string* router::getDVCost()
{
    string* temp = new string[RTSize];
    
    for (int i = 0; i < RTSize; ++i) {
        temp[i] = cost[i];
    }
    
    return temp;
}

bool router::updateRTDV(string* destDV, string* costDV, string nextHopDV, int dvSize, string nCost)
{    
    bool newEntry = true;
    bool triggeredUpdate = false;
    
    for (int i = 0; i < dvSize; ++i) {
        newEntry = true;
        for (int j = 0; j < RTSize; ++j) {
            //if DV destination entry is the destination router, skip entry
            if (destDV[i] == getRouterName())
            {
                newEntry = false;
                break;
            }
            if (destination[j] == destDV[i])
            {
                
                
                //convert costs to ints so they can determine if a shorter path was found
                int costNum = stoi(cost[j]);
                int costDVNum = stoi(costDV[i]);
                int costDVSrcNum = stoi(nCost);
                costDVNum += costDVSrcNum;
                if (costNum > costDVNum)
                {
                    cost[j] = to_string(costDVNum);
                    nextHop[j] = nextHopDV;
                    newEntry = false;
                    triggeredUpdate = true;
                    break;
                }
                else
                {
                    newEntry = false;
                    break;
                }
            }
        }
        
        if (newEntry)
        {
            triggeredUpdate = true;
            int costDVN = stoi(costDV[i]);
            int costTotal = stoi(nCost);
            costTotal += costDVN;
            
            //no allocation needed
            if (RTSize < maxRTSize)
            {
                destination[RTSize] = destDV[i];
                cost[RTSize] = to_string(costTotal);
                nextHop[RTSize] = nextHopDV;
                ++RTSize;
            }
            //allocation needed
            else
            {
                allocateRoutingTable();
                
                destination[RTSize] = destDV[i];
                cost[RTSize] = to_string(costTotal);
                nextHop[RTSize] = nextHopDV;
                ++RTSize;
            }
        }
    }
    return triggeredUpdate;
}

//router::DVPacket router::getDVPacket()
//{
//    DVPacket dvp;
//    dvp.dest = destination;
//    dvp.cost = cost;
//    return dvp;
//}

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

