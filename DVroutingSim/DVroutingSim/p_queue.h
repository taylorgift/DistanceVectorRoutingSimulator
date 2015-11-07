//
//  p_queue.h
//  DVroutingSim
//
//  Created by Taylor Gift on 10/30/15.
//  Copyright (c) 2015 Taylor Gift. All rights reserved.
//

#ifndef __DVroutingSim__p_queue__
#define __DVroutingSim__p_queue__

#include <stdio.h>
#include "router.h"

using namespace std;

class p_queue
{
public:
    p_queue();
    ~p_queue();
    void insert(double time, char type, router& src, router& dest, string *dvDest, string *dvCost, int dvSize);
    void del();
    void printQueue();
    void printDVPackets();
    void processEvent();
    double getCurrentTime();
    char getCurrentType();
    string* getCurrentDVDest();
    string* getCurrentDVCost();
    int getCurrentDVSize();
    string getCurrentSrcName();
    string getCurrentDestName();
    bool isEmpty();
    
private:
    struct event
    {
        double time;
        char type;
        router *src;
        router *dest;
        string *dvDest;
        string *dvCost;
        int dvSize;
        event *link;
    };
    
    event *front;
};


#endif /* defined(__DVroutingSim__p_queue__) */
