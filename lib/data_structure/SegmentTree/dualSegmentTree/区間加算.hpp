// verify : http://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=4229072#1
// verify : https://atcoder.jp/contests/abc153/submissions/10509594

#include <cstdio>
#include <vector>
#include <cassert>

// 区間加算 & 点取得
template <typename element_t, const element_t id>
class RangeAddTree{
private:
    unsigned int size;
    std::vector<element_t> data;

    static unsigned int lsb(unsigned int x){
        #ifdef __has_builtin
            return __builtin_ctz(x);
        #else 
            unsigned int res = 32;
            if(x << 16){ x <<= 16; res -= 16; }
            if(x << 8){ x <<= 8; res -= 8; }
            if(x << 4){ x <<= 4; res -= 4; }
            if(x << 2){ x <<= 2; res -= 2; }
            return (x >> 31) ? res - 1: res;
        #endif
    }

    static unsigned int msb(unsigned int x){
        #ifdef __has_builtin
            return 31 - __builtin_clz(x);
        #else 
            unsigned int res = 0;
            if(x >> 16) { x >>= 16; res += 16; }
            if(x >> 8) { x >>= 8; res += 8; }
            if(x >> 4) { x >>= 4; res += 4; }
            if(x >> 2) { x >>= 2; res += 2; }
            return res + (x >> 1);
        #endif
    }

    inline void propagate(unsigned int idx){
        // if(idx == 0) return;
        const unsigned int boundary = lsb(idx);
        for(unsigned int i = msb(idx); i > boundary; --i){
            unsigned j = idx >> i;
            data[j << 1] += data[j]; data[(j << 1) | 1U] += data[j];
            data[j] = id;
        }
    }

    void initialize(const std::vector<element_t>& A){
        const unsigned int n = A.size();
        size = 1; while(size < n) size <<= 1;
        data.assign(size << 1, id);
        for(unsigned int i = 0, j = size; i < n; ++i, ++j) data[j] = A[i];
    }
    template <class InputIterator>
    void initialize(const InputIterator first, const InputIterator last){
        const unsigned int n = last - first;
        size = 1; while(size < n) size <<= 1;
        data.assign(size << 1, id);
        InputIterator itr = first;
        for(unsigned int i = size; itr != last; ++i, ++itr) data[i] = *itr;
    }
public:
    // コンストラクタ
    RangeAddTree(const unsigned int n){
        size = 1; while(size < n) size <<= 1;
        data.assign(size << 1, id);
    }
    RangeAddTree(const std::vector<element_t>& A){ initialize(A); }
    template <class InputIterator>
    RangeAddTree(const InputIterator first, const InputIterator last){
        initialize(first, last);
    }

    // 区間更新 : 半開区間[l, r) に値 val を加える (0-indexed)  | 計算量O(logN)
    inline void update(unsigned int l, unsigned int r, const element_t& val){
        if(l >= r) return;
        propagate(l += size); propagate((r += size) - 1);
        while(l < r){
            if(l & 1U) data[l++] += val; l >>= 1;
            if(r & 1U) data[--r] += val; r >>= 1;
        }
    }
    // 点取得 : idx番目の値を取得 (0-indexed) | 計算量 O(logN)
    inline element_t operator [](unsigned int idx) const {
        element_t res = data[idx += size];
        while(idx >>= 1) res += data[idx];
        return res;
    }
    inline element_t at(unsigned int idx) const {
        assert(0 <= idx and idx < size);
        return this[idx];
    }
};


int main(void){
    unsigned int n, q; scanf("%u%u", &n, &q);
    RangeAddTree<unsigned int, 0> RAQ(n);
    while(q--){
        char c; scanf("%*c%c", &c);
        if(c == '0'){
            unsigned int s, t, x; scanf("%u%u%u", &s, &t, &x); s--;
            RAQ.update(s, t, x);
        }
        else{
            unsigned int idx; scanf("%u", &idx);
            printf("%u\n", RAQ[--idx]);
        }
    }
    return 0;
}