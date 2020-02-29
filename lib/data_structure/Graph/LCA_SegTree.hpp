#include <cstdio>
#include <utility>
#include <vector>

// LCAのクラス : {重みの型, 無限の重み} : SegTreeで <O(N), O(logN)>
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
    unsigned int segLength;         // セグ木の長さ
    static constexpr rmq_type inf_rmq  = rmq_type(UINT32_MAX, UINT32_MAX); // rmqの単位元
    std::vector<rmq_type> RmQ_data; // RmQ の内部

    bool doneInitialized;                     // initialize が行われているか

    static constexpr edge_type inf_edge = edge_type(inf_dist, UINT32_MAX);

    // [l, r) の最小値を返す : O(logN)
    inline rmq_type getMin(unsigned int l, unsigned int r) const noexcept {
        rmq_type res = inf_rmq;
        for(unsigned int ll = l + segLength, rr = r + segLength; ll < rr; ll >>= 1, rr >>= 1){
            if(ll & 1U){
                if(res > RmQ_data[ll]) res = RmQ_data[ll]; ll++;
            }
            if(rr & 1U){
                --rr; if(res > RmQ_data[rr]) res = RmQ_data[rr];
            }
        }
        return res;
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

    // 2点 u, v (0-indexed) の LCA を取得する関数 ; O(logN) | 初期化の後に用いる
    inline unsigned int getLCA(unsigned int u, unsigned int v) const noexcept {

        assert(doneInitialized); // 初期化が行われているかの確認

        if(u == v) return u;
        unsigned int indexOfU = fstVisit[u],
                     indexOfV = fstVisit[v];
        if(indexOfU > indexOfV) std::swap(indexOfU, indexOfV);
        return getMin(indexOfU, indexOfV + 1).second;
    }

    // パスの重みを計算する関数 ; O(logN) | 初期化の後に用いる
    inline distance_type distanceBetween(unsigned int u, unsigned int v) const noexcept {
        unsigned int lca = getLCA(u, v);
        return distance[u] + distance[v] - 2 * distance[lca];
    }

    void setRoot(unsigned int newRoot){
        theRoot = newRoot; doneInitialized = false;
    }

    // 初期化 : O(N)
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
        segLength = 1; while(segLength < length) segLength *= 2;
        RmQ_data.resize(2 * segLength);
        for(unsigned int i = 0     ; i < length   ; ++i) RmQ_data[segLength + i] = rmq_type(depth[eulerTour[i]], eulerTour[i]);
        for(unsigned int i = length; i < segLength; ++i) RmQ_data[segLength + i] = inf_rmq;
        for(unsigned int i = segLength - 1;  i > 0   ; --i){
            RmQ_data[i] = (RmQ_data[2 * i] < RmQ_data[2 * i + 1]) ? RmQ_data[2 * i] : RmQ_data[2 * i + 1];
        }
        doneInitialized = true;
    }
};