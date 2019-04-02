#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "numeric.h"

int main()
{
    const char * a = "1234567890.0987654321";
    const char * b = "543.21";
    short an[6];
    short bn[2];
    NumericSchema asch = {3,3};
    NumericSchema bsch = {1,1};
    StringToNumeric(a, asch, an);
    StringToNumeric(b, bsch, bn);
    double d = division(asch, an, bsch, bn);
    printf("%s / %s = %lf\n", a, b, d);

    printf("size of long: %ld\n", sizeof(long));
    printf("size of long long: %ld\n", sizeof(long long));
}