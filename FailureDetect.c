//rm으로 solution_results하고 target_results 둘다 없애기.
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
    pid_t child;
    child = fork();

    if(child == 0)
    {
        FILE *fp1, *fp2, *fp3, *fp4;
        int results[10];
        char solutions[10][100];
        char targets[10][100];
        char s_times[3][10];
        char t_times[3][10];
        float max, min, total = 0;
        int success = 0 , failure = 0;
        char buf[100];

        /* solution_results.txt 값 가져오기 */
        fp1 = fopen("solution_results.txt", "r");
        if(fp1 == NULL) return -1;
        for(int i=0; i<10; i++)
        {
            if(fgets(buf, sizeof(buf), fp1) == NULL) break;
            strcpy(solutions[i], buf);
        }
        fclose(fp1);

        /* target_results.txt 값 가져오기 */
        fp2 = fopen("target_results.txt", "r");
        if(fp2 == NULL) return -1;
        for(int i=0; i<10; i++)
        {
            if(fgets(buf, sizeof(buf), fp2) == NULL) break;
            if(strcmp(buf, "")==0)
                strcpy(targets[i], "\n");
            strcpy(targets[i], buf);
        }
        fclose(fp2);

        /* 시간 가져오기 */
        fp3 = fopen("s_times.txt", "r");
        if(fp3 == NULL) return -1;
        for(int i=0; i<3; i++)
        {
            if(fgets(buf, sizeof(buf), fp3) == NULL) break;
            strcpy(s_times[i], buf);
        }
        fclose(fp3);

        fp4 = fopen("t_times.txt", "r");
        if(fp4 == NULL) return -1;
        for(int i=0; i<3; i++)
        {
            if(fgets(buf, sizeof(buf), fp3) == NULL) break;
            strcpy(t_times[i], buf);
        }
        fclose(fp4);


        /* 성공 및 실패 분석 */
        for(int i=0; i<10; i++)
        {
            int temp = strcmp(solutions[i], targets[i]);
            if(temp == 0)
            { 
                results[i] = 1;  //Same results
                success++;
            }
            else 
            {
                results[i] = 0;  //Different results
                failure++;
            }
        }

        printf("=================================================================\n");
        printf("Target Success: %d times\n", success);
        printf("Target Failure: %d times\n", failure);
        printf("Success rate: %f %% \n", ( (float)success/(success+failure)*100) );
        printf("=================================================================\n\n");
        printf("Fastest Execution time from solution = %lf(milisec) \n", atof(s_times[0]));
        printf("Slowest Execution time from solution = %lf(milisec) \n", atof(s_times[1]));
        printf("Total   Execution time from solution = %lf(milisec) \n", atof(s_times[2]));
        printf("=================================================================\n\n");
        printf("Fastest Execution time from   target = %lf(milisec) \n", atof(t_times[0]));
        printf("Slowest Execution time from   target = %lf(milisec) \n", atof(t_times[1]));
        printf("Total   Execution time from   target = %lf(milisec) \n", atof(t_times[2]));
        printf("=================================================================\n\n");

        execl("/bin/rm", "rm", "solution_results.txt", "target_results.txt", "t_times.txt", "s_times.txt", (char*)NULL);
        //exit(0);
    }
    else
    {
       pid_t term_pid ;
	   int exit_code ;
	   term_pid = wait(&exit_code) ;
       printf("Thank you!\n");
       return 0;
    }


   

    
}