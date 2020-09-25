// ----------------------------------------------------------- 
// NAME : Bill Chang                         User ID: 108598021 
// DUE DATE : 06/22/2020                                       
// PROGRAM ASSIGNMENT 6                                        
// FILE NAME : thread.h            
// PROGRAM PURPOSE :                                           
//    interface of PrimeThread, MasterThread and functions       
// -----------------------------------------------------------
#include<iostream>
#include<cstring>
#include "ThreadClass.h"

using namespace std;
extern int prime[100];
extern Mutex *m;

class PrimeThread : public Thread{
     public:
          PrimeThread(int index, int number);
          ~PrimeThread();
          SynOneToOneChannel *channel;
     private:
          void ThreadFunc();
          int index;
          int number;
          PrimeThread *neighbor;
};

class MasterThread : public Thread{
     public:
          MasterThread(int number);
     private:
          int number;
          void ThreadFunc();
};

void print_string(char*);
void print_by_indentation(int, char*);