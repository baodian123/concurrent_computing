// ----------------------------------------------------------- 
// NAME : Bill Chang                         User ID: 108598021 
// DUE DATE : 05/23/2020                                       
// PROGRAM ASSIGNMENT 4                                        
// FILE NAME : thread-support.cpp            
// PROGRAM PURPOSE :                                           
//    Supporting function for baby eagle and mother eagle thread        
// ----------------------------------------------------------- 
#include "thread.h"

// ----------------------------------------------------------- 
// FUNCTION  ready_to_eat                          
//     Simulate baby eagle which want to eat active                            
// PARAMETER USAGE :                                           
//    int               
// FUNCTION CALLED :                                           
//    baby_eagle_t          
// ----------------------------------------------------------- 
void ready_to_eat(int i){
    char buf[100], tmp[100];
    sprintf(tmp, "Baby eagle %d is ready to eat.\n", i);
    use_string(buf, i, tmp);
    print_string(buf);
    waiting_eagle_can_eat_mutex->Wait();
    pot_count_mutex->Wait();
    if(pot_count == 0){
	pot_count_mutex->Signal();
	no_eagle_eating_mutex->Wait();
	sprintf(tmp, "Baby eagle %d sees all feeding pots are empty and wakes up the mother.\n", i);
	use_string(buf, i, tmp);
	print_string(buf);
	wake_eagle = i;
        wake_mutex->Signal();
        food_ready_mutex->Wait();
        pot_count_mutex->Wait();
    }
    pot_count--;
    sprintf(tmp, "Baby eagle %d is eating using feeding pot %d.\n", i, pot_count+1);
    use_string(buf, i, tmp);
    print_string(buf);
    pot_count_mutex->Signal();
    waiting_eagle_can_eat_mutex->Signal();
    eating_eagle_mutex->Wait();
    eating_eagle++;
    eating_eagle_mutex->Signal();
}

// ----------------------------------------------------------- 
// FUNCTION  finish_eating                          
//     Simulate baby eagle which finish eating active                            
// PARAMETER USAGE :                                           
//    int               
// FUNCTION CALLED :                                           
//    baby_eagle_t          
// ----------------------------------------------------------- 
void finish_eating(int i){
    pot_count_mutex->Wait();
    eating_eagle_mutex->Wait();
    char buf[100], tmp[50];
    sprintf(tmp, "Baby eagle %d finishes eating.\n", i);
    use_string(buf, i, tmp);
    print_string(buf);
    eating_eagle--;
    if(eating_eagle == 0 && pot_count <= 0){
        no_eagle_eating_mutex->Signal();
    }
    eating_eagle_mutex->Signal();
    pot_count_mutex->Signal();
}

// ----------------------------------------------------------- 
// FUNCTION  goto_sleep                          
//     Simulate mother eagle go to sleep active                            
// PARAMETER USAGE :                                           
//    int               
// FUNCTION CALLED :                                           
//    mom_eagle_t          
// ----------------------------------------------------------- 
void goto_sleep(int round){
    if(round != 0) print_string("Mother eagle takes a nap.\n");
    wake_mutex->Wait();
}

// ----------------------------------------------------------- 
// FUNCTION  food_ready                          
//     Simulate mother eagle prepared the food active                            
// PARAMETER USAGE :                                           
//    int  int               
// FUNCTION CALLED :                                           
//    mom_eagle_t          
// ----------------------------------------------------------- 
void food_ready(int prepare_food, int round){
    pot_count_mutex->Wait();
    char buf[100];
    sprintf(buf, "Mother eagle is awoke by baby eagle %d and starts preparing food.\n", wake_eagle);
    print_string(buf);
    pot_count = prepare_food;
    sprintf(buf, "Mother eagle says \"Feeding (%d)\"\n", round);
    print_string(buf);
    food_ready_mutex->Signal();
    pot_count_mutex->Signal();
}

// ----------------------------------------------------------- 
// FUNCTION  print_string :                                    
//    print out the message                                    
// PARAMETER USAGE :                                           
//    char*                                                    
// FUNCTION CALLED :                                           
//    main  mom_eagle_t  baby_eagle_t                       
// ----------------------------------------------------------- 
void print_string(char* s){
    write(1, s, strlen(s));
}

// ----------------------------------------------------------- 
// FUNCTION  use_string :                                    
//    make indentation of destination string (des) and 
//    concatenate the other string (s)                                    
// PARAMETER USAGE :                                           
//    char*  int  char*                                                   
// FUNCTION CALLED :                                           
//    baby_eagle_t                       
// ----------------------------------------------------------- 
void use_string(char* des, int i, char* s){
    char space[] = " ";
    sprintf(des, " ");
    for(int k = 0; k < i; k++){
	strcat(des, space);
    }
    strcat(des, s);
}
