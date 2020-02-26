#include <cstdio>
#include <vector>

/*
namespace MyInputAndOutput{
    class user_input  {} cin;
    class user_output {} cout;

    // 入力関係
    inline user_input& operator >> (user_input& stream, int& int_arg){
        scanf("%d", &int_arg); return stream;
    }
    inline user_input& operator >> (user_input& stream, long long int& int_arg){
        scanf("%lld", &int_arg); return stream;
    }
    template <typename S, typename T>
    inline user_input& operator >>(user_input& stream, std::pair<S, T>& pair_arg){
        stream >> pair_arg.first >> pair_arg.second; return stream;
    }
    template <typename T>
    inline user_input& operator >>(user_input& stream, std::vector<T>& vec){
        for(T& ele : vec) stream >> ele; return stream;
    }

    // 出力関係
    inline user_output& operator << (user_output& stream, const int int_arg){
        printf("%d", int_arg); return stream;
    }
    inline user_output& operator << (user_output& stream, const long long int int_arg){
        printf("%lld", int_arg); return stream;
    }
    inline user_output& operator << (user_output& stream, const double ld_arg){
        printf("%.16lf", ld_arg); return stream;
    }
    inline user_output& operator << (user_output& stream, const long double ld_arg){
        printf("%.16Lf", ld_arg); return stream;
    }
    inline user_output& operator << (user_output& stream, const char char_arg){
        putchar(char_arg); return stream;
    }
    inline user_output& operator << (user_output& stream, const char* str_arg){
        fputs(str_arg, stdout); return stream;
    }
    template <typename S, typename T>
    inline user_output& operator >>(user_output& stream, const std::pair<S, T>& pair_arg){
        stream << pair_arg.first << ' ' << pair_arg.second; return stream;
    }
    template <typename T>
    inline user_output& operator << (user_output& stream, std::vector<T>& vec){
        const size_t size_of_vector = vec.size();
        if(size_of_vector == 0) return stream;
        stream << vec[0]; for(size_t index = 1; index < size_of_vector; ++index) stream << ' ' << vec[index];
        return stream;
    }
}; namespace MIO = MyInputAndOutput;
*/

/*
namespace MyInputAndOutput{
    // 入力関係 (cin)
    class user_input{
    public:
        inline const user_input& operator >> (int& int_arg) const {
            scanf("%d", &int_arg); return *this;
        }
        inline const user_input& operator >> (long long int& llint_arg) const {
            scanf("%lld", &llint_arg); return *this;
        }
        inline const user_input& operator >> (unsigned int& uint_arg) const {
            scanf("%u", &uint_arg); return *this;
        }
        inline const user_input& operator >> (unsigned long long int& ullint_arg) const {
            scanf("%llu", &ullint_arg); return *this;
        }
        inline const user_input& operator >> (char* str_arg) const {
            scanf("%s", str_arg); return *this;
        }
        template <typename S, typename T>
        inline const user_input& operator >>(std::pair<S, T>& pair_arg) const{
            (*this) >> pair_arg.first >> pair_arg.second; return *this;
        }
        template <typename T>
        inline const user_input& operator >>(std::vector<T>& vec) const {
            for(T& ele : vec) (*this) >> ele; return *this;
        }
    } cin;

    // 出力関係 (cout)
    class user_output{
    public:
        user_output(void){}
        inline const user_output& operator << (const int int_arg) const{
            printf("%d", int_arg); return *this;
        }
        inline const user_output& operator << (const unsigned int uint_arg) const{
            printf("%u", uint_arg); return *this;
        }
        inline const user_output& operator << (const long long int llint_arg) const {
            printf("%lld", llint_arg); return *this;
        }
        inline const user_output& operator << (const unsigned long long int ullint_arg) const {
            printf("%llu", ullint_arg); return *this;
        }
        inline const user_output& operator << (const double ld_arg) const {
            printf("%.16lf", ld_arg); return *this;
        }
        inline const user_output& operator << (const long double ld_arg) const {
            printf("%.16Lf", ld_arg); return *this;
        }
        inline const user_output& operator << (const char char_arg) const {
            putchar(char_arg); return *this;
        }
        inline const user_output& operator << (const unsigned char uchar_arg) const {
            putchar(uchar_arg); return *this;
        }
        inline const user_output& operator << (const char* str_arg) const {
            fputs(str_arg, stdout); return *this;
        }
        template <typename S, typename T>
        inline const user_output& operator <<(const std::pair<S, T>& pair_arg) const{
            (*this) << pair_arg.first << ' ' << pair_arg.second; return *this;
        }
        template <typename Tp_name>
        inline const user_output& operator << (const std::vector<Tp_name>& vec) const {
            const size_t size_of_vec = vec.size();
            if(size_of_vec <= 0) return *this;
            (*this) << vec[0]; 
            for(size_t index = 1; index < size_of_vec; ++index) (*this) << ' ' << vec[index];
            return *this;
        }
    } cout;

    // その他出力関数
    void binary_output(int64_t value, size_t length = 64){
        char out[length + 1];
        for(size_t i = 0; i < length; ++i) out[length - 1 - i] = ((value >> i) & 1) ? '1' : '0';
        out[length] = '\0'; puts(out);
    }
    template <typename InputType>
    void print(InputType first, InputType last, const char separate_c = ' ', const char end_c = '\n'){
        InputType it = first;
        while(true){
            MyInputAndOutput::cout << *it;
            if(++it == last){std::cout << end_c; return;}
            MyInputAndOutput::cout << separate_c;  
        }
    }
}; namespace MIO = MyInputAndOutput;
*/

namespace MyInputAndOutput{
    // 入力関係 (cin)
    class user_input{
    public:
        inline const user_input& operator >> (int& int_arg) const {
            scanf("%d", &int_arg); return *this;
        }
        inline const user_input& operator >> (long long int& llint_arg) const {
            scanf("%lld", &llint_arg); return *this;
        }
        inline const user_input& operator >> (unsigned int& uint_arg) const {
            scanf("%u", &uint_arg); return *this;
        }
        inline const user_input& operator >> (unsigned long long int& ullint_arg) const {
            scanf("%llu", &ullint_arg); return *this;
        }
        inline const user_input& operator >> (unsigned long& ullint_arg) const {
            scanf("%lu", &ullint_arg); return *this;
        }
        inline const user_input& operator >> (char* str_arg) const {
            scanf("%s", str_arg); return *this;
        }
        inline const user_input& operator >> (char& char_arg) const {
            scanf("%*c%c", &char_arg); return *this;
        }
        template <typename S, typename T>
        inline const user_input& operator >>(std::pair<S, T>& pair_arg) const{
            (*this) >> pair_arg.first >> pair_arg.second; return *this;
        }
        template <typename T>
        inline const user_input& operator >>(std::vector<T>& vec) const {
            for(T& ele : vec) (*this) >> ele; return *this;
        }
    } cin;

    void ends(void) {putchar('\n'); }
    void endl(void) {putchar('\n'); fflush(stdout);}
    void flush(void) {fflush(stdout);}

    // 出力関係 (cout)
    class user_output{
    public:
        user_output(void){}
        inline const user_output& operator << (const int int_arg) const{
            printf("%d", int_arg); return *this;
        }
        inline const user_output& operator << (const unsigned int uint_arg) const{
            printf("%u", uint_arg); return *this;
        }
        inline const user_output& operator << (const long long int llint_arg) const {
            printf("%lld", llint_arg); return *this;
        }
        inline const user_output& operator << (const unsigned long long int ullint_arg) const {
            printf("%llu", ullint_arg); return *this;
        }
        inline const user_output& operator << (const unsigned long ulint_arg) const {
            printf("%lu", ulint_arg); return *this;
        }
        inline const user_output& operator << (const double ld_arg) const {
            printf("%.16lf", ld_arg); return *this;
        }
        inline const user_output& operator << (const long double ld_arg) const {
            printf("%.16Lf", ld_arg); return *this;
        }
        inline const user_output& operator << (const char char_arg) const {
            putchar(char_arg); return *this;
        }
        inline const user_output& operator << (const unsigned char uchar_arg) const {
            putchar(uchar_arg); return *this;
        }
        inline const user_output& operator << (const char* str_arg) const {
            fputs(str_arg, stdout); return *this;
        }
        inline const user_output& operator << (void(* const func_arg)(void)) const {
            func_arg(); return *this;
        }
        template <typename S, typename T>
        inline const user_output& operator <<(const std::pair<S, T>& pair_arg) const{
            (*this) << pair_arg.first << ' ' << pair_arg.second; return *this;
        }
        template <typename Tp_name>
        inline const user_output& operator << (const std::vector<Tp_name>& vec) const {
            const size_t size_of_vec = vec.size();
            if(size_of_vec <= 0) return *this;
            (*this) << vec[0]; 
            for(size_t index = 1; index < size_of_vec; ++index) (*this) << ' ' << vec[index];
            return *this;
        }
    } cout;

    // その他出力関数
    void binary_output(int64_t value, size_t length = 64){
        char out[length + 1];
        for(size_t i = 0; i < length; ++i) out[length - 1 - i] = ((value >> i) & 1) ? '1' : '0';
        out[length] = '\0'; puts(out);
    }
    template <typename InputType>
    void print(InputType first, InputType last, const char separate_c = ' ', const char end_c = '\n'){
        InputType it = first;
        while(true){
            MyInputAndOutput::cout << *it;
            if(++it == last){MyInputAndOutput::cout << end_c; return;}
            MyInputAndOutput::cout << separate_c;  
        }
    }
}; namespace MIO = MyInputAndOutput;
