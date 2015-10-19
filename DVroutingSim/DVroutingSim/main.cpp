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

int main(int argc, const char * argv[]) {
    
    vector<router> r;
    
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
        
        cout << "1\n";
        
        if (in_file.is_open()) {
            
            cout << "2\n";
            
            //scan file for number of router objects to make
            while (!in_file.eof()) {
                
                cout << "3\n";
                
                newRouter = true;
                in_file.getline(topologyLine, maxLineLength);
                for (int i = 0; i < 5; ++i)
                {
                    
                    cout << "4\n";
                    
                    if (topologyLine[0] == routerNames[i]) {
                        //If the first input number is a routerName...
                        //Give to object of correct name
                        newRouter = false;
                    }
                    
                }
                
                if (newRouter == true)
                {
                    
                    cout << "5\n";
                    
                    src = strtok(topologyLine, "\t");
                    dest = strtok(NULL, "\t");
                    cost = strtok(NULL, "\t");
                    delay = strtok(NULL, "\t");
                    
                    //If it's a new router, create a new object
                    router r(char& src, char& dest, char& cost, char& delay);
                    
                    ++numOfRouters;
                    
                    cout << "6\n";
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
    
    
    for (int time = 0; time < 10; ++time)
    {
        
    }
    return 0;
}
