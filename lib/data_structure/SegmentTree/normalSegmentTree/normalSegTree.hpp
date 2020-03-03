// verify : http://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=4226257#1
// verify : http://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=4226280#1

#include <vector>
#include <functional>

// 点更新セグメント木　< 構築O(N), 点取得 O(1), 範囲取得 O(logN) >
template <typename element_t, const element_t id, class func_type = std::plus<element_t>>
class SegmentTree{
private:
    unsigned int size;
    std::vector<element_t> data;

    void initialize(const std::vector<element_t>& A){
        const unsigned int n = A.size();
        size = 1; while(size < n) size <<= 1; data.resize(2 * size);
        unsigned int i, j;
        for(i = 0, j = size; i < n; ++i, ++j) data[j] = A[i];
        for(; i < size; ++i, ++j) data[j] = id;
        for(j = size - 1; j ; --j) data[j] = func_type()(data[j << 1], data[(j << 1) | 1]);
    }
    template <class InputIterator>
    void initialize(const InputIterator first, const InputIterator last){
        const unsigned int n = last - first;
        size = 1; while(size < n) size <<= 1; data.resize(2 * size);
        unsigned int i, boundary = 2 * size;
        InputIterator itr = first;
        for(i = size; itr != last; ++i, ++itr) data[i] = *itr;
        for(; i < boundary; ++i) data[i] = id;
        for(i = size - 1; i ; --i) data[i] = func_type()(data[i << 1], data[(i << 1) | 1]);
    }
public:
    // ----------------- コンストラクタ ----------------------- 
    SegmentTree(const unsigned int n = 0){
        size = 1; while(size < n) size <<= 1; data.assign(2 * size, id);
    }
    SegmentTree(const std::vector<element_t>& A){
        initialize(A);
    }
    template <class InputIterator>
    SegmentTree(const InputIterator first, const InputIterator last){
        initialize(first, last);
    }
    // ----------------- resize ----------------------- 
    void resize(const unsigned int n){
        size = 1; while(size < n) size <<= 1; data.assign(2 * size, id);
    }
    template <class InputIterator>
    void resize(const InputIterator first, const InputIterator last){
        initialize(first, last);
    }

    // idx番目(0-indexed)の値 を val に変更する : O(logN)
    void update(unsigned int idx, const element_t& val){
        data[idx += size] = val;
        while(idx >>= 1) data[idx] = func_type()(data[idx << 1], data[(idx << 1) | 1]);
    }
    // [l, r) (0-indexed) の値を取得する
    element_t query(unsigned int l, unsigned int r) const {
        element_t vl = id, vr = id;
        l += size; r += size;
        while(l < r){
            if(l & 1U) vl = func_type()(vl, data[l++]); l >>= 1;
            if(r & 1U) vr = func_type()(data[--r], vr); r >>= 1;
        }
        return func_type()(vl, vr);
    }

    // idx番目(0-indexed)の値を(点)取得
    const element_t& at(const unsigned int idx) const {
        return data.at(idx + size);
    }
    const element_t& operator [](const unsigned int idx) const noexcept{
        return data[idx + size];
    }
};
