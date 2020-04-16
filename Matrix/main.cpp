#include <iostream>
#include <random>
#include <sys/time.h>

using namespace std;


template<class T>
void initMatrix(T **a, int t){
	for(int i=0; i<t; i++){
		a[i] = new T[t];
		for(int j=0; j<t; j++){
			a[i][j] = (T)rand()%10;
		}
	}
}


template<class T>
void initMatrix_Zero(T **a,int t){
    for(int i=0; i<t; i++){
		a[i] = new T[t];
		for(int j=0; j<t; j++){
			a[i][j] = 0;
		}
	}
}


template<class T>
void mult(T **a, T **b, T **r, int t){
	for( int i=0; i<t; i++){
        for(int j=0; j<t; j++){
            for(int z=0; z<t; z++)
                r[i][j] += a[i][z]*b[z][j];
        }
    }
}

template<class T>
void mul_blocks(T **a, T **b, T **r, int t, int bl){

	for (int k = 0; k < t; k += bl)
		for (int j = 0; j < t; j += bl)
			for (int i = 0; i < t; ++i)
				for (int jj = j; jj < min(j + bl, t); ++jj)
					for (int kk = k; kk < min(k + bl, t); ++kk)
						r[i][jj] += a[i][kk] * b[kk][jj];
}



int main()
{
    int t = 512;
    int bl = 16;
	int **a = new int *[t];
	int **b = new int *[t];
	int **r = new int *[t];

	initMatrix(a,t);
	initMatrix(b,t);
	initMatrix(r,t);

	struct timeval ti, tf;
	double time1,time2;

	gettimeofday(&ti, NULL);
	mult(a,b,r,t);
	gettimeofday(&tf, NULL);
	time1 = (tf.tv_sec - ti.tv_sec)*1000 + (tf.tv_usec - ti.tv_usec)/1000;

	printf("time normal_mult %.10f s\n", time1/1000);

/*
    gettimeofday(&ti, NULL);
	mul_blocks(a,b,r,t,bl);
	gettimeofday(&tf, NULL);
	time2 = (tf.tv_sec - ti.tv_sec)*1000 + (tf.tv_usec - ti.tv_usec)/1000;

	printf("time normal_mult %.10f s\n", time2/1000);
*/
	delete []a;		delete []r;
    return 0;
}
