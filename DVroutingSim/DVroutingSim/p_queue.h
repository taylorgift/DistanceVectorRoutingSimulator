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
    void insert(double time, char type, router& src, router& dest);
    void del();
    void printQueue();
    void processEvent();
    double getCurrentTime();
    char getCurrentType();
    string getCurrentSrcName();
    string getCurrentDestName();
    
private:
    struct event
    {
        double time;
        char type;
        router *src;
        router *dest;
        event *link;
    };
    
    event *front;
};


#endif /* defined(__DVroutingSim__p_queue__) */
