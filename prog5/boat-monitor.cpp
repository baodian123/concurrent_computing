#include "boat-monitor.h"
// ----------------------------------------------------------- 
// NAME : Bill Chang                         User ID: 108598021 
// DUE DATE : 06/08/2020                                       
// PROGRAM ASSIGNMENT 5                                        
// FILE NAME : boat-monitor.cpp            
// PROGRAM PURPOSE :                                           
//    Implement method of m_monitor.h        
// ----------------------------------------------------------- 

// ----------------------------------------------------------- 
// FUNCTION  m_monitor                          
//     Constructor of m_monitor                           
// PARAMETER USAGE :                                           
//    none               
// FUNCTION CALLED :                                           
//    none          
// ----------------------------------------------------------- 
m_monitor::m_monitor() : Monitor("", HOARE){
    c = new Condition("c");
    m = new Condition("m");
    left = new Condition("left");
    boat_start = new Condition("boat start");
    for(int i=0; i<3; i++){
        load[i] = -1;
        type[i] = 'x';
    }
}

// ----------------------------------------------------------- 
// FUNCTION  cannibal_arrives                          
//     simulate cannibal arrives the river side                           
// PARAMETER USAGE :                                           
//    int               
// FUNCTION CALLED :                                           
//    cannibal          
// ----------------------------------------------------------- 
void m_monitor::cannibal_arrives(int t_count){
    MonitorBegin();   
    this->c_count++;
    if((c_count > 2 || m_count >= 2) && is_boat_ready){
        load[seat] = t_count;
        type[seat] = 'c';
        seat++;
        int temp = round;
        boat_start->Signal();
        if(temp == round) left->Wait();
    }
    else{
        c->Wait();
        load[seat] = t_count;
        type[seat] = 'c';
        seat++;
        left->Wait();
    }
    MonitorEnd();
}

// ----------------------------------------------------------- 
// FUNCTION  missionary_arrives                          
//     simulate missionary arrives the river side                           
// PARAMETER USAGE :                                           
//    int               
// FUNCTION CALLED :                                           
//    missionary          
// ----------------------------------------------------------- 
void m_monitor::missionary_arrives(int t_count){
    MonitorBegin();
    this->m_count++;
    if((m_count > 2 || (m_count > 1 && c_count > 0)) && is_boat_ready){
        load[seat] = t_count;
        type[seat] = 'm';
        seat++;
        int temp = round;
        boat_start->Signal();
        if(temp == round) left->Wait();
    }
    else if(c_count > 2 && is_boat_ready){
        boat_start->Signal();
        m->Wait();
        load[seat] = t_count;
        type[seat] = 'm';
        seat++;
        left->Wait();
    }
    else{
        m->Wait();
        load[seat] = t_count;
        type[seat] = 'm';
        seat++;
        left->Wait();
    }
    MonitorEnd();
}

// ----------------------------------------------------------- 
// FUNCTION  boat_ready                          
//     simulate boat is ready for the next round                           
// PARAMETER USAGE :                                           
//    none               
// FUNCTION CALLED :                                           
//    boat          
// ----------------------------------------------------------- 
void m_monitor::boat_ready(char *buf, char* pbuf, bool boat_round, int boat_load){
    MonitorBegin();
    if(boat_round){
        lock_stdout->Wait();
        sprintf(buf, "MONITOR: %d crosses have been made.\n", boat_load);
        write(1, buf, strlen(buf));
        sprintf(buf, "MONITOR: This river cross is closed indefinitely for renovation.\n");
        write(1, buf, strlen(buf));
        Exit();
    }
    if(!(m_count > 2 || c_count > 2 || (m_count > 1 && c_count > 0))){
        is_boat_ready = true;
        boat_start->Wait();
    }
    is_boat_ready = false;
    if(m_count > 2 && c_count > 2){
        int ran = rand()%2;
        if(ran == 0){
            m_count -= 3;
            for(int i=0; i<3; i++) m->Signal();
        }
        else{
            c_count -= 3;
            for(int i=0; i<3; i++) c->Signal();
        }
    }
    else if(m_count > 2){
        m_count -= 3;
        for(int i=0; i<3; i++) m->Signal();
        
    }
    else if(c_count > 2){
        c_count -= 3;
        for(int i=0; i<3; i++) c->Signal();
    }
    else{
        m_count -= 2;
        c_count--;
        m->Signal();
        m->Signal();
        c->Signal();
    }
    if(type[0] == 'm' && type[1] == 'm' && type[2] == 'm'){
        sprintf(buf, "MONITOR(%d): three missionaries (%d, %d, %d) are selected\n", round, load[0], load[1], load[2]);
    }
    else if(type[0] == 'c' && type[1] == 'c' && type[2] == 'c'){
        sprintf(buf, "MONITOR(%d): three cannibals (%d, %d, %d) are selected\n", round, load[0], load[1], load[2]);
    }
    else if(type[0] == 'c'){
        sprintf(buf, "MONITOR(%d): one cannibal (%d) and two missionaries (%d, %d) are selected\n", round, load[0], load[1], load[2]);
    }
    else if(type[1] == 'c'){
        sprintf(buf, "MONITOR(%d): one cannibal (%d) and two missionaries (%d, %d) are selected\n", round, load[1], load[0], load[2]);
    }
    else{
        sprintf(buf, "MONITOR(%d): one cannibal (%d) and two missionaries (%d, %d) are selected\n", round, load[2], load[0], load[1]);
    }
    sprintf(pbuf, "***** Boat load (%d): Passenger list (%c%d, %c%d, %c%d)\n", round, type[0], load[0], type[1], load[1], type[2], load[2]);
    MonitorEnd();
}

// ----------------------------------------------------------- 
// FUNCTION  boat_done                          
//     simulate boat is done for the current round                           
// PARAMETER USAGE :                                           
//    none               
// FUNCTION CALLED :                                           
//    boat          
// ----------------------------------------------------------- 
void m_monitor::boat_done(char* buf){
    MonitorBegin();
    seat = 0;
    sprintf(buf, "***** Boat load (%d): Completed\n", round);
    round++;
    while(!left->IsEmpty()) left->Signal();
    MonitorEnd();
}

// ----------------------------------------------------------- 
// FUNCTION  print_line                          
//     print information with indentation                          
// PARAMETER USAGE :                                           
//    int  int  int               
// FUNCTION CALLED :                                           
//    cannibal_arrives  missionary_arrives        
// ----------------------------------------------------------- 
void print_line(int indentation, int state, int role){
    char buf[200] = "", tmp[5];
    for(int i=0; i<indentation; i++) strcat(buf, " ");
    switch(role){
        case 1:
            strcat(buf, "Cannibal ");
            break;
        case 2:
            strcat(buf, "Missionary ");
        default:
            break;
    }
    sprintf(tmp, "%d ", indentation);
    strcat(buf, tmp);
    switch(state){
        case 1:
            strcat(buf, "starts\n");
            break;
        case 2:
            strcat(buf, "arrives\n");
        default:
            break;
    }
    lock_stdout->Wait();
    write(1, buf, strlen(buf));
    lock_stdout->Signal();
}

// ----------------------------------------------------------- 
// FUNCTION  print_string                          
//     print information                          
// PARAMETER USAGE :                                           
//    char*                   
// FUNCTION CALLED :                                           
//    boat_ready  boat_done  ThreadFunc(cannibal, boat, missionary)  
// ----------------------------------------------------------- 
void print_string(char* s){
    lock_stdout->Wait();
    write(1, s, strlen(s));
    lock_stdout->Signal();
}