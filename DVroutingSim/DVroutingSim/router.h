//
//  router.h
//  DVroutingSim
//
//  Created by Taylor Gift on 10/15/15.
//  Copyright (c) 2015 Taylor Gift. All rights reserved.
//

#ifndef __DVroutingSim__router__
#define __DVroutingSim__router__

#include <stdio.h>
#include <string>
#include <vector>

using namespace std;

class router
{
public:
    router(char& src, char& dest, char& cost, char& delay);
    ~router();
    void updateRT(char& dest, char& cost, char& nextHop);
    void printRT() const;
    short getRTSize() const;
    string getRouterName() const;
    string getRTDest(int item);
    string getRTNextHop(int item);
    void updateNeighbor(char& name, char& cost, char& delay);
    void printNeighbors() const;
    
    //DV Packet implemented as parallel array
    //these give new dest/cost arrays so that their data isn't overwritten by an event
    string* getDVDest();
    string* getDVCost();
    bool updateRTDV(string* destDV, string* costDV, string nextHopDV, int dvSize, string nCost);
    
    
//    //DV Packet implemented as struct
//    struct DVPacket
//    {
//        string *dest;
//        string *cost;
//    };
//    DVPacket getDVPacket();
    //Pass neighbor information
    short getNumOfNeighbors();
    string getNeighborName(int item);
    string getNeighborCost(int item);
    string getNeighborDelay(int item);
    
    
    
private:
    //Routing table variables/functions
    string *destination;
    string *cost;
    string *nextHop;
    short RTSize;
    short maxRTSize;
    void allocateRoutingTable();
    //Router Attributes
    string routerName;
    
    /*
     *  Neighbor information!!!
     */
    vector<string> neighborName;
    vector<string> neighborCost;
    vector<string> neighborDelay;
    short numOfNeighbors;
    
    //Update Routing Table temps
    char srcRT[7];
    char destRT[7];
    char costRT[7];
    char delayRT[15];
};

#endif /* defined(__DVroutingSim__router__) */
