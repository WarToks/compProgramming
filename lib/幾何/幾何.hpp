// verify on AOJ
// projection : http://judge.u-aizu.ac.jp/onlinejudge/review.jsp?rid=4244574#1 : (Line.projectionOf)
//

namespace Geometory{
    // 点(ベクトル) を表すクラス
    template <typename real_type>
    class Point{
    private:
        static constexpr real_type PI = std::acos<real_type>(-1); // 円周率
        static constexpr real_type real_abs(const real_type v){ return std::fabs<real_type>(v); }
        static real_type real_sqrt(const real_type v);
    public:
        static constexpr real_type EPS = 1e-10; // 許容誤差
        // 誤差を許してゼロであるかを判定する関数
        static constexpr bool isZero(const real_type& v){ return -EPS < v and v < EPS;}
        // 符号を得る関数 : (a > 0) → 1,  (a ≒ 0) → 0, (a < 0) → -1
        static constexpr int sign(real_type v){
            return (v > EPS) ? 1 : ((v < -EPS) ? -1 : 0); 
        }

        real_type x, y; // メンバ変数 : 点 (x, y) を表す

        // コンストラクタ;
        constexpr Point(const real_type& x = 0, const real_type& y = 0): x(x), y(y){}

        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 基本的演算 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // 四則演算
        constexpr Point& operator += (const Point& other){ x += other.x; y += other.y; return *this; }
        constexpr Point& operator -= (const Point& other){ x -= other.x; y -= other.y; return *this; }
        constexpr Point& operator *= (const Point& other){
            const real_type&& tmp_x = x * other.x - y * other.y;
            (y *= other.x) += x * other.y; x = tmp_x; return *this;
        }
        constexpr Point& operator *= (const real_type& scalar){ x *= scalar; y *= scalar; return *this; }
        constexpr Point& operator /= (const real_type& scalar){ x /= scalar; y /= scalar; return *this; } 
        constexpr Point operator +  (const Point& other) const { return Point(*this) += other; }
        constexpr Point operator -  (const Point& other) const { return Point(*this) -= other; }
        constexpr Point operator -  (void) const { return Point(-x, -y); }
        constexpr Point operator *  (const Point& other) const { return Point(*this) *= other; }
        constexpr Point operator *  (const real_type& scalar) const { return Point(*this) *= scalar; }
        constexpr Point operator /  (const real_type& scalar) const { return Point(*this) /= scalar; }
        // ノルム(2乗), 絶対値, 一致判定, 内積 determinant, 偏角, rotate
        real_type norm(void) const { return x * x + y * y; }    // ノルム
        real_type abs(void) const { return real_sqrt(norm()); } // 絶対値 |A|
        inline constexpr bool operator == (const Point& other) const { // 一致判定
            return real_abs(x - other.x) < EPS and real_abs(y - other.y) < EPS;
        }
        // 内積を求める関数
        real_type dot(const Point& other) const { return x * other.x + y * other.y; }
        // 行列積を求める関数
        real_type determinant(const Point& other) const { return x * other.y - y * other.x; }
        // 偏角を求める関数 : rad
        real_type arg(void) const { return std::atan2<real_type>(y, x); }
        // 偏角を求める関数 : 180°単位
        real_type arg_degree(void) const { return arg() * 180 / PI; }
        // θ rad 回転(rotate)させる
        inline Point& rotate(const real_type& theta){
            const real_type 
                cos_theta = std::cos<real_type>(theta),
                sin_theta = std::sin<real_type>(theta);
            const real_type&& tmp_x = x * cos_theta - y * sin_theta;
            (y *= cos_theta) += x * sin_theta; x = tmp_x; return *this;
        }
        // xの値によってソート (ほぼ同じ場合は y の値でソート)
        inline constexpr bool  operator < (const Point& other) const {
            const int sgn = sign(x - other.x); if(sgn) return sgn < 0;
            return sign(y - other.y) < 0;
        }
        //friend const MIO::user_input& operator >> (const MIO::user_input& stream, Point<real_type>& point);
    };
    // テンプレート特殊化
    template <> float Point<float>::real_sqrt(const float v){ return sqrtf(v); }
    template <> double Point<double>::real_sqrt(const double v){ return sqrt(v); }
    template <> long double Point<long double>::real_sqrt(const long double v){ return sqrtl(v); }
    // 点クラス の入出力
    template <typename real_type>
    const MIO::user_input& operator >> (const MIO::user_input& stream, Point<real_type>& point){
        stream >> point.x >> point.y; return stream;
    }
    template <typename real_type>
    const MIO::user_output& operator << (const MIO::user_output& stream, const Point<real_type>& point){
        stream << point.x << ' ' << point.y; return stream;
    }

    // reference https://www.ioi-jp.org/camp/2017/2017-sp_camp-hide.pdf
    // see also  http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=CGL_1_C&lang=ja
    template <typename real_type> int iSP( const Point<real_type>& a,
                                       const Point<real_type>& b,
                                       const Point<real_type>& c ){
        const Point<real_type> b_a = b - a, c_a = c - a;
        const real_type a_origin_det = b_a.determinant(c_a);
        // 直線上にない場合
        if(a_origin_det >  Point<real_type>::EPS) return +1; // 左に曲がる : 反時計回り
        if(a_origin_det < -Point<real_type>::EPS) return -1; // 右に曲がる : 時計回り
        // 3点が一直線上の場合
        if(b_a.dot(c_a) < 0)        return +2; // c-a-b の順に並んでいる場合
        if(b_a.norm() < c_a.norm()) return -2; // a-b-c の順に並んでいる場合
        return  0; // それ以外の順番に並んでいる場合
    }

    // 直線 を表すクラス (2点 p0, p1 を通る直線)
    template <typename real_type> class Line{
    public:
        std::array<Point<real_type>, 2> point; // 通るべき2点

        // コンストラクタ : 点 p1, p2 を通る直線
        constexpr Line( const Point<real_type>& p1, const Point<real_type>& p2): point({{p1, p2}}){}
        // コンストラクタ : 直線 Ax + By = C
        constexpr Line(const real_type& A = 1, const real_type& B = 1, const real_type& C = 0){
            if(std::fabs<real_type>(A) < Point<real_type>::EPS){ point[0](0, C / B); point[1](1, C / B); }
            else if(std::fabs<real_type>(B) < Point<real_type>::EPS){ point[0](C / A, 0); point[1](C / A, 1); }
            else{ point[0](0, C / B); point[1](C / A, 0); } 
        }
        // 交差するか判定する : 交差する場合は true
        bool intersect(const Line& other) const {
            const real_type v1_det_v2 = (point[1] - point[0]).determinant(other.point[1] - other.point[0]);
            return not Point<real_type>::isZero(v1_det_v2);
        }
        // 平行か判定する : 交差する場合は true
        bool pararell(const Line& other) const {
            const real_type v1_det_v2 = (point[1] - point[0]).determinant(other.point[1] - other.point[0]);
            return Point<real_type>::isZero(v1_det_v2);
        }
        // 点 P をこの直線上に射影した点を返す (垂線の足)
        Point<real_type> projectionOf(const Point<real_type>& P) const {
            const Point<real_type> v = point[1] - point[0];
            return point[0] + (v * v.dot(P - point[0]) / v.norm());
        }
        // 直線が一致するか判定する
        inline bool operator == (const Line& other) const {
            const Point<real_type> v1 = point[1] - point[0];
            if(not Point<real_type>::isZero( v1.determinant(other.point[1] - other.point[0]))) return false;
            return Point<real_type>::isZero( v1.determinant(other.point[1] - point[0]) );
        }
    };
    // 線分 AB を表すクラス
    template <typename real_type> class LineSegment{
    public:
        std::array<Point<real_type>, 2> point; // 2点

        // コンストラクタ : 点 p1, p2による線分
        constexpr LineSegment( const Point<real_type>& p1, 
                               const Point<real_type>& p2): point({{p1, p2}}){}
        // 線分が交差するか判定する関数 ; 端点で交わる場合, 直線が一致する場合も true を返すことに注意せよ
        constexpr bool intersect(const LineSegment& other) const {
            return (iSP(point[0], point[1], other.point[0]) * iSP(point[0], point[1], other.point[1]) <= 0)
                    and (iSP(other.point[0], other.point[1], point[0]) * iSP(other.point[0], other.point[1], point[1]) <= 0);
        }
        inline bool operator == (const LineSegment& other) const {
            return (point[0] == other.point[0] and point[1] == other.point[1])
                    or (point[0] == other.point[1] and point[1] == other.point[0]);
        }
    };
    // 円を表すクラス
    template <typename real_type> class Circle{
    public:
        real_type radius; // 半径
        Point<real_type> center_point; // 中心
        constexpr Circle( const real_type radius = 0, 
                          const Point<real_type>& point = {}): 
                          radius(radius), center_point(point){}
    };

    #undef EPSILON_VALUE
};
