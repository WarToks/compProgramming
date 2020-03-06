#include <cstdio>
#include <cstdint>
#include <string>
#include <tuple>
#include <vector>

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
            //scanf("%*c%c", &char_arg); return *this;
            scanf(" %c", &char_arg); return *this;
        }
        inline const user_input& operator >> (std::string& str_arg) const {
            /*  < definition of getchar >
                reference
                MacOS   : https://developer.apple.com/library/archive/documentation/System/Conceptual/ManPages_iPhoneOS/man3/getchar.3.html
                Windows : https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/getchar-nolock-getwchar-nolock?view=vs-2019
                Linux   : https://linux.die.net/man/3/unlocked_stdio 
                Ubuntu  : http://manpages.ubuntu.com/manpages/trusty/man3/getchar_unlocked.3posix.html
            */

            #if defined(__APPLE__)
                #define DAGGER_GETCHAR_UNLOCKED_DAGGER getchar_unlocked
            #elif defined(_WIN32) || defined(_WIN64)
                #define DAGGER_GETCHAR_UNLOCKED_DAGGER _getchar_nolock
            #elif defined(__linux) 
                #if _POSIX_C_SOURCE >= 1 || _XOPEN_SOURCE || _POSIX_SOURCE || _BSD_SOURCE || _SVID_SOURCE
                    #define DAGGER_GETCHAR_UNLOCKED_DAGGER getchar_unlocked
                #else
                    #define DAGGER_GETCHAR_UNLOCKED_DAGGER getchar
                #endif
            #else
                #define DAGGER_GETCHAR_UNLOCKED_DAGGER getchar
            #endif

            // 計算本体
            str_arg.erase();
            char c; while(true){ 
                c = DAGGER_GETCHAR_UNLOCKED_DAGGER();
                if(c == EOF) return *this;
                if(c != '\n' and c != ' ' and c != '\t') break;
            }
            constexpr std::string::size_type buffer_size = 128; 
            char buffer_input[buffer_size]; std::string::size_type buffer_length = 0;
            do{
                buffer_input[buffer_length++] = c;
                if(buffer_length == buffer_size){
                    buffer_length = 0; str_arg.append(buffer_input, buffer_size);
                }
                c = DAGGER_GETCHAR_UNLOCKED_DAGGER();
            } while(c != EOF and c != '\n' and c != ' ' and c != '\t');
            str_arg.append(buffer_input, buffer_length);
            #undef DAGGER_GETCHAR_UNLOCKED_DAGGER
            return *this;
        }
        template <typename S, typename T>
        inline const user_input& operator >>(std::pair<S, T>& pair_arg) const{
            (*this) >> pair_arg.first >> pair_arg.second; return *this;
        }
        template <typename T>
        inline const user_input& operator >>(std::vector<T>& vec) const {
            for(T& ele : vec) (*this) >> ele; 
            return *this;
        }
    } cin;

    void ends(void) {putchar('\0'); }
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
        inline const user_output& operator << (const std::string& str_arg) const {
            fputs(str_arg.c_str(), stdout); return *this;
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

/*
#include <cstdio>
#include <vector>
#include <cstdint>

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
*/