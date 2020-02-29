// verified in https://judge.yosupo.jp/submission/4294
#include <cstdio>
#include <vector>
#include <functional>
#include "../IO/IO.hpp"

// 静的なRmQ ; SparseTable | 構築 O(NlogN), クエリ O(1) 
// element_t := 要素の型, Compare_t := 比較関数
template <typename element_t, class Compare_t = std::less<element_t>>
class RangeMinimumQuery_static{
private:
    const element_t inf;   // 無限大

    unsigned int length;   // the length of array
    unsigned int logLen;   // <=> { 2^k ≥ length となる最大の数k }
    std::vector<std::vector<element_t>> data;

    // 2^k ≤ x なる最大の k を返す
    static unsigned int getLogOf(unsigned int x) noexcept {
        #ifdef __has_builtin
            return 31 - __builtin_clz(x);
        #else
            unsigned int res = 0;
            if(x >> 16){x >>= 16; res += 16;}
            if(x >> 8) {x >>= 8; res += 8;}
            if(x >> 4) {x >>= 4; res += 4;}
            if(x >> 2) {x >>= 2; res += 2;}
            return res + (x >> 1);
        #endif
    }

public:
    // コンストラクタ
    RangeMinimumQuery_static(const std::vector<element_t>& A, const element_t inf):inf(inf){ 
        initialize(A.cbegin(), A.cend());
    }
    template <class InputIterator>
    RangeMinimumQuery_static(InputIterator first, InputIterator last, const element_t inf):inf(inf) {
        initialize(first, last);
    }
    // 初期化
    template <class InputIterator>
    void initialize(const InputIterator first, const InputIterator last){
        length = last - first; logLen = 0;
        while((1U << logLen) < length) ++logLen;
        data.assign(logLen + 1, std::vector<element_t>(length));
        data[0].assign(first, last);
        for(unsigned int k = 0 ; k < logLen; ++k){
            const unsigned int boundary = length - (1U << k); // 2^k ≤ length
            for(unsigned int i = 0, j = (1U << k); i < boundary; ++i, ++j) {
                data[k+1][i] = Compare_t()(data[k][i], data[k][j]) ? data[k][i] : data[k][j];
            }
            for(unsigned int i = boundary; i < length; ++i){
                data[k+1][i] = Compare_t()(data[k][i], data[k][length-1]) ? data[k][i] : data[k][length];
            }
        }
    }
    
    // [l, r) の最小値を返す
    inline element_t getMin(unsigned l, unsigned r) const noexcept {
        if(l >= r) return inf;
        const unsigned int idx = getLogOf(r - l);
        const unsigned int rr  = r - (1U << idx);
        return Compare_t()(data[idx][l], data[idx][rr]) ? data[idx][l] : data[idx][rr];
    }
};


/*
int main(void){
    int n, q; MIO::cin >> n >> q;
    std::vector<unsigned int> A(n); MIO::cin >> A;
    RangeMinimumQuery_static ST(A, UINT32_MAX);
    while(q--){
        unsigned int l, r; MIO::cin >> l >> r;
        MIO::cout << ST.getMin(l, r) << '\n';
    }
    return 0;
}
*/