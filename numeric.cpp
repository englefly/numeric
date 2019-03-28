#include <stdio.h>
#include <stdlib.h>

#include "numeric.h"

void NumericToString(const NumericSchema schema, short * pdata, char* pStr)
{
    int precision = schema.fracLength + schema.intLength;
    short * frac = pdata + schema.intLength; //小数部分的起点
    short * head = pdata;
    //skip head 0
    for (int i=0; i<precision && *head == 0 ; i++)
    {
        head ++;
    }
    if ( *head == 0 ) return ;//err code;

    bool negtive = *head < 0;
    if (negtive) *pStr ++ = '-';
    while(head < frac)
    {
        sprintf(pStr, "%04hd", negtive? -1 * *head : *head);
        head ++;
        pStr = pStr + 4;
    }
    
    *pStr = '.';
    pStr ++;
    for(int i=0; i<schema.fracLength; i++)
    {
        sprintf(pStr, "%04hd", negtive? -1 * *head : *head);
        head ++;
        pStr = pStr + 4;
    }
    *pStr = 0;
    
}

/**
 * return 0, if converted
 *      -1, numeric field overflow
 *      -2, 
 **/
int StringToNumeric(const char* pStr, const NumericSchema schema, short * pdata)
{
    for( int i=0; i<schema.fracLength + schema.intLength; i++)
    {
        *(pdata + i)  = 0;
    }
    short * intPart = pdata + schema.intLength - 1; //整数部分从低位向高位填写
    short * fracPart = pdata + schema.intLength; //小数部分从高位向低位填写
    const char * cursor = pStr;
    bool negtive = (*pStr == '-');
    const char * head = negtive ? pStr + 1 : pStr;
    const char * point = head; // move to end of integer part
    while( *point != '.' && *point != 0)
    {
        point ++;
    }
    const char * end = point;
    while( *end != 0) end ++;

    int loopcount = 0;
    //convert integer part
    cursor = point;
    while (cursor > head)
    {
        short val = 0;

        cursor --;
        if (cursor >= head) val += *cursor - 48;

        cursor --;
        if (cursor >= head) val += (*cursor  - 48) * 10;

        cursor --;
        if (cursor >= head) val += (*cursor  - 48) * 100;

        cursor --;
        if (cursor >= head) val += (*cursor  - 48) * 1000;

        if (intPart < pdata) return -1; //numeric field overflow
        *intPart = negtive ? -1 * val : val;
        intPart --;
    }

    //convert fractional part
    loopcount = 0;
    cursor = point;
    while (cursor < end && loopcount < schema.fracLength)
    {
        short val = 0;

        cursor ++;
        if (cursor < end) val += (*cursor  - 48) * 1000;

        cursor ++;
        if (cursor < end) val += (*cursor  - 48) * 100;

        cursor ++;
        if (cursor < end) val += (*cursor  - 48) * 10;

        cursor ++;
        if (cursor < end) val += (*cursor  - 48);

        *fracPart = negtive ? -1 * val : val;
        fracPart ++;

        loopcount ++;
    }
    return 0;
}



NumericSchema mergeSchema(NumericSchema lschema, NumericSchema rschema)
{
    NumericSchema schema = { max(lschema.intLength, rschema.intLength),
             max(lschema.fracLength, rschema.fracLength),
            };
    return schema;
}

//index: 0 based
short GetValByIndex(NumericSchema schema, short * a, NumericSchema resSchema, int index)
{
    int n = index - resSchema.intLength;
    if (n >= 0)
    {
        if (n >= schema.fracLength) return 0;
        return a[schema.intLength + n];
    }else{
        if (schema.intLength + n < 0) return 0; 
        return a[schema.intLength + n];
    }
}

/**
 * return 1 if positive
 *        -1 if negtive
 *        0 if c == 0
 **/
int GetSign(NumericSchema schema, short * c)
{
    for( int i=0; i<schema.len(); i++)
    {
        if (c[i]>0) return 1;
        if (c[i]<0) return -1;
    }
    return 0;
}

int add(NumericSchema aschema, short * a, NumericSchema bschema, short * b, short * c)
{
    NumericSchema ms = mergeSchema(aschema, bschema);
    short carry = 0;
    for (int i = ms.len() - 1; i >=0; i--)
    {
        int v = GetValByIndex(aschema, a, ms, i) + GetValByIndex(bschema, b, ms, i) + carry;
        if (v >= 10000){
            v = v - 10000;
            carry = 1;
        }else if (v <= -10000){
            v = v + 10000;
            carry = -1;
        }else{
            carry = 0;
        }
        c[i] = v;
    }

    // positive or negtive
    int borrow = 0;
    int sign = GetSign(ms, c);
    const short clen = ms.len();
    if (sign > 0){
        for(int i = clen - 1; i >= 0; i--)
        {
            c[i] = c[i] - borrow;
            if ( c[i] < 0)
            {
                borrow = 1;
                c[i] = 10000 + c[i];
            }else{
                borrow = 0;
            }
        }
    }else if(sign < 0){
        for(int i = clen - 1; i >= 0; i--)
        {
            c[i] = c[i] + borrow;
            if ( c[i] > 0)
            {
                borrow = 1;
                c[i] = -10000 + c[i];
            }else{
                borrow = 0;
            }
        }
    }
    return carry;
}

int substract(NumericSchema aschema, short * a, NumericSchema bschema, short * b, short * c)
{
    NumericSchema ms = mergeSchema(aschema, bschema);
    short carry = 0;
    for (int i = ms.len() - 1; i >=0; i--)
    {
        int v = GetValByIndex(aschema, a, ms, i) - GetValByIndex(bschema, b, ms, i) + carry;
        if (v >= 10000){
            v = v - 10000;
            carry = 1;
        }else if (v <= -10000){
            v = v + 10000;
            carry = -1;
        }else{
            carry = 0;
        }
        c[i] = v;
    }

    // positive or negtive
    int borrow = 0;
    int sign = GetSign(ms, c);
    const short clen = ms.len();
    if (sign > 0){
        for(int i = clen - 1; i >= 0; i--)
        {
            c[i] = c[i] - borrow;
            if ( c[i] < 0)
            {
                borrow = 1;
                c[i] = 10000 + c[i];
            }else{
                borrow = 0;
            }
        }
    }else if(sign < 0){
        for(int i = clen - 1; i >= 0; i--)
        {
            c[i] = c[i] + borrow;
            if ( c[i] > 0)
            {
                borrow = 1;
                c[i] = -10000 + c[i];
            }else{
                borrow = 0;
            }
        }
    }
    return carry;
}

int multiply(NumericSchema aschema, short * a, NumericSchema bschema, short * b, NumericSchema cschema, short * c)
{
    short blen = bschema.len();
    short alen = aschema.len();
    short clen = cschema.len();

    int carry = 0;
    for(int i=0; i<clen; i++) c[i] = 0; 

    for(int i = 0; i < blen; i ++)
    {
        int y = (int) b[ blen - i - 1 ];
        for (int j=0; j<alen; j++)
        {
            int x = (int) a[ alen -j - 1];
            int z = x*y;
            c[clen - i - j - 1] = c[clen - i - j - 1] + z % 10000 + carry ; //z后4位
            c[clen - i - j - 2] = z / 10000; //z前4位
            
        }
    }
}
