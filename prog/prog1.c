/* ----------------------------------------------------------- */
/* NAME : Bill Chang                        User ID: 108598021 */
/* DUE DATE : 03/30/2020                                       */
/* PROGRAM ASSIGNMENT #1                                       */
/* FILE NAME : prog1.c                                         */
/* PROGRAM PURPOSE :                                           */
/*    Fork four processes list below and run them concurrently */
/*   -fabonacci-  -buffon needle-  -ellipse area-  -pinball-   */
/* ----------------------------------------------------------- */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<time.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

/* all child process num */
#define PROCESS_NUM 4

/* ----------------------------------------------------------- */
/* FUNCTION  print_err :                                       */
/*     print error message and exit with value -1              */
/* PARAMETER USAGE :                                           */
/*    char*                                                    */
/* FUNCTION CALLED :                                           */
/*    NONE                                                     */
/* ----------------------------------------------------------- */
void print_err(char* s);

/* ----------------------------------------------------------- */
/* FUNCTION  print_string :                                    */
/*     print out the message                                   */
/* PARAMETER USAGE :                                           */
/*    char*                                                    */
/* FUNCTION CALLED :                                           */
/*    NONE                                                     */
/* ----------------------------------------------------------- */
void print_string(char* s);

/* ----------------------------------------------------------- */
/* FUNCTION  fork_fabonacci_process :                          */
/*     fork fabonacci process                                  */
/* PARAMETER USAGE :                                           */
/*    void(*func_ptr)(int), int                                */
/* FUNCTION CALLED :                                           */
/*    print_err, fabonacci_func                                */
/* ----------------------------------------------------------- */
void fork_fabonacci_process(void(*func_ptr)(int), int);

/* ----------------------------------------------------------- */
/* FUNCTION  fabonacci_func :                                  */
/*     simulate fabonacci calculate                            */
/* PARAMETER USAGE :                                           */
/*     int                                                     */
/* FUNCTION CALLED :                                           */
/*    print_string, cal_fabonacci                              */
/* ----------------------------------------------------------- */
void fabonacci_func(int);

/* ----------------------------------------------------------- */
/* FUNCTION  cal_fabonacci :                                   */
/*     calculate fabonacci number and return the value         */
/* PARAMETER USAGE :                                           */
/*     int                                                     */
/* FUNCTION CALLED :                                           */
/*    cal_fabonacci                                            */
/* ----------------------------------------------------------- */
long cal_fabonacci(int);

/* ----------------------------------------------------------- */
/* FUNCTION  fork_buffon_needle_process :                      */
/*     fork buffon needle process                              */
/* PARAMETER USAGE :                                           */
/*    void(*func_ptr)(int), int                                */
/* FUNCTION CALLED :                                           */
/*    print_err, buffon_needle_func                            */
/* ----------------------------------------------------------- */
void fork_buffon_needle_process(void(*func_ptr)(int), int);

/* ----------------------------------------------------------- */
/* FUNCTION  buffon_needle_func :                              */
/*     simulate buffon needle problem                          */
/* PARAMETER USAGE :                                           */
/*     int                                                     */
/* FUNCTION CALLED :                                           */
/*    print_string, throw_needle                               */
/* ----------------------------------------------------------- */
void buffon_needle_func(int);

/* ----------------------------------------------------------- */
/* FUNCTION  buffon_needle_func :                              */
/*     simulate throw buffon needle and return whether needle  */
/*     is crossing the line or not                             */
/* PARAMETER USAGE :                                           */
/*     NULL                                                    */
/* FUNCTION CALLED :                                           */
/*     NULL                                                    */
/* ----------------------------------------------------------- */
int throw_needle();

/* ----------------------------------------------------------- */
/* FUNCTION  fork_ellipse_area_process :                       */
/*     fork ellipse area process                               */
/* PARAMETER USAGE :                                           */
/*    void(*func_ptr)(int, int, int), int, int, int            */
/* FUNCTION CALLED :                                           */
/*    print_err, ellipse_area_func                             */
/* ----------------------------------------------------------- */
void fork_ellipse_area_process(void(*func_ptr)(int, int, int), int, int, int);

/* ----------------------------------------------------------- */
/* FUNCTION  ellipse_area_func :                               */
/*     simulate the probability question about the ellipse     */
/* PARAMETER USAGE :                                           */
/*     int, int, int                                           */
/* FUNCTION CALLED :                                           */
/*    print_string, point_in_ellipse                           */
/* ----------------------------------------------------------- */
void ellipse_area_func(int, int, int);

/* ----------------------------------------------------------- */
/* FUNCTION  point_in_ellipse :                                */
/*     random a point and return whether this point is in      */
/*     ellipse area or not                                     */
/* PARAMETER USAGE :                                           */
/*     int, int, int                                           */
/* FUNCTION CALLED :                                           */
/*     NULL                                                    */
/* ----------------------------------------------------------- */
int point_in_ellipse(int, int, int);

/* ----------------------------------------------------------- */
/* FUNCTION  fork_simple_pinball_process :                     */
/*     fork simple pinball process                             */
/* PARAMETER USAGE :                                           */
/*    void(*func_ptr)(int, int), int, int                      */
/* FUNCTION CALLED :                                           */
/*    print_err, simple_pinball_func                           */
/* ----------------------------------------------------------- */
void fork_simple_pinball_process(void(*func_ptr)(int, int), int, int);

/* ----------------------------------------------------------- */
/* FUNCTION  simple_pinball_func :                             */
/*     simulate the probability question about the pinball drop*/
/* PARAMETER USAGE :                                           */
/*     int, int                                                */
/* FUNCTION CALLED :                                           */
/*    print_string, cal_pinball                                */
/* ----------------------------------------------------------- */
void simple_pinball_func(int, int);

/* ----------------------------------------------------------- */
/* FUNCTION  cal_pinball :                                     */
/*     simulate the pinball drop in the bin and return value   */
/*     of each bin                                             */
/* PARAMETER USAGE :                                           */
/*     int*, int, int                                          */
/* FUNCTION CALLED :                                           */
/*     NULL                                                    */
/* ----------------------------------------------------------- */
void cal_pinball(int*, int, int);

/* ----------------------------------------------------------- */
/* FUNCTION  print_info :                                      */
/*     print out the argument info                             */
/* PARAMETER USAGE :                                           */
/*    char**                                                   */
/* FUNCTION CALLED :                                           */
/*    print_string                                             */
/* ----------------------------------------------------------- */
void print_info(char**);

int main(int argc, char** argv){
    if(argc != 8) print_err("wrong input!\n");
    print_info(argv);
    void(*run_fabonacci)(int) = fabonacci_func;
    void(*run_buffon_needle)(int) = buffon_needle_func;
    void(*run_ellipse_area)(int, int, int) = ellipse_area_func;
    void(*run_simple_pinball)(int, int) = simple_pinball_func;
    srand(time(NULL));
    print_string("Fibonacci Process Created\n");
    fork_fabonacci_process(run_fabonacci, atoi(argv[1]));
    print_string("Buffon's Needle Process Created\n");
    fork_buffon_needle_process(run_buffon_needle, atoi(argv[2]));
    print_string("Ellipse Area Process Created\n");
    fork_ellipse_area_process(run_ellipse_area, atoi(argv[3]), atoi(argv[4]), atoi(argv[5]));
    print_string("Pinball Process Created\n");
    fork_simple_pinball_process(run_simple_pinball, atoi(argv[6]), atoi(argv[7]));
    
    int i;
    print_string("Main Process Waits\n");
    for(i = 0; i < PROCESS_NUM; i++)
        wait(NULL);
    print_string("Main Process Exits\n");
    return EXIT_SUCCESS;
}

void print_info(char** s){
    print_string("Main Process Started\n");
    char out[100];
    sprintf(out, "Fibonacci Input            = %d\n", atoi(s[1]));
    print_string(out);
    sprintf(out, "Buffon's Needle Iterations = %d\n", atoi(s[2]));
    print_string(out);
    sprintf(out, "Total random Number Pairs  = %d\n", atoi(s[5]));
    print_string(out);
    sprintf(out, "Semi-Major Axis Length     = %d\n", atoi(s[3]));
    print_string(out);
    sprintf(out, "Semi-Minor Axis Length     = %d\n", atoi(s[4]));
    print_string(out);
    sprintf(out, "Number of Bins             = %d\n", atoi(s[6]));
    print_string(out);
    sprintf(out, "Number of Ball Droppings   = %d\n\n", atoi(s[7]));
    print_string(out);
}

void print_err(char* s){
    write(1, s, strlen(s));
    exit(-1);
}

void print_string(char* s){
    write(1, s, strlen(s));
}

void fork_fabonacci_process(void(*func_ptr)(int), int n){
    pid_t pid;
    int status;
    if( (pid = fork()) < 0) print_err("fork fabonacci process fail!\n");
    else if(pid == 0) func_ptr(n);
}

void fabonacci_func(int n){
    print_string("   Fibonacci Process Started\n");
    char out[100];
    sprintf(out, "   Input Number %d\n", n);
    print_string(out);
    long result = cal_fabonacci(n);
    sprintf(out, "   Fibonacci Number f(%d) is %ld\n", n, result);
    print_string(out);
    print_string("   Fibonacci Process Exits\n");
    exit(0);
}

long cal_fabonacci(int n){
    if(n == 1 || n == 2) return 1;
    else return cal_fabonacci(n-1) + cal_fabonacci(n-2);
}

void fork_buffon_needle_process(void(*func_ptr)(int), int r){
    pid_t pid;
    int status;
    if( (pid = fork()) < 0) print_err("fork buffon needle process fail!\n");
    else if(pid == 0) func_ptr(r);
}

void buffon_needle_func(int r){
    print_string("      Buffon's Needle Process Started\n");
    char out[100];
    sprintf(out, "      Input Number %d\n", r);
    print_string(out);
    int i, t = 0;
    for(i = 0; i < r; i++)
        t += throw_needle();
    sprintf(out, "      Estimated Probability is %.5f\n", (float)t/(float)r);
    print_string(out);
    print_string("      Buffon's Needle Process Exits\n");
    exit(0);
}

int throw_needle(){
    float d = (float)rand()/RAND_MAX;
    float a = (float)rand()/RAND_MAX*(2.0*(acos(-1.0)));
    return ((d + sin(a)) < 0) || (d + sin(a) > 1);
}

void fork_ellipse_area_process(void(*func_ptr)(int, int, int), int a, int b, int s){
    pid_t pid;
    int status;
    if( (pid = fork()) < 0) print_err("fork ellipse area process fail!\n");
    else if(pid == 0) func_ptr(a, b, s);
}

void ellipse_area_func(int a, int b, int s){
    print_string("         Ellipse Area Process Started\n");
    int i, t = 0;
    float area, res;
    char out[100];
    for(i = 0; i < s; i++)
        t += point_in_ellipse(a, b, s);
    res = (float)t / (float)s * (float)(4*a*b);
    area = acos(-1.0)*a*b;
    
    sprintf(out, "         Total random Number Pairs %d\n", s);
    print_string(out);
    sprintf(out, "         Semi-Major Axis Length %d\n", a);
    print_string(out);
    sprintf(out, "         Semi-Minor Axis Length %d\n", b);
    print_string(out);
    sprintf(out, "         Total Hits %d\n", t);
    print_string(out);
    sprintf(out, "         Estimated Area is %.5f\n", res);
    print_string(out);
    sprintf(out, "         Actual Area is %.5f\n", area);
    print_string(out);
    print_string("         Ellipse Area Process Exits\n");
    exit(0);
}

int point_in_ellipse(int a, int b, int s){
    float x = (float)rand()/RAND_MAX*(float)a;
    float y = (float)rand()/RAND_MAX*(float)b;

    return (x*x / (float)(a*a) + y*y / (float)(b*b)) <= 1;
}

void fork_simple_pinball_process(void(*func_ptr)(int, int), int x, int y){
    pid_t pid;
    int status;
    if( (pid = fork()) < 0) print_err("fork simple pinball process fail!\n");
    else if(pid == 0) func_ptr(x, y);
}

void simple_pinball_func(int x, int y){
    print_string("Simple Pinball Process Started\n");
    char out[100];
    sprintf(out, "Number of Bins %d\n", x);
    print_string(out);
    sprintf(out, "Number of Ball Droppings %d\n", y);
    print_string(out);
    int bin[x], i, j;
    for(i = 0; i < x; i++) bin[i] = 0;
    cal_pinball(bin, x, y);
    int max = bin[0];
    for(i = 1; i < x; i++) if(bin[i] > max) max = bin[i];
    for(i = 0; i < x; i++){
        sprintf(out, "%3d-", i+1);
        print_string(out);
        sprintf(out, "(%7d)-", bin[i]);
        print_string(out);
        sprintf(out, "(%5.2f%c)|", 100.0*(double)(bin[i])/(double)y, 0x25);
        print_string(out);
        int star = bin[i] * 50 / max;
        for(j = 0; j < star; j++) print_string("*");
        print_string("\n");
    }
    print_string("Simple Pinball Process Exits\n");
    exit(0);
}

void cal_pinball(int* bin, int x, int y){
    int i, j;
    for(i = 0; i < y; i++){
        int ball = 0;
        for(j = 0; j < (x - 1); j++){
            ball += rand() % 2;
        }
        bin[ball] += 1;
    }
}