#include <stdio.h>
#include <stdlib.h>

#include "numeric.h"

void NumericToString(const NumericSchema schema, PtrNumeric pdata, char* pStr)
{
    for(int i=0; i<schema.intLength; i++)
    {
        sprintf(pStr, "%04hu", *pdata);
        pdata ++;
        pStr = pStr + 4;
    }
    
    *pStr = '.';
    pStr ++;
    for(int i=0; i<schema.decimalLength; i++)
    {
        sprintf(pStr, "%04hu", *pdata);
        pdata ++;
        pStr = pStr + 4;
    }
    *pStr = 0;
    
}

void StringToNumeric(const char* pStr, const NumericSchema *schema, PtrNumeric pdata)
{
    
}

unsigned short max(unsigned short a, unsigned short b)
{
    return a>b ? a : b;
}

NumericSchema mergeSchema(NumericSchema lschema, NumericSchema rschema)
{
    NumericSchema schema = { max(lschema.intLength, rschema.intLength),
             max(lschema.decimalLength, rschema.decimalLength),
            };
    return schema;
}

int add(NumericSchema schema, PtrNumeric ldata, PtrNumeric rdata, PtrNumeric odata)
{
    unsigned short count = schema.decimalLength + schema.intLength;
    int carry = 0;
    for (int i = count - 1; i >= 0; i-- )
    {
        int res = (int) *(ldata + i) + (int) *(rdata + i) + carry;
        printf(">>>[%d] %d = %d + %d + %d\n", i, res, (int) *(ldata + i), (int) *(rdata + i), carry);
        if (res > 10000) 
        {
            carry = 1;
            res -= 10000;
        }
        else carry = 0;
        *(odata + i) = (unsigned short) res;
        printf("### %hu\n", *(odata + i));
    }
    if (carry) printf("overflow...\n");
    return carry;
}

/**
 * 
 *  alen >= blen
 *  纯小数部分相加，返回值表示是否进位
 **/
int addDecimalPart(unsigned short alen, PtrNumeric a, unsigned short blen, PtrNumeric b, PtrNumeric c)
{
    for ( int i = 0; i < alen - blen; i++ )
    {
        *( c + alen - i - 1) = *( a + alen - i - 1);
    }
    int carry = 0;
    for ( int i = 0; i < blen; i++ )
    {
        int res = (int) *(a + blen - i - 1) + (int) *(b + blen - i - 1) + carry;
        if (res  >= 10000)
        {
            res -= 10000;
            carry = 1;
        }
        else
        {
            carry = 0;
        }
        *(c + blen - i - 1) = (unsigned short) res;
    }
    return carry;
}

/**
 * alen >= blen
 * return -1, if overflow
 */
int addIntPart(unsigned short alen, PtrNumeric a, unsigned short blen, PtrNumeric b, int carry, PtrNumeric c)
{
    PtrNumeric atail = a + alen - 1;
    PtrNumeric btail = b + blen - 1;
    PtrNumeric ctail = c + alen - 1;
    for( int i = 0; i < blen; i++ )
    {
        int x = (int) *(btail) + (int) *(atail) + carry;
        if (x >= 10000)
        {
            carry = 1;
            x -= 10000;
        }else{
            carry = 0;
        }
        *ctail = (unsigned short)x;
        atail --;
        btail --;
        ctail --;
    }
    for( int i = 0; i < alen - blen; i++)
    {
        int x = (int) *(atail) + carry;
        if (x >= 10000)
        {
            carry = 1;
            x -= 10000;
        }else{
            carry = 0;
        }
        *ctail = (unsigned short)x;
        atail --;
        ctail --;
    }
    return carry;
}

int add(NumericSchema aschema, PtrNumeric a, NumericSchema bschema, PtrNumeric b, PtrNumeric c)
{
    PtrNumeric adecimal = a + aschema.intLength;
    PtrNumeric bdecimal = b + bschema.intLength;
    PtrNumeric cdecimal = c + (aschema.intLength >= bschema.intLength ? aschema.intLength : bschema.intLength);
    int carry = 0;
    if ( aschema.decimalLength >= bschema.decimalLength ){
        carry = addDecimalPart(aschema.decimalLength, adecimal, bschema.decimalLength, bdecimal, cdecimal);
    }else{
        carry = addDecimalPart(bschema.decimalLength, bdecimal, aschema.decimalLength, adecimal, cdecimal);
    }

    if ( aschema.intLength >= bschema.intLength ){
        carry = addIntPart(aschema.intLength, a, bschema.intLength, b, carry, c);
    }else{
        carry = addIntPart(bschema.intLength, b, aschema.intLength, a, carry, c);
    }
    return carry;
}

//同schema相加
void test1()
{
    unsigned short u = 123;
    int x = (int) u;
    printf("x=%d\n", x);
    x=0x10000;
    u = (unsigned short)x;
    printf("u=%hu\n", u);

    unsigned short a[3];
    a[0] = 432;
    a[1] =   33;
    a[2] = 4000;
    unsigned short b[3];
    b[0] = 77;
    b[1] = 9876;
    b[2] = 8200;
    
    unsigned short c[3];
    c[0] = 0;
    c[1] = 0;
    c[2] = 0;

    char out[100];
    NumericSchema schema = {2, 1};
    for(int i=0; i<100; i++)
        out[i]=0;
    
    add(schema, a, b, c);
    NumericToString( schema, c, out);
}


int test_decimaladd()
{
    unsigned short a[3];
    a[0] = 9999;
    a[1] = 9999;
    a[2] = 9999;
    unsigned short b[2];
    b[0] = 0;
    b[1] = 1;
 
    
    unsigned short c[3];
    c[0] = 0;
    c[1] = 0;
    c[2] = 0;

    char out[100];
    NumericSchema schema = {0, 3};
    for(int i=0; i<100; i++)
        out[i]=0;
    
    int carry = addDecimalPart(3, a, 2, b, c);
    for(int i=0; i<3; i++)
    {
        printf("c[%d] = %hu \n", i, c[i]);
    }

    NumericToString(schema, c, out);
    printf("out: %d%s\n", carry, out);
    return 0;
}

int main()
{
    unsigned short a[3];
    a[0] = 1234;
    a[1] = 6767;
    a[2] = 44;
    unsigned short b[2];
    b[0] = 3233;
    b[1] = 66;
 
    unsigned short c[3];
    c[0] = 0;
    c[1] = 0;
    c[2] = 0;

    NumericSchema aschema = {2, 1};
    NumericSchema bschema = {1, 1};
    NumericSchema cschema = {2 ,1};

    char out[100];
    NumericToString(aschema, a, out);
    printf("%s + ", out);
    NumericToString(bschema, b, out);
    printf("%s = ", out);
    int carry = add(aschema, a, bschema, b, c);
    NumericToString(cschema, c, out);
    printf("%s\n", out);
    return 0;
}
