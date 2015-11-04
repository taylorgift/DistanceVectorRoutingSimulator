//
//  p_queue.cpp
//  DVroutingSim
//
//  Created by Taylor Gift on 10/30/15.
//  Copyright (c) 2015 Taylor Gift. All rights reserved.
//

#include "p_queue.h"
#include <iostream>

p_queue::p_queue()
{
    front = NULL;
}

p_queue::~p_queue()
{
    
}

void p_queue::insert(double time, char type, router& src, router& dest)
{
    //create new event
    event *tmp, *q;
    tmp = new event;
    tmp->time = time;
    tmp->type = type;
    tmp->src = &src;
    tmp->dest = &dest;
    //if it's the first event/most prioritized event...
    if (front == NULL || time < front->time)
    {
        tmp->link = front;
        front = tmp;
    }
    else
    {
        //scan to find the proper place in the priority queue
        q = front;
        while (q->link != NULL && q->link->time <= time)
            q = q->link;
        tmp->link = q->link;
        q->link = tmp;
    }
}

void p_queue::del()
{
    event *tmp;
    if (front == NULL)
        cout << "Cannot delete... Queue empty\n";
    else
    {
        tmp = front;
        front = front->link;
        free(tmp);
    }
}

void p_queue::printQueue()
{
    int eventNumber = 0;
    event *ptr;
    ptr = front;
    if (front == NULL)
        cout << "Event Queue is empty...\n";
    else
    {
        cout << "\t\t\t*** Event Queue ***\n";
        cout << "Time\tType\tSrcRouter\t\tDestRouter\n";
        while (ptr != NULL) {
            cout << ptr->time << "\t" << ptr->type << "\t" << "(" << ptr->src->getRouterName() << ")" << ptr->src << "\t"
                 << "(" << ptr->dest->getRouterName() << ")" << ptr->dest << "\t\tEvent " << eventNumber << "\n";
            ++eventNumber;
            ptr = ptr->link;
        }
    }
}

double p_queue::getCurrentTime()
{
    return front->time;
}

char p_queue::getCurrentType()
{
    return front->type;
}

string p_queue::getCurrentSrcName()
{
    return front->src->getRouterName();
}

string p_queue::getCurrentDestName()
{
    return front->dest->getRouterName();
}


