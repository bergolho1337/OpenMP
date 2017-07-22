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

const int MAX_THREADS = 1024;
const int MAX_VALUE = 4;

/* Only executed by main thread */
void Get_args(int argc, char* argv[], int *thread_count, int *num_method);
void Usage(char* prog_name);
double* allocateVector (int n);
double** allocateMatrix (int n);
void freeMatrix (double **A);
void readMatrix (double **A, int n);
void readVector (double *b, int n);
void generateMatrix (double **A, int n);
double generateNumber ();
void BackSubstitution_Row (double **A, double *b, double *x, int n);
void BackSubstitution_Column (double **A, double *b, double *x, int n);
void printMatrix (double **A, int n);
void printVector (double *b, int n);
void error (const char msg[]);

int main(int argc, char* argv[]) 
{
   double **A, *x, *b;
   double start, finish, elapsed;
   int thread_count, n, num_method;

   /* Get number of threads from command line */
   Get_args(argc,argv,&thread_count,&num_method);

   omp_set_num_threads(thread_count);

   /* Read input */
   if (!scanf("%d",&n)) error("Reading file");
   A = allocateMatrix(n);
   readMatrix(A,n);
   //printf("Matrix A\n");
   //printMatrix(A,n);

   b = allocateVector(n);
   readVector(b,n);
   //printf("Vector b\n");
   //printVector(b,n);

   x = allocateVector(n);

   GET_TIME(start);
   if (num_method == 1) BackSubstitution_Row(A,b,x,n);
   else                 BackSubstitution_Column(A,b,x,n);
   
   GET_TIME(finish);
   elapsed = finish - start;

   //printf("Vector x\n");
   //printVector(x,n);
   printf("Time elapsed = %.10lf\n",elapsed);

   freeMatrix(A);
   free(b);

   return 0;
}  /* main */

/*------------------------------------------------------------------
 * Function:  allocateMatrix
 * Purpose:   Allocate memory for a n x n matrix
 * In arg:    Size of the matrix
 */
double** allocateMatrix (int n)
{
    int i;
    double **A = (double**)malloc(sizeof(double*)*n);
    A[0] = (double*)malloc(sizeof(double)*n*n);
    for (i = 1; i < n; i++)
        A[i] = &A[0][i*n];
    return A;
}

/*------------------------------------------------------------------
 * Function:  allocateVector
 * Purpose:   Allocate memory for an n size vector
 * In arg:    Size of the vector
 */
double* allocateVector (int n)
{
    double *b = (double*)malloc(sizeof(double)*n);
    return b;
}

/*------------------------------------------------------------------
 * Function:  readVector
 * Purpose:   Read an n size vector from standart input
 * In arg:    Reference to the vector and the size
 */
void readVector (double *b, int n)
{
    int i;
    for (i = 0; i < n; i++)
        if (!scanf("%lf",&b[i])) error("Reading file");;
}

/*------------------------------------------------------------------
 * Function:  printVector
 * Purpose:   Print a vector to the stdout
 * In arg:    Double array and size of the vector
 */
void printVector (double *b, int n)
{
    int i;
    for (i = 0; i < n; i++)
        printf("%lf\n",b[i]);
}

/*------------------------------------------------------------------
 * Function:  freeMatrix
 * Purpose:   Free memory for the matrix structure
 * In arg:    The matrix
 */
void freeMatrix (double **A)
{
    free(A[0]);
    free(A);
    A = NULL;
}

/*------------------------------------------------------------------
 * Function:  readMatrix
 * Purpose:   Read an n x n size matrix from standart input
 * In arg:    Reference to the matrix and the size
 */
void readMatrix (double **A, int n)
{
    int i, j;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            if (!scanf("%lf",&A[i][j])) error("Reading file");;
}

/*------------------------------------------------------------------
 * Function:  BackSubstitution_Row
 * Purpose:   Solve a linear system by back substitution using row-oriented algorithm
 * In arg:    Matrix A, right-handed side vector b, answer vector x and the size of the system
 * Parallel:  The outer loop cannot be parallized because there is a dependency between the rows
              The inner loop can be parallized, but this will create a critical section
 */
void BackSubstitution_Row (double **A, double *b, double *x, int n)
{
    int row, col;
    for (row = n-1; row >= 0; row--)
    {
        x[row] = b[row];
        #pragma omp parallel for
        for (col = row+1; col < n; col++)
        {
            #pragma omp critical
            x[row] -= A[row][col]*x[col];
        }
            
        x[row] /= A[row][row];
    }
}

/*------------------------------------------------------------------
 * Function:  BackSubstitution_Column
 * Purpose:   Solve a linear system by back substitution using column-oriented algorithm
 * In arg:    Matrix A, right-handed side vector b, answer vector x and the size of the system
 * Parallel:  The first loop that copies the b array to x can be parallelized as well
              The outer loop cannot be parallelized because there is a dependency between the column
              The inner loop can be parallelized without a critical section
 */
void BackSubstitution_Column (double **A, double *b, double *x, int n)
{
    int row, col;

    #pragma omp parallel for
    for (row = 0; row < n; row++)
        x[row] = b[row];

    for (col = n-1; col >= 0; col--)
    {
        x[col] /= A[col][col];

        #pragma omp parallel for
        for (row = 0; row < col; row++)
            x[row] -= A[row][col]*x[col];
    }
}

/*------------------------------------------------------------------
 * Function:  generateMatrix
 * Purpose:   Generate a random matrix of size n x n
 * In arg:    Size of the matrix
 */
void generateMatrix (double **A, int n)
{
    int i, j;
    for (i = 0; i < n; i++)
        for (j = i; j < n; j++)
            A[i][j] = generateNumber();
}

/*------------------------------------------------------------------
 * Function:  generateData
 * Purpose:   Genrate a random vector of data
 * In arg:    Integer array and size of the vector
 */
double generateNumber ()
{
    if (rand() % 2 == 0)
        return (double)(1 + rand() % MAX_VALUE);
    else
        return (double)(-(1 + rand() % MAX_VALUE));
}

/*------------------------------------------------------------------
 * Function:  printData
 * Purpose:   Print the vector of data
 * In arg:    Integer array and size of the vector
 */
void printMatrix (double **A, int n)
{
    int i, j;
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
            printf("%lf ",A[i][j]);
        printf("\n");
    }
        
}

/*------------------------------------------------------------------
 * Function:    Get_args
 * Purpose:     Get the command line args
 * In args:     argc, argv
 * Globals out: thread_count, total_tosses
 */
void Get_args(int argc, char* argv[], int *thread_count, int *num_method) 
{
   if (argc != 3) Usage(argv[0]);
   *thread_count = atoi(argv[1]);  
   if (*thread_count <= 0 || *thread_count > MAX_THREADS) Usage(argv[0]);
   *num_method = atoi(argv[2]);
   if (*num_method < 0 || *num_method > 2) Usage(argv[0]);
}  /* Get_args */

/*------------------------------------------------------------------
 * Function:  Usage
 * Purpose:   Print a message explaining how to run the program
 * In arg:    prog_name
 */
void Usage(char* prog_name) 
{
   fprintf(stderr, "=============== BACKWARD SUBSTITUION ====================\n"); 
   fprintf(stderr, "Usage: %s <number of threads> <num_method>\n", prog_name);
   fprintf(stderr, "   num_method is the number of the method\n");
   fprintf(stderr, "   1 - Row-oriented\n");
   fprintf(stderr, "   2 - Column-oriented\n");
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