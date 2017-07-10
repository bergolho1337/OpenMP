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

int main(int argc, char* argv[]) 
{
   double **A, *x, *b;
   double start, finish, elapsed;
   int thread_count, n, num_method;

   /* Get number of threads from command line */
   Get_args(argc,argv,&thread_count,&num_method);

   /* Read input */
   scanf("%d",&n);
   A = allocateMatrix(n);
   readMatrix(A,n);
   printf("Matrix A\n");
   printMatrix(A,n);

   b = allocateVector(n);
   readVector(b,n);
   printf("Vector b\n");
   printVector(b,n);

   x = allocateVector(n);

   //BackSubstitution_Row(A,b,x,n);
   BackSubstitution_Column(A,b,x,n);

   printf("Vector x\n");
   printVector(x,n);

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
        scanf("%lf",&b[i]);
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
            scanf("%lf",&A[i][j]);
}

/*------------------------------------------------------------------
 * Function:  BackSubstitution_Row
 * Purpose:   Solve a linear system by back substitution using row-oriented algorithm
 * In arg:    Matrix A, right-handed side vector b, answer vector x and the size of the system
 */
void BackSubstitution_Row (double **A, double *b, double *x, int n)
{
    int row, col;
    for (row = n-1; row >= 0; row--)
    {
        x[row] = b[row];
        for (col = row+1; col < n; col++)
            x[row] -= A[row][col]*x[col];
        x[row] /= A[row][row];
    }
}

/*------------------------------------------------------------------
 * Function:  BackSubstitution_Column
 * Purpose:   Solve a linear system by back substitution using column-oriented algorithm
 * In arg:    Matrix A, right-handed side vector b, answer vector x and the size of the system
 */
void BackSubstitution_Column (double **A, double *b, double *x, int n)
{
    int row, col;
    for (row = 0; row < n; row++)
        x[row] = b[row];
    for (col = n-1; col >= 0; col--)
    {
        x[col] /= A[col][col];
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