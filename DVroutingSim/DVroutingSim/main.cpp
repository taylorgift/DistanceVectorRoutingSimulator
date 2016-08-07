//
//  main.cpp
//  DVroutingSim
//
//  Created by Taylor Gift on 10/15/15.
//  Copyright (c) 2015 Taylor Gift. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <string>
#include "router.h"
#include "p_queue.h"

using namespace std;

/*  Priority Queue
 *  Make the priority queue a linked list with time as the priority.
 *  Each entry of the linked list needs...
 *  time (float): the priority
 *  event: so it knows what to do in this situation
 *  pointers: ??? To send data to neighbors?
 */

/*  Router Assumption
 *  You can assume router names start from 0 -> n
 */

/*
 *  !!!Event Types!!!
 *  s = send DV Packet
 *  f = forward DV Packet
 *  p = process DV Packet
 */

void commandLineInterface(vector<router>&, int argc, const char * argv[], short& numOfRouters, short& maxRouters, string *& routerNames, double& time, int& forwardDataScenario);
void printRouterNamesArray(const string * rNames, const short numRouters);
void printAllRT(const vector<router>&);
void printAllNeighbors(const vector<router>&);
void allocateRouterNames(string *& rNames, const short numOfRouters, short& maxRouters);
int nameToIndex(const string name, vector<router>&, const short numRouters);
//Events
void initializeEventQueue(vector<router>&, const short numOfRouters, p_queue&);
void periodicUpdate(vector<router>&, short &numOfRouters, p_queue&, int currentTime);
char getNextEvent(p_queue&);
void sendDVPacket(vector<router>&, short& numOfRouters, p_queue &, double currentTime);
void processDVPacket(vector<router>&, short numOfRouters, p_queue &, double& currentTime);
void forwardDataPacket(vector<router>& r, short numOfRouters, p_queue & q, double currentTime);

int main(int argc, const char * argv[]) {
    
    p_queue queue;
    vector<router> rObject;
    
    short numOfRouters = 0;
    short maxRouters = 0;
    string *routerNames = NULL;
    double simulationTime = 0;
    double currentTime = 0;
    int periodicSeconds = 1;
    bool networkConvergence = false;
    double convergenceTime = 0;
    string lastNode;
    string lastConvergingNode;
    int routingMessagesCount = 0;
    int finalRoutingMessagesCount = 0;
    int forwardDataScenario = 0;
    
    allocateRouterNames(routerNames, numOfRouters, maxRouters);
    commandLineInterface(rObject, argc, argv, numOfRouters, maxRouters, routerNames, simulationTime, forwardDataScenario);
    initializeEventQueue(rObject, numOfRouters, queue);
    
    //fill priority queue with events
    while (currentTime <= simulationTime)
    {
        if (queue.isEmpty())
        {
            if (networkConvergence == false)
            {
                //Place Data Packet forwarding events after convergence (so convergence can be detected correctly)
                int s = nameToIndex("0", rObject, numOfRouters);
                if (forwardDataScenario == 1 && simulationTime > 10)
                {
                    int d = nameToIndex("3", rObject, numOfRouters);
                    queue.insert(10, 'f', rObject[s], rObject[d], NULL, NULL, 0);
                }
                else if (forwardDataScenario == 2 && simulationTime > 20)
                {
                    int d = nameToIndex("7", rObject, numOfRouters);
                    queue.insert(20, 'f', rObject[s], rObject[d], NULL, NULL, 0);
                }
                else if (forwardDataScenario == 3 && simulationTime > 30)
                {
                    int d = nameToIndex("23", rObject, numOfRouters);
                    queue.insert(30, 'f', rObject[s], rObject[d], NULL, NULL, 0);
                }
                
                convergenceTime = currentTime;
                lastConvergingNode = lastNode;
                finalRoutingMessagesCount = routingMessagesCount;
                networkConvergence = true;
            }
            
            currentTime = periodicSeconds;
        }
        else
        {
            //Update current time
            currentTime = queue.getCurrentTime();
        }
        
        //periodic update
        if (currentTime >= periodicSeconds)
        {
            periodicUpdate(rObject, numOfRouters, queue, periodicSeconds);
            ++periodicSeconds;
        }
        
        //begin processing events
        switch (queue.getCurrentType()) {
            case 's': sendDVPacket(rObject, numOfRouters, queue, currentTime);
                      ++routingMessagesCount;
                      break;
            case 'p': processDVPacket(rObject, numOfRouters, queue, currentTime);
                      ++routingMessagesCount;
                      break;
            case 'f': forwardDataPacket(rObject, numOfRouters, queue, currentTime);
                      break;
            default: cout << "Event Type '" << queue.getCurrentType() << "' not recognized...\n";
                     cout << "Terminating Program...\n";
                     exit(EXIT_FAILURE);
        }
        lastNode = queue.getCurrentDestName();
        queue.del();
    }
    
    printAllRT(rObject);
    
    cout << "Network converged at time " << convergenceTime << endl;
    cout << "Last Node to converge was " << lastConvergingNode << endl;
    cout << "Total routing-control messages = " << finalRoutingMessagesCount << endl;
    
    return 0;
}

void commandLineInterface(vector<router>& rObject, int argc, const char * argv[], short& numOfRouters, short& maxRouters, string *& routerNames, double& time, int& forwardDataScenario)
{
    short maxLineLength = 100;
    char topologyLine[maxLineLength];
    char* src;
    char* dest;
    char* cost;
    char* delay;
    bool newRouter = true;
    bool newDestRouter = true;
    
    //checking proper command line entries...
    if (argc == 3) {
        //Set simulation time
        time = atof(argv[2]);
        //Open topology file
        ifstream in_file;
        in_file.open(argv[1]);
        
        if (in_file.is_open()) {
            
            //set data packet forwarding scenario
            if (!strcmp(argv[1], "topology1.txt"))
            {
                forwardDataScenario = 1;
            }
            else if (!strcmp(argv[1], "topology2.txt"))
            {
                forwardDataScenario = 2;
            }
            else if (!strcmp(argv[1], "topology3.txt"))
            {
                forwardDataScenario = 3;
            }
            
            //scan file for number of router objects to make
            in_file.getline(topologyLine, maxLineLength);
            while (!in_file.eof()) {

                //Parse line into string tokens
                src = strtok(topologyLine, "\t");
                dest = strtok(NULL, "\t");
                cost = strtok(NULL, "\t");
                delay = strtok(NULL, "\t");
                
                newRouter = true;
                newDestRouter = true;
                for (int i = 0; i < numOfRouters; ++i)
                {
                    if (src == routerNames[i]) {
                        //If the source number is a routerName...
                        //Give to object of correct name
                        rObject[i].updateRT(*dest, *cost, *dest);
                        rObject[i].updateNeighbor(*dest, *cost, *delay);
                        newRouter = false;
                    }
                    
                    /*
                     */
                    if (dest == routerNames[i])
                    {
                        //If the destination number is a routerName...
                        //Give to object of correct name
                        rObject[i].updateRT(*src, *cost, *src);
                        rObject[i].updateNeighbor(*src, *cost, *delay);
                        newDestRouter = false;
                    }
                    
                }
                if (newRouter == true)
                {
                    //If it's a new router, create a new object
                    router r(*src, *dest, *cost, *delay);
                    rObject.push_back(r);
                    
                    //routerName allocation not needed
                    if (numOfRouters < maxRouters)
                    {
                        routerNames[numOfRouters] = src;
                        ++numOfRouters;
                    }
                    //routerName allocation needed
                    else
                    {
                        allocateRouterNames(routerNames, numOfRouters, maxRouters);
                        
                        routerNames[numOfRouters] = src;
                        ++numOfRouters;
                    }
                }
                
                /*
                 */
                if (newDestRouter == true)
                {
                    //If it's a new router, create a new object
                    router r(*dest, *src, *cost, *delay);
                    rObject.push_back(r);
                    
                    //routerName allocation not needed
                    if (numOfRouters < maxRouters)
                    {
                        routerNames[numOfRouters] = dest;
                        ++numOfRouters;
                    }
                    //routerName allocation needed
                    else
                    {
                        allocateRouterNames(routerNames, numOfRouters, maxRouters);
                        
                        routerNames[numOfRouters] = dest;
                        ++numOfRouters;
                    }
                }
                
                in_file.getline(topologyLine, maxLineLength);
            }
        }
        else {
            cout << "topology file did not open correctly.\n";
            cout << "Program terminated...\n";
            exit(EXIT_FAILURE);
        }
        in_file.close();
    }
    else {
        cout << "Incorrect command line input!\nArgument 0 = program, Argument 1 = topology file, Argument 2 = time\n";
        cout << "Program terminated...\n";
        exit(EXIT_FAILURE);
    }
}

void printRouterNamesArray(const string * rNames, const short numRouters)
{
    cout << "Total routers = " << numRouters << "\n";
    cout << "routerNames array: { ";
    for (int i = 0; i < numRouters; ++i) {
        cout << rNames[i] << ", ";
    }
    cout << "}\n";
}

void printAllRT(const vector<router>& r)
{
    unsigned long size = r.size();
    
    for (unsigned int i = 0; i < size; ++i) {
        r[i].printRT();
    }
}

void printAllNeighbors(const vector<router>& r)
{
    unsigned long size = r.size();
    
    for (unsigned int i = 0; i < size; ++i) {
        r[i].printNeighbors();
    }
}

void allocateRouterNames(string *& rNames, const short numOfRouters, short& maxRouters)
{
    maxRouters += 2;
    
    string *tempRouterArrayPointer = new string[maxRouters];
    
    for (int i = 0; i < numOfRouters; ++i) {
        tempRouterArrayPointer[i] = rNames[i];
    }
    
    delete [] rNames;
    
    rNames = tempRouterArrayPointer;
    
    //Initialize new router slots to -1
    for (int j = numOfRouters; j < maxRouters; ++j) {
        rNames[j] = "-1";
    }
}

int nameToIndex(const string name, vector<router>& r, const short numRouters)
{
    for (int i = 0; i < numRouters; ++i) {
        if (name == r[i].getRouterName())
        {
            return i;
        }
    }
    return -1;
}

/*
 *  EVENTS
 */

void initializeEventQueue(vector<router>& r, const short numOfRouters, p_queue& q)
{
    int index;
    
    for (int i = 0; i < numOfRouters; ++i) {
        for (int j = 0; j < r[i].getNumOfNeighbors(); ++j) {
            index = nameToIndex(r[i].getNeighborName(j), r, numOfRouters);
            if (index == -1)
            {
                cout << "nameToIndex failed during event queue initialization...\nTerminating program.\n";
                exit(EXIT_FAILURE);
            }
            q.insert(0, 's', r[i], r[index], r[i].getDVDest(), r[i].getDVCost(), r[i].getRTSize());
        }
    }
}

void periodicUpdate(vector<router>& r, short &numOfRouters, p_queue& q, int currentTime)
{
    int index;
    
    for (int i = 0; i < numOfRouters; ++i) {
        for (int j = 0; j < r[i].getNumOfNeighbors(); ++j) {
            index = nameToIndex(r[i].getNeighborName(j), r, numOfRouters);
            if (index == -1)
            {
                cout << "nameToIndex failed during event queue initialization...\nTerminating program.\n";
                exit(EXIT_FAILURE);
            }
            q.insert(currentTime, 's', r[i], r[index], r[i].getDVDest(), r[i].getDVCost(), r[i].getRTSize());
        }
    }
}

void sendDVPacket(vector<router>& r, short& numOfRouters, p_queue & q, double currentTime)
{
    int srcIndex = nameToIndex(q.getCurrentSrcName(), r, numOfRouters);
    int destIndex = nameToIndex(q.getCurrentDestName(), r, numOfRouters);
    double newTime;
    
    //Find delay value from src to dest
    for (int i = 0; i < r[srcIndex].getNumOfNeighbors(); ++i) {
        if (r[srcIndex].getNeighborName(i) == r[destIndex].getRouterName())
        {
            newTime = stod(r[srcIndex].getNeighborDelay(i)) + currentTime;
            q.insert(newTime, 'p', r[srcIndex], r[destIndex], q.getCurrentDVDest(), q.getCurrentDVCost(), q.getCurrentDVSize());    //Last 2 parameters need to be passed by value?
                                                                                                                                    //Otherwise values may be changed between this
                                                                                                                                    //event and the event where the packet is
                                                                                                                                    //processed.
            return;
        }
        else
        {
            /*
             *   This can detect a router going down!!!!
             */
        }
    }
}

void processDVPacket(vector<router>& r, short numOfRouters, p_queue & q, double& currentTime)
{
    int srcIndex = nameToIndex(q.getCurrentSrcName(), r, numOfRouters);
    int destIndex = nameToIndex(q.getCurrentDestName(), r, numOfRouters);
    int neighborIndex = -1;
    
    for (int i = 0; i < r[destIndex].getNumOfNeighbors(); ++i) {
        if (r[srcIndex].getRouterName() == r[destIndex].getNeighborName(i))
        {
            neighborIndex = i;
        }
    }
    
    if (r[destIndex].updateRTDV(q.getCurrentDVDest(), q.getCurrentDVCost(), q.getCurrentSrcName(), q.getCurrentDVSize(), r[destIndex].getNeighborCost(neighborIndex)))
    {
        //triggered update occurs
        for (int i = 0; i < r[destIndex].getNumOfNeighbors(); ++i) {
            //split horizon
            if (r[i].getRouterName() == r[srcIndex].getRouterName())
            {
                continue;
            }
            q.insert(currentTime, 's', r[destIndex], r[i], r[destIndex].getDVDest(), r[destIndex].getDVCost(), r[destIndex].getRTSize());
        }
    }
    else
    {
        //nothing in the routing table was updated, no triggered update
    }
    
    //Delete DV packet pointers
}

void forwardDataPacket(vector<router>& r, short numOfRouters, p_queue & q, double currentTime)
{
    int srcIndex = nameToIndex(q.getCurrentSrcName(), r, numOfRouters);
    int destIndex = nameToIndex(q.getCurrentDestName(), r, numOfRouters);
    double newTime;
    int newSrcIndex;
    
    if (srcIndex == destIndex)
    {
        cout << "Data Packet has arrived at node " << r[destIndex].getRouterName() << ".\n";
        cout << "It arrived at time " << currentTime << endl;
        return;
    }
    
    for (int i = 0; i < r[srcIndex].getRTSize(); ++i) {
        if (r[srcIndex].getRTDest(i) == r[destIndex].getRouterName())
        {
            //Data Packet tracing
            cout << "Forwarding Data Packet from " << r[srcIndex].getRouterName() << " to " << r[srcIndex].getRTNextHop(i) << " at time " << currentTime << endl;
            cout << "Data Packet's final destination is " << r[destIndex].getRouterName() << endl;
            
            //Find new time to insert next forwarding event
            for (int j = 0; j < r[srcIndex].getNumOfNeighbors(); ++j) {
                if (r[srcIndex].getNeighborName(j) == r[srcIndex].getRTNextHop(i))
                {
                    newTime = stod(r[srcIndex].getNeighborDelay(j)) + currentTime;
                    newSrcIndex = nameToIndex(r[srcIndex].getNeighborName(j), r, numOfRouters);
                    q.insert(newTime, 'f', r[newSrcIndex], r[destIndex], NULL, NULL, 0);
                    return;
                }
            }
        }
    }
    
}

