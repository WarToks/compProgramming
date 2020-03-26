// verify : https://judge.yosupo.jp/submission/6323
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
constexpr NumberTheoreticTransform<3, 30, 5> NTT;

int main(void){
    unsigned int n; cin >> n;

    std::vector<unsigned int> A(n+1), B(n+1); A[0] = 0; B[0] = 0;
    for(unsigned int i = 1; i <= n; ++i) cin >> A[i] >> B[i];

    const auto v = NTT.convolve(A, B);
    for(unsigned int i = 1; i <= 2 * n; ++i) cout << v[i].value << eol;

    return 0;
}