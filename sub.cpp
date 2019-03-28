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
       {"123.123", {3,2}, "222.2", {3 ,1}, "-0099.07700000"}, //
       {"1234567812345678", {4,1}, "1234.12345678",{1, 2}, "1234567812344443.87654322"}, //schema upgrade
       {"123.123", {3,2}, "0", {3 ,1}, "0123.12300000"},      //
       {"99999.99", {2,1}, ".1", {2,1}, "00099999.8900"},    //
       {"-1.9", {2,1}, "-0.2", {2,1}, "-0001.7000"},      //
       {"-999.9", {2, 1}, "-0.11", {2,1}, "-0999.7900"},   //
       {"10000002", {2, 0}, "-900008", {2,0}, "10900010."},
       {"10000002.", {3, 1}, "-900008.1", {3,1}, "10900010.1000"},
       {"-10000002.", {3, 1}, "900008.1", {3,1}, "-10900010.1000"},
       {"10000002.", {3, 1}, "900008.1", {3,1}, "09099993.9000"},
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

       substract(a.lschema, lshort, a.rschema, rshort, res);

       NumericSchema ms = mergeSchema(a.lschema, a.rschema);
       NumericToString(ms, res, out);
       printf("%s - %s = %s\n", a.lnum, a.rnum, out);
       if (strcmp(out, a.exp)) printf(">>>failed, expect: %s, actually get: %s\n", a.exp, out);
   }
}