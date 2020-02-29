#include <cstdio>
// ref : https://cp-algorithms.com/algebra/prime-sieve-linear.html


template <size_t N> struct shuumai{
    /*
    std::array<unsigned int, N + 1> minPrime;
    std::array<unsigned int, N + 1> Next;
    std::array<unsigned int, N + 1> Primes; */
    unsigned int minPrime[N + 1];
    unsigned int Next[N + 1];
    unsigned int Primes[N + 1];
    unsigned int size_;

    constexpr shuumai(void): minPrime(), Primes(), Next(), size_(0U){
        minPrime[0] = 1; minPrime[1] = 1;
        for(int even = 2, odd = 3; odd <= N; even += 2, odd += 2){
            minPrime[even] = 2;
            minPrime[odd] = odd;
        }
        if(!(N & 1)) minPrime[N] = 2;
        Primes[size_++] = 2;
        
        for(unsigned int x = 3; x <= N; x += 2){
            if(minPrime[x] == x) Primes[size_++] = x;
            for(int y = 0; y < size_ and Primes[y] <= minPrime[x] and x * Primes[y] <= N; ++y)
                    minPrime[x * Primes[y]] = Primes[y];
        }
        for(unsigned  int x = N; x >= 2; --x) Next[x] = x / minPrime[x];
    }
    // 範囲for文用の begin() と end()
    const unsigned int *begin(void) const noexcept{ return Primes; }
    const unsigned int *end(void) const noexcept{ return Primes + size_;}
    const unsigned int size(void) const {return size_;}
};


constexpr int N = 100;
constexpr shuumai<N> D;

int main(void){
    for(int prime : D) printf("%d\n", prime);
    
    return 0;
}