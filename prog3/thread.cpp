// ----------------------------------------------------------- 
// NAME : Bill Chang                        User ID: 108598021 
// DUE DATE : 05/04/2020                                      
// PROGRAM ASSIGNMENT #3                                       
// FILE NAME : thread.cpp                                      
// PROGRAM PURPOSE :                                           
//    implement thread.h class                                 
// ----------------------------------------------------------- 

#include "thread.h"

// -----------------------------------------------------------
// FUNCTION  set :                                            
//    set initial value of thread                              
// PARAMETER USAGE :                                           
//    int**, int, int, int                                     
// FUNCTION CALLED :                                           
//    main                                                     
// ----------------------------------------------------------- 
void prefix_sum_thread::set(int** arr, int gap, int num, int count){
    this->arr = arr;
    this->gap = gap;
    this->num = num;
    this->count = count;
}

// ----------------------------------------------------------- 
// FUNCTION  ThreadFunc :                                      
//     run the thread to calculate prefix sum problem          
// PARAMETER USAGE :                                           
//    NONE                                                     
// FUNCTION CALLED :                                          
//    main                                                     
// ----------------------------------------------------------- 
void prefix_sum_thread::ThreadFunc(){
	char buf[200];
    Thread::ThreadFunc();
	sprintf(buf, "     Thread %d Created\n", this->count);
	print_string(buf);
    if(this->count < (int)pow(2.0, (double)this->gap-1)){
        this->arr[this->gap][this->count] = this->arr[this->gap-1][this->count];
		sprintf(buf, "     Thread %d copies x[%d]\n", this->count, this->count);
		print_string(buf);
    }
    else{
        this->arr[this->gap][this->count] = this->arr[this->gap-1][this->count] + this->arr[this->gap-1][this->count-(int)pow(2.0, (double)this->gap-1)];
		sprintf(buf, "     Thread %d computes x[%d] + x[%d]\n", this->count, this->count, (int)((double)this->count-pow(2.0, (double)this->gap-1.0)));
		print_string(buf);
    }
	sprintf(buf, "     Thread %d exits\n\n", this->count);
	print_string(buf);
    Exit();
}

// ----------------------------------------------------------- 
// FUNCTION  print_string :                                    
//    print out the message                                    
// PARAMETER USAGE :                                           
//    char*                                                    
// FUNCTION CALLED :                                           
//    main, prefix_sum_thread.ThreadFunc                       
// ----------------------------------------------------------- 
void print_string(char* s){
    write(1, s, strlen(s));
}
