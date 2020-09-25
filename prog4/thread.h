#ifndef __THREAD__
#define __THREAD__

// ----------------------------------------------------------- 
// NAME : Bill Chang                        User ID: 108598021 
// DUE DATE : 05/04/2020                                       
// PROGRAM ASSIGNMENT #4                                      
// FILE NAME : thread.h                                       
// PROGRAM PURPOSE :                                          
//    inferface of class mom_eagle_t and baby_eagle_t                    
// -----------------------------------------------------------

#include<thread>
#include<iostream>
#include<unistd.h>
#include<cstring>
#include<time.h>
#include<ThreadClass.h>

using namespace std;

extern int wake_eagle;
extern int pot_count;
extern int eating_eagle;
extern Semaphore* pot_count_mutex;
extern Semaphore* eating_eagle_mutex;
extern Semaphore* wake_mutex;
extern Semaphore* no_eagle_eating_mutex;
extern Semaphore* waiting_eagle_can_eat_mutex;
extern Semaphore* food_ready_mutex;

class mom_eagle_t : public Thread{
    public:
        int prepare_food;
        int round;
	int total;
        void ThreadFunc();
};

class baby_eagle_t : public Thread{
    public:
	int i;
        void ThreadFunc();
};

void print_string(char* s);
void use_string(char* des, int i, char* s);
void ready_to_eat(int);
void finish_eating(int);
void goto_sleep(int);
void food_ready(int, int);

#endif
