// ----------------------------------------------------------- 
// NAME : Bill Chang                         User ID: 108598021 
// DUE DATE : 06/22/2020                                       
// PROGRAM ASSIGNMENT 6                                        
// FILE NAME : thread-main.cpp            
// PROGRAM PURPOSE :                                           
//    Use channel to communicate between threads        
// ----------------------------------------------------------- 
#include "thread.h"

int prime[100];
Mutex *m;

// ----------------------------------------------------------- 
// FUNCTION  main                          
//     start masterThread                            
// PARAMETER USAGE :                                           
//    int  char**               
// FUNCTION CALLED :                                           
//    none          
// ----------------------------------------------------------- 
int main(int argc, char** argv){
     m = new Mutex("m", 0);
     for(int i=0; i<100; i++) prime[i] = 0;
     MasterThread *masterThread;
     if(argc == 1) masterThread = new MasterThread(30);
     else masterThread = new MasterThread(atoi(argv[1]));
     masterThread->Begin();
     masterThread->Join();
     delete(masterThread);
     m->~Mutex();
     Exit();
}
