// verify : https://judge.yosupo.jp/submission/4631

#include <cstdio>
#include <vector>
#include <tuple>
#include <stack>
#include <cassert>

// SWAG : 半群 : struct { constexpr Tp operator() (const Tp& rhs, const Tp& lhs){} };
template <typename monoid_t>
class SlidingWindowAggregation{
private:
    using element_t = typename monoid_t::value_type;
    static constexpr element_t id = monoid_t::id;
    inline constexpr element_t operation(const element_t& lhs, const element_t& rhs) const {
        return monoid_t()(lhs, rhs);
    }

    // {値, 累積和}
    using element_datum = std::pair<element_t, element_t>;
    std::stack<element_datum, std::vector<element_datum>> front_stack, back_stack;
public:
    // コンストラクタ
    SlidingWindowAggregation(void){}

    // 末尾に要素を追加するメソッド
    void push_back(const element_t& val){
        if(back_stack.empty()) back_stack.emplace(val, val);
        else back_stack.emplace(val, operation(back_stack.top().second, val));
    }
    template <typename... Args>
    void emplace_back(Args&&... args){
        const auto&& val = element_t(args...);
        if(back_stack.empty()) back_stack.emplace(val, val);
        else back_stack.emplace(val, operation(back_stack.top().second, val));
    }

    // 先頭から要素を取るメソッド : 空からは取り出せないことに注意
    void pop_front(void){
        assert(not empty());
        if(front_stack.empty()){
            {
                const element_t val = back_stack.top().first; back_stack.pop();
                front_stack.emplace(val, val);
            }
            while(not back_stack.empty()){
                const element_t val = back_stack.top().first; back_stack.pop();
                front_stack.emplace(val, operation(val, front_stack.top().second));
            }
        }
        front_stack.pop();
    }

    // スタックに入っている累積和 a0 + a1 + ... + an-1 の値を返す関数
    element_t sum(void) const noexcept {
        const element_t value_front = front_stack.empty() ? id : front_stack.top().second;
        const element_t value_back  =  back_stack.empty() ? id :  back_stack.top().second;
        return operation(value_front, value_back);
    }
    // 現在保持している要素の数を返す関数
    unsigned int size(void) const noexcept {
        return front_stack.size() + back_stack.size();
    }
    // 空であるかを判定する関数
    bool empty(void) const noexcept {
        return front_stack.empty() and back_stack.empty();
    }
};

constexpr unsigned int mod = 998244353;

struct monoid{
    using value_type = std::pair<unsigned int, unsigned int>;
    static constexpr value_type id = value_type(1, 0);
    inline constexpr value_type operator() (const value_type& lhs, const value_type& rhs) const {
        unsigned long long int p = lhs.first; p *= rhs.first; p %= mod;
        unsigned long long int q = lhs.second; q *= rhs.first; q %= mod;
        if((q += rhs.second) >= mod) q -= mod;
        return value_type(p, q);
    }
};

using SWAG = SlidingWindowAggregation<monoid>;

int main(void){
    SWAG D;
    unsigned int Q; scanf("%u", &Q);
    while(Q--){
        char c; scanf("%*c%c", &c);
        if(c == '0'){
            unsigned int a, b; scanf("%u%u", &a, &b);
            D.emplace_back(a, b);
        }
        else if(c == '1') D.pop_front();
        else{
            unsigned int x; scanf("%u", &x);
            unsigned int a, b;
            std::tie(a, b) = D.sum();
            unsigned long long int res = a; res *= x; res %= mod;
            if((res += b) >= mod) res -= mod;
            printf("%llu\n", res);
        }
    }
    return 0;
}