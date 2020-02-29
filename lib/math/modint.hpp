#include <cassert>
template <unsigned int modulus> struct modint{
    static_assert(modulus > 0, "modulus must be a positive integer.");
    int val; // 値本体

    // コンストラクタ
    constexpr modint():val(0){}
    template <class T> constexpr modint(T v){
        val = v % (int)(modulus); 
        if(val < 0) val += modulus;
    }
    constexpr modint& operator+= (const modint<modulus> other) noexcept {
        val += other.val; if(val >= modulus) val -= modulus;
        return *this;
    }
    constexpr modint& operator -= (const modint<modulus> other) noexcept {
        val -= other.val; if(val < 0) val += modulus;
        return *this;
    }
    constexpr modint& operator *= (const modint<modulus> other) noexcept {
        val = (unsigned long long int)(val) * other.val % modulus;
        return *this; 
    }
    constexpr modint& operator /= (const modint<modulus> other) noexcept {
        val = (unsigned long long int)(val) * other.inverse() % modulus;
        return *this;
    }
    constexpr modint operator +(const modint<modulus> v)const noexcept{
        return modint<modulus>(*this) += v;
    }
    constexpr modint operator -(const modint<modulus> v)const noexcept{
        return modint<modulus>(*this) -= v;
    }
    constexpr modint operator *(const modint<modulus> v)const noexcept{
        return modint<modulus>(*this) *= v;
    }
    constexpr modint operator /(const modint<modulus> v)const noexcept{
        return modint<modulus>(*this) /= v;
    }
    constexpr modint& operator ++(void) noexcept {
        if(++val == modulus) val = 0;
        return *this;
    }
    constexpr modint& operator --(void) noexcept {
        if(val-- == 0) val = modulus - 1;
        return *this;
    }
    constexpr modint operator -()const noexcept{
        return modint<modulus>((val == 0 ? 0 : modulus - val));
    }

    // aの逆元を求める関数
    constexpr unsigned int inverse(unsigned int a) const noexcept {
        unsigned int b = modulus;
                 int u = 1,   v = 0;
        while (b != 0) {
            unsigned int t = a / b; 
            a -= t * b; std::swap(a, b);
            u -= t * v; std::swap(u, v);
        }
        if(u < 0) u += modulus;
        return u;
    }
    constexpr unsigned int inverse(void) const noexcept {return inverse(val);}
    // a^nを返す関数 : nは負の数でも可
    constexpr unsigned int power(unsigned long long int a, long long int n)const noexcept{
        if(n == 0) return 1;
        unsigned long long res = 1, waiting = a;
        if(n < 0) waiting = inverse(), n = -n;
        while(n != 0){ 
            if((n & 1UL) != 0){ res *= waiting; res %= modulus; }
            waiting *= waiting; waiting %= modulus; 
            n /= 2;
        }
        return res;
    }
    constexpr unsigned int power(long long int n) const noexcept {return power(val, n);}
};