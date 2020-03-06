#include <vector>

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