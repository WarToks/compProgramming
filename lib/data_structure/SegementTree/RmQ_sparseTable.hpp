// verified in https://judge.yosupo.jp/submission/4245
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
    inline unsigned int getLogOf(unsigned int x) const noexcept {
        return 31 - __builtin_clz(x);
    }

public:
    // コンストラクタ
    RangeMinimumQuery_static(std::vector<element_t>& A, const element_t inf):inf(inf){ init(A); }
    // 初期化
    void init(std::vector<element_t>& A){
        length = A.size(); logLen = 0;
        while((1U << logLen) < length) ++logLen;
        data.assign(length, std::vector<element_t>(logLen + 1));
        for(unsigned int i = 0 ; i < length; ++i) data[i][0] = A[i];
        for(unsigned int k = 0 ; k < logLen; ++k){
            for(unsigned int i = 0; i < length; ++i){
                unsigned int j = i + (1U << k); if(j > length - 1) j = length - 1;
                data[i][k+1] = Compare_t()(data[i][k], data[j][k]) ? data[i][k] : data[j][k];
            }
        }
    }
    
    // [l, r) の最小値を返す
    inline element_t getMin(unsigned l, unsigned r) const noexcept {
        if(l >= r) return inf;
        unsigned int idx = getLogOf(r - l);
        return Compare_t()(data[l][idx], data[r - (1U << idx)][idx]) ? data[l][idx] : data[r - (1U << idx)][idx];
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