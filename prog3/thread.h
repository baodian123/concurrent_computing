#ifndef __THREAD__
#define __THREAD__

// ----------------------------------------------------------- 
// NAME : Bill Chang                        User ID: 108598021 
// DUE DATE : 05/04/2020                                       
// PROGRAM ASSIGNMENT #3                                      
// FILE NAME : thread.h                                       
// PROGRAM PURPOSE :                                          
//    inferface of class prefix_sum_thread                    
// -----------------------------------------------------------

#include<thread>
#include<iostream>
#include<unistd.h>
#include<math.h>
#include<cstring>
#include<ThreadClass.h>

using namespace std;

void print_string(char* s);

class prefix_sum_thread : public Thread{
    public:
        void set(int**, int, int, int);
        void ThreadFunc();
    private:
        int** arr;
        int gap;
        int num;
        int count;
};

#endif
