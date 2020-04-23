// verify : 

// ReRootに渡す構造
class reroot_structure{
public:
    using element_t = std::pair<mint, mint>; // 各頂点の情報(DP)
    using weight_t = char; // 辺の重み

    // (子ノードのdp値) x (親ノードへの辺の重み) → dp値への寄与 : ≒ dpの漸化式があった時に, その'引数' を変化させるのにも用いることが可能
    static element_t add_weight(const element_t& child_dp, const weight_t& weight){ return child_dp; }
    // 上の重みを付け加えて伝搬する関数の 重みの単位元
    static constexpr weight_t id_weight(void){ return EOF; }

    // (各子ノードのdp値の寄与)をマージする関数 (二項演算) : 結合律を満たす演算
    static element_t merge(const element_t& child_dp1, const element_t& child_dp2){
        const mint count = child_dp1.second + child_dp2.second;
        return element_t(child_dp1.first * child_dp2.first * Cmb.combination(count.val, child_dp2.second.val), count);
    }
    // 上のマージ関数の単位元
    static constexpr element_t id_merge(void){
        return element_t(mint(1), mint(0));
    }

    // 頂点に来た時の更新関数
    static constexpr element_t update(const element_t& dp_value){
        return element_t(dp_value.first, dp_value.second + mint(1));
    }
};

// ReRootに渡す構造
class reroot_structure{
public:
    using element_t = std::pair<mint, mint>; // 各頂点の情報(DP)
    using weight_t = char; // 辺の重み

    // (子ノードのdp値) x (親ノードへの辺の重み) → dp値への寄与 : ≒ dpの漸化式があった時に, その'引数' を変化させるのにも用いることが可能
    static element_t add_weight(const element_t& child_dp, const weight_t& weight){}
    // 上の重みを付け加えて伝搬する関数の 重みの単位元
    static weight_t id_weight(void){}

    // (各子ノードのdp値の寄与)をマージする関数 (二項演算) : 結合律を満たす演算
    static element_t merge(const element_t& child_dp1, const element_t& child_dp2){}
    // 上のマージ関数の単位元
    static element_t id_merge(void){}

    // 頂点に来た時の更新関数
    static element_t update(const element_t& dp_value){}
};

// 全方位木DPクラス : build関数を忘れないこと！
template <class reroot_structure_type> 
class ReRooting{
private:
    const unsigned int n;       // 頂点の数
    const unsigned int theRoot; // 根

    using element_t = typename reroot_structure_type::element_t;
    using weight_t  = typename reroot_structure_type::weight_t;
    // 辺の型
    struct edge_t{
        unsigned int to;  // 頂点
        weight_t weight; // 辺の重み
        edge_t( const unsigned int to = 0, 
                const weight_t& weight = reroot_structure_type::id_weight())
                : to(to), weight(weight){}
    };

    std::vector<element_t> dp;
    std::vector<std::vector<edge_t>> edge;  // 枝集合
    std::vector<unsigned int> order;        // 計算順序 (DFSの順序が格納されている)
    std::vector<unsigned int> parent;       // 親ノード
    std::vector<std::vector<element_t>> SL; // 左累積和
    std::vector<std::vector<element_t>> SR; // 右累積和
    
    // DFS で 全ての頂点の深さを計算する & 計算順序を構成
    void make_order(void){
        std::vector<bool> yet(n, true);
        order.resize(n); parent.resize(n); 

        std::vector<unsigned int> stk; stk.reserve(n);
        unsigned int idx = 0;
        yet[theRoot] = false;
        stk.emplace_back(theRoot); order[idx++] = theRoot;
        while(not stk.empty()){
            unsigned int v = stk.back(); stk.pop_back();
            for(const edge_t& x : edge[v]) if(yet[x.to]){
                yet[x.to] = false;
                parent[x.to] = v; order[idx++] = x.to;
                stk.emplace_back(x.to);
            }
        }
    }
    void dfs(void){
        dp.assign(n, reroot_structure_type::id_merge());
        SL.assign(n, std::vector<element_t>(0)); SR.assign(n, std::vector<element_t>(0));
        for(unsigned int i = 0; i < n; ++i){
            SL[i].reserve(edge[i].size()); SR[i].reserve(edge[i].size());
        }

        for(unsigned int i = n - 1; i > 0; --i){
            const unsigned int v = order[i]; const unsigned int nv = parent[v];
            for(const edge_t& x: edge[v]) if(x.to == nv){
                dp[v]  = reroot_structure_type::add_weight(reroot_structure_type::update(dp[v]), x.weight);
                SL[nv].push_back(dp[v]); SR[nv].push_back(dp[v]);
                dp[nv] = reroot_structure_type::merge(dp[v], dp[nv]);
                break;
            }
        }

        {
            const unsigned int v = order[0];
            const unsigned int sz = SL[v].size();
            dp[v] = reroot_structure_type::update(dp[v]);
            element_t vl = reroot_structure_type::id_merge(), vr = reroot_structure_type::id_merge();
            for(unsigned int j1 = 0, j2 = sz - 1; j1 < sz; ++j1, --j2){
                SR[v][j1] = reroot_structure_type::merge(vr, SR[v][j1]); std::swap(SR[v][j1], vr);
                SL[v][j2] = reroot_structure_type::merge(vl, SL[v][j2]); std::swap(SL[v][j2], vl);
            }
            unsigned int idx = sz - 1;
            for(const edge_t& x : edge[v]){
                const element_t tmp = reroot_structure_type::add_weight(
                                                    reroot_structure_type::update(
                                                            reroot_structure_type::merge(SL[v][idx], SR[v][idx])
                                                    ), 
                                                    x.weight
                                    );
                dp[x.to] = reroot_structure_type::merge(tmp, dp[x.to]);
                SL[x.to].push_back(tmp); SR[x.to].push_back(tmp);
                --idx;
            }
        }
        for(unsigned int i = 1; i < n; ++i){
            const unsigned int v = order[i];
            const unsigned int sz = SL[v].size();
            // dp[v] = reroot_structure_type::update(dp[v]);
            element_t vl = reroot_structure_type::id_merge(), vr = reroot_structure_type::id_merge();
            for(unsigned int j1 = 0, j2 = sz - 1; j1 < sz; ++j1, --j2){
                SR[v][j1] = reroot_structure_type::merge(vr, SR[v][j1]); std::swap(SR[v][j1], vr);
                SL[v][j2] = reroot_structure_type::merge(vl, SL[v][j2]); std::swap(SL[v][j2], vl);
            }
            dp[v] = reroot_structure_type::update(vl);
            unsigned int idx = sz - 2;
            for(const edge_t& x : edge[v]) if(x.to != parent[v]){
                const element_t tmp = reroot_structure_type::add_weight(
                                    reroot_structure_type::update(
                                            reroot_structure_type::merge(SL[v][idx], SR[v][idx])
                                    ), 
                                    x.weight
                    );
                dp[x.to] = reroot_structure_type::merge(tmp, dp[x.to]);
                SL[x.to].push_back(tmp); SR[x.to].push_back(tmp);
                --idx;
            }
        }
    }

public:
    ReRooting(const unsigned int n, const unsigned int theRoot = 0):n(n), theRoot(theRoot){ edge.assign(n, std::vector<edge_t>(0)); }
    // (u - v) の無向辺 の 追加 (0-index)
    inline void add(const unsigned int u, const unsigned int v, const weight_t& weight = reroot_structure_type::id_weight()){
        edge[u].emplace_back(v, weight);
    }
    // 実行
    inline void build(void){ make_order(); dfs(); }
    inline const element_t& operator [](const unsigned int idx) const { return dp[idx]; }
};


/*
// 全方位木DPクラス : build関数を忘れないこと！
template <class reroot_structure_type> 
class ReRooting{
private:
    const unsigned int n;       // 頂点の数
    const unsigned int theRoot; // 根

    using element_t = typename reroot_structure_type::element_t;
    using weight_t  = typename reroot_structure_type::weight_t;
    // 辺の型
    struct edge_t{
        unsigned int to;  // 頂点
        weight_t weight; // 辺の重み
        edge_t( const unsigned int to = 0, 
                const weight_t& weight = reroot_structure_type::id_weight())
                : to(to), weight(weight){}
    };

    std::vector<element_t> dp;
    std::vector<std::vector<edge_t>> edge;  // 枝集合
    std::vector<unsigned int> order;        // 計算順序 (DFSの順序が格納されている)
    std::vector<unsigned int> parent;       // 親ノード
    std::vector<std::vector<element_t>> SL; // 左累積和
    std::vector<std::vector<element_t>> SR; // 右累積和
    
    // DFS で 全ての頂点の深さを計算する & 計算順序を構成
    void make_order(void){
        std::vector<bool> yet(n, true);
        order.resize(n); parent.resize(n); 

        std::vector<unsigned int> stk; stk.reserve(n);
        unsigned int idx = 0;
        yet[theRoot] = false;
        stk.emplace_back(theRoot); order[idx++] = theRoot;
        while(not stk.empty()){
            unsigned int v = stk.back(); stk.pop_back();
            for(const edge_t& x : edge[v]) if(yet[x.to]){
                yet[x.to] = false;
                parent[x.to] = v; order[idx++] = x.to;
                stk.emplace_back(x.to);
            }
        }
    }
    void dfs(void){
        dp.assign(n, reroot_structure_type::id_merge());
        SL.assign(n, std::vector<element_t>(0)); SR.assign(n, std::vector<element_t>(0));
        for(unsigned int i = 0; i < n; ++i){
            SL[i].reserve(edge[i].size()); SR[i].reserve(edge[i].size());
        }

        for(unsigned int i = n - 1; i > 0; --i){
            const unsigned int v = order[i]; const unsigned int nv = parent[v];
            dp[v]  = reroot_structure_type::update(dp[v]);
            SL[nv].push_back(dp[v]); SR[nv].push_back(dp[v]);
            dp[nv] = reroot_structure_type::merge(dp[v], dp[nv]);
        }

        {
            const unsigned int v = order[0];
            const unsigned int sz = SL[v].size();
            dp[v] = reroot_structure_type::update(dp[v]);
            element_t vl = reroot_structure_type::id_merge(), vr = reroot_structure_type::id_merge();
            for(unsigned int j1 = 0, j2 = sz - 1; j1 < sz; ++j1, --j2){
                SR[v][j1] = reroot_structure_type::merge(SR[v][j1], vr); std::swap(SR[v][j1], vr);
                SL[v][j2] = reroot_structure_type::merge(vl, SL[v][j2]); std::swap(SL[v][j2], vl);
            }
            unsigned int idx = sz - 1;
            for(const edge_t& x : edge[v]){
                const element_t tmp = reroot_structure_type::update(reroot_structure_type::merge(SL[v][idx], SR[v][idx]));
                dp[x.to] = reroot_structure_type::merge(tmp, dp[x.to]);
                SL[x.to].push_back(tmp); SR[x.to].push_back(tmp);
                --idx;
            }
        }
        for(unsigned int i = 1; i < n; ++i){
            const unsigned int v = order[i];
            const unsigned int sz = SL[v].size();
            // dp[v] = reroot_structure_type::update(dp[v]);
            element_t vl = reroot_structure_type::id_merge(), vr = reroot_structure_type::id_merge();
            for(unsigned int j1 = 0, j2 = sz - 1; j1 < sz; ++j1, --j2){
                SR[v][j1] = reroot_structure_type::merge(SR[v][j1], vr); std::swap(SR[v][j1], vr);
                SL[v][j2] = reroot_structure_type::merge(vl, SL[v][j2]); std::swap(SL[v][j2], vl);
            }
            dp[v] = reroot_structure_type::update(vl);
            unsigned int idx = sz - 2;
            for(const edge_t& x : edge[v]) if(x.to != parent[v]){
                const element_t tmp = reroot_structure_type::update(reroot_structure_type::merge(SL[v][idx], SR[v][idx]));
                dp[x.to] = reroot_structure_type::merge(tmp, dp[x.to]);
                SL[x.to].push_back(tmp); SR[x.to].push_back(tmp);
                --idx;
            }
        }
    }
public:
    ReRooting(const unsigned int n, const unsigned int theRoot = 0):n(n), theRoot(theRoot){ edge.assign(n, std::vector<edge_t>(0)); }
    // (u - v) の無向辺 の 追加 (0-index)
    inline void add(const unsigned int u, const unsigned int v, const weight_t& weight = reroot_structure_type::id_weight()){
        edge[u].emplace_back(v, weight);
    }
    // 実行
    inline void build(void){ make_order(); dfs(); }
    inline const element_t& operator [](const unsigned int idx) const { return dp[idx]; }
};*/