template <unsigned int sizeOfBuffer>
class FastInputKyomu{
private:
    unsigned char Input[sizeOfBuffer];
    unsigned char* ptrOfBuffer;
public:
    FastInputKyomu(void): ptrOfBuffer(Input){
        fread(Input, sizeof(unsigned int), sizeOfBuffer, stdin);
    }
    inline int getInt(void){
        while(*ptrOfBuffer < '-') ++ptrOfBuffer;
        int res = 0;
        if(*ptrOfBuffer == '-'){
            ++ptrOfBuffer;
            do { res = 10 * res + *ptrOfBuffer - '0';} while(*(++ptrOfBuffer) >= '0');
            return -res;
        }
        do{ res = 10 * res + *ptrOfBuffer - '0'; } while(*(++ptrOfBuffer) >= '0');
        return res;
    }
    inline unsigned int getUint(void){
        while(*ptrOfBuffer < '0') ++ptrOfBuffer;
        unsigned int res = 0;
        do{ res = 10 * res + *ptrOfBuffer - '0'; } while(*(++ptrOfBuffer) >= '0');
        return res;
    }
    inline int getLLInt(void){
        while(*ptrOfBuffer < '-') ++ptrOfBuffer;
        long long int res = 0;
        if(*ptrOfBuffer == '-'){
            ++ptrOfBuffer;
            do { res = 10 * res + *ptrOfBuffer - '0';} while(*(++ptrOfBuffer) >= '0');
            return -res;
        }
        do{ res = 10 * res + *ptrOfBuffer - '0'; } while(*(++ptrOfBuffer) >= '0');
        return res;
    }
    inline unsigned long long int getULLint(void){
        while(*ptrOfBuffer < '0') ++ptrOfBuffer;
        unsigned long long int res = 0;
        do{ res = 10 * res + *ptrOfBuffer - '0'; } while(*(++ptrOfBuffer) >= '0');
        return res;
    }
    // 1文字を読み込む
    inline unsigned char getChar(void){
        while(*ptrOfBuffer < '0') ++ptrOfBuffer;
        return *(ptrOfBuffer++);
    }
    // 文字列を入力 str に与える関数
    inline void scanStr(char* str){
        char* str_ptr = str;
        while(*ptrOfBuffer <= ' ') ++ptrOfBuffer;
        while(*ptrOfBuffer > ' ') *(str_ptr++) = *(ptrOfBuffer++);
    }
};