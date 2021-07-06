/* #################################################### */
/*                    21600786                          */
/*                      홍승훈                            */
/* #################################################### */
/* ./pctest -i <testdir> -t <timeout> <solution> <target> */
/* ./pctest -i /home/s21600786/OShw1/TESTS -t 10 solution target */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <time.h>
#include <sys/time.h>

void handler(int sig);

void Eliminate(char *str, char ch);


int main(int argc, char **argv) {
	 /*Input argument Test */
	signal(SIGALRM, handler);
	

	if(strcmp(argv[1], "-i")!=0)
	{
		printf("Bad argument typing\n\n");
		return -1;
	}
    if(strcmp(argv[3], "-t")!=0)
	{
		printf("Bad argument typing\n\n");
		return -1;
	}

	/* argument */
	char* solution_c, target_c;
	char* testdir = argv[2];
	char* stime = argv[4];
	char* solution = argv[5];
	char* target = argv[6];
	struct itimerval t;
	int time = atoi(stime);
	
	if(time > 10)
	{
		printf("Overtime \n");
		return -1;
	}

	/*타이머 세팅*/
	t.it_value.tv_sec = time;
	t.it_interval = t.it_value;
	setitimer(ITIMER_REAL, &t, NULL);
	
	
	pid_t child1, child1_1, child2, child2_1, child3 ;

	printf("PCtest start.\n\n") ;
	char *gccdir = "/usr/bin/gcc";

	child1 = fork();
	if (child1 == 0) { /* child 1 is for test solution */
		pid_t term_pid ;
		int exit_code ;
		
		child1_1 = fork();
		
		if(child1_1 == 0) /* child1_1 is for gcc the solution.c */
		{
			execl(gccdir, "gcc", "solution.c", "-o", "solution", (char*) NULL);
		}
		else		
		{
			term_pid = wait(&exit_code) ;  /* wait for child 1-1 */
			
			/* child 2 is for execute target execute file */
			/* making pipe */
			execl("./tester", "tester", "solution", testdir , stime,  (char*) NULL);
		}

	} else { 
		/* parent for making child2*/
		child2 = fork() ;

		if (child2 == 0) /* child 2 making */
		{   
			/* child 2 is for test target */
			pid_t term_pid ;
			int exit_code ;
		
			child2_1 = fork();
		
			if(child2_1 == 0) /* child2_1 is for gcc the target.c */
			{
				execl(gccdir, "gcc", "target.c" , "-o", "target", (char*) NULL);
			}
			else
			{   
 				term_pid = wait(&exit_code) ;  /* wait for child 2-1 */

				/* child 2 is for execute target execute file */
				execl("./tester", "tester", "target", testdir , stime, (char*) NULL);
			}

			
		} else { 
			signal(SIGCHLD, handler);
			/* parent after making child1, child2*/
			pid_t term_pid ;
			int exit_code ;

			/* Wating for execute solution, target */
			term_pid = wait(&exit_code) ;
			term_pid = wait(&exit_code) ;

			/* After test execute, making child 3 */		
			child3 = fork();
			if(child3 == 0)
			{
				execl("./FailureDetect", "FailureDetect",  (char*) NULL); // fail detecter
			}
			else 	/* parent run */
			{
				term_pid = wait(&exit_code) ;
				return 0;
			}
		}
	}
}



/* Functions */

void Eliminate(char *str, char ch)
{
    for (; *str != '\0'; str++)
    {
        if (*str == ch)
        {
            strcpy(str, str + 1);
            str--;
        }
    }
}

void handler(int sig)
{
    if(sig == SIGALRM)
    {
        printf("TIME OVER\n");
        exit(1);
    }
	else if(sig == SIGCHLD)
    {
        pid_t pid;
        int exit_code;
        pid = wait(&exit_code);
        if(WIFEXITED(exit_code))
        {
            //printf("...\n");
            return;
        }
        else
        {
            perror("Illegal execution Sensing \n");
            exit(1);
            return;
            //execl("/bin/rm", "rm", "solution_results.txt", "target_results.txt", "t_times.txt", "s_times.txt", (char*)NULL);  
        }
    }  
}

