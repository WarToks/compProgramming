#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <complex>
#include <cmath>
#include <cstring>
#include <deque>
#include <functional>
#include <map>
#include <initializer_list>
#include <iostream>
#include <random>
#include <queue>
#include <string>
#include <set>
#include <stack>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

 
template <typename S, typename T> inline bool chmin(S& a, const T b){ if(b < a){ a = b; return true; } return false; }
template <typename S, typename T> inline bool chmax(S& a, const T b){ if(a < b){ a = b; return true; } return false; }
template <typename T> inline bool bitUP(const T state, const unsigned int k) { return (state >> k) & 0b01; }

inline bool isIn(int x, int y, int H, int W){return 0 <= x and x < H and 0 <= y and y < W;}
inline auto isInTheGrid(const int H, const int W){
    return std::bind(isIn, std::placeholders::_1, std::placeholders::_2, H, W);
}

template <class element_t> 
std::vector<element_t> make_vector(const unsigned int n, const element_t& init_value){ return std::vector<element_t>(n, init_value); }
template <class element_t, class ...Tp>
auto make_vector(const unsigned int n, const unsigned int m, Tp... args){
    return std::vector<decltype(make_vector<element_t>(m, args...))>(n, make_vector<element_t>(m, args...));
}

namespace MyInputAndOutput{
    // 入力関係 (cin)
    class user_input{
    private:
        static constexpr unsigned int sizeOfAscii = 128;
        bool isBlankChar[sizeOfAscii];
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
        // 符号あり整数を getchar で読み込んで構成する (もし, 0 ~ 9 以外の文字が含まれると困る)
        template <typename Tp>
        inline void charToInteger(Tp& val) const {
            val = 0; int c; while(true){
                c = DAGGER_GETCHAR_UNLOCKED_DAGGER();
                if(c == EOF) return;
                if('-' or (not isBlankChar[c])) break;
            }
            if(c == '-'){
                while(true){
                    c = DAGGER_GETCHAR_UNLOCKED_DAGGER();
                    if(c < '0' or c > '9') break;
                    val = 10 * val + c - '0';
                } val = -val;
            }
            else{
                if(c < '0' or c > '9') return;
                do{
                    val = 10 * val + c - '0';
                    c = DAGGER_GETCHAR_UNLOCKED_DAGGER();
                } while('0' <= c and c <= '9');
            }
        }
        // 符号なし整数を getchar で読み込んで構成する (もし, 符号付きだとバグる)
        template <typename Tp>
        inline void charToUnsignedInteger(Tp& val) const {
            val = 0; int c; while(true){
                c = DAGGER_GETCHAR_UNLOCKED_DAGGER();
                if(c == EOF) return;
                if(not isBlankChar[c]) break;
            }
            if(c < '0' or c > '9') return;
            do{
                val = 10 * val + c - '0';
                c = DAGGER_GETCHAR_UNLOCKED_DAGGER();
            } while(not (c == EOF or isBlankChar[c]));
        }
    public:
        constexpr user_input(void) : isBlankChar(){
            for(unsigned int i = 0; i < sizeOfAscii; ++i) isBlankChar[i] = false;
            isBlankChar[int('\n')] = true; isBlankChar[int('\t')] = true; 
            isBlankChar[int(' ')]  = true; isBlankChar[int('\v')] = true;
        }
        inline const user_input& operator >> (int& int_arg) const {
            charToInteger<int>(int_arg); return *this;
        }
        inline const user_input& operator >> (long long int& llint_arg) const {
            charToInteger<long long int>(llint_arg); return *this;
        }
        inline const user_input& operator >> (unsigned int& uint_arg) const {
            charToUnsignedInteger<unsigned int>(uint_arg); return *this;
        }
        inline const user_input& operator >> (unsigned long long int& ullint_arg) const {
            charToUnsignedInteger<unsigned long long int>(ullint_arg); return *this;
        }
        inline const user_input& operator >> (unsigned long& ulint_arg) const {
            charToUnsignedInteger<unsigned long>(ulint_arg); return *this;
        }
        inline const user_input& operator >> (float& float_arg) const {
            scanf("%f", &float_arg); return *this;
        }
        inline const user_input& operator >> (double& double_arg) const {
            scanf("%lf", &double_arg); return *this;
        }
        inline const user_input& operator >> (long double& ldouble_arg) const {
            scanf("%Lf", &ldouble_arg); return *this;
        }
        inline const user_input& operator >> (char* str_arg) const {
            scanf("%s", str_arg); 
            return *this;
        }
        inline const user_input& operator >> (char& char_arg) const {
            do{
                if((char_arg = DAGGER_GETCHAR_UNLOCKED_DAGGER()) == EOF) return *this;
            } while(isBlankChar[int(char_arg)]);
            return *this;
        }
        inline const user_input& operator >> (std::string& str_arg) const {
            str_arg.erase();
            int c; while(true){ 
                c = DAGGER_GETCHAR_UNLOCKED_DAGGER();
                if(c == EOF) return *this;
                if(not isBlankChar[c]) break;
            }
            constexpr unsigned int buffer_size = 128; 
            char buffer_input[buffer_size]; unsigned int buffer_length = 0;
            do{
                buffer_input[buffer_length++] = c;
                if(buffer_length == buffer_size){
                    buffer_length = 0; str_arg.append(buffer_input, buffer_size);
                }
                c = DAGGER_GETCHAR_UNLOCKED_DAGGER();
            } while(c != EOF and (not isBlankChar[c]) );
            str_arg.append(buffer_input, buffer_length);
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
        // getchar の define の解除
        #undef DAGGER_GETCHAR_UNLOCKED_DAGGER
    }; constexpr user_input cin;

    void ends(void) {putchar('\0'); }
    void endl(void) {putchar('\n'); fflush(stdout);}
    void flush(void) {fflush(stdout);}
    constexpr char eol = '\n';

    // 出力関係 (cout)
    class user_output{
    public:
        constexpr user_output(void){}
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
        inline const user_output& operator << (const float flt_arg) const {
            printf("%.16f", flt_arg); return *this;
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
    }; constexpr user_output cout;

    // その他出力関数
    template <typename Integer = unsigned long long int>
    void binary_output(const Integer value, const unsigned int length = 64){
        char out[length + 1];
        for(unsigned int i = 0, j = length - 1; i < length; ++i, --j) out[j] = ((value >> i) & 1) ? '1' : '0';
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

// z-algorithm
template <class T> std::vector<unsigned int> z_algorithm(const T &str) {
    const size_t n = str.size();
    std::vector<unsigned int> resOfCP(n); resOfCP[0] = n;
    int i = 1, j = 0;
    while (i < n) {
        while (i + j < n and str[j] == str[i + j]) ++j;
        resOfCP[i] = j;
        if (j == 0) { ++i; continue;}
        int k = 1;
        while (i + k < n and k + resOfCP[k] < j) resOfCP[i + k] = resOfCP[k], ++k;
        i += k; j -= k;
    }
    return resOfCP;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// グリッド上の縦横移動 
constexpr std::array<std::pair<int, int>, 4> dxdy = {
    { {1, 0}, {-1, 0}, {0, 1}, {0, -1} }
};

// 型エイリアス
using unint = unsigned int;
using lli = long long int;
using ulli = unsigned long long int;
using pii = std::pair<int, int>;

#define REP(i, n) for(unsigned int i = 0; i < n; ++i)
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

using MIO::cin; using MIO::cout; using MIO::eol;

int main(void){

    return 0;
}