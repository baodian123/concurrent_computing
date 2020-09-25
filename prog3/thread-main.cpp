// ----------------------------------------------------------- 
// NAME : Bill Chang                        User ID: 108598021 
// DUE DATE : 05/04/2020                                       
// PROGRAM ASSIGNMENT #3                                       
// FILE NAME : thread-main.cpp                                 
// PROGRAM PURPOSE :                                           
//    simulate prefix sum problem and run concurrently         
// ----------------------------------------------------------- 

#include "thread.h"

// ----------------------------------------------------------- 
// FUNCTION  main :                                            
//     run thread to simulate prefix sum problem               
// PARAMETER USAGE :                                           
//    NONE                                                     
// FUNCTION CALLED :                                           
//    NONE                                                    
// -----------------------------------------------------------
int main(){
    int i, j, num, gap = 1;
    char buf[300], cat_buf[10];
    cin >> num;
    int temp = num;
    while((temp >>= 1) != 1) gap++;
    int **arr = new int*[gap+1];
    for(i=0; i<gap+1; i++) arr[i] = new int[num];
    for(i=0; i<num; i++) cin >> arr[0][i];
    sprintf(buf, "Concurrent Prefix Sum Computation\n\nNumber of input data = %d\nInput array:\n", num);
	for(i=0; i<num; i++){
		sprintf(cat_buf, "%4d", arr[0][i]);
		strcat(buf, cat_buf);
	}
	strcat(buf, "\n\n");
    print_string(buf);
    prefix_sum_thread pst[num];
    for(i=1; i<gap+1; i++){
		sprintf(buf, "Run %d:\n", i);
		print_string(buf);
        for(j=0; j<num; j++){
            pst[j].set(arr, i, num, j);
            pst[j].Begin();
        }
        for(j=0; j<num; j++) pst[j].Join();
		sprintf(buf, "Result after run %d:\n", i);
		print_string(buf);
		for(j=0; j<num; j++){
			sprintf(buf, "%4d", arr[i][j]);
			print_string(buf);
		}
		print_string("\n\n");
    }
    sprintf(buf, "Final result after run %d:\n", gap);
    for(i=0; i<num; i++){
	sprintf(cat_buf, "%4d", arr[gap][i]);
	strcat(buf, cat_buf);
    }
    strcat(buf, "\n");
    print_string(buf);

    return 0;
}
