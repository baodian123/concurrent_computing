// ----------------------------------------------------------- 
// NAME : Bill Chang                         User ID: 108598021 
// DUE DATE : 06/22/2020                                       
// PROGRAM ASSIGNMENT 6                                        
// FILE NAME : thread.cpp            
// PROGRAM PURPOSE :                                           
//    implement method of PrimeThread and MasterThread        
// ----------------------------------------------------------- 
#include "thread.h"

// ----------------------------------------------------------- 
// FUNCTION  PrimeThread                          
//     initial PrimeThread                            
// PARAMETER USAGE :                                           
//    int  int               
// FUNCTION CALLED :                                           
//    MasterThread          
// ----------------------------------------------------------- 
PrimeThread::PrimeThread(int index, int number){
    this->index = index;
    this->number = number;
    this->neighbor = NULL;
    UserDefinedThreadID = index;
    channel = new SynOneToOneChannel("c", index-1, index);
    int i = 0;
    while(prime[i] != 0) i++;
    prime[i] = this->number;
}

// ----------------------------------------------------------- 
// FUNCTION  ThreadFunc(PrimeThread)                          
//     active of PrimeThread                             
// PARAMETER USAGE :                                           
//    none                 
// FUNCTION CALLED :                                           
//    none          
// ----------------------------------------------------------- 
void PrimeThread::ThreadFunc(){
    Thread::ThreadFunc();
    char tmp[200] = "";
    int r_number;
    sprintf(tmp, "P%d starts and memorizes %d\n", this->number, this->number);
    print_by_indentation(index, tmp);
    while(true){
        channel->Receive(&r_number, sizeof(int));
        if(r_number == -1) sprintf(tmp, "P%d receives END\n", this->number);
        else sprintf(tmp, "P%d receives %d\n", this->number, r_number);
        print_by_indentation(index, tmp);
        if(r_number%this->number != 0 && r_number != -1){
            if(neighbor == NULL){
                sprintf(tmp, "P%d creates P%d\n", this->number, r_number);
                print_by_indentation(index, tmp);
                neighbor = new PrimeThread(this->index+1, r_number);
                neighbor->Begin();
            }
            else{
                sprintf(tmp, "P%d sends %d to P%d\n", this->number, r_number, neighbor->number);
                print_by_indentation(index, tmp);
                neighbor->channel->Send(&r_number, sizeof(int));
            }
        }
        else if(r_number == -1){
            if(neighbor == NULL) Exit();
            else{
                neighbor->channel->Send(&r_number, sizeof(int));
                neighbor->Join();
                delete(neighbor);
                Exit();
            }
        }
        else if(r_number%this->number == 0){
            sprintf(tmp, "P%d ignores %d\n", this->number, r_number);
            print_by_indentation(index, tmp);
        }
        else print_string("error catch\n");
    }
}

// ----------------------------------------------------------- 
// FUNCTION  ~PrimeThread                          
//     destruct PrimeThread                            
// PARAMETER USAGE :                                           
//    none                 
// FUNCTION CALLED :                                           
//    none          
// ----------------------------------------------------------- 
PrimeThread::~PrimeThread(){
    delete(channel);
}

// ----------------------------------------------------------- 
// FUNCTION  MasterThread                          
//     initial MasterThread                            
// PARAMETER USAGE :                                           
//    int                 
// FUNCTION CALLED :                                           
//    main          
// ----------------------------------------------------------- 
MasterThread::MasterThread(int number){
    this->number = number;
    UserDefinedThreadID = 0;
}

// ----------------------------------------------------------- 
// FUNCTION  ThreadFunc(MasterThread)                          
//     active of MasterThread                            
// PARAMETER USAGE :                                           
//    none                 
// FUNCTION CALLED :                                           
//    none          
// ----------------------------------------------------------- 
void MasterThread::ThreadFunc(){
    Thread::ThreadFunc();
    print_string("Master starts\n");
    PrimeThread *first_thread;
    first_thread = new PrimeThread(1, 2);
    print_string("  P2 starts and memorizes 2\n");
    first_thread->Begin();
    for(int i=3; i<this->number+1; i++){
        char buf[100];
        sprintf(buf, "Master sends %d to P2\n", i);
        print_string(buf);
        first_thread->channel->Send(&i, sizeof(int));
    }
    int end_of_data = -1;
    print_string("Master sends END\n");
    first_thread->channel->Send(&end_of_data, sizeof(int));
    first_thread->Join();
    delete(first_thread);
    print_string("Master prints the complete result:\n ");
    for(int j=0; prime[j]!=0; j++){
        char tmp[100] = "";
        sprintf(tmp, " %d", prime[j]);
        print_string(tmp);
    }
    print_string("\nMaster terminates\n");
    Exit();
}

// ----------------------------------------------------------- 
// FUNCTION  print_string                          
//     print information                          
// PARAMETER USAGE :                                           
//    char*                   
// FUNCTION CALLED :                                           
//    MasterThread  PrimeThread 
// ----------------------------------------------------------- 
void print_string(char* s){
    m->Lock();
    write(1, s, strlen(s));
    m->Unlock();
}

// ----------------------------------------------------------- 
// FUNCTION  print_by_indentation                          
//     print info with indentation                            
// PARAMETER USAGE :                                           
//    int  char*               
// FUNCTION CALLED :                                           
//    PrimeThread          
// ----------------------------------------------------------- 
void print_by_indentation(int index, char* s){
    char tmp[100] = "";
    for(int i=0; i<index; i++) strcat(tmp, "  ");
    strcat(tmp, s);
    print_string(tmp);
}