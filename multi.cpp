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
    //    {"123.123", {3,2}, "222.2", {3 ,1}, "0345.32300000"}, //
    //    {"1234567812345678", {4,1}, "1234.12345678",{1, 2}, "1234567812346912.12345678"}, //schema upgrade
    //    {"123.123", {3,2}, "0", {3 ,1}, "0123.12300000"},      //+0
    //    {"99999.99", {2,1}, ".1", {2,1}, "00100000.0900"},    //进位
    //    {"-1.9", {2,1}, "-0.2", {2,1}, "-0002.1000"},      //negtive
    //    {"-999.9", {2, 1}, "-0.11", {2,1}, "-1000.0100"},   //负数相加进位
    // {"10000002", {2, 0}, "-900008", {2,0}, "09099994."},
    // {"10000002.", {3, 1}, "-900008.1", {3,1}, "09099993.9000"},
    // {"-10000002.", {3, 1}, "900008.1", {3,1}, "-09099993.9000"},
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