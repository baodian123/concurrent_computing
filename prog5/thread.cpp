// ----------------------------------------------------------- 
// NAME : Bill Chang                         User ID: 108598021 
// DUE DATE : 06/08/2020                                       
// PROGRAM ASSIGNMENT 5                                        
// FILE NAME : thread.cpp            
// PROGRAM PURPOSE :                                           
//    Implement method of cannibal and missionary and boat thread        
// ----------------------------------------------------------- 
#include "thread.h"

static m_monitor *monitor = new m_monitor();

// ----------------------------------------------------------- 
// FUNCTION  ThreadFunc (cannibal)                          
//     Simulate cannibal active                            
// PARAMETER USAGE :                                           
//    none               
// FUNCTION CALLED :                                           
//    none          
// ----------------------------------------------------------- 
void cannibal::ThreadFunc(){
    Thread::ThreadFunc();
    print_line(tid, 1, 1);
	while(1){
	    int c = rand()%10000;
        for(int i=0; i<c; i++) Delay();
        print_line(tid, 2, 1);
        monitor->cannibal_arrives(this->tid);
    }
}

// ----------------------------------------------------------- 
// FUNCTION  ThreadFunc (missionary)                          
//     Simulate missionary active                            
// PARAMETER USAGE :                                           
//    none               
// FUNCTION CALLED :                                           
//    none          
// ----------------------------------------------------------- 
void missionary::ThreadFunc(){
    Thread::ThreadFunc();
    print_line(tid, 1, 2);
    while(1){
        int c = rand()%10000;
        for(int i=0; i<c; i++) Delay();
        print_line(tid, 2, 2);
        monitor->missionary_arrives(this->tid);
    }
}

// ----------------------------------------------------------- 
// FUNCTION  ThreadFunc (boat)                          
//     Simulate boat active                            
// PARAMETER USAGE :                                           
//    none               
// FUNCTION CALLED :                                           
//    none          
// ----------------------------------------------------------- 
void boat::ThreadFunc(){
    Thread::ThreadFunc();
    print_string("***** BOAT thread starts\n");
    for(int i=0; i<load+1; i++){
        int c = rand()%10000;
        for(int j=0; j<c; j++) Delay();
        if(i != load){
            print_string("***** The boat is ready\n");
            monitor->boat_ready(buf, pbuf, false, load);
        }
        else monitor->boat_ready(buf, pbuf, true, load);
        print_string(buf);
        print_string(pbuf);
        for(int k=0; k<c; k++) Delay();
        monitor->boat_done(buf);
        print_string(buf);
    }
}