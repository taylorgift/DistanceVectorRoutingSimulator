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

using namespace std;

class router
{
public:
    router(char& src, char& dest, char& cost, char& delay);
    ~router();
    void updateRT(char& dest, char& cost, char& nextHop);
    void printRT() const;
    void setLink(char linkData);
    string getRouterName() const;
    //events
    void processDVPacket();
    void forwardDataPacket();
    void sendDVPacket();
    
    
    
private:
    //Link struct
    struct link
    {
        
    };
    //Routing table variables/functions                                           How many slots should be reserved for routing table?
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
    
    //Update Routing Table temps
    char srcRT[7];
    char destRT[7];
    char costRT[7];
    char delayRT[15];
};

#endif /* defined(__DVroutingSim__router__) */
