#include <iostream>
#include <memory>

// 行列クラス : サイズはコンパイル時に決定しなくとも良いが, 配列の大きさは変えられない
// < 備考 > Matrix型 M に関して
//  * M[i][j] の形で値にアクセスすることが可能
//  * 全体に走査したい場合は for(auto x : M) の形で走査が可能
//  * i 行を走査したい場合は for(auto x : M.row(x)) の形で走査が可能
template <class Tp, class Allocator = std::allocator<Tp>> 
class Matrix {
    // 計算時に行列のサイズチェックを行うか否かのマクロ (行う → true)
    #define CHECKING_SIZE_OF_MATRIX_IN_SOME_OPERATION_FOR_THIS_MATRIX_MADE_BY_AROUNDERSTAND true
    Allocator alloc;                 // アロケータ
    Tp* matrix_ptr;                  // 確保したメモリの先頭を指すポインタ
    const std::size_t size_row;      // 行のサイズを表す (r x c 行列 の r)
    const std::size_t size_column;   // 列のサイズを表す (r x c 行列 の c)
    const std::size_t size_matrix;   // 行列の要素数を表す (r x c)

    // 可変イテレーター
    class iterator {
        Tp* pointer;
    public:
        iterator(Tp* pointer):pointer(pointer){}
        Tp& operator *(void) { return *pointer; }
        iterator& operator ++(void) { ++pointer; return *this; }
        bool operator != (const iterator& other) const { return pointer != other.pointer; }
        bool operator == (const iterator& other) const { return pointer == other.pointer; }
    };
    // const イテレーター
    class const_iterator {
        Tp* const pointer;
    public:
        const_iterator(Tp* pointer):pointer(pointer){}
        const Tp& operator *(void) const { return *pointer; }
        iterator& operator ++(void){ ++pointer; return *this; }
        bool operator != (const iterator& other) const { return pointer != other.pointer; }
        bool operator == (const iterator& other) const { return pointer == other.pointer; }
    };
    // A[i][j] の形で アクセスするために必要なクラス
    class accessor{
        Tp* pointer;
    public:
        accessor(Tp* pointer):pointer(pointer){}
        Tp& operator [](std::size_t idx){ return *(pointer + idx); }
    };
    // 1行を返すためのクラス
    class row_matrix{
        Tp* pointer;
        std::size_t size_row_matrix;
    public:
        row_matrix(Tp* pointer, std::size_t size_row_matrix):pointer(pointer), size_row_matrix(size_row_matrix){}
        Tp& operator [](std::size_t idx){ return *(pointer + idx); }
        Tp& at(std::size_t idx){ assert(idx < size_row_matrix); return *(pointer + idx); }
        iterator begin(void){ return iterator(pointer); }
        iterator end(void){ return iterator(pointer + size_row_matrix); }
        const_iterator cbegin(void) const { return const_iterator(pointer); }
        const_iterator cend(void) const { return const_iterator(pointer + size_row_matrix); }
    };
public:

    // @ コンストラクタ (要素数)
    // @ 引数 : size_row     |  行サイズ
    //         size_column  |  列のサイズ 
    //         init_val     |  行列の初期値
    //   -> 行列の大きさを受け取って(初期値を与えることもできる)行列を生成する
    Matrix(const std::size_t size_row, const std::size_t size_column, const Tp init_val = Tp()):
                                                            size_row(size_row), 
                                                            size_column(size_column), 
                                                            size_matrix(size_row * size_column) {
        matrix_ptr = alloc.allocate(size_matrix);
        for(std::size_t i = 0; i < size_matrix; ++i) matrix_ptr[i] = init_val;
    }

    // @ コンストラクタ (初期化リスト)
    // @ 引数 : init_list : 初期化リスト (二次元)
    //   -> 初期化リストによって行列を構築する
    //      初期化リスト中の各リストは同じ大きさでなくてはならない
    Matrix(std::initializer_list<std::initializer_list<Tp>> init_list):
                                                            size_row(init_list.size()),
                                                            size_column(init_list.begin() -> size()),
                                                            size_matrix(size_row * size_column){
        matrix_ptr = alloc.allocate(size_matrix);
        Tp* ptr = matrix_ptr;
        auto init_arr = init_list.begin(); const auto end_arr = init_list.end();
        while(init_arr != end_arr){
            #if CHECKING_SIZE_OF_MATRIX_IN_SOME_OPERATION_FOR_THIS_MATRIX_MADE_BY_AROUNDERSTAND
                assert(init_arr -> size() == size_column);
            #endif
            std::copy(init_arr -> begin(), init_arr -> end(), ptr);
            ++init_arr; ptr += size_column;
        }
    }
    // @ コピーコンストラクタ(デフォルト)
    Matrix(const Matrix& original): size_row(original.size_row),
                                    size_column(original.size_column),
                                    size_matrix(original.size_matrix){
        matrix_ptr = alloc.allocate(size_matrix);
        std::copy(original.matrix_ptr, original.matrix_ptr + original.size_matrix, matrix_ptr);
    }

    Matrix(Matrix&& rhs): matrix_ptr(alloc.allocate(0)),
                          size_row(rhs.size_row),
                          size_column(rhs.size_column),
                          size_matrix(rhs.size_matrix){
        if(this != &rhs){
            std::swap(alloc, rhs.alloc);
            std::swap(matrix_ptr, rhs.matrix_ptr);
        }
    }

    // @ デストラクタ
    ~Matrix(void){ alloc.deallocate(matrix_ptr, size_matrix); }

    // ================== 演算子 ==========================
    Matrix& operator += (const Matrix& other){
        #if CHECKING_SIZE_OF_MATRIX_IN_SOME_OPERATION_FOR_THIS_MATRIX_MADE_BY_AROUNDERSTAND
            assert(size_row == other.size_row and size_column == other.size_column);
        #endif
        for(std::size_t i = 0; i < size_matrix; ++i) matrix_ptr[i] += other.matrix_ptr[i];
        return *this;
    }
    Matrix& operator -= (const Matrix& other){
        #if CHECKING_SIZE_OF_MATRIX_IN_SOME_OPERATION_FOR_THIS_MATRIX_MADE_BY_AROUNDERSTAND
            assert(size_row == other.size_row and size_column == other.size_column);
        #endif
        for(std::size_t i = 0; i < size_matrix; ++i) matrix_ptr[i] -= other.matrix_ptr[i];
        return *this;
    }
    Matrix& operator = (const Matrix& other){
        #if CHECKING_SIZE_OF_MATRIX_IN_SOME_OPERATION_FOR_THIS_MATRIX_MADE_BY_AROUNDERSTAND
            assert(size_row == other.size_row and size_column == other.size_column);
        #endif
        std::copy(other.matrix_ptr, other.matrix_ptr + other.size_matrix, matrix_ptr);
        return *this;
    }
    Matrix& operator = (Matrix&& rhs){
        #if CHECKING_SIZE_OF_MATRIX_IN_SOME_OPERATION_FOR_THIS_MATRIX_MADE_BY_AROUNDERSTAND
            assert(size_row == rhs.size_row and size_column == rhs.size_column);
        #endif
        std::swap(alloc, rhs.alloc);
        std::swap(matrix_ptr, rhs.matrix_ptr);
        return *this;
    }
    Matrix operator + (const Matrix& other) const { return Matrix(*this) += other; }
    Matrix operator - (const Matrix& other) const { return Natrix(*this) -= other; }
    // 行列積
    Matrix operator * (const Matrix& other) const {
        #if CHECKING_SIZE_OF_MATRIX_IN_SOME_OPERATION_FOR_THIS_MATRIX_MADE_BY_AROUNDERSTAND
            assert(size_column == other.size_row);
        #endif
        Matrix res(size_row, other.size_column);
        std::size_t i = 0, k = 0, idx0 = 0;
        while(i < res.size_matrix){
            for(std::size_t idx1 = i, idx2 = k; idx2 < other.size_matrix; ++idx1, idx2 += other.size_column)
                res.matrix_ptr[idx0] += matrix_ptr[idx1] * other.matrix_ptr[idx2];
            ++idx0; if(++k == res.size_column){ i += size_column; k = 0; }
        }
        return res;
    }
    // @ 出力演算子
    //   -> 空白改行の形で出力する
    friend std::ostream& operator << (std::ostream& os, const Matrix& mtrx){
        std::size_t i = 0, j = mtrx.size_column;
        while(i < mtrx.size_matrix){
            os << mtrx.matrix_ptr[i++];
            if(i == j){ os << '\n'; j += mtrx.size_column; }
            else os << ' ';
        }
        return os;
    }
    // i行目へのアクセスを取得
    accessor operator [](std::size_t idx){ return accessor(matrix_ptr + size_column * idx); }

    // @ 行を取得する関数 : O(1)
    row_matrix row(std::size_t idx){ return row_matrix(matrix_ptr + size_column * idx, size_column); }


    // 要素数を返す関数
    std::size_t size(void) const { return size_matrix; }

    // イテレーター群
    iterator begin(void) { return iterator(matrix_ptr); }
    iterator end(void)   { return iterator(matrix_ptr + size_matrix); }
    const_iterator cbegin(void) const { return matrix_ptr; }
    const_iterator cend(void)   const { return matrix_ptr + size_matrix; }

    // @ swap関数
    // @ 引数 : other | 行と列サイズが等しい Matrix 関数
    //   -> 行列の要素を交換
    void swap(Matrix& other){
        #if CHECKING_SIZE_OF_MATRIX_IN_SOME_OPERATION_FOR_THIS_MATRIX_MADE_BY_AROUNDERSTAND
            assert(size_row == other.size_row and size_column == other.size_column);
        #endif
        std::swap<Tp*>(matrix_ptr, other.matrix_ptr);
        std::swap<Allocator>(alloc, other.alloc);
    }

    // @ 行列累乗(power関数) : 計算量 O(N^3 log n)
    // @ 引数 : n        |  指数部
    //         plus_id  |  Tp の加法 + に関する単位元 (1)
    //         mult_id  |  Tp の乗法 * に関する単位元 (0)
    Matrix power(unsigned long long int n, const Tp& plus_id, const Tp& mult_id = Tp()) const {
        #if CHECKING_SIZE_OF_MATRIX_IN_SOME_OPERATION_FOR_THIS_MATRIX_MADE_BY_AROUNDERSTAND
            assert(size_row == size_column);
        #endif
        Matrix res(size_row, size_column, mult_id), coef = Matrix(*this);
        for(std::size_t i = 0; i < size_matrix; i += size_column + 1) res.matrix_ptr[i] = plus_id;
        while(n){
            if(n & 1) res = coef * res;
            if(n >>= 1) coef = coef * coef;
        }
        return res;
    }
    #undef CHECKING_SIZE_OF_MATRIX_IN_SOME_OPERATION_FOR_THIS_MATRIX_MADE_BY_AROUNDERSTAND
};

int main(void){
    Matrix<long long int> A{{1LL, 2LL}, {2LL, 1LL}};
    std::cout << A.power(10, 1, 0);
    return 0;
}