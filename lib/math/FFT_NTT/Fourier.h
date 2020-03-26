// フーリエ変換を愚直に行う関数 複素数列{Ai} から Âm を計算する関数
template <class precision = double>
std::complex<precision> fourier_transform(const std::vector<std::complex<precision>>& A, unsigned int m){
    using number_type = std::complex<precision>; // 複素数の型
    constexpr number_type pi = std::acos<precision>(-1); // 円周率
    const unsigned int n = A.size(); // 配列の長さ

    number_type res = 0; // 返り値
    const number_type omega = std::exp(number_type(0, - (2 * pi * m) / n)); // (w_n)^(-m) = exp(-2π(m/n)i)
    number_type w = 1;
    for(unsigned int i = 0; i < n; ++i, w *= omega) res += A[i] * w;
    return res;
}

// フーリエ逆変換を愚直に行う関数 複素数列{Âi} から A を計算する関数
template <class precision = double>
std::complex<precision> fourier_inverse_transform(const std::vector<std::complex<precision>>& A, unsigned int m){
    using number_type = std::complex<precision>; // 複素数の型
    constexpr number_type pi = std::acos<precision>(-1); // 円周率
    const unsigned int n = A.size(); // 配列の長さ

    number_type res = 0; // 返り値
    const number_type omega = std::exp(number_type(0, (2 * pi * m) / n)); // (w_n)^m = exp(2π(m/n)i)
    number_type w = 1;
    for(unsigned int i = 0; i < n; ++i, w *= omega) res += A[i] * w;
    return res;
}



template <class precision = double>
class FFT{
private:
    using number_type = std::complex<precision>;

    static constexpr precision PI_VALUE = static_cast<precision>(3.1415926535897932384626433832795028841971);

    std::vector<number_type> omega;
    unsigned int omega_k; //omega[idx] = exp(i * idx π/2^(omega_k))

    static unsigned int bitreverse(unsigned int n){
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
    
    // omegaを設定
    void set_omega(const unsigned int k){
        if(omega_k == k) return;
        omega_k = k;
        const unsigned int N = 1 << k; omega.assign(N, 1);
        for(unsigned int k = 1, Nk = N; k < N; k <<= 1, Nk >>= 1){
            const precision theta = PI_VALUE / Nk;
            const number_type w( std::cos(theta), -std::sin(theta) );

            for(unsigned int i = k; i < N; i += k) for(unsigned int j = 0; j < k; ++j) omega[i++] *= w;
        }
    }
public:

    // 2^nの長さの配列に対し高速フーリエ変換を実行する関数 : 2^n ≤ A.size() である必要がある
    std::vector<number_type> fft(const std::vector<number_type>& A, const unsigned int n){
        const unsigned int N = 1 << n; 
        assert(A.size() <= N);

        std::vector<number_type> res(N, 0);{
            const unsigned int len = A.size(); 
            for(unsigned int i = 0; i < len; ++i) res[i] = A[i]; // 0 埋め
        }
        set_omega(n);

        for(unsigned int i = 0; i < N; ++i){
            unsigned int j = bitreverse(i) >> (32 - n);
            if(i < j){ std::swap(res[i], res[j]);/* std::swap(omega[i], omega[j]); */}
        }

        for(unsigned int len1 = 1, len2 = 2; len1 < N; len1 <<= 1, len2 <<= 1){
            for(unsigned int begin_idx1 = 0, begin_idx2 = len1; begin_idx1 < N; begin_idx1 += len2, begin_idx2 += len2){
                for(unsigned int idx = 0; idx < len1; ++idx){
                    const unsigned int t = (N / len1) * idx;
                    res[begin_idx2 + idx] = res[begin_idx1 + idx] - omega[t] * res[begin_idx2 + idx];
                    res[begin_idx1 + idx] = res[begin_idx1 + idx] + res[begin_idx1 + idx] -       res[begin_idx2 + idx];
                }
            }
        }
        return res;
    }

    // {Ai}_{i=0}^{n-1}, {Bi}_{i=0}^{m-1} に対して畳み込み
    // Ck = ∑_{i + j = k} Ai * Bj を計算する
    std::vector<number_type> convolve(const std::vector<number_type>& A, const std::vector<number_type>& B){
        const unsigned int n = A.size(); const unsigned int m = B.size();
        // (n - 1) + (m - 1) ≤ 2^k - 1 : n + m - 1 ≤ 2^k
        unsigned int k = 0; { const unsigned int degree = n + m - 1; while((1U << k) < degree) ++k; }
        const unsigned int K = 1 << k; 
        std::vector<number_type> A2 = fft(A, k), B2 = fft(B, k);
        for(unsigned int i = 0; i < K; ++i) A2[i] = std::conj(A2[i] * B2[i]);
        A2 = fft(A2, k);
        const precision waru = 1.0 / K;
        for(unsigned int i = 0; i < K; ++i) A2[i] = std::conj(A2[i]) * waru;
        return A2;
    }
    std::vector<precision> convolve(const std::vector<precision>& A, const std::vector<precision>& B){
        const unsigned int n = A.size(); const unsigned int m = B.size();
        unsigned int k = 0; { const unsigned int degree = n + m - 1; while((1U << k) < degree) ++k; }
        const unsigned int K = 1 << k; 
        std::vector<number_type> C(K, 0); 
        for(unsigned int i = 0; i < n; ++i) C[i].real(A[i]);
        for(unsigned int i = 0; i < m; ++i) C[i].imag(B[i]);
        C = fft(C, k); std::vector<number_type> D(K); 
        D[0] = C[0].real() * C[0].imag();
        constexpr precision divide_four = 1.0 / 4;
        for(unsigned int i = 1, j = K - 1; i < K; ++i, --j){
            const number_type&& tmp = ( std::conj(C[i] * C[i]) - (C[j] * C[j]) ) * divide_four;
            D[i].real(-tmp.imag()); D[i].imag( tmp.real());
        }
        D = fft(D, k);
        const precision waru = 1.0 / K;
        std::vector<precision> res(K);
        for(unsigned int i = 0; i < K; ++i) res[i] = D[i].real() * waru;
        return res;
    }
    FFT(void):omega_k(0){}
};