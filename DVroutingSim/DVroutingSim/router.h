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

class router
{
public:
    router(char& src, char& dest, char& cost, char& delay);
    ~router();
    void updateRT(short dest, short cost, short nextHop);
    void setDestination(char dest);
    void setCost(char cost);
    void setNextHop(char nextHop);
    void setLink(char linkData);
    char getRouterName();
    //events
    void processDVPacket();
    void forwardDataPacket();
    void sendDVPacket();
    
    
    
private:
    //Link struct
    struct link
    {
        
    };
    //Routing table variables                                           How many slots should be reserved for routing table?
    short destination[50];
    short cost[50];
    short nextHop[50];
    short RTSize;
    //Router Attributes
    char routerName;
};

#endif /* defined(__DVroutingSim__router__) */
