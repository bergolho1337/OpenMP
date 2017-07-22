/*
********************************************
    Programming Assignment 5.4 (p.269)
********************************************
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include "../timer.h"

/* Only executed by main thread */
void Get_args(int argc, char* argv[], int *n);
void GenerateInput (int n);
void Usage(char* prog_name);
void error (const char msg[]);

int main(int argc, char* argv[]) 
{
   int n; 
   
   Get_args(argc,argv,&n);
   
   GenerateInput(n);
   
   return 0;
}  /* main */

/*------------------------------------------------------------------
 * Function:    Get_args
 * Purpose:     Get the command line args
 * In args:     argc, argv
 * Globals out: thread_count, total_tosses
 */
void GenerateInput (int n) 
{
    int i, j;
    FILE *in = fopen("in","w+");
    fprintf(in,"%d\n",n);
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            if (j >= i) fprintf(in,"1 ");
            else        fprintf(in,"0 ");
        }
        fprintf(in,"\n");
    }
    for (i = n; i >= 1; i--) fprintf(in,"%d\n",i);
    fclose(in);
}  

/*------------------------------------------------------------------
 * Function:    Get_args
 * Purpose:     Get the command line args
 * In args:     argc, argv
 * Globals out: thread_count, total_tosses
 */
void Get_args(int argc, char* argv[], int *n) 
{
   if (argc != 2) Usage(argv[0]);
   *n = atoi(argv[1]);  
}  /* Get_args */

/*------------------------------------------------------------------
 * Function:  Usage
 * Purpose:   Print a message explaining how to run the program
 * In arg:    prog_name
 */
void Usage(char* prog_name) 
{
   fprintf(stderr, "=============== BACKWARD SUBSTITUION ====================\n"); 
   fprintf(stderr, "Usage: %s <n>\n", prog_name);
   fprintf(stderr, "   n is the size of the system\n");
   fprintf(stderr, "=========================================================\n");
   exit(EXIT_FAILURE);
}  /* Usage */

/*------------------------------------------------------------------
 * Function:  error
 * Purpose:   Print an error message
 * In arg:    msg
 */
void error (const char msg[])
{
   fprintf(stderr,"[-] ERROR! %s\n",msg);
   exit(EXIT_FAILURE);
}