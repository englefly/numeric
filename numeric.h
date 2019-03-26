/**
 * 2字节（一个short）对应4位十进制位。（9999）对应 0x270F，最高两个bit空闲，其中最高位表示符号： (0：正) (1 负)
 * 
 */

typedef unsigned short* PtrNumeric;
typedef struct {
    unsigned short intLength;       //counted by SHORT(2bytes)
    unsigned short decimalLength;   //counted by SHORT(2bytes)
} NumericSchema;



void NumericToString(const NumericSchema schema, PtrNumeric pdata, char* pStr);
void StringToNumeric(const char* pStr, const NumericSchema *schema, PtrNumeric pdata);

NumericSchema PrecisionScaleToNumericSchema(unsigned short precision, unsigned short scale);
NumericSchema mergeSchema(NumericSchema lschema, NumericSchema rschema);
//int add(NumericSchema schema, PtrNumeric ldata, PtrNumeric rdata, PtrNumeric odata);
int add(NumericSchema aschema, PtrNumeric a, NumericSchema bschema, PtrNumeric b, PtrNumeric c);
