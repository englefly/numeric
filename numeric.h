/**
 * 2字节（一个short）对应4位十进制位。（9999）对应 0x270F，最高两个bit空闲，其中最高位表示符号： (0：正) (1 负)
 * 
 */

typedef struct {
    short intLength;       //counted by SHORT(2bytes)
    short fracLength;   //counted by SHORT(2bytes)
    short len(){ return intLength + fracLength;}
} NumericSchema;



void NumericToString(const NumericSchema schema, short * pdata, char* pStr);

/**
 * return 0, if pStr is legal numeric
 * else return -1
 * */
int StringToNumeric(const char* pStr, const NumericSchema schema, short* pdata);

NumericSchema PrecisionScaleToNumericSchema(short precision, short scale);
NumericSchema mergeSchema(NumericSchema lschema, NumericSchema rschema);

NumericSchema estimateSchemaForAdd(NumericSchema aschema, short * a, NumericSchema bschema, short * b);

NumericSchema estimateSchemaForMultiply(NumericSchema aschema, short * a, NumericSchema bschema, short * b);


int add(NumericSchema aschema, short * a, NumericSchema bschema, short * b, short * c);
int substract(NumericSchema aschema, short * a, NumericSchema bschema, short * b, short * c);


int multiply(NumericSchema aschema, short * a, NumericSchema bschema, short * b, NumericSchema cschema, short * c);

double division(NumericSchema aschema, short * a, NumericSchema bschema, short *b);

__inline__ short max(short a, short b)
{
    return a>b ? a : b;
}