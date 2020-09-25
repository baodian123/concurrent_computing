#ifndef __THREAD__
#define __THREAD__

// ----------------------------------------------------------- 
// NAME : Bill Chang                        User ID: 108598021 
// DUE DATE : 06/08/2020                                       
// PROGRAM ASSIGNMENT #5                                      
// FILE NAME : thread.h                                       
// PROGRAM PURPOSE :                                          
//    inferface of class cannibal and missionary and boat                    
// -----------------------------------------------------------

#include<iostream>
#include<unistd.h>
#include<ThreadClass.h>
#include "boat-monitor.h"
using namespace std;

class cannibal : public Thread{
    public:
        int tid;
        char buf[200];
        void ThreadFunc();
};

class missionary : public Thread{
    public:
	    int tid;
        char buf[200];
        void ThreadFunc();
};

class boat : public Thread{
    public:
        int load;
        char buf[200];
        char pbuf[200];
        void ThreadFunc();
};

#endif