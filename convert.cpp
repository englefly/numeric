#include <stdio.h>
#include <stdlib.h>

#include "numeric.h"

int main()
{
    short c[5];
    
    NumericSchema schema = { 3 , 2 };  
    char out[100];

    const char * nums[] = 
    {
        "112233445566.112233445566",    //小数部分自动截断
        "112233445566",             //只有整数部分
        ".1122334455",              //只有小数部分
        "11223344556677",           //overflow
        "-112233445566.778899",     //negtive
        "123.4"                     //自动补全
    };
    int count = sizeof(nums) / sizeof(char*);
    for (int i = 0; i<count; i++)
    {
        for(int j=0; j<20; j++) out[j] = 0;
        for(int j=0; j<5; j++) c[j] = 0;
        int res = 0;
        res = StringToNumeric(nums[i], schema, c);
        if (res) {
            printf("%s convert failed: %d\n", nums[i], res);
            continue;
        }
        NumericToString(schema, c, out);
        printf("%s => %s\n", nums[i], out);
    }
    return 0;
}
