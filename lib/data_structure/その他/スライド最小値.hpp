// verifty : http://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=4229757#1
#include <cstdio>
#include <vector>
#include <deque>

// < スライド最小値 >
// スライド最小値を用いて, 固定した サイズ k と 配列 A に対し
// 各 i = 0, 1, ..., n に対し A[i, i + k) の最小値を O(N) で求める関数
template <typename element_t>
std::vector<element_t> slideMinimum(const std::vector<element_t>& A, unsigned int k){
    const unsigned int n = A.size();
    if(n == 0) return std::vector<element_t>(0);

    if(n < k) k = n;
    std::vector<element_t> res(n); // 返り値
    // インデックス列 : インデックスは単調増加であり, A[インデックス] も単調増加になる
    std::deque<unsigned int> deq;
    unsigned int i, j;
    for(i = 0; i < k; ++i){
        while((not deq.empty()) and A[i] <= A[deq.back()]) deq.pop_back(); 
        deq.push_back(i);
    }
    for(j = 0; i < n; ++i, ++j){
        res[j] = A[deq.front()]; if(j == deq.front()) deq.pop_front();
        while((not deq.empty()) and A[i] <= A[deq.back()]) deq.pop_back();
        deq.push_back(i);
    }
    for(; j < n; ++j){ res[j] = A[deq.front()]; if(j == deq.front()) deq.pop_front();}
    return res;
}




// < スライド最大値 >
// スライド最大値を用いて, 固定した サイズ k と 配列 A に対し
// 各 i = 0, 1, ..., n に対し A[i, i + k) の最小値を O(N) で求める関数
template <typename element_t>
std::vector<element_t> slideMaximum(const std::vector<element_t>& A, unsigned int k){
    const unsigned int n = A.size();
    if(n == 0) return std::vector<element_t>(0);
    
    if(n < k) k = n;
    std::vector<element_t> res(n); // 返り値
    // インデックス列 : インデックスは単調増加であり, A[インデックス] も単調増加になる
    std::deque<unsigned int> deq;
    unsigned int i, j;
    for(i = 0; i < k; ++i){
        while((not deq.empty()) and A[i] >= A[deq.back()]) deq.pop_back(); 
        deq.push_back(i);
    }
    for(j = 0; i < n; ++i, ++j){
        res[j] = A[deq.front()]; if(j == deq.front()) deq.pop_front();
        while((not deq.empty()) and A[i] >= A[deq.back()]) deq.pop_back();
        deq.push_back(i);
    }
    for(; j < n; ++j){ res[j] = A[deq.front()]; if(j == deq.front()) deq.pop_front();}
    return res;
}






// < 抽象スライド最小値 >
// スライド最小値を用いて, 固定した サイズ k と 配列 A に対し
// 各 i = 0, 1, ..., n に対し A[i, i + k) の最小値を O(N) で求める関数
template <typename element_t, class func_type = std::less_equal<element_t>>
std::vector<element_t> slideMinimum(const std::vector<element_t>& A, unsigned int k){
    const unsigned int n = A.size();
    if(n == 0) return std::vector<element_t>(0);
    
    if(n < k) k = n;
    std::vector<element_t> res(n); // 返り値
    // インデックス列 : インデックスは単調増加であり, A[インデックス] も単調増加になる
    std::deque<unsigned int> deq;
    unsigned int i, j;
    for(i = 0; i < k; ++i){
        while((not deq.empty()) and func_type()(A[i], A[deq.back()]) ) deq.pop_back(); 
        deq.push_back(i);
    }
    for(j = 0; i < n; ++i, ++j){
        res[j] = A[deq.front()]; if(j == deq.front()) deq.pop_front();
        while((not deq.empty()) and func_type(A[i], A[deq.back()]) ) deq.pop_back();
        deq.push_back(i);
    }
    for(; j < n; ++j){ res[j] = A[deq.front()]; if(j == deq.front()) deq.pop_front();}
    return res;
}
