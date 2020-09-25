#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<string.h>

/* ----------------------------------------------------------- */
/* FUNCTION  print_string :                                    */
/*     print out the message                                   */
/* PARAMETER USAGE :                                           */
/*    char*                                                    */
/* FUNCTION CALLED :                                           */
/*    main                                                     */
/* ----------------------------------------------------------- */
void print_string(char* s);

/* ----------------------------------------------------------- */
/* FUNCTION  print_err :                                       */
/*     print error message and exit with value -1              */
/* PARAMETER USAGE :                                           */
/*    char*                                                    */
/* FUNCTION CALLED :                                           */
/*    main                                                     */
/* ----------------------------------------------------------- */
void print_err(char*);

/* ----------------------------------------------------------- */
/* FUNCTION  print_err :                                       */
/*     fork a child process to simulate merge sort             */
/*     wait for it, and print out the result                   */
/* PARAMETER USAGE :                                           */
/*    int, char**                                              */
/* FUNCTION CALLED :                                           */
/*    NONE                                                     */
/* ----------------------------------------------------------- */
int main(int argc, char** argv){
    char t_num[10], s[500];
    fgets(t_num, 10, stdin);
    print_string("Merge Sort with Multiple Processes:\n\n");
    key_t key = ftok("./", 'x');
    sprintf(s, "*** MAIN: shared memory key = %d\n", key);
    print_string(s);
    int shm_id = shmget(key, sizeof(int)*atoi(t_num)*2, IPC_CREAT | 0666);
    print_string("*** MAIN: shared memory created\n");
    int *arr, i;
    arr = (int*)shmat(shm_id, NULL, 0);
    if(shm_id == -1 || arr == (void*)-1){
        perror("Shared memory allocate fail");
        return EXIT_FAILURE;
    }
    print_string("*** MAIN: shared memory attached and is ready to use\n");
    sprintf(s, "\nInput array for mergesort has %d elements:\n", atoi(t_num));
    print_string(s);
    for(i=0; i<atoi(t_num); i++) scanf("%d", &arr[i]);
    for(i=0; i<atoi(t_num); i++){
        sprintf(s, "   %d", arr[i]);
        print_string(s);
    }
    print_string("\n\n*** MAIN: about to spawn the merge sort process\n");
    pid_t pid;
    if((pid = fork()) < 0) print_err("fork process fail!\n");
    else if(pid == 0){
        char left[2], right[10], skey[20];
        sprintf(s, "   ### M-PROC(%d): entering with a[0..%d]\n   ", getpid(), atoi(t_num)-1);
        print_string(s);
        for(i=0; i<atoi(t_num); i++){
            sprintf(s, "   %d", arr[i]);
            print_string(s);
        }
        sprintf(s, " <------- elements of a[0] to a[%d]\n", atoi(t_num)-1);
        print_string(s);
        print_string("       ..........\n");
        sprintf(left, "0");
        sprintf(right, "%d", atoi(t_num)-1);
        sprintf(skey, "%d", key);
        char *s[] = {"./merge", left, right, skey, "l", "f", NULL};
        execvp(s[0], s);
    }
    else{
        wait(NULL);
        char res[10];
        print_string("*** MAIN: merged array:\n   ");
        for(i=0; i<atoi(t_num); i++){
            sprintf(res, "%4d", arr[i]);
            print_string(res);
        } 
        print_string("\n\n");
        shmdt(arr);
        print_string("*** MAIN: shared memory successfully detached\n");
        shmctl(shm_id, IPC_RMID, NULL);
        print_string("*** MAIN: shared memory successfully removed\n");
    }
    print_string("*** MAIN: exits\n");
    return EXIT_SUCCESS;
}

void print_err(char* s){
    write(1, s, strlen(s));
    exit(-1);
}

void print_string(char* s){
    write(1, s, strlen(s));
}