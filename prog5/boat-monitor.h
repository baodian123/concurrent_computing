#ifndef __BOAT_MONITOR__
#define __BOAT_MONITOR__

// ----------------------------------------------------------- 
// NAME : Bill Chang                         User ID: 108598021 
// DUE DATE : 06/08/2020                                       
// PROGRAM ASSIGNMENT 5                                        
// FILE NAME : boat-monitor.h            
// PROGRAM PURPOSE :                                           
//    inferface of class m_monitor and print function         
// ----------------------------------------------------------- 

#include<iostream>
#include<unistd.h>
#include<ThreadClass.h>
#include<cstring>

using namespace std;

extern Semaphore *lock_stdout;

class m_monitor : public Monitor{
    private:
        int m_count = 0, c_count = 0;
        Condition *c, *m, *left, *boat_start;
        int load[4];
        char type[4];
        int seat = 0;
        int round = 1;
        bool is_boat_ready = false;

    public:
        m_monitor();
        void cannibal_arrives(int);
        void missionary_arrives(int);
        void boat_ready(char*, char*, bool, int);
        void boat_done(char*);
};

void print_line(int, int, int);
void print_string(char*);

#endif