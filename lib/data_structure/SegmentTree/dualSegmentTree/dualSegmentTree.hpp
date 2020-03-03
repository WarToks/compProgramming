// verify : http://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=4226770#1
// verify : http://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=4226781#1

#include <cstdio>
#include <vector>
#include <functional>
#include <cassert>

// 双対セグメント木 : 区間更新点取得
// i ∈ [l, r) に関して xi ← f(xi) に変更する
// f(x ; op) := x operator op
template <typename element_t, const element_t id, class func_type = std::plus<element_t>>
class dualSegmentTree{
private:
    unsigned int height;
    unsigned int size;
    std::vector<element_t> data;

    inline void propagate(unsigned int idx){
        unsigned int i, j;
        for(i = height; i ; --i){
            j = idx >> i;
            data[j << 1] = func_type()(data[j << 1], data[j]);
            data[(j << 1) | 1U] = func_type()(data[(j << 1) | 1U], data[j]);
            data[j] = id;
        }
    }
public:
    dualSegmentTree(const unsigned int n = 1){
        size = 1, height = 0; while(size < n) size <<= 1, ++height;
        data.assign(size << 1, id);
    }
    dualSegmentTree(const unsigned int n, const element_t& val){
        size = 1, height = 0; while(size < n) size <<= 1, ++height;
        data.assign(size << 1, id);
        for(unsigned int i = 0, j = size; i < n; ++i, ++j) data[j] = val;
    }
    template <class InputIterator>
    dualSegmentTree(const InputIterator first, const InputIterator last){
        const unsigned int n = last - first;
        size = 1, height = 0; while(size < n) size <<= 1, ++height;
        data.assign(size << 1, id);
        InputIterator itr = first;
        for(unsigned int j = size; itr != last; ++j, ++itr) data[j] = *itr;
    }
    // 区間更新 : O(logN)
    inline void update(unsigned int l, unsigned int r, const element_t& op){
        propagate(l + size); propagate(r + size - 1);
        for(l += size, r += size; l < r; l >>= 1, r >>= 1){
            if(l & 1U) { data[l] = func_type()(data[l], op); l++; }
            if(r & 1U) { --r; data[r] = func_type()(data[r], op); }
        }
    }

    // 点取得 : 計算量 O(logN)
    inline element_t operator [](unsigned int idx) const {
        element_t res = id; 
        for(idx += size; idx ; idx >>= 1) res = func_type()(res, data[idx]);
        return res;
    }
    inline element_t at(unsigned int idx) const {
        assert(0 <= idx and idx < size);
        element_t res = id;
        for(idx += size; idx ; idx >>= 1) res = func_type()(res, data[idx]);
        return res;
    }
};

// 代入 : rhs == id ?  lhs : rhs
template <typename element_t>
struct assignment{
    element_t operator()(const element_t& lhs, const element_t& rhs){
        return rhs == UINT32_MAX ? lhs : rhs;
    }
};

int main(void){
    unsigned int n, q; scanf("%u%u", &n, &q);
    dualSegmentTree<unsigned int, UINT32_MAX, assignment<unsigned int>> S(n, INT32_MAX);
    while(q--){
        char c; scanf("%*c%c", &c);
        switch(c){
            case '0':
                {
                    unsigned int l, r, x; scanf("%u%u%u", &l, &r, &x); ++r;
                    S.update(l, r, x);
                }
                break;
            case '1':
                {
                    unsigned int i; scanf("%u", &i);
                    printf("%u\n", S[i]);
                }
                break;
        }
    }
    return 0;
}