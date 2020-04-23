#include <iostream>
#include <string>
#include <chrono>

/* 時間計測
    括弧でProcessingTimeをローカル変数にして, 以下のようにして時間計測を行う
    {
        ProcessingTime Pt;
        // 計測したいコード
    }
*/
class  ProcessingTime{
private:
    const std::string name;
    const std::chrono::system_clock::time_point start;
public:
    ProcessingTime(const std::string&& name = "Process"): 
                    name(std::move(name)), start(std::chrono::system_clock::now()){
    }
    ~ProcessingTime(void){
        const auto end = std::chrono::system_clock::now();
        const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds >(end - start).count();
        std::cout << name << " : " << elapsed << " ms\n";
    }
};























class reroot_structure{
public:
    using element_t = ; // 各頂点の情報(DP)
    using weight_t = ; // 辺の重み

    // (子ノードのdp値) x (親ノードへの辺の重み) → dp値への寄与
    static element_t add_weight(const element_t& child_dp, const weight_t& weight){}
    // 上の重みを付け加えて伝搬する関数の 重みの単位元
    static weight_t id_weight(void){}

    // (各子ノードのdp値の寄与)をマージする関数 (二項演算)
    static element_t merge(const element_t& child_dp1, const element_t& child_dp2){}
    // 上のマージ関数の単位元
    static element_t id_merge(void) {}
};

// 全方位木DPクラス : build関数を忘れないこと！
template <class reroot_structure_type> 
class ReRooting{
private:
    const unsigned int n;       // 頂点の数
    const unsigned int theRoot; // 根

    using element_t = reroot_structure_type::element_t;
    using weight_t  = reroot_structure_type::weight_t;
    // 辺の型
    struct edge_t{
        unsigned int to;  // 頂点
        weight_t weight; // 辺の重み
        edge_t( const unsigned int to, 
                const weight& weight = reroot_structure_type::id_weight())
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
    void dfs1(void){
        dp.assign(n, reroot_structure_type::id_merge());
        SL.assign(n, std::vector<element_t>(0)); 
        SR.assign(n, std::vector<element_t>(0));
        for(unsigned int i = n - 1, cntr = 0; cntr < n; --i, ++cntr){
            const unsigned int v = order[i]; unsigned int idx = 0;
            const unsigned int sz = (v == theRoot) ? edge[v].size() : edge[v].size() - 1;
            if(sz == 0) continue;
            SL[v].resize(sz); SR[v].resize(sz);
            // 子ノードからボトムアップに dp値 を受け取る
            for(const edge_t& x : edge[v]) if(x.to != parent[v]){ 
                SL[v][idx++] = reroot_structure_type::add_weight(dp[x.to], x.weight);
            }
            // 右累積和 (i, size) を計算
            SR[sz - 1] = reroot_structure_type::id_merge(); 
            for(unsigned int j1 = sz - 2, j2 = sz - 1; j2 > 0; --j1, --j2){
                SR[v][j1] = reroot_structure_type::merge(SL[v][j2], SR[v][j2]);
            }
            // 左累積和 [0, i) を計算
            for(unsigned int j = 0; j < sz; ++j){
                std::swap(dp[v], SL[v][j]); dp[v] = reroot_structure_type::merge(SL[v][j], dp[v]);
            }
        }
    }
    void dfs2(void){
        for(unsigned int i = 0; i < n; ++i){
            const unsigned int v = order[i]; unsigned int idx = 0;
            for(const edge_t& x : edge[v]) if(x.to != parent[v]){
                const element_t&& tmp = reroot_structure_type::merge(SL[v][idx], SR[v][idx]);
                const element_t&& tmp2 = reroot_structure_type::add_weight(tmp, x.weight);
                dp[x.to] = reroot_structure_type::merge(dp[x.to], tmp2);
                idx++;
            }
        }
    }

public:
    ReRooting(const unsigned int n, const element_t& id):n(n), id(id){ edge.assign(n, std::vector<edge_t>(0)); }
    // (u - v) の無向辺 の 追加 (0-index)
    inline void add(const unsigned int u, const unsigned int v, const element_t& weight = element_t()){
        edge[u].emplace(v, weight); edge[v].emplace(u, weight);
    }
    // 実行
    inline void build(void){ make_order(); dfs1(); dfs2(); }
    inline const element_t& operator [](const unsigned int idx) const { return dp[idx]; }
};



std::istream& operator >> (std::istream& is, std::vector<char>& str){
    using size_type_vec = std::vector<char>::size_type;
    char buf[128];
    size_type_vec len = 0, extracted = 0;
    std::istream::ios_base::iostate err = std::istream::ios_base::iostate(std::istream::ios_base::goodbit);
    const std::streamsize w_size = is.width();
    const size_type_vec n = (w_size > 0) ? static_cast<size_type_vec>(w_size) : str.max_size();
    auto* sb = is.rdbuf();
    char cr = static_cast<char>(sb -> sgetc());
    // 空白文字とかは適宜
    while(extracted < n and !(cr == EOF or cr == ' ' or cr == '\n')){
        if(len == sizeof(buf) / sizeof(char)){
            str.insert(str.end(), buf, buf + 128);
            len = 0;
        }
        buf[len++] = cr;
        cr = static_cast<char>(sb -> snextc());
        ++extracted;
    } str.insert(str.end(), buf, buf + len);
    if(cr == EOF or (!extracted)) err |= std::ios_base::eofbit;
    is.width(0);
    if(err) is.setstate(err); 
    return is;
}
std::ostream& operator << (std::ostream& os, const std::vector<char>& str){
    const std::streamsize w_size = os.width(), n_size = str.size();
    if(w_size > n_size){
        const bool left = os.flags() & std::ostream::ios_base::adjustfield) == std::ostream::ios_base::left;
        if(!left){
            const char cr = os.fill();
            std::streamsize lp_size = w_size - n_size;
            while(lp_size > 0){
                const std::char_traits<char>::int_type put_val = os.rdbuf()->sputc(cr);
                if(std::char_traits<char>::eq_int_type(put_val, std::char_traits<char>::eof())){
                    os.setstate(std::ostream::badbit); break;
                } --lp_size;
            }
        }
        if(os.good()){
            const std::streamsize put_val = os.rdbuf()->sputn(__s, __n)
            if (put_val != __n) os.setstate(std::ostream::ios_base::badbit);
        }
    }
    else{

    }
    return os;
}