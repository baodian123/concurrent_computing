// ----------------------------------------------------------- 
// NAME : Bill Chang                         User ID: 108598021 
// DUE DATE : 05/23/2020                                       
// PROGRAM ASSIGNMENT 4                                        
// FILE NAME : thread-main.cpp            
// PROGRAM PURPOSE :                                           
//    Simulate the hungry eagle problem using semaphore        
// ----------------------------------------------------------- 
#include "thread.h"

int wake_eagle;
int pot_count;
int eating_eagle;
Semaphore* pot_count_mutex;
Semaphore* eating_eagle_mutex;
Semaphore* wake_mutex;
Semaphore* no_eagle_eating_mutex;
Semaphore* food_ready_mutex;
Semaphore* waiting_eagle_can_eat_mutex;

int main(int argc, char** argv){
    if(argc != 4) return -1;
    srand(time(NULL));
    wake_eagle = -1;
    pot_count = 0;
    eating_eagle = 0;;
    pot_count_mutex = new Semaphore("", 1);
    eating_eagle_mutex = new Semaphore("", 1);
    wake_mutex = new Semaphore("", 0);
    no_eagle_eating_mutex = new Semaphore("", 1);
    food_ready_mutex = new Semaphore("", 0);
    waiting_eagle_can_eat_mutex = new Semaphore("", 1);

    int baby_eagles = atoi(argv[2]);
    if(baby_eagles == 0) baby_eagles = 10;

    int prepare_food = atoi(argv[1]);
    if(prepare_food == 0) prepare_food = 10;

    int round = atoi(argv[3]);
    if(round == 0) round = 10;

    mom_eagle_t met;
    met.prepare_food = prepare_food;
    met.round = round;
    met.total = round;

    baby_eagle_t bet[baby_eagles];

    char buf[100];
    sprintf(buf, "MAIN: There are %d baby eagles, %d feeding pots, and %d feedings.\n", baby_eagles, prepare_food, round);
    print_string(buf);
    print_string("MAIN: Game starts!!!!!\n");
    print_string("Mother eagle started.\n");
    met.Begin();
    for(int i = 0; i < baby_eagles; i++){
	char tmp[50];
	sprintf(tmp, "Baby eagle %d started.\n", i);
	use_string(buf, i, tmp);
	print_string(buf);
	bet[i].i = i;
	bet[i].Begin();
    }

    met.Join();
    sprintf(buf, "Mother eagle retires after serving %d feedings. Game ends!!!\n", round);
    print_string(buf);

    pot_count_mutex->~Semaphore();
    eating_eagle_mutex->~Semaphore();
    wake_mutex->~Semaphore();
    no_eagle_eating_mutex->~Semaphore();
    food_ready_mutex->~Semaphore();
    waiting_eagle_can_eat_mutex->~Semaphore();

    Exit();
}

