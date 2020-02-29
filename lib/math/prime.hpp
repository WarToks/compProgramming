#include <cstdio>

template <size_t N> class Primes{
private:
    unsigned int size_;
    /*
    std::bitset<N + 1> isPrime_;
    std::array<int, N + 1> prime_; */
    bool isPrime_[N + 1];
    int  prime_[N + 1];
public:
    // コンストラクタ
    constexpr Primes(void): isPrime_(), prime_(), size_(0U){
        isPrime_[0] = false; isPrime_[1] = false;
        prime_[size_++] = 2;
        for(int i = 2; i <= N; ++i) isPrime_[i] = (i & 1);
        for(int i = 3; i <= N; i += 2) if(isPrime_[i]){
            prime_[size_++] = i;
            for(int j = 3 * i; j <= N; j += (i << 1)) isPrime_[j] = false;
        }
    }

    // 範囲for文用の begin() と end()
    const int *begin(void) const noexcept{ return prime_; }
    const int *end(void) const noexcept{ return prime_ + size_;}
    
    // メソッド
    const bool isPrime(int number) const {return isPrime_[number];} // 素数か判定する
    const unsigned int size(void) const noexcept {return size_;} // 素数の数
    const int at(int idx) const noexcept {return prime_[idx];}   // i番目の素数
};

constexpr int N = 100;
constexpr Primes<N> P;

int main(void){
    for(auto v : P) printf("%d\n", v);
    return 0;
}