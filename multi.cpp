#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "numeric.h"

typedef struct {
    const char* lnum;
    NumericSchema lschema;
    const char* rnum;
    NumericSchema rschema;
    const char* exp;
} Addition;

int main()
{
   Addition adds[] = {
       {"12121212.8", {2,1}, "2", {1,1}, "24242425.60000000"},
       {"12121212.8", {2,1}, "2.1", {2,1}, "25454546.88000000"},
       {"1212.81119", {2,2}, "2.2", {1,1}, "2668.184618000000"},
       {"1212.81119", {2,2}, "-2.2", {1,1}, "-2668.184618000000"},
       {"-1212.81119", {2,2}, "2.2", {1,1}, "-2668.184618000000"},
       {"1212.81119", {2,2}, "2.2", {1,1}, "2668.184618000000"},
   };

   short lshort[10];
   short rshort[10];
   short res[10];
   char out[100];
   int count = sizeof(adds) / sizeof(Addition);
   printf("count=%d\n", count);
   for (int i=0; i<count; i++)
   {
       for(int j = 0; j<10; j++){
           lshort[j] = 0;
           rshort[j] = 0;
           res[j] = 0;
       }
       for(int j = 0; j<100; j++) out[i] = 0;
       Addition a = adds[i];
       StringToNumeric(a.lnum, a.lschema, lshort);
       StringToNumeric(a.rnum, a.rschema, rshort);
        NumericSchema resSchema = {a.lschema.intLength + a.rschema.intLength, a.lschema.fracLength + a.rschema.fracLength};
       multiply(a.lschema, lshort, a.rschema, rshort, 
        resSchema, res);

       
       NumericToString(resSchema, res, out);
       printf("%s X %s = %s\n", a.lnum, a.rnum, out);
       if (strcmp(out, a.exp)) printf(">>>failed, expect: %s, actually get: %s\n", a.exp, out);
   }
}