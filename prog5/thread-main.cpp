// ----------------------------------------------------------- 
// NAME : Bill Chang                         User ID: 108598021 
// DUE DATE : 06/08/2020                                       
// PROGRAM ASSIGNMENT 5                                        
// FILE NAME : thread-main.cpp            
// PROGRAM PURPOSE :                                           
//    Simulate river crossing problem        
// ----------------------------------------------------------- 
#include "thread.h"
Semaphore *lock_stdout;

// ----------------------------------------------------------- 
// FUNCTION  main                          
//     execute river crossing program                            
// PARAMETER USAGE :                                           
//    int  char**               
// FUNCTION CALLED :                                           
//    none          
// ----------------------------------------------------------- 
int main(int argc, char** argv){
    if(argc != 4) return -1;
    srand(time(NULL));

    lock_stdout = new Semaphore("lock_stdout", 1);

    int c, m, b;
    if(atoi(argv[1]) == 0) c = 8;
    else c = atoi(argv[1]);
    if(atoi(argv[2]) == 0) m = 8;
    else m = atoi(argv[2]);
    if(atoi(argv[3]) == 0) b = 5;
    else b = atoi(argv[3]);

    boat boat;
    boat.load = b;
    boat.Begin();
    cannibal c_t[c];
    missionary m_t[m];
    for(int i=0; i<c; i++){
        c_t[i].tid = i+1;
        c_t[i].Begin();
    }
    for(int k=0; k<m; k++){
        m_t[k].tid = k+1;
        m_t[k].Begin();
    }
    boat.Join();
    lock_stdout->~Semaphore();
    Exit();
}

