#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<string.h>

#define SWAP(a, b) do {a ^= b; b ^= a; a ^= b; } while(0)

/* ----------------------------------------------------------- */
/* FUNCTION  binary_search :                                   */
/*     use binary search find the x[i]'s index in final result */
/* PARAMETER USAGE :                                           */
/*    int*, int, int, int                                      */
/* FUNCTION CALLED :                                           */
/*    binary_merge                                             */
/* ----------------------------------------------------------- */
int binary_search(int* info, int i, int l, int r);

/* ----------------------------------------------------------- */
/* FUNCTION  print_string :                                    */
/*     print out the message                                   */
/* PARAMETER USAGE :                                           */
/*    char*                                                    */
/* FUNCTION CALLED :                                           */
/*    main , binary_merge                                      */
/* ----------------------------------------------------------- */
void print_string(char* s);

/* ----------------------------------------------------------- */
/* FUNCTION  print_err :                                       */
/*     print error message and exit with value -1              */
/* PARAMETER USAGE :                                           */
/*    char*                                                    */
/* FUNCTION CALLED :                                           */
/*    main , binary_merge                                      */
/* ----------------------------------------------------------- */
void print_err(char*);

/* ----------------------------------------------------------- */
/* FUNCTION  binary_merge :                                    */
/*     attach temp array(from parent), and read origin array   */
/*     store the value to temp(sorted) then detach both array  */
/* PARAMETER USAGE :                                           */
/*    int , int , int , char*, int, int                        */
/* FUNCTION CALLED :                                           */
/*    main                                                     */
/* ----------------------------------------------------------- */
void binary_merge(int m_key, int b_key, int range, char* llindex, int i, int position);

/* ----------------------------------------------------------- */
/* FUNCTION  main :                                            */
/*     simulate merge sort and run in concurrent way           */
/* PARAMETER USAGE :                                           */
/*    int , char**                                             */
/* FUNCTION CALLED :                                           */
/*    main(recursion), main.c                                  */
/* ----------------------------------------------------------- */
int main(int argc, char** argv){
    int range = atoi(argv[2]) - atoi(argv[1]);
    if(range == 1){
        /* last split */
        int shm_id = shmget(atoi(argv[3]), sizeof(int)*(range+1)*2, 0666);
        int *arr, i;
        arr = (int*)shmat(shm_id, NULL, 0);
        if(shm_id == -1 || arr == (void*)-1){
            perror("Last split: shared memory allocate fail");
            return EXIT_FAILURE;
        }
        if(strcmp("l", argv[4]) == 0)
            if(arr[0] > arr[1])
                SWAP(arr[0], arr[1]);
        if(strcmp("r", argv[4]) == 0)
            if(arr[2] > arr[3])
                SWAP(arr[2], arr[3]);
        char s[1000], s1[100];
        sprintf(s, "   ### M-PROC(%d) created by M-PROC(%d): entering with a[%s..%s] -- sorted   \n", getpid(), getppid(), argv[1], argv[2]);
        if(strcmp("l", argv[4]) == 0) sprintf(s1, "%4d%4d <------------------------------ sorted elements of a[%s] to a[%s]\n       ..........\n", arr[0], arr[1], argv[1], argv[2]);
        if(strcmp("r", argv[4]) == 0) sprintf(s1, "%4d%4d <------------------------------ sorted elements of a[%s] to a[%s]\n       ..........\n", arr[2], arr[3], argv[1], argv[2]);
        strcat(s, s1);
        print_string(s);
        shmdt(arr);
        return EXIT_SUCCESS;
    }
    else if(range > 1){
        /* need split */
        char s[10000];
        int middle = (atoi(argv[1]) + atoi(argv[2]))/2;
        int shm_id = shmget(atoi(argv[3]), sizeof(int)*(range+1)*2, 0666); /* origin */
        int *arr;
        arr = (int*)shmat(shm_id, NULL, 0);
        if(shm_id == -1 || arr == (void*)-1){
            perror("Split: parent shared memory allocate fail");
            return EXIT_FAILURE;
        }
        key_t m_key = ftok("./", middle);
        int m_shm_id = shmget(m_key, sizeof(int)*(range+1), IPC_CREAT | 0666); /* new */
        int *m_arr, i, j=0;
        m_arr = (int*)shmat(m_shm_id, NULL, 0);
        if(m_shm_id == -1 || m_arr == (void*)-1){
            perror("Split: self shared memory allocate fail");
            return EXIT_FAILURE;
        }
        sprintf(s, "\n*** MERGE: shared memory key = %d\n*** MERGE: shared memory created\n*** MERGE: shared memory attached and is ready to use for copy array from parent purpose.\n\n", m_key);
        print_string(s);
        if(strcmp(argv[4], "l") == 0) for(i=0; i<range+1; i++) m_arr[i] = arr[i];
        else for(i=range+1; i<(range+1)*2; i++) m_arr[i-range-1] = arr[i];

        char r_left_index[10], r_right_index[10], l_left_index[10], l_right_index[10], n_key[20];
        sprintf(l_left_index, "%d", atoi(argv[1]));
        sprintf(l_right_index, "%d", middle);
        sprintf(r_left_index, "%d", middle+1);
        sprintf(r_right_index, "%d", atoi(argv[2]));
        sprintf(n_key, "%d", m_key);
        char *p_left[] = {"./merge", l_left_index, l_right_index, n_key, "l", "n", NULL};
        char *p_right[] = {"./merge", r_left_index, r_right_index, n_key, "r", "n", NULL};
        pid_t lpid;
        if((lpid = fork()) < 0) print_err("fork lprocess fail!\n"); /* fork left */
        else if(lpid == 0){
            char s[10000], s1[1000];
            sprintf(s, "   ### M-PROC(%d) created by M-PROC(%d): entering with a[%s..%s]\n   ", getpid(), getppid(), p_left[1], p_left[2]);
            for(i=0; i<range/2+1; i++){
                sprintf(s1, "%4d", m_arr[i]);
                strcat(s, s1);
            }
            sprintf(s1, "  <---------------------- elements of a[%s] to a[%s]\n", p_left[1], p_left[2]);
            strcat(s, s1);
            sprintf(s1, "       ..........\n");
            strcat(s, s1);
            print_string(s);
            execvp(p_left[0], p_left);
        }
        else{
            pid_t rpid;
            if((rpid = fork()) < 0) print_err("fork rprocess fail!\n"); /* fork right */
            else if(rpid == 0){
                char s[10000], s1[1000];
                sprintf(s, "   ### M-PROC(%d) created by M-PROC(%d): entering with a[%s..%s]\n   ", getpid(), getppid(), p_right[1], p_right[2]);
                for(i=range/2+1; i<range+1; i++){
                    sprintf(s1, "%4d", m_arr[i]);
                    strcat(s, s1);
                }
                sprintf(s1, "  <---------------------- elements of a[%s] to a[%s]\n", p_right[1], p_right[2]);
                strcat(s, s1);
                sprintf(s1, "       ..........\n");
                strcat(s, s1);
                print_string(s);
                execvp(p_right[0], p_right);
            }
            else{
                int i=2;
                char s[10000], s1[1000];
                while(i-- && range > 3){
                    int id = waitpid(-1, NULL, 0);
                    sprintf(s, "   ### M-PROC(%d) created by M-PROC(%d): both array section sorted. start merging\n       ..........\n", id, getpid());
                    print_string(s);
                }
                for(i=0; i<2; i++) wait(NULL);
                if(strcmp(argv[4], "l") == 0) for(i=0; i<range+1; i++) arr[i] = m_arr[i];
                else for(i=range+1; i<(range+1)*2; i++) arr[i] = m_arr[i-range-1];
                /* do binary merge */
                key_t b_key = ftok("../..", atoi(argv[1])+atoi(argv[2]));
                int b_shm_id = shmget(b_key, sizeof(int)*(range+1), IPC_CREAT | 0666); /* generate temp array */
                int *temp;
                temp = (int*)shmat(b_shm_id, NULL, 0);
                if(b_shm_id == -1 || temp == (void*)-1){
                    perror("Merge: shared memory allocate fail");
                    return EXIT_FAILURE;
                }
                sprintf(s, "\n*** MERGE: shared memory key = %d\n*** MERGE: shared memory created\n*** MERGE: shared memory attached and is ready to use for temporarily store the sorted array purpose.\n\n", b_key);
                print_string(s);
                pid_t pid;
                for(i=0; i<range+1; i++){
                    if((pid = fork()) < 0) print_err("fork lbinary process fail.\n");
                    else if(pid == 0){
                        if(i > range/2) binary_merge(m_key, b_key, range, l_left_index, i, 1); /* right side */
                        else binary_merge(m_key, b_key, range, l_left_index, i, 0); /* left side */
                        exit(0);
                    }
                }
                for(i=0; i<range+1; i++) wait(NULL);
                if(strcmp(argv[4], "l") == 0) for(i=0; i<range+1; i++) arr[i] = temp[i];
                else for(i=range+1; i<(range+1)*2; i++) arr[i] = temp[i-range-1];
                if(strcmp("f", argv[5]) == 0) sprintf(s, "   ### M-PROC(%d): entering with a[%s..%s] completed:\n   ", getpid(), argv[1], argv[2]);
                else sprintf(s, "   ### M-PROC(%d) created by M-PROC(%d): merge sort a[%s..%s] completed:\n   ", getpid(), getppid(), argv[1], argv[2]);
                for(i=0; i<range+1; i++){
                    if(strcmp(argv[4], "l") == 0){
                        sprintf(s1, "%4d", arr[i]);
                        strcat(s, s1);
                    }
                    else{
                        sprintf(s1, "%4d", arr[i+range+1]);
                        strcat(s, s1);
                    }
                }
                sprintf(s1, "\n       ..........\n");
                strcat(s, s1);
                print_string(s);
                shmdt(temp);
                shmctl(b_shm_id, IPC_RMID, NULL);
                sprintf(s, "\n*** MERGE: temporarily store array shared memory successfully detached\n*** MERGE: temporarily store array shared memory successfully removed\n\n");
                print_string(s);
            }
        }
        shmdt(m_arr);
        shmctl(m_shm_id, IPC_RMID, NULL);
        sprintf(s, "\n*** MERGE: copy array from parent shared memory successfully detached\n*** MERGE: copy array from parent shared memory successfully removed\n\n");
        print_string(s);
        shmdt(arr);
    }

    return EXIT_SUCCESS;
}

void print_err(char* s){
    write(1, s, strlen(s));
    exit(-1);
}

void print_string(char* s){
    write(1, s, strlen(s));
}

void binary_merge(int m_key, int b_key, int range, char* llindex, int i, int position){
    int m_shm_id = shmget(m_key, sizeof(int)*(range+1), 0666); /* get parent array */
    int b_shm_id = shmget(b_key, sizeof(int)*(range+1), 0666); /* generate temp array */
    int *info, *btemp;
    info = (int*)shmat(m_shm_id, NULL, 0); /* read info */
    btemp = (int*)shmat(b_shm_id, NULL, 0); /* write to temp */
    if(btemp == (void*)-1 || info == (void*)-1){
        perror("Merge: shared memory attach fail");
        return ;
    }
    char s[2000];
    sprintf(s, "      $$$ B-PROC(%d): created by M-PROC(%d) for a[%d] = %d is created\n       ..........\n", getpid(), getppid(), i+atoi(llindex), info[i]);
    print_string(s);
    int ll = 0;
    int rr = range;
    int lr = range/2;
    int rl = range/2+1;
    if(position == 0){ /* do left merge back */
        if(info[i] < info[rl]){
            sprintf(s, "      $$$ B-PROC(%d): a[%d] = %d is smaller than a[%d] = %d and is written to temp[%d]\n       ..........\n", getpid(), i+atoi(llindex), info[i], rl+atoi(llindex), info[rl], i);
            print_string(s);
            btemp[i] = info[i];
        }
        else if(info[i] > info[rr]){
            sprintf(s, "      $$$ B-PROC(%d): a[%d] = %d is larger than a[%d] = %d and is written to temp[%d]\n       ..........\n", getpid(), i+atoi(llindex), info[i], rr+atoi(llindex), info[rr], i+rr-rl+1);
            print_string(s);
            btemp[i+rr-rl+1] = info[i];
        }
        else{
            int count = binary_search(info, i, rl, rr);
            sprintf(s, "      $$$ B-PROC(%d): a[%d] = %d is between a[%d] = %d and a[%d] = %d and is written to temp[%d]\n       ..........\n", getpid(), i+atoi(llindex), info[i], lr+atoi(llindex)+count, info[lr+atoi(llindex)+count], lr+atoi(llindex)+count+1, info[lr+atoi(llindex)+count+1], i+count);
            print_string(s);
            btemp[i+count] = info[i];
        }
    }
    else if(position == 1){ /* do right merge back */
        if(info[i] < info[ll]){
            sprintf(s, "      $$$ B-PROC(%d): a[%d] = %d is smaller than a[%d] = %d and is written to temp[%d]\n       ..........\n", getpid(), i+atoi(llindex), info[i], ll+atoi(llindex), info[ll], i-rl+ll);
            print_string(s);
            btemp[i-rl+ll] = info[i];
        }
        else if(info[i] > info[lr]){
            sprintf(s, "      $$$ B-PROC(%d): a[%d] = %d is larger than a[%d] = %d and is written to temp[%d]\n       ..........\n", getpid(), i+atoi(llindex), info[i], lr+atoi(llindex), info[lr], i);
            print_string(s);
            btemp[i] = info[i];
        }
        else{
            int count = binary_search(info, i, ll, lr);
            sprintf(s, "      $$$ B-PROC(%d): a[%d] = %d is between a[%d] = %d and a[%d] = %d and is written to temp[%d]\n       ..........\n", getpid(), i+atoi(llindex), info[i], ll+atoi(llindex)+count-1, info[ll+count-1], ll+atoi(llindex)+count, info[ll+count], i-rl+ll+count);
            print_string(s);
            btemp[i-rl+ll+count] = info[i];
        }
    }
    else print_err("wrong position.\n");
    sprintf(s, "      $$$ B-PROC(%d): created by M-PROC(%d) for a[%d] = %d is terminated\n       ..........\n", getpid(), getppid(), i+atoi(llindex), info[i]);
    print_string(s);
    shmdt(info);
    shmdt(btemp);
}

int binary_search(int* info, int i, int l, int r){
    int count = 0;
    int ol = l;

    while(r>=l){
        int middle = l + (r-l)/2;
        if(info[i] > info[middle] && info[i] < info[middle+1]){
            count = middle-ol+1;
            break;
        }
        else if(info[i] < info[middle]) r = middle;
        else if(info[i] > info[middle]) l = middle+1;
    }

    return count;
}