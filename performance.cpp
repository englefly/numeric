#include <stdio.h>
#include <stdlib.h> 
#include <sys/time.h>
#include <string.h>

#include "numeric.h"

struct CPU_Timer{
    long start;
    long stop;
    void begin()
    {
        struct timeval tv;
        gettimeofday(&tv,NULL);
        start = tv.tv_sec * 1000 + tv.tv_usec/1000;
    }
    long end()
    {
        struct timeval tv;
        gettimeofday(&tv,NULL);
        stop = tv.tv_sec * 1000 + tv.tv_usec/1000;
        long elapsed = stop -start;
        printf("cpu time: %ld ms\n", elapsed);
        return elapsed;
    }
};

void fillArray(double* d, int len)
{
    for (int i=0; i<len; i++)
    {
        long long z = rand() * 10000L + rand();
        long long f = rand() * 10000L + rand();
        d[i] = z + f / 100000000.0;
        // printf("%lld : %lld => %lf \n", z, f, d[i]);
    }

}

int main()
{
    const long len = 10000000;
    double* d1 = (double*) malloc(len * sizeof(double));
    double* d2 = (double*) malloc(len * sizeof(double));

    fillArray(d1, len);
    fillArray(d2, len);

    CPU_Timer timer;
    timer.begin();
    double x = 0;
    for( int i=0; i<len; i++)
    {
        x = d1[i] + d2[i];
    }
    printf("+ double ");
    timer.end();

    short** s1 = (short **) malloc( len * sizeof(short*));
    short** s2 = (short **) malloc( len * sizeof(short*));

    char str[24];
    NumericSchema sch = {4,2};
    for(int i=0; i<len; i++)
    {
        memset(str, 0, 24);
        sprintf(str, "%lf", d1[i]);
        *(s1 + i) = (short*) malloc(6 * sizeof(short));
        StringToNumeric(str, sch, s1[i]);

        memset(str, 0, 24);
        sprintf(str, "%lf", d2[i]);
        *(s2 + i) = (short*) malloc(6 * sizeof(short));
        StringToNumeric(str, sch, s2[i]);
    }

    short res[28];
    timer.begin();
    for(int i=0; i<len; i++)
    {
        add(sch, s1[i], sch, s2[i], res);
    }
    printf("+ decimal: ");
    timer.end();

    timer.begin();
    x = 0;
    for( int i=0; i<len; i++)
    {
        x = d1[i] * d2[i];
    }
    printf("* double ");
    timer.end();


    short res2[28];
    timer.begin();
    for(int i=0; i<len; i++)
    {
        multiply(sch, s1[i], sch, s2[i], {8,4}, res2);
    }
    printf("* decimal: ");
    timer.end();
    

    free(d1);
    free(d2);
    return 0;
}