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

void printRouterNamesArray(const short rNames[], const short numRouters);
void printAllRT(const vector<router>&);

int main(int argc, const char * argv[]) {
    
    vector<router> rObject;
    
    short maxLineLength = 32;                                               //Hardcoded line length!!!
    char topologyLine[maxLineLength];
    char* src;
    char* dest;
    char* cost;
    char* delay;
    short numOfRouters = 0;
    short routerNames[5] = {-1, -1, -1, -1, -1};                            //Hardcoded number of routers for topology1!!!
    bool newRouter = true;
    
    //checking proper command line entries...
    if (argc == 3) {
        //Open topology file
        ifstream in_file;
        in_file.open(argv[1]);

        if (in_file.is_open()) {
            //scan file for number of router objects to make
            while (!in_file.eof()) {
                newRouter = true;
                in_file.getline(topologyLine, maxLineLength);
                for (int i = 0; i < 5; ++i)
                {
                    if (topologyLine[0] == routerNames[i]) {
                        //If the first input number is a routerName...
                        //Give to object of correct name
                        src = strtok(topologyLine, "\t");
                        dest = strtok(NULL, "\t");
                        cost = strtok(NULL, "\t");
                        delay = strtok(NULL, "\t");
                        
                        rObject[i].updateRT(*dest, *cost, *dest);
                        newRouter = false;
                    }
                }
                
                if (newRouter == true)
                {
                    src = strtok(topologyLine, "\t");
                    dest = strtok(NULL, "\t");
                    cost = strtok(NULL, "\t");
                    delay = strtok(NULL, "\t");
                    
                    //If it's a new router, create a new object
                    router r(*src, *dest, *cost, *delay);
                    rObject.push_back(r);
                    
                    routerNames[numOfRouters] = *src;
                    ++numOfRouters;
                }
                
            }
        }
        else {
            cout << "topology file did not open correctly.\n";
            cout << "Program terminated...\n";
            return 0;
        }
        
    }
    else {
        cout << "Incorrect command line input!\nArgument 1 = topology file, Argument 2 = time\n";
        cout << "Program terminated...\n";
        return 0;
    }
    
    printRouterNamesArray(routerNames, numOfRouters);
    
    printAllRT(rObject);
    
    return 0;
}

void printRouterNamesArray(const short rNames[], const short numRouters)
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





