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
    void updateRT(char& dest, char& cost, char& nextHop);
    void printRT() const;
    void setLink(char linkData);
    short getRouterName() const;
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
    char destination[50];
    char cost[50];
    char nextHop[50];
    short RTSize;
    //Router Attributes
    short routerName;
};

#endif /* defined(__DVroutingSim__router__) */
