
constexpr long  long int inf = 1e18 + 334 + 810 + 1919;

// 大小関係のある配列について, 個数を数える | クエリ形式の場合はクエリ先読みBITを使え
template <typename element_type> class CountingVector{
private:
    unsigned int size_of_vec;             // 座圧済み配列の長さ
    const unsigned int size_of_arg_vec;   // 元の引数の配列の長さ | = counter[size_of_vec-1] 
    std::vector<element_type> vec;        // ソート & 座標圧縮 済み配列
    std::vector<unsigned int> counter;    // 累積和 counter[i] = vec[0, i)の数

    using const_itr_type = typename std::vector<element_type>::const_iterator;
public:
    // コンストラクタ : 破壊的操作を行うので引数はコピー
    template <typename element_type_castable>
    CountingVector(std::vector<element_type_castable> arg_vec): size_of_arg_vec(arg_vec.size()){
        const unsigned int arg_size = arg_vec.size();
        counter.reserve(arg_size + 1); counter.push_back(0);
        vec.reserve(arg_size);
        std::sort(arg_vec.begin(), arg_vec.end());
        unsigned int idx = 0;
        while(idx < arg_size){
            unsigned int cnt = 1;
            vec.push_back(arg_vec[idx]);
            while(idx + cnt < arg_size and arg_vec[idx] == arg_vec[idx + cnt]) cnt++;
            unsigned int p = counter.back() + cnt; counter.push_back(p);
            idx += cnt;
        }
        size_of_vec = vec.size();
    }

    // arg_vec[i] < val なる i の個数を数える
    inline unsigned int count_smallerThan(const element_type& val) const noexcept {
        unsigned int idx = std::lower_bound(vec.begin(), vec.end(), val) - vec.begin();
        return counter[idx];
    }
    inline unsigned int count_smallerThan(const const_itr_type& itr) const noexcept {
        unsigned int idx = itr - vec.cbegin(); return counter[idx];
    }

    // arg_vec[i] ≤ val なる i の個数を数える
    inline unsigned int count_notGreaterThan(const element_type& val) const noexcept {
        unsigned int idx = std::upper_bound(vec.begin(), vec.end(), val) - vec.begin();
        return counter[idx];
    }
    inline unsigned int count_notGreaterThan(const const_itr_type& itr) const noexcept {
        unsigned int idx = itr - vec.cbegin(); return counter[idx + 1];
    }

    // arg_vec[i] > val なる i の個数を数える
    inline unsigned int count_greaterThan(const element_type& val) const noexcept {
        unsigned int idx = std::upper_bound(vec.begin(), vec.end(), val) - vec.begin();
        return size_of_arg_vec - counter[idx];
    }
    inline unsigned int count_greaterThan(const const_itr_type& itr) const noexcept {
        unsigned int idx = itr - vec.cbegin(); return size_of_arg_vec - counter[idx + 1];
    }

    // arg_vec[i] ≥ val なる i の個数を数える
    inline unsigned int count_notSmallerThan(const element_type& val) const noexcept {
        unsigned int idx = std::lower_bound(vec.begin(), vec.end(), val) - vec.begin();
        return size_of_arg_vec - counter[idx];
    }
    inline unsigned int count_notSmallerThan(const const_itr_type& itr) const noexcept {
        unsigned int idx = itr - vec.cbegin(); return size_of_arg_vec - counter[idx];
    }

    // vec[idx] = val なる idx を返す (存在しない場合は -1を返す)
    inline int key(const element_type& val) const noexcept {
        int idx = std::lower_bound(vec.begin(), vec.end(), val) - vec.begin();
        return (vec[idx] == val) ? idx : -1;
    }
    inline int key(const const_itr_type& itr) const noexcept { return itr - vec.cbegin(); }

    // arg_vec[i] = val なる i の個数を数える (存在しないなら 0)
    inline unsigned int count(const element_type& val) const noexcept {
        unsigned int idx = std::lower_bound(vec.begin(), vec.end(), val) - vec.begin();
        return (vec[idx] == val) ? counter[idx + 1] - counter[idx] : 0;
    }
    inline unsigned int count(const const_itr_type& itr) const noexcept {
        unsigned int idx = itr - vec.cbegin(); return counter[idx+1] - counter[idx];
    }

    // lower_bound, upper_bound
    inline const_itr_type lower_bound(const element_type& val) const noexcept {
        return std::lower_bound(vec.cbegin(), vec.cend(), val);
    }
    inline const_itr_type upper_bound(const element_type& val) const noexcept {
        return std::upper_bound(vec.cbegin(), vec.cend(), val);
    }

    // イテレーター関係
    const_itr_type cbegin(void) const noexcept{ return vec.cbegin(); }
    const_itr_type cend(void)   const noexcept{ return vec.cend(); }
};


int main(void){

    return 0;
}