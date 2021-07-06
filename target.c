#define _CRT_SECURE_NO_WARNINGS
#define ERRORMODE_1 1 
#define INFINTLOOP_1 0
#define RUNTIME_ERROR_1 1
#define DifferentAnswer_1 0
#define FILEOPEN_1 0
#define LONG_EXECUTION_1 0

#define ERRORMODE_2 0
#define INFINTLOOP_2 0
#define DifferentAnswer_2 0
#define FILEOPEN_2 0

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    if(strcmp(argv[1], "0")==0)
    {
        if(ERRORMODE_1 == 1)
        {
            if(INFINTLOOP_1 == 1)
            {
                for(;;);  //It makes TIMEOUT
            }
            if(RUNTIME_ERROR_1 == 1)
            {
                int a = atoi(argv[1]);  //It makes 0-divide ERROR (TYPE of RUNTIME ERROR) 
                int b;
                b = 10/a;
            }
            if(FILEOPEN_1 == 1)
            {
                FILE *fp;
                fp = fopen("TEMP", "r");
                if(fp == NULL)
                {
                    printf("FP error \n");
                } 
                else
                fclose(fp); 
            }
            if(DifferentAnswer_1 == 1)
            {
                printf("incorrect answer \n");
            }
            if(LONG_EXECUTION_1 == 1)
            {
                for(int i=0; i<999999999; i++)
                {
                    int a = 1;
                    int b = 2;
                }
                for(int i=0; i<999999999; i++)
                {
                    int a = 1;
                    int b = 2;
                }
                for(int i=0; i<999999999; i++)
                {
                    int a = 1;
                    int b = 2;
                }
                printf("longlong\n");
            }
        }
        else printf("%s\n", argv[1]);
    }
    else if (strcmp(argv[1], "3")==0)
    {
        if(ERRORMODE_2 == 1)
        {
            if(INFINTLOOP_2 == 1)
            {
                for(;;);  //It makes TIMEOUT
            }

            if(FILEOPEN_2 == 1)
            {
                FILE *fp;
                fp = fopen("TEMP", "r");
                if(fp == NULL)
                {
                    printf("FP error \n");
                } 
                else
                fclose(fp); 
            }

            if(DifferentAnswer_2 == 1)
            {
                printf("incorrect answer \n");
            }

        }else printf("%s\n", argv[1]);
    }
    else 
        printf("%s\n", argv[1]);  //NOERROR == ECHO
    
    return 0;
}
