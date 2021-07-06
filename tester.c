/* execl("./tester", "tester", "solution", testdir ,  (char*) NULL); */

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <error.h>
#include <time.h>
#include <sys/time.h>
#include <stdio_ext.h>
#include <stdint.h>

void handler(int sig);
int compare(const void *a, const void *b);
int pipes_1[2];
int pipes_2[2];
char R_ERROR[10] = "RUNERROR\n";


int main(int argc, char **argv)
{
    signal(SIGALRM, handler);
    char total[10];
    char min[10];
    char max[10];
    
    char* stime = argv[3];
    char* directory = argv[2];
    char* exe_file = argv[1];
    int timeout = atoi(stime);

    /*time out setting*/
    struct itimerval t;
    //printf("timeout = %d\n",timeout);
    t.it_value.tv_sec = timeout;
    t.it_value.tv_usec = 0;
    t.it_interval = t.it_value;
    setitimer(ITIMER_REAL, &t, (struct itimerval*)NULL);

   

    char exe_file_c[12] = "./";
    int boolean = (strcmp(argv[1], "solution")==0 );
    int i = 0;

    DIR* dp = NULL;
    struct dirent* file = NULL;
    char list[10][10];
    char contents[10][100];

    if ((dp = opendir(directory)) == NULL)  /*opendir*/
    { return -1; }
    else{
        while((file = readdir(dp)) != NULL) /*fill list[]*/
        {
            if(strcmp(file -> d_name, ".") && strcmp(file->d_name,".."))
            {
                strcpy(list[i], file -> d_name);
                i++;
            }
        }
    }

    
    for(int k=0; k<10; k++)  /*fill contests[]*/
    {
        char temp[15] = "TESTS/";
        strcat(temp, list[k]);
        FILE *fp;
        fp = fopen(temp, "r");
        if(fp != NULL)
        {
            fgets(contents[k], 50, fp);
            fclose(fp);
        }else printf("FP error \n");
    }


    struct timespec start1, end1, temppp;
    //float t_result;
    double total_time = 0;
    double times[10];

    pid_t temp[10];
    pid_t test_child;

    test_child = fork();

    /* 10번 solution.exe와 target.exe를 실행시킴 -> 각각의 child로 멀티프로세스 구현 */
    if(test_child == 0)
    { /*make a pipe*   ->   run the exe_file*/ 
        //printf("tester.c start exefile\n");
        
        if(boolean == 1)
        {   /* solution execute */
            for(int i=0; i<10; i++)
            {
                if (pipe(pipes_1) != 0) 
                {
                  perror("Error") ;
                  exit(1) ;
               }

                clock_gettime(CLOCK_MONOTONIC, &start1);
                
                temp[i] = fork();
                if(temp[i] == 0) //child
                {
                    dup2(pipes_1[1], 1); /* standard output*/   
                    execl("./solution", "solution", contents[i], (char*)NULL); 
                }
                else //parent
                {
                    //signal(SIGCHLD, handler);
                    char buf[101];
                    ssize_t s;
                    close(pipes_1[1]);
                    while((s = read(pipes_1[0], buf, 100))>0)
                    {
                        buf[s] = 0x0;
                        FILE *fp2; //pipe storing.
                        if ((fp2 = fopen("solution_results.txt", "a")) == NULL) { /*File Open*/
                            printf("FILE ERROR!\n");
                            return -1;
                        }
                        fwrite(buf, strlen(buf), 1, fp2);
                        fclose(fp2);
                    } 
                    wait(NULL);
                    clock_gettime(CLOCK_MONOTONIC, &end1);
                    times[i] = ( (end1.tv_sec - start1.tv_sec) + (end1.tv_nsec - start1.tv_nsec) ) / (double)1000000;
                    
                }
                
            }
        }    
        else
        {   

            /* target execute */
            for(int i=0; i<10; i++)
            {
                if (pipe(pipes_2) != 0) 
                {
                  perror("Error") ;
                  exit(1) ;
               }

                clock_gettime(CLOCK_MONOTONIC, &start1);
                temp[i] = fork();
                if(temp[i] == 0)
                {
                    //printf("Running target.exe \n");
                    dup2(pipes_2[1], 1); /* standard output*/    
                    execl("./target", "target", contents[i], (char*)NULL);
                }
                else
                {
                    signal(SIGCHLD, handler);
                    char buf[101];
                    ssize_t s;
                    close(pipes_2[1]);
                    while((s = read(pipes_2[0], buf,  100))>0)
                    {
                        buf[s] = 0x0;
                        //printf("> %s\n",buf);
                        FILE *fp2; //pipe storing.
                        if ((fp2 = fopen("target_results.txt", "a")) == NULL) { /*File Open*/
                            printf("FILE ERROR!\n");
                            return -1;
                        }
                        fwrite(buf, strlen(buf), 1, fp2);
                        fclose(fp2);
                    }
                    pid_t term_pid ;
                    int exit_code ;
                    /* Wating for execute solution, target */
                    term_pid = wait(&exit_code) ;
                    //printf("Process %d is finisehd with exit code %d\n", term_pid, exit_code) ;
                    
                    //wait(NULL);
                    clock_gettime(CLOCK_MONOTONIC, &end1);
                    times[i] = ( (end1.tv_sec - start1.tv_sec)*1.0e3 ) + ((end1.tv_nsec - start1.tv_nsec)/1.0e6);
                }
            }
        }
       

        qsort(times, sizeof(times)/sizeof(double), sizeof(double), compare);
        for(int k=0; k<10; k++)
            total_time = total_time + times[k];
    
        /* 시간기록 파일로 저장 */
        FILE *fp3; // time 기록
        if(boolean == 1)
        {
            if ((fp3 = fopen("s_times.txt", "w")) == NULL) 
                { /*File Open*/
                    printf("FILE ERROR!\n");
                    return -1;
                }
            fprintf(fp3, "%lf\n", times[0]);
            fprintf(fp3, "%lf\n", times[9]);
            fprintf(fp3, "%lf\n", total_time);
            fclose(fp3);
        }
        else
        {
            if ((fp3 = fopen("t_times.txt", "w")) == NULL) 
                { /*File Open*/
                    printf("FILE ERROR!\n");
                    return -1;
                }
            fprintf(fp3, "%lf\n", times[0]);
            fprintf(fp3, "%lf\n", times[9]);
            fprintf(fp3, "%lf\n", total_time);
            fclose(fp3);
        }

        exit(0);
    }
    else
    { 
        /*record the result file*/
        waitpid(test_child, NULL, 0);
        pid_t term_pid ;
        int exit_code ;
        /* Wating for execute solution, target */
        term_pid = wait(&exit_code) ;
        //printf("tester(%d) is finisehd with exit code %d\n", term_pid, exit_code) ;
    }
    
    return 0;
}


int compare(const void *a, const void *b)
{
    double num1 = *(double *)a;    // void 포인터를 float 포인터로 변환한 뒤 역참조하여 값을 가져옴
    double num2 = *(double *)b;    // void 포인터를 float 포인터로 변환한 뒤 역참조하여 값을 가져옴

    if (num1 < num2)    // a가 b보다 작을 때는
        return -1;      // -1 반환
    
    if (num1 > num2)    // a가 b보다 클 때는
        return 1;       // 1 반환
    
    return 0;    // a와 b가 같을 때는 0 반환
}



void handler(int sig)
{
    if(sig == SIGALRM)
    {
        printf("ONE or MORE EXECUTION TIME OVER \n");
        
        //execl("/bin/rm", "rm", "solution_results.txt", "target_results.txt", "t_times.txt", "s_times.txt", (char*)NULL);  
        exit(-1);
    }
    else if(sig == SIGCHLD)
    {
        FILE *fp5;
        pid_t pid;
        int exit_code;
        pid = wait(&exit_code);
        if(WIFEXITED(exit_code))
        {
            //printf("test1\n");
            return;
        }
        else
        {   
       
            if(exit_code != 0) // RUNTIME ERROR
            {
                
                if ((fp5 = fopen("target_results.txt", "a")) == NULL) 
                { /*File Open*/
                    printf("FILE ERROR!\n");
                    exit(-1);
                }
                //printf("R_ERROR\n");
                fwrite(R_ERROR, strlen(R_ERROR), 1, fp5);
                fclose(fp5);
                kill(pid, -1);
                return;
            }
            //fwrite("\n", strlen("\n"), 1, fp5);

            //execl("/bin/rm", "rm", "solution_results.txt", "target_results.txt", "t_times.txt", "s_times.txt", (char*)NULL);  
        }
    }
    
}