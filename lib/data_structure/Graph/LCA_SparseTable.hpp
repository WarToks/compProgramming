// verified in : https://judge.yosupo.jp/submission/4296
#include <cstdio>
#include <vector>
#include <tuple>
#include <stack>
#include <cassert>
#include "../../IO/IO.hpp"


// LCAのクラス : {重みの型, 無限の重み} : SparseTableを使って <O(NlogN), O(1)?>
// 初期化を忘れずに！！！！
template <typename distance_type, const distance_type inf_dist>
class LowestCommonAncestor{
private:
    using edge_type = std::pair<unsigned int, distance_type>;

    unsigned int numberOfVertex;              // 頂点の数
    unsigned int theRoot;                     // 根
    std::vector<std::vector<edge_type>> edge; // 枝 : {向かう頂点, 重み}
    std::vector<unsigned int> depth;          // 根からの深さ
    std::vector<distance_type> distance;      // 根からの距離

    std::vector<unsigned int> eulerTour;      // オイラーツアー
    std::vector<unsigned int> fstVisit;       // 初めて訪れたもののオイラーツアー上のインデックス

    // {depth, eulerTour}
    using rmq_type = std::pair<unsigned int, unsigned int>;
    std::vector<std::vector<rmq_type>> RmQ_data; // RmQ の内部

    bool doneInitialized;                     // initialize が行われているかどうか

    static constexpr edge_type inf_edge = edge_type(inf_dist, UINT32_MAX);

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
            return res +  (x >> 1);
        #endif
    }
    // [l, r) の最小値を返す
    inline rmq_type getMin(unsigned int l, unsigned int r) const noexcept {
        const unsigned int idx = getLogOf(r - l);
        const unsigned int rr  = r - (1U << idx);
        return (RmQ_data[idx][l] < RmQ_data[idx][rr]) ? RmQ_data[idx][l] : RmQ_data[idx][rr];
    }
public:
    // コンストラクタ : 初期化を忘れずに！
    LowestCommonAncestor(const unsigned int n): numberOfVertex(n), theRoot(0){
        edge.assign(n, std::vector<edge_type>(0));
        doneInitialized = false;
    }

    // 辺の追加(0-indexed) : u <-> v  with weight cost 
    inline void push(unsigned int u, unsigned int v, const distance_type cost = 1){
        edge[u].emplace_back(v, cost); edge[v].emplace_back(u, cost);
    }

    // 2点 u, v (0-indexed) の LCA を取得する関数 ; O(1)? | 初期化の後に用いる
    inline unsigned int getLCA(unsigned int u, unsigned int v) const noexcept {

        assert(doneInitialized); // 初期化が行われているかの確認

        if(u == v) return u;
        unsigned int indexOfU = fstVisit[u],
                     indexOfV = fstVisit[v];
        if(indexOfU > indexOfV) std::swap(indexOfU, indexOfV);
        return getMin(indexOfU, indexOfV + 1).second;
    }

    // パスの重みを計算する関数 ; O(1)? | 初期化の後に用いる
    inline distance_type distanceBetween(unsigned int u, unsigned int v) const noexcept {
        unsigned int lca = getLCA(u, v);
        return distance[u] + distance[v] - 2 * distance[lca];
    }
    // 根を設定
    void setRoot(unsigned int newRoot){
        theRoot = newRoot; doneInitialized = false;
    }

    // 初期化 : O(NlogN)
    void initialize(void){
        // ----------------- オイラーツアー部分 -----------------------
        // 初期化
        eulerTour.clear(); eulerTour.reserve(2 * numberOfVertex);
        fstVisit.assign(numberOfVertex, UINT32_MAX);
        depth.assign(numberOfVertex, UINT32_MAX);
        distance.assign(numberOfVertex, inf_dist);

        // オイラーツアー本体 : DFS を行う
        unsigned int idx = 0;
        std::stack<unsigned int, std::vector<unsigned int>> Stk;
        Stk.push(theRoot); depth[theRoot] = 0; distance[theRoot] = 0;
        while(not Stk.empty()){
            unsigned int v = Stk.top(); Stk.pop();
            eulerTour.push_back(v);
            if(fstVisit[v] != UINT32_MAX){ idx++; continue; }
            fstVisit[v] = idx++;
            unsigned int nv; distance_type cost;
            for(const edge_type& an_edge : edge[v]){
                std::tie(nv, cost) = an_edge;
                if(depth[nv] == UINT32_MAX){
                    Stk.push(v); Stk.push(nv);
                    depth[nv] = depth[v] + 1;
                    distance[nv] = distance[v] + cost;
                }
            }
        }
        
        // ----------------- RmQ の構成部分 -----------------------
        const unsigned int length = eulerTour.size();
        unsigned int logLen = 0;
        while((1U << logLen) < length) ++logLen;
        RmQ_data.assign(logLen + 1, std::vector<rmq_type>(length));
        for(unsigned int i = 0; i < length ; ++i) RmQ_data[0][i] = rmq_type(depth[eulerTour[i]], eulerTour[i]);
        for(unsigned int k = 0; k < logLen ; ++k){
            const unsigned int boundary = length - (1U << k); // 2^k ≤ length
            for(unsigned int i = 0, j = (1U << k); i < boundary; ++i, ++j){
                RmQ_data[k + 1][i] = (RmQ_data[k][i] < RmQ_data[k][j]) ? RmQ_data[k][i] : RmQ_data[k][j];
            }
            for(unsigned int i = boundary; i < length; ++i){
                RmQ_data[k + 1][i] = (RmQ_data[k][i] < RmQ_data[k][length - 1]) ? RmQ_data[k][i] : RmQ_data[k][length - 1];
            }
        }
        doneInitialized = true;
    }
};
/*
int main(void){
    unsigned int n, q; MIO::cin >> n >> q;
    LowestCommonAncestor<int, INT32_MAX> LCA(n);
    for(unsigned int i = 1; i < n; ++i){
        unsigned int p; MIO::cin >> p; LCA.push(i, p);
    }
    LCA.initialize();
    while(q--){
        unsigned int u, v; MIO::cin >> u >> v;
        MIO::cout << LCA.getLCA(u, v) << '\n';
    }
    return 0;
}
*/