#include <stdio.h>
#include <algorithm>
#include <utility>

template <unsigned int modulus> struct modint{
    static_assert(modulus > 0, "modulus must be a positive integer.");
    int val; // 値本体

    // コンストラクタ
    constexpr modint():val(0){}
    template <class T> constexpr modint(T v){
        val = v % (int)(modulus); 
        if(val < 0) val += modulus;
    }
    constexpr modint& operator+= (const modint<modulus> other) noexcept {
        val += other.val; if(val >= modulus) val -= modulus;
        return *this;
    }
    constexpr modint& operator -= (const modint<modulus> other) noexcept {
        val -= other.val; if(val < 0) val += modulus;
        return *this;
    }
    constexpr modint& operator *= (const modint<modulus> other) noexcept {
        val = (unsigned long long int)(val) * other.val % modulus;
        return *this; 
    }
    constexpr modint& operator /= (const modint<modulus> other) noexcept {
        val = (unsigned long long int)(val) * other.inverse() % modulus;
        return *this;
    }
    constexpr modint operator +(const modint<modulus> v)const noexcept{
        return modint<modulus>(*this) += v;
    }
    constexpr modint operator -(const modint<modulus> v)const noexcept{
        return modint<modulus>(*this) -= v;
    }
    constexpr modint operator *(const modint<modulus> v)const noexcept{
        return modint<modulus>(*this) *= v;
    }
    constexpr modint operator /(const modint<modulus> v)const noexcept{
        return modint<modulus>(*this) /= v;
    }
    constexpr modint& operator ++(void) noexcept {
        if(++val == modulus) val = 0;
        return *this;
    }
    constexpr modint& operator --(void) noexcept {
        if(val-- == 0) val = modulus - 1;
        return *this;
    }
    constexpr modint operator -()const noexcept{
        return modint<modulus>((val == 0 ? 0 : modulus - val));
    }

    // aの逆元を求める関数
    constexpr unsigned int inverse(unsigned int a) const noexcept {
        unsigned int b = modulus;
                 int u = 1,   v = 0;
        while (b != 0) {
            unsigned int t = a / b; 
            a -= t * b; std::swap(a, b);
            u -= t * v; std::swap(u, v);
        }
        if(u < 0) u += modulus;
        return u;
    }
    constexpr unsigned int inverse(void) const noexcept {return inverse(val);}
    // a^nを返す関数 : nは負の数でも可
    constexpr unsigned int power(unsigned long long int a, long long int n)const noexcept{
        if(n == 0) return 1;
        unsigned long long res = 1, waiting = a;
        if(n < 0) waiting = inverse(), n = -n;
        while(n != 0){ 
            if((n & 1UL) != 0){ res *= waiting; res %= modulus; }
            waiting *= waiting; waiting %= modulus; 
            n /= 2;
        }
        return res;
    }
    constexpr unsigned int power(long long int n) const noexcept {return power(val, n);}
};

template <std::size_t N, unsigned int modulus>
struct combinationMod{
public:
    modint<modulus> Power[N + 1];
    modint<modulus> powInverse[N + 1];
    modint<modulus> inverse[N + 1];
//public:
    constexpr combinationMod(void): Power(), powInverse(), inverse(){
        Power[0] = 1;
        int x = 0; modint<modulus> mx;
        for(x = 1, mx = 1; x <= N; ++x, ++mx) Power[x] = Power[x - 1] * mx;
        powInverse[N] = Power[N].inverse();
        for(x = N, mx = N; x > 0; --x, --mx){
            powInverse[x - 1] = powInverse[x] * mx;
            inverse[x] = powInverse[x] * Power[x - 1];
        }
        inverse[0] = 1;
    }
    constexpr modint<modulus> combination(int n, int k){
        if(k < 0 or n < k) return modint<modulus>();
        return Power[n] * powInverse[k] * powInverse[n - k];
    }
};


constexpr int mod = 1e9 + 7;
constexpr int N = 1e5;
using mint = modint<mod>;
combinationMod<N, mod> Cmb;

int main(void){
    return 0;
}

/*
template <typename element_t, class Compare_t = std::less<element_t> >
class RBTree{
private:
    struct node{
    private:
        enum color_t : char {BLACK, RED, DUMMY_BEGIN, DUMMY_END};
        using tree_pointer = node*;
    public:
        element_t val;
        tree_pointer l_tree, r_tree, p_tree;
        unsigned int tree_size; // 木の頂点数
        color_t color;
        node(const element_t& val, const color_t color = BLACK, unsigned int tree_size = 1U): val(val), 
                                                                          l_tree(nullptr), 
                                                                          r_tree(nullptr), 
                                                                          p_tree(nullptr), 
                                                                          tree_size(tree_size),
                                                                          color(color){
        }
        node* predecessor(void) const {
            if(this -> color == DUMMY_BEGIN) return this; // beginの場合
            node* ptr;
            if(this -> l_tree == nullptr){
                ptr = this;
                while(ptr -> p_tree -> r_tree != ptr) ptr = ptr -> p_tree;
                return ptr -> p_tree;
            }
            // else
            ptr = this -> l_tree;
            while(ptr -> r_tree != nullptr) ptr = ptr -> r_tree;
            return ptr;
        }
        node* successor(void) const {
            if(this -> color == DUMMY_END) return this; // end の場合
            node* ptr;
            if(this -> r_tree == nullptr){
                ptr = this;
                while(ptr -> p_tree -> l_tree != ptr) ptr = ptr -> p_tree;
                return ptr -> p_tree;
            }
            ptr = this -> r_tree;
            while(ptr -> l_tree != nullptr) ptr = ptr -> l_tree;
            return ptr;
        }

        // expected 0-indexed ; 0-th, 1-th, ...
        const node* get_kth_element(int k) const {
            if(k < 0 or (this -> tree_size) <= k) return nullptr;
            node* ptr = this;
            while(true){
                unsigned int l_size = (ptr -> l_tree == nullptr) ? 0U : (ptr -> l_tree -> tree_size);
                if(k < l_size) ptr = ptr -> l_tree;
                else if(k == l_size) return ptr;
                else return ptr = ptr -> l_tree, k -= l_size;
            }
            return nullptr;
        }
        unsigned int size(void) const {return tree_size;}
    };
private:
    node* tree;
public:
    // コンストラクタ
    RBTree(void): tree(nullptr){}
    /*
    void insert(const element_t& val){
        if(tree == nullptr) tree = new node(val);
        else tree -> insert(val);
    } */
    unsigned int size(void) const {return (tree == nullptr) ? 0U : tree -> size();}

};
*/


/*
struct seg_monoid{
    using element_type  = int;
    using operator_type = int;

    struct element_structure{
        static inline constexpr element_type id = INT32_MAX;
        constexpr element_type operator() (element_type lhs, element_type rhs){  
        return lhs > rhs ? rhs : lhs;
        }
    };

    struct operator_structure{
        static inline constexpr operator_type id = INT32_MIN;     
        constexpr operator_type operator() (operator_type lhs, operator_type rhs){ 
            return rhs == id ? lhs : rhs;
        }
    };

    constexpr element_type operator()(element_type lhs, operator_type rhs){
        return rhs == operator_structure::id ? lhs : rhs;
    }
};

template <class seg_structure>
class lazy_segment_tree{
private:
    using element_t    = typename seg_structure::element_type;
    using operator_t   = typename seg_structure::operator_type;
    using ele_struct_t = typename seg_structure::element_structure;
    using ope_struct_t = typename seg_structure::operator_structure;

    size_t length;
    size_t height;
    std::vector<element_t>   data;
    std::vector<operator_t>  lazy;

    // lazy[idx] の情報を子ノードに伝搬する → dataの更新を行う
    void eval(size_t idx){
        if(lazy[idx] == ope_struct_t::id) return;
        if(idx < length){ 
            lazy[2 * idx    ] = ope_struct_t()(lazy[2 * idx],     lazy[idx]);
            lazy[2 * idx + 1] = ope_struct_t()(lazy[2 * idx + 1], lazy[idx]);
        }
        data[idx] = seg_structure()(data[idx], lazy[idx]);
        lazy[idx] = ope_struct_t::id;
    }

    // 遅延評価から
    void calculateData(size_t idx) {
        while((idx /= 2) > 0) data[idx] = ele_struct_t()(reflect(2 * idx), reflect(2 * idx + 1));
    }

    // 遅延評価を考慮した値を返す
    element_t reflect(size_t idx){
        if(lazy[idx] == ope_struct_t::id) return data[idx];
        return seg_structure()(data[idx], lazy[idx]);
    }

public:
    lazy_segment_tree(size_t N){
        height = 0; length = 1; 
        while(length < N) length *= 2, height++;
        data.assign(2 * length, ele_struct_t::id);
        lazy.assign(2 * length, ope_struct_t::id);
    }
    lazy_segment_tree(const std::vector<element_t>& A){
        const size_t n = A.size();
        height = 0; length = 1; 
        while(length < n) length *= 2, height++;
        data.resize(2 * length);
        for(int i = 0; i < n; ++i) data[i + length] = A[i];
        for(int i = length - 1; i >= 0; --i) data[i] = ele_struct_t()(data[2 * i], data[2 * i + 1]);
        lazy.assign(2 * length, ope_struct_t::id);
    }

    void update(int left, int right, const operator_t& op){
        if(left >= right) return;

        size_t l = left + length;      for(size_t i = height; i > 0; --i) eval(l >> i);
        size_t r = right + length - 1; for(size_t i = height; i > 0; --i) eval(r >> i);

        for(size_t ll = l, rr = r + 1; ll < rr; ll /= 2, rr /= 2){
            if(ll % 2 == 1){
                lazy[ll] = ope_struct_t()(lazy[ll], op); ++ll;
            }
            if(rr % 2 == 1){
                --rr; lazy[rr] = ope_struct_t()(lazy[rr], op);
            }
        }

        calculateData(l); calculateData(r);
    }

    // 区間 [left, right) (0-indexed) の値を求める
    inline element_t query(size_t left, size_t right){
        if(left >= right) return ele_struct_t::id;
        // ボトムアップに伝搬を伝える
        size_t l = left + length;      for(size_t i = height; i > 0; --i) eval(l >> i);
        size_t r = right + length - 1; for(size_t i = height; i > 0; --i) eval(r >> i);

        element_t vl = ele_struct_t::id, vr = ele_struct_t::id;
        for(size_t ll = l, rr = r + 1; ll < rr; ll /= 2, rr /= 2) {
            if (ll % 2 == 1) {
                vl = ele_struct_t()(vl, seg_structure()(data[ll], lazy[ll])); ++ll;
            }
            if (rr % 2 == 1) {
                --rr; vr = ele_struct_t()(seg_structure()(data[rr], lazy[rr]), vr);
            }
        }
        return ele_struct_t()(vl, vr);
    }
};

using RmQ = lazy_segment_tree<seg_monoid>;
*/
