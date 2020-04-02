#include <iostream>
#include <chrono>
#include <random>
using namespace std;



int getMilisegundos(clock_t c)
{
    int tiempo=0;
    tiempo = (int)((c/(double)CLOCKS_PER_SEC)*1000) ;
return tiempo;
}

int getRandomNumber(int min, int max) {
    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type> dist( min, max); // distribution in range [min, max]

    return static_cast<int>( dist(rng) );
};



int main()
{

    int Max = 512;
    int** A = new int*[Max];
    int *x;
    int *y;


    for (int i = 0; i < Max; i++){
        A[i] = new int[Max];

    }

    for (int i = 0; i < Max ; i++){
        for (int j = 0; j < Max ; j++){
            A[i][j] = getRandomNumber(0,100);
        }
    }

    x= new int[Max];
    y= new int[Max];

    for(int i=0;i<Max;i++)
    {
        *(x+i) = getRandomNumber(0,100);
        *(y+i) = 0;
    }




    for (int i = 0; i < Max ; i++){
        for (int j = 0; j < Max ; j++){
            A[i][j] = getRandomNumber(0,100);
        }
    }

    for (int i = 0; i < Max ; i++){
        x[i] = getRandomNumber(0,100);
        y[i] = 0;
    }

    for( int i=0;i<Max;i++){
        y[i] = 0;
    }


    /////////////////////////
    auto t0 = std::chrono::system_clock::now();

    for( int i = 0 ; i < Max ; i++){
        for( int j = 0 ; j < Max ; j++){
            y[i] += A[i][j]*x[j];
        }
    }
    auto t1 = std::chrono::system_clock::now();

    ////////////////////////////

    for( int i=0;i<Max;i++){
        y[i] = 0;
    }


/////////////////////////////////
    auto t2 = std::chrono::system_clock::now();

    for(int j=0;j<Max;j++)
        for(int i=0;i<Max;i++)
            y[i] += A[i][j]* x[j];

    auto t3 = std::chrono::system_clock::now();

////////////////////////////////////


    std::chrono::duration<float,std::milli> duration = t1 - t0;
    cout << "Execution Time 1: " << duration.count() << endl;

    std::chrono::duration<float,std::milli> duration2 = t3 - t2;

    cout << "Execution Time 2: " <<  duration2.count() << "s\n" << endl;


    return 0;
}
