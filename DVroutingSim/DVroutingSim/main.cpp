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

void commandLineInterface(vector<router>&, int argc, const char * argv[], short& numOfRouters, string routerNames[], double& time);
void printRouterNamesArray(const string rNames[], const short numRouters);
void printAllRT(const vector<router>&);
//Events
void processDVPacket(vector<router>&, short numOfRouters, short routerNames[]);     //numOfRouters passed by val or ref???

int main(int argc, const char * argv[]) {
    
    vector<router> rObject;
    
    short numOfRouters = 0;
    string routerNames[5] = {"-1", "-1", "-1", "-1", "-1"};                            //Hardcoded number of routers for topology1!!!
    double simulationTime = 0;
    double currentTime = 0;
    
    cout << "Pre commandLineInterface, simulationTime = " << simulationTime << endl;
    
    commandLineInterface(rObject, argc, argv, numOfRouters, routerNames, simulationTime);
    
    cout << "Post commandLineInterface, simulationTime = " << simulationTime << endl;
    
    printRouterNamesArray(routerNames, numOfRouters);
    
    printAllRT(rObject);
    
    return 0;
}

void commandLineInterface(vector<router>& rObject, int argc, const char * argv[], short& numOfRouters, string routerNames[], double& time)
{
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
                for (int i = 0; i < 5; ++i)
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
                    
                    routerNames[numOfRouters] = src;
                    ++numOfRouters;
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
    
    cout << "Before functions...\n";

}

void printRouterNamesArray(const string rNames[], const short numRouters)
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





