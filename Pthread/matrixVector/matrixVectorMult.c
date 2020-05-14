/* File:     
 *     pth_mat_vect.c 
 *
 * Purpose:  
 *     Computes a parallel matrix-vector product.  Matrix
 *     is distributed by block rows.  Vectors are distributed by 
 *     blocks.
 *
 * Input:
 *     m, n: order of matrix
 *     A, x: the matrix and the vector to be multiplied
 *
 * Output:
 *     y: the product vector
 *
 * Compile:  gcc -g -Wall -o pth_mat_vect pth_mat_vect.c -lpthread
 * Usage:
 *     pth_mat_vect <thread_count>
 *
 * Notes:  
 *     1.  Local storage for A, x, y is dynamically allocated.
 *     2.  Number of threads (thread_count) should evenly divide both 
 *         m and n.  The program doesn't check for this.
 *     3.  We use a 1-dimensional array for A and compute subscripts
 *         using the formula A[i][j] = A[i*n + j]
 *     4.  Distribution of A, x, and y is logical:  all three are 
 *         globally shared.
 *
 * IPP:    Section 4.3 (pp. 159 and ff.).  Also Section 4.10 (pp. 191 and 
 *         ff.)
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>

#define MAXNUMBER 100

/* Global variables */
int thread_count;
int m, n;
int* matrix;
int* vector;
int* result;

/* Serial functions */
void Usage(char* prog_name);
void Read_matrix(char* prompt, int A[], int m, int n);
void Read_vector(char* prompt, int x[], int n);
void Print_matrix(char* title, int A[], int m, int n);
void Print_vector(char* title, int y[], int m);

void VectorRandomValues(int vector[], int size);
void MatrixRandomValues(int matrix[], int m, int n);

/* Parallel function */
void *Pth_mat_vect(void* rank);

/*------------------------------------------------------------------*/
int main(int argc, char* argv[]) {
   srand(time(NULL));
   long       thread;
   pthread_t* thread_handles;
   clock_t start, finish;
   float elapsedTime;

   //if (argc != 2) Usage(argv[0]);
   thread_count = atoi(argv[1]);
   thread_handles = malloc(thread_count*sizeof(pthread_t));
   m = atoi(argv[2]);
   n = atoi(argv[3]);

   //printf("Enter m and n\n");
   //scanf("%i%i", &m, &n);

   matrix = malloc(m*n*sizeof(int));
   vector = malloc(n*sizeof(int));
   result = malloc(m*sizeof(int));
   
   //Read_matrix("Enter the matrix", A, m, n);
   MatrixRandomValues( matrix, m, n);
   //Print_matrix("We read", matrix, m, n);

   //Read_vector("Enter the vector", x, n);
   VectorRandomValues( vector, n);
   //Print_vector("We read", vector, n);

   start = clock();
   for (thread = 0; thread < thread_count; thread++)
      pthread_create(&thread_handles[thread], NULL,
         Pth_mat_vect, (void*) thread);
   
   for (thread = 0; thread < thread_count; thread++)
      pthread_join(thread_handles[thread], NULL);
   finish = clock();
   //Print_vector("The product is", result, m);
   elapsedTime = (float) (finish-start)/CLOCKS_PER_SEC;
   printf("Tam matrix   rows:%i  cols:%i\n", m,n);
   printf("N threads: %i\n",thread_count );
   printf("Tiempo transcurrido %0.15f\n", elapsedTime);

   free(matrix);
   free(vector);
   free(result);

   return 0;
}  /* main */

void VectorRandomValues(int vector[], int size){
   for (int i = 0; i < size; ++i){
      vector[i] = rand() % MAXNUMBER;
   }
}
void MatrixRandomValues(int matrix[], int m, int n){
   for (int i = 0; i < m*n; ++i){
      matrix[i] = rand() % MAXNUMBER;
   }
}

/*------------------------------------------------------------------
 * Function:  Usage
 * Purpose:   print a message showing what the command line should
 *            be, and terminate
 * In arg :   prog_name
 */
void Usage (char* prog_name) {
   fprintf(stderr, "usage: %s <thread_count>\n", prog_name);
   exit(0);
}  /* Usage */

/*------------------------------------------------------------------
 * Function:    Read_matrix
 * Purpose:     Read in the matrix
 * In args:     prompt, m, n
 * Out arg:     A
 */
void Read_matrix(char* prompt, int A[], int m, int n) {
   int             i, j;

   printf("%s\n", prompt);
   for (i = 0; i < m; i++) 
      for (j = 0; j < n; j++)
         scanf("%i", &A[i*n+j]);
}  /* Read_matrix */


/*------------------------------------------------------------------
 * Function:        Read_vector
 * Purpose:         Read in the vector x
 * In arg:          prompt, n
 * Out arg:         x
 */
void Read_vector(char* prompt, int x[], int n) {
   int   i;

   printf("%s\n", prompt);
   for (i = 0; i < n; i++) 
      scanf("%i", &x[i]);
}  /* Read_vector */


/*------------------------------------------------------------------
 * Function:       Pth_mat_vect
 * Purpose:        Multiply an mxn matrix by an nx1 column vector
 * In arg:         rank
 * Global in vars: A, x, m, n, thread_count
 * Global out var: y
 */
void *Pth_mat_vect(void* rank) {
   long my_rank = (long) rank;
   int i, j;
   int local_m = m/thread_count; 
   int my_first_row = my_rank*local_m;
   int my_last_row = (my_rank+1)*local_m - 1;

   for (i = my_first_row; i <= my_last_row; i++) {
      result[i] = 0;
      for (j = 0; j < n; j++)
          result[i] += matrix[i*n+j]*vector[j];
   }

   return NULL;
}  /* Pth_mat_vect */


/*------------------------------------------------------------------
 * Function:    Print_matrix
 * Purpose:     Print the matrix
 * In args:     title, A, m, n
 */
void Print_matrix( char* title, int A[], int m, int n) {
   int   i, j;

   printf("%s\n", title);
   for (i = 0; i < m; i++) {
      for (j = 0; j < n; j++)
         printf("%i ", A[i*n + j]);
      printf("\n");
   }
}  /* Print_matrix */


/*------------------------------------------------------------------
 * Function:    Print_vector
 * Purpose:     Print a vector
 * In args:     title, y, m
 */
void Print_vector(char* title, int y[], int m) {
   int   i;

   printf("%s\n", title);
   for (i = 0; i < m; i++)
      printf("%i ", y[i]);
   printf("\n");
}  /* Print_vector */
