/* File:    loop_dependency.c
 * Purpose: Calculate an array with loop dependency and eliminate this problem in order to
 * be able to paralelize the for loop.
 *
 * Input:   
 * Output:  an array
 *
 * Compile: make
 * Usage:   ./omp_code
 *
 * Notes:
 *   1.  The function f(x) is hardwired.
 *   2.  In this version, each thread explicitly computes the integral
 *       over its assigned subinterval, a critical directive is used
 *       for the global sum.
 *   3.  This version assumes that n is evenly divisible by the
 *       number of threads
 *
 * IPP:  Exercise 5.8 (p. 264)
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

void Usage(char* prog_name);
void fillArrayWithoutDependency (int arr[], const int n);
void fillArrayDependency (int arr[], const int n);

int main(int argc, char* argv[]) 
{
  /* Size of the array */
  int n;                    

  if (argc != 2) Usage(argv[0]);
  // Parse input arguments
  n = strtol(argv[1],NULL,10);

  int array1[n], array2[n];
  
  fillArrayDependency(array1,n);
  fillArrayWithoutDependency(array2,n);
  
  int i;
  for (i = 0; i < n; i++)
    printf("%d %d %d\n",i,array1[i],array2[i]);

  return 0;
}  /* main */


/*--------------------------------------------------------------------
 * Function:    fillArrayWithoutDependency
 * Purpose:     Fill the array using a recurrence formula that eliminates the
 *              dependency on the loop.
 * In arg:      arr, n
 */
void fillArrayWithoutDependency (int arr[], const int n) 
{
  int i;
  for (i = 0; i < n; i++)
    arr[i] = i * (i+1) / 2;
}

/*--------------------------------------------------------------------
 * Function:    fillArrayDependency
 * Purpose:     Fill the array using a dependency loop
 * In arg:      arr, n
 */
void fillArrayDependency (int arr[], const int n) 
{
  int i;
  arr[0] = 0;
  for (i = 1; i < n; i++)
    arr[i] = arr[i-1] + i;
}  

/*--------------------------------------------------------------------
 * Function:    Usage
 * Purpose:     Print command line for function and terminate
 * In arg:      prog_name
 */
void Usage(char* prog_name) 
{

   fprintf(stderr, "Usage: %s <n>\n", prog_name);
   fprintf(stderr, "<n> = Size of the array\n");
   exit(0);
}  /* Usage */

