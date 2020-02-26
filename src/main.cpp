#include <iostream>
#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <cstring>
#include <deque>
#include <functional>
#include <initializer_list>
#include <math.h>
#include <map>
#include <random>
#include <set>
#include <stack>
#include <tuple>
#include <type_traits>
#include <queue>
#include <vector>


using lli = long long int;
using pii = std::pair<int, int>;

// グリッド上の縦横移動
constexpr std::array<std::pair<int, int>, 4> dxdy = {
    { {1, 0}, {-1, 0}, {0, 1}, {0, -1} }
};


template <typename T> inline bool chmin(T& a, T b){ if(b < a){ a = b; return true; } return false; }
template <typename T> inline bool chmax(T& a, T b){ if(a < b){ a = b; return true; } return false; }

inline bool isIn(int x, int y, int H, int W){return 0 <= x and x < H and 0 <= y and y < W;}
inline auto isInTheGrid(const int H, const int W){
    return std::bind(isIn, std::placeholders::_1, std::placeholders::_2, H, W);
}
inline bool bitUP(int state, int k){ return (state >> k) & 1; }
inline bool bitUP(long long int state, int k){ return (state >> k) & 1;}


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

// z-algorithm
template <class T> std::vector<int> z_algorithm(const T &str) {
    const size_t n = str.size();
    std::vector<int> resOfCP(n); resOfCP[0] = n;
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



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


int main(void){

    return 0;
}