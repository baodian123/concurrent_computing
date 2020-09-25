// ----------------------------------------------------------- 
// NAME : Bill Chang                         User ID: 108598021 
// DUE DATE : 05/23/2020                                       
// PROGRAM ASSIGNMENT 4                                        
// FILE NAME : thread.cpp            
// PROGRAM PURPOSE :                                           
//    Implement method of mom eagle and baby eagle thread        
// ----------------------------------------------------------- 
#include "thread.h"

// ----------------------------------------------------------- 
// FUNCTION  ThreadFunc (mom_eagle_t)                          
//     Simulate mom eagle active                            
// PARAMETER USAGE :                                           
//    none               
// FUNCTION CALLED :                                           
//    none          
// ----------------------------------------------------------- 
void mom_eagle_t::ThreadFunc(){
    Thread::ThreadFunc();
	while(1){
	int c = rand()%10000;
	char buf[100];
        goto_sleep(round);
        if(round-- == 0) Exit();
        for(int i=0; i<c; i++) Delay();
        food_ready(prepare_food, total-round);
        Delay();
    }
}

// ----------------------------------------------------------- 
// FUNCTION  ThreadFunc (mom_eagle_t)                          
//     Simulate baby eagle active                            
// PARAMETER USAGE :                                           
//    none               
// FUNCTION CALLED :                                           
//    none          
// ----------------------------------------------------------- 
void baby_eagle_t::ThreadFunc(){
    Thread::ThreadFunc();
    while(1){
	int c = rand()%10000;
        for(int i=0; i<c; i++) Delay();
        ready_to_eat(i);
	for(int i=0; i<c; i++) Delay();
        finish_eating(i);
    }
}
