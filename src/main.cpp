#include <iostream>
#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <cstring>
#include <complex>
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

#define REP(i, n) for(int (i) = 0; (i) < (n); ++(i))

// グリッド上の縦横移動
constexpr std::array<std::pair<int, int>, 4> dxdy = {
    { {1, 0}, {-1, 0}, {0, 1}, {0, -1} }
};

template <typename T> inline bool chmin(T& a, const T b){ if(b < a){ a = b; return true; } return false; }
template <typename T> inline bool chmax(T& a, const T b){ if(a < b){ a = b; return true; } return false; }
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
            //scanf("%d", &int_arg);
            charToInteger<int>(int_arg); return *this;
        }
        inline const user_input& operator >> (long long int& llint_arg) const {
            //scanf("%lld", &llint_arg); return *this;
            charToInteger<long long int>(llint_arg); return *this;
        }
        inline const user_input& operator >> (unsigned int& uint_arg) const {
            //scanf("%u", &uint_arg); 
            charToUnsignedInteger<unsigned int>(uint_arg); return *this;
        }
        inline const user_input& operator >> (unsigned long long int& ullint_arg) const {
            //scanf("%llu", &ullint_arg);
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

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

using MIO::cin; using MIO::cout; using MIO::eol;


template <const unsigned int modulus>
struct ModInt {
private:
    constexpr unsigned long long int power(const unsigned long long int a, unsigned int n) const {
        if(n == 0) return 1;
        unsigned long long int res = 1, waiting = a;
        while(n){ 
            if(n & 1) (res *= waiting) %= modulus; 
            (waiting *= waiting) %= modulus; 
            n >>= 1;
        }
        return res;
    }
    constexpr unsigned long long int inverse(const unsigned long long int a) const { return power(a, modulus - 2); }
public:
    unsigned long long int value;
    constexpr ModInt(void):value(0){}
    template <class element_t> constexpr ModInt(const element_t& val):value(){ 
        const long long int tmp = val % modulus; value = (tmp >= 0) ? tmp : tmp + modulus; 
    }
    constexpr ModInt<modulus>& operator += (const ModInt<modulus>& other){
        if((value += other.value) >= modulus) value -= modulus; return *this;
    }
    constexpr ModInt<modulus>& operator -= (const ModInt<modulus>& other){
        if(value < other.value) value += modulus - other.value; else value -= other.value;
        return *this;
    }
    constexpr ModInt<modulus>& operator *= (const ModInt<modulus>& other){
        (value *= other.value) %= modulus; return *this;
    }
    constexpr ModInt<modulus>& operator /= (const ModInt<modulus>& other){
        (value *= inverse(other.value)) %= modulus; return *this;
    }
    constexpr bool operator == (const ModInt<modulus>& other) const { return value == other.value; }
    constexpr bool operator != (const ModInt<modulus>& other) const { return value != other.value; }
    constexpr ModInt<modulus> operator + (const ModInt<modulus>& other) const { return ModInt<modulus>(*this) += other; }
    constexpr ModInt<modulus> operator - (const ModInt<modulus>& other) const { return ModInt<modulus>(*this) -= other; }
    constexpr ModInt<modulus> operator * (const ModInt<modulus>& other) const { return ModInt<modulus>(*this) *= other; }
    constexpr ModInt<modulus> operator / (const ModInt<modulus>& other) const { return ModInt<modulus>(*this) /= other; }
    constexpr ModInt<modulus> power(const unsigned int n) const { return ModInt<modulus>(power(value, n)); }
    constexpr ModInt<modulus> inverse(void) const { return ModInt<modulus>(inverse(value)); }
};

// p = coefficient * 2^index + 1 型 素数 → 原始根 w (<=> w^i ≠ 1 (1 ≤ i < p - 1), w^(p-1) = 1)
template <const unsigned int coefficient, const unsigned int index, const unsigned int origin>
class NumberTheoreticTransform {
private:
    static constexpr unsigned int mod = (coefficient << index) + 1;
    using mod_int = ModInt<mod>;
    static constexpr mod_int origin_mod = mod_int(origin);

    static_assert(0 < origin and origin < mod, "the origin element must be smaller than mod and positive.");

    // p = k * 2^n + 1 : 原始根 w
    // omega[i] := w ただし w^(2^i) = 1 となる数 = w^(k * 2^(n-i))　<->
    mod_int omega[index + 1];

    constexpr unsigned int bitreverse(unsigned int n) const {
        #ifdef __has_builtin
            #if __has_builtin(__builtin_bitreverse32)
                return __builtin_bitreverse32(n);
            #else
                n = ((n & 0x55555555) << 1) | ((n & 0xAAAAAAAA) >> 1); // 1ビットの交換
                n = ((n & 0x33333333) << 2) | ((n & 0xCCCCCCCC) >> 2);
                n = ((n & 0x0F0F0F0F) << 4) | ((n & 0xF0F0F0F0) >> 4);
                n = ((n & 0x00FF00FF) << 8) | ((n & 0xFF00FF00) >> 8);
                n = (n << 16) | (n >> 16);
                return n;
            #endif
        #else
            n = ((n & 0x55555555) << 1) | ((n & 0xAAAAAAAA) >> 1); // 1ビットの交換
            n = ((n & 0x33333333) << 2) | ((n & 0xCCCCCCCC) >> 2);
            n = ((n & 0x0F0F0F0F) << 4) | ((n & 0xF0F0F0F0) >> 4);
            n = ((n & 0x00FF00FF) << 8) | ((n & 0xFF00FF00) >> 8);
            n = (n << 16) | (n >> 16);
            return n;
        #endif
    }

    constexpr unsigned long long int power(const unsigned long long int a, unsigned int n) const {
        if(n == 0) return 1;
        unsigned long long int res = 1, waiting = a;
        while(n){ 
            if(n & 1) (res *= waiting) %= mod; 
            (waiting *= waiting) %= mod; 
            n >>= 1;
        }
        return res;
    }
    constexpr unsigned long long int inverse(const unsigned long long int a) const { return power(a, mod - 2); }
    template <typename element_t>
    std::vector<mod_int> ntt(const std::vector<element_t>& A, const unsigned int n) const {
        const unsigned int N = 1 << n; 
        assert(A.size() <= N);
        std::vector<mod_int> res(N);{
            const unsigned int len = A.size(); 
            for(unsigned int i = 0; i < len; ++i) res[i] = A[i];
        }
        for(unsigned int i = 0; i < N; ++i){
            unsigned int j = bitreverse(i) >> (32 - n);
            if(i < j) std::swap(res[i], res[j]);
        }

        for(unsigned int len1 = 1, len2 = 2, t = 1; len1 < N; len1 <<= 1, len2 <<= 1, ++t){
            const mod_int omegan = omega[t].inverse();
            for(unsigned int begin_idx1 = 0, begin_idx2 = len1; begin_idx1 < N; begin_idx1 += len2, begin_idx2 += len2){
                mod_int w = 1;
                for(unsigned int idx = 0, index_1 = begin_idx1, index_2 = begin_idx2; idx < len1; ++idx, ++index_1, ++index_2){
                    res[index_2] =                res[index_1] - w * res[index_2]; 
                    res[index_1] = res[index_1] + res[index_1] -     res[index_2];
                    w *= omegan;
                }
            }
        }
        return res;
    }

    std::vector<mod_int> ntt_inv(const std::vector<mod_int>& A, const unsigned int n) const {
        const unsigned int N = 1 << n; 
        assert(A.size() <= N);
        std::vector<mod_int> res(N);{
            const unsigned int len = A.size(); 
            for(unsigned int i = 0; i < len; ++i) res[i] = A[i];
        }
        for(unsigned int i = 0; i < N; ++i){
            unsigned int j = bitreverse(i) >> (32 - n);
            if(i < j) std::swap(res[i], res[j]);
        }

        for(unsigned int len1 = 1, len2 = 2, t = 1; len1 < N; len1 <<= 1, len2 <<= 1, ++t){
            for(unsigned int begin_idx1 = 0, begin_idx2 = len1; begin_idx1 < N; begin_idx1 += len2, begin_idx2 += len2){
                mod_int w = 1;
                for(unsigned int idx = 0, index_1 = begin_idx1, index_2 = begin_idx2; idx < len1; ++idx, ++index_1, ++index_2){
                    res[index_2] = res[index_1] - w * res[index_2];
                    res[index_1] = res[index_1] + res[index_1] - res[index_2];
                    w *= omega[t];
                }
            }
        }
        const mod_int inv_N = mod_int(N).inverse();
        for(unsigned int i = 0; i < N; ++i) res[i] *= inv_N;
        return res;
    }

public:
    // コンストラクタ
    constexpr NumberTheoreticTransform(void):omega(){
        mod_int w_k = origin_mod.power(coefficient);
        for(int i = index; i >= 0; --i){ omega[i] = w_k; w_k *= w_k; }
    } 
    // {ai}_{i=0}^{n-1}, {bj}_{j=0}^{m-1} の畳み込み 
    // { ck = ∑_{i+j = k} ai * bj }_{k = 0}^{n + m - 2} を計算
    template <typename element_t>
    std::vector<mod_int> convolve(const std::vector<element_t>& A, const std::vector<element_t>& B) const {
        const unsigned int n = A.size(), m = B.size();
        unsigned int k = 0, L = 1; while(L < (n + m - 1)) ++k, L <<= 1;
        std::vector<mod_int> A2 = ntt(A, k), B2 = ntt(B, k);
        for(unsigned int i = 0; i < L; ++i) A2[i] *= B2[i];

        return ntt_inv(A2, k);
    }
};

// 3 * 2^30 + 1 → 原始根 5
constexpr NumberTheoreticTransform<119, 23, 3> NTT;

int main(void){
    unsigned int n, m; cin >> n >> m;
    std::vector<unsigned int> A(n), B(m); cin >> A >> B;
    const auto v = NTT.convolve(A, B);
    for(unsigned int i = 0; i < n + m - 1; ++i){
        cout << v[i].value << (i == n + m - 2 ? '\n' : ' ');
    }

    return 0;
}