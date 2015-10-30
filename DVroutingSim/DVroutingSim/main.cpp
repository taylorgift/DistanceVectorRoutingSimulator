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
#include "router.h"

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

void commandLineInterface(vector<router>&, int argc, const char * argv[], short& numOfRouters, short& maxRouters, string *& routerNames, double& time);
void printRouterNamesArray(const string * rNames, const short numRouters);
void printAllRT(const vector<router>&);
void allocateRouterNames(string *& rNames, const short numOfRouters, short& maxRouters);
//Events
void processDVPacket(vector<router>&, short numOfRouters, short routerNames[]);     //numOfRouters passed by val or ref???

int main(int argc, const char * argv[]) {
    
    vector<router> rObject;
    
    short numOfRouters = 0;
    short maxRouters = 0;
    string *routerNames = NULL;
    //string routerNames[5] = {"-1", "-1", "-1", "-1", "-1"};                            //Hardcoded number of routers for topology1!!!
    double simulationTime = 0;
    double currentTime = 0;
    
    allocateRouterNames(routerNames, numOfRouters, maxRouters);
    
    cout << "numOfRouters: " << numOfRouters << ", maxRouters: " << maxRouters << endl;
    
    cout << "Pre commandLineInterface, simulationTime = " << simulationTime << endl;
    
    commandLineInterface(rObject, argc, argv, numOfRouters, maxRouters, routerNames, simulationTime);
    
    cout << "Post commandLineInterface, simulationTime = " << simulationTime << endl;
    
    cout << "routerNames[0-4]: " << routerNames[0] << routerNames[1] << routerNames[2] << routerNames[3] << routerNames[4] << endl;
    
    printRouterNamesArray(routerNames, numOfRouters);
    
    printAllRT(rObject);
    
    return 0;
}

void commandLineInterface(vector<router>& rObject, int argc, const char * argv[], short& numOfRouters, short& maxRouters, string *& routerNames, double& time)
{
    cout << "CL INTERFACE\n";
    
    short maxLineLength = 32;                                               //Hardcoded line length!!!
    char topologyLine[maxLineLength];
    char* src;
    char* dest;
    char* cost;
    char* delay;
    bool newRouter = true;
    
    //checking proper command line entries...
    if (argc == 3) {
        //Set simulation time
        time = atof(argv[2]);
        //Open topology file
        ifstream in_file;
        in_file.open(argv[1]);
        
        if (in_file.is_open()) {
            //scan file for number of router objects to make
            in_file.getline(topologyLine, maxLineLength);
            while (!in_file.eof()) {
                //Parse line into string tokens
                src = strtok(topologyLine, "\t");
                dest = strtok(NULL, "\t");
                cost = strtok(NULL, "\t");
                delay = strtok(NULL, "\t");
                
                newRouter = true;
                for (int i = 0; i < numOfRouters; ++i)
                {
                    if (src == routerNames[i]) {
                        //If the source number is a routerName...
                        //Give to object of correct name
                        
                        rObject[i].updateRT(*dest, *cost, *dest);
                        newRouter = false;
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
                        cout << "*** Before routerNames array allocation ***\n";
                        cout << "Old Max Routers: " << maxRouters << endl;
                        cout << "Old Router Allocation: { ";
                        for (int i = 0; i < maxRouters; ++i) {
                            cout << routerNames[i] << ", ";
                        }
                        cout << "}\n";
                        
                        allocateRouterNames(routerNames, numOfRouters, maxRouters);
                        

                        
                        routerNames[numOfRouters] = src;
                        ++numOfRouters;
                        
                        cout << "*** After routerNames array allocation ***\n";
                        cout << "MAX ROUTERS: " << maxRouters << endl;
                        cout << "New Router Allocation: { ";
                        for (int i = 0; i < maxRouters; ++i) {
                            cout << routerNames[i] << ", ";
                        }
                        cout << "}\n";
                        
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
    }
    else {
        cout << "Incorrect command line input!\nArgument 1 = topology file, Argument 2 = time\n";
        cout << "Program terminated...\n";
        exit(EXIT_FAILURE);
    }
}

void printRouterNamesArray(const string * rNames, const short numRouters)
{
    cout << "Inside printArray function...\n";
    
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

void allocateRouterNames(string *& rNames, const short numOfRouters, short& maxRouters)
{
    cout << "Inside allocateRouterNames function...\n";
    
    maxRouters += 2;
    
    string *tempRouterArrayPointer = new string[maxRouters];
    
    for (int i = 0; i < numOfRouters; ++i) {
        tempRouterArrayPointer[i] = rNames[i];
    }
    
    delete [] rNames;
    
    rNames = tempRouterArrayPointer;
    
    cout << "Newly allocated routerNames array: { ";
    
    for (int i = 0; i < numOfRouters; ++i) {
        cout << rNames[i] << ", ";
    }
    
    //Initialize new router slots
    cout << "{new slots ->} ";
    for (int j = numOfRouters; j < maxRouters; ++j) {
        rNames[j] = "-1";
        cout << rNames[j] << ", ";
    }
    cout << "}\n";
}



