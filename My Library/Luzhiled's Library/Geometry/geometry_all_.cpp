using Real = long double;
using Point = complex< Real >;
const Real EPS = 1e-8, PI = acos(-1);

inline bool eq(Real a, Real b) { return fabs(b - a) < EPS; }

Point operator*(const Point &p, const Real &d) {
    return Point(real(p) * d, imag(p) * d);
}
Point operator/(const Point &p, const Real &d) {
    return Point(real(p) / d, imag(p) / d);
}

istream &operator>>(istream &is, Point &p) {
    Real a, b;
    is >> a >> b;
    p = Point(a, b);
    return is;
}

ostream &operator<<(ostream &os, Point &p) {
    return os << fixed << setprecision(10) << p.real() << " " << p.imag();
}

// rotate point p counterclockwise by theta rad
Point rotate(Real theta, const Point &p) {
    return Point(cos(theta) * p.real() - sin(theta) * p.imag(), sin(theta) * p.real() + cos(theta) * p.imag());
}

Real radian_to_degree(Real r) {
    return (r * 180.0 / PI);
}

Real degree_to_radian(Real d) {
    return (d * PI / 180.0);
}

// smaller angle of the a-b-c
Real get_angle(const Point &a, const Point &b, const Point &c) {
    const Point v(b - a), w(c - b);
    Real alpha = atan2(v.imag(), v.real()), beta = atan2(w.imag(), w.real());
    if(alpha > beta) swap(alpha, beta);
    Real theta = (beta - alpha);
    return min(theta, 2 * acos(-1) - theta);
}

namespace std {
bool operator<(const Point &a, const Point &b) {
    return std::abs(real(a) - real(b)) > EPS ? real(a) < real(b) : imag(a) < imag(b);
}
bool operator>(const Point &a, const Point &b) {
    return std::abs(real(b) - real(a)) > EPS ? real(b) < real(a) : imag(b) < imag(a);
}
}


struct Line {
    Point a, b;

    Line() = default;

    Line(Point a, Point b) : a(a), b(b) {}

    Line(Real A, Real B, Real C) // Ax + By = C
    {
        if(eq(A, 0)) a = Point(0, C / B), b = Point(1, C / B);
        else if(eq(B, 0)) b = Point(C / A, 0), b = Point(C / A, 1);
        else a = Point(0, C / B), b = Point(C / A, 0);
    }

    friend ostream &operator<<(ostream &os, Line &p) {
        return os << p.a << " to " << p.b;
    }

    friend istream &operator>>(istream &is, Line &a) {
        return is >> a.a >> a.b;
    }
};

struct Segment : Line {
    Segment() = default;

    Segment(Point a, Point b) : Line(a, b) {}
};

struct Circle {
    Point p;
    Real r;

    Circle() = default;

    Circle(Point p, Real r) : p(p), r(r) {}
};

using Points = vector< Point >;
using Polygon = vector< Point >;
using Segments = vector< Segment >;
using Lines = vector< Line >;
using Circles = vector< Circle >;

Real cross(const Point &a, const Point &b) {
    return real(a) * imag(b) - imag(a) * real(b);
}

Real dot(const Point &a, const Point &b) {
    return real(a) * real(b) + imag(a) * imag(b);
}

// http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=CGL_1_C
// aを中心にbをどう回転したらcか
int ccw(const Point &a, Point b, Point c) {
    b = b - a, c = c - a;
    if(cross(b, c) > EPS) return +1;  // "COUNTER_CLOCKWISE"
    if(cross(b, c) < -EPS) return -1; // "CLOCKWISE"
    if(dot(b, c) < 0) return +2;      // "ONLINE_BACK" c-a-b
    if(norm(b) < norm(c)) return -2;  // "ONLINE_FRONT" a-b-c
    return 0;                         // "ON_SEGMENT" a-c-b
}

// http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=CGL_2_A
//平行
bool parallel(const Line &a, const Line &b) {
    return eq(cross(a.b - a.a, b.b - b.a), 0.0);
}

// http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=CGL_2_A
//垂直
bool orthogonal(const Line &a, const Line &b) {
    return eq(dot(a.a - a.b, b.a - b.b), 0.0);
}

// http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=CGL_1_A
//正射影
Point projection(const Line &l, const Point &p) {
    double t = dot(p - l.a, l.a - l.b) / norm(l.a - l.b);
    return l.a + (l.a - l.b) * t;
}

//直線p1-p2に向けてpから引いた垂線の足
Point projection(const Point &p1, const Point &p2, const Point &p) {
    const Line l(p1, p2);
    return projection(l, p);
}

Point projection(const Segment &l, const Point &p) {
    double t = dot(p - l.a, l.a - l.b) / norm(l.a - l.b);
    return l.a + (l.a - l.b) * t;
}

// http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=CGL_1_B
//対称点
Point reflection(const Line &l, const Point &p) {
    return p + (projection(l, p) - p) * 2.0;
}

Point reflection(const Point &p1, const Point &p2, const Point &p) {
    const Line l(p1, p2);
    return reflection(l, p);
}

//交わる？
bool intersect(const Line &l, const Point &p) {
    return abs(ccw(l.a, l.b, p)) != 1;
}

bool intersect(const Line &l, const Line &m) {
    return abs(cross(l.b - l.a, m.b - m.a)) > EPS || abs(cross(l.b - l.a, m.b - l.a)) < EPS;
}

bool intersect(const Segment &s, const Point &p) {
    return ccw(s.a, s.b, p) == 0;
}

bool intersect(const Line &l, const Segment &s) {
    return cross(l.b - l.a, s.a - l.a) * cross(l.b - l.a, s.b - l.a) < EPS;
}

Real distance(const Line &l, const Point &p);

bool intersect(const Circle &c, const Line &l) {
    return distance(l, c.p) <= c.r + EPS;
}

bool intersect(const Circle &c, const Point &p) {
    return abs(abs(p - c.p) - c.r) < EPS;
}

// http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=CGL_2_B
bool intersect(const Segment &s, const Segment &t) {
    return ccw(s.a, s.b, t.a) * ccw(s.a, s.b, t.b) <= 0 && ccw(t.a, t.b, s.a) * ccw(t.a, t.b, s.b) <= 0;
}

// 交点の数
int intersect(const Circle &c, const Segment &l) {
    if(norm(projection(l, c.p) - c.p) - c.r * c.r > EPS) return 0;
    auto d1 = abs(c.p - l.a), d2 = abs(c.p - l.b);
    if(d1 < c.r + EPS && d2 < c.r + EPS) return 0;
    if(d1 < c.r - EPS && d2 > c.r + EPS || d1 > c.r + EPS && d2 < c.r - EPS) return 1;
    const Point h = projection(l, c.p);
    if(dot(l.a - h, l.b - h) < 0) return 2;
    return 0;
}

// http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=CGL_7_A&lang=jp
// 交点の数
//4 -- 交点なし, 3 -- 外接, 2 -- 交点2個, 1 -- 内接, 0 -- 交点なし
//共通接線の数
int intersect(Circle c1, Circle c2) {
    if(c1.r < c2.r) swap(c1, c2);
    Real d = abs(c1.p - c2.p);
    if(c1.r + c2.r < d) return 4;
    if(eq(c1.r + c2.r, d)) return 3;
    if(c1.r - c2.r < d) return 2;
    if(eq(c1.r - c2.r, d)) return 1;
    return 0;
}

//距離
Real distance(const Point &a, const Point &b) {
    return abs(a - b);
}

Real distance(const Line &l, const Point &p) {
    return abs(p - projection(l, p));
}

Real distance(const Line &l, const Line &m) {
    return intersect(l, m) ? 0 : distance(l, m.a);
}

Real distance(const Segment &s, const Point &p) {
    Point r = projection(s, p);
    if(intersect(s, r)) return abs(r - p);
    return min(abs(s.a - p), abs(s.b - p));
}

// http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=CGL_2_D
Real distance(const Segment &a, const Segment &b) {
    if(intersect(a, b)) return 0;
    return min({distance(a, b.a), distance(a, b.b), distance(b, a.a), distance(b, a.b)});
}

Real distance(const Line &l, const Segment &s) {
    if(intersect(l, s)) return 0;
    return min(distance(l, s.a), distance(l, s.b));
}

//交点の座標
Point crosspoint(const Line &l, const Line &m) {
    Real A = cross(l.b - l.a, m.b - m.a);
    Real B = cross(l.b - l.a, l.b - m.a);
    if(eq(abs(A), 0.0) && eq(abs(B), 0.0)) return m.a;
    return m.a + (m.b - m.a) * B / A;
}

// http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=CGL_2_C
Point crosspoint(const Segment &l, const Segment &m) {
    return crosspoint(Line(l), Line(m));
}

// http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=CGL_7_D
pair< Point, Point > crosspoint(const Circle &c, const Line l) {
    Point pr = projection(l, c.p);
    Point e = (l.b - l.a) / abs(l.b - l.a);
    if(eq(distance(l, c.p), c.r)) return {pr, pr};
    double base = sqrt(c.r * c.r - norm(pr - c.p));
    return {pr - e * base, pr + e * base};
}

//todo
pair< Point, Point > crosspoint(const Circle &c, const Segment &l) {
    Line aa = Line(l.a, l.b);   
    if(intersect(c, l) == 2) return crosspoint(c, aa);
    auto ret = crosspoint(c, aa);
    if(dot(l.a - ret.first, l.b - ret.first) < 0) ret.second = ret.first;
    else ret.first = ret.second;
    return ret;
}

// http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=CGL_7_E
pair< Point, Point > crosspoint(const Circle &c1, const Circle &c2) {
    Real d = abs(c1.p - c2.p);
    Real a = acos((c1.r * c1.r + d * d - c2.r * c2.r) / (2 * c1.r * d));
    Real t = atan2(c2.p.imag() - c1.p.imag(), c2.p.real() - c1.p.real());
    Point p1 = c1.p + Point(cos(t + a) * c1.r, sin(t + a) * c1.r);
    Point p2 = c1.p + Point(cos(t - a) * c1.r, sin(t - a) * c1.r);
    return {p1, p2};
}

// http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=CGL_7_F
// tangent of circle c through point p接線
//接点
pair< Point, Point > tangent(const Circle &c1, const Point &p2) {
    return crosspoint(c1, Circle(p2, sqrt(norm(c1.p - p2) - c1.r * c1.r)));
}

// http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=CGL_7_G
// common tangent of circles c1 and c2
//二円の共通接線
Lines tangent(Circle c1, Circle c2) {
    Lines ret;
    if(c1.r < c2.r) swap(c1, c2);
    Real g = norm(c1.p - c2.p);
    if(eq(g, 0)) return ret;
    Point u = (c2.p - c1.p) / sqrt(g);
    Point v = rotate(PI * 0.5, u);
    for(int s : {-1, 1}) {
        Real h = (c1.r + s * c2.r) / sqrt(g);
        if(eq(1 - h * h, 0)) {
        ret.emplace_back(c1.p + u * c1.r, c1.p + (u + v) * c1.r);
        } else if(1 - h * h > 0) {
        Point uu = u * h, vv = v * sqrt(1 - h * h);
        ret.emplace_back(c1.p + (uu + vv) * c1.r, c2.p - (uu + vv) * c2.r * s);
        ret.emplace_back(c1.p + (uu - vv) * c1.r, c2.p - (uu - vv) * c2.r * s);
        }
    }
    return ret;
}

// http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=CGL_3_B
bool is_convex(const Polygon &p) {
    int n = (int) p.size();
    for(int i = 0; i < n; i++) {
        if(ccw(p[(i + n - 1) % n], p[i], p[(i + 1) % n]) == -1) return false;
    }
    return true;
}

// http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=CGL_4_A
// うまく使えないToDo
//頂点のみ(辺上の点は含まれない)
Polygon convex_hull(Polygon &p) {
    int n = (int) p.size(), k = 0;
    if(n <= 2) return p;
    sort(p.begin(), p.end());
    vector< Point > ch(2 * n);
    for(int i = 0; i < n; ch[k++] = p[i++]) {
        while(k >= 2 && cross(ch[k - 1] - ch[k - 2], p[i] - ch[k - 1]) < EPS) --k;
    }
    for(int i = n - 2, t = k + 1; i >= 0; ch[k++] = p[i--]) {
        while(k >= t && cross(ch[k - 1] - ch[k - 2], p[i] - ch[k - 1]) < EPS) --k;
    }
    ch.resize(k - 1);
    return ch;
}
//辺上の点も含まれる
Polygon convex_hull_containing_points_on_edges(Polygon &p) {
    int n = (int)p.size();
    int k = 0;
    if(n <= 2) return p;
    sort(p.begin(), p.end());
    vector<Point> ch(2 * n);
    for(int i = 0; i < n; ch[k++] = p[i++]) {
        while(k >= 2 && cross(ch[k - 1] - ch[k - 2], p[i] - ch[k - 1]) < 0) --k;
    }
    for(int i = n - 2, t = k + 1; i >= 0; ch[k++] = p[i--]) {
        while(k >= t && cross(ch[k - 1] - ch[k - 2], p[i] - ch[k - 1]) < 0) --k;
    }
    ch.resize(k - 1);
    return ch;
}

// http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=CGL_3_C
enum {
OUT, ON, IN
};

int contains(const Polygon &Q, const Point &p) {
    bool in = false;
    for(int i = 0; i < Q.size(); i++) {
        Point a = Q[i] - p, b = Q[(i + 1) % Q.size()] - p;
        if(a.imag() > b.imag()) swap(a, b);
        if(a.imag() <= 0 && 0 < b.imag() && cross(a, b) < 0) in = !in;
        if(cross(a, b) == 0 && dot(a, b) <= 0) return ON;
    }
    return in ? IN : OUT;
}

// http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=0412
int convex_contains(const Polygon &Q, const Point &p) {
    int N = (int) Q.size();
    Point g = (Q[0] + Q[N / 3] + Q[N * 2 / 3]) / 3.0;
    if(g == p) return IN;
    Point gp = p - g;
    int l = 0, r = N;
    while(r - l > 1) {
        int mid = (l + r) / 2;
        Point gl = Q[l] - g;
        Point gm = Q[mid] - g;
        if(cross(gl, gm) > 0) {
        if(cross(gl, gp) >= 0 && cross(gm, gp) <= 0) r = mid;
        else l = mid;
        } else {
        if(cross(gl, gp) <= 0 && cross(gm, gp) >= 0) l = mid;
        else r = mid;
        }
    }
    r %= N;
    Real v = cross(Q[l] - p, Q[r] - p);
    return eq(v, 0.0) ? ON : v < 0.0 ? OUT : IN;
}

// http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=1033
// deduplication of line segments
void merge_segments(vector< Segment > &segs) {

    auto merge_if_able = [](Segment &s1, const Segment &s2) {
        if(abs(cross(s1.b - s1.a, s2.b - s2.a)) > EPS) return false;
        if(ccw(s1.a, s2.a, s1.b) == 1 || ccw(s1.a, s2.a, s1.b) == -1) return false;
        if(ccw(s1.a, s1.b, s2.a) == -2 || ccw(s2.a, s2.b, s1.a) == -2) return false;
        s1 = Segment(min(s1.a, s2.a), max(s1.b, s2.b));
        return true;
    };

    for(int i = 0; i < segs.size(); i++) {
        if(segs[i].b < segs[i].a) swap(segs[i].a, segs[i].b);
    }
    for(int i = 0; i < segs.size(); i++) {
        for(int j = i + 1; j < segs.size(); j++) {
            if(merge_if_able(segs[i], segs[j])) {
                segs[j--] = segs.back(), segs.pop_back();
            }
        }
    }
}

// http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=1033
// construct a graph with the vertex of the intersection of any two line segments
vector< vector< int > > segment_arrangement(vector< Segment > &segs, vector< Point > &ps) {
    vector< vector< int > > g;
    int N = (int) segs.size();
    for(int i = 0; i < N; i++) {
        ps.emplace_back(segs[i].a);
        ps.emplace_back(segs[i].b);
        for(int j = i + 1; j < N; j++) {
        const Point p1 = segs[i].b - segs[i].a;
        const Point p2 = segs[j].b - segs[j].a;
        if(cross(p1, p2) == 0) continue;
        if(intersect(segs[i], segs[j])) {
            ps.emplace_back(crosspoint(segs[i], segs[j]));
        }
        }
    }
    sort(begin(ps), end(ps));
    ps.erase(unique(begin(ps), end(ps)), end(ps));

    int M = (int) ps.size();
    g.resize(M);
    for(int i = 0; i < N; i++) {
        vector< int > vec;
        for(int j = 0; j < M; j++) {
        if(intersect(segs[i], ps[j])) {
            vec.emplace_back(j);
        }
        }
        for(int j = 1; j < vec.size(); j++) {
        g[vec[j - 1]].push_back(vec[j]);
        g[vec[j]].push_back(vec[j - 1]);
        }
    }
    return (g);
}

// http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=CGL_4_C
// cut with a straight line l and return a convex polygon on the left
Polygon convex_cut(const Polygon &U, Line l) {
    Polygon ret;
    for(int i = 0; i < U.size(); i++) {
        Point now = U[i], nxt = U[(i + 1) % U.size()];
        if(ccw(l.a, l.b, now) != -1) ret.push_back(now);
        if(ccw(l.a, l.b, now) * ccw(l.a, l.b, nxt) < 0) {
        ret.push_back(crosspoint(Line(now, nxt), l));
        }
    }
    return (ret);
}

// http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=CGL_3_A
//多角形の面積
Real area(const Polygon &p) {
    Real A = 0;
    for(int i = 0; i < p.size(); ++i) {
        A += cross(p[i], p[(i + 1) % p.size()]);
    }
    return A * 0.5;
}

// http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=CGL_7_H
//多角形と円の共通部分の面積
Real area(const Polygon &p, const Circle &c) {
    if(p.size() < 3) return 0.0;
    function< Real(Circle, Point, Point) > cross_area = [&](const Circle &c, const Point &a, const Point &b) {
        Point va = c.p - a, vb = c.p - b;
        Real f = cross(va, vb), ret = 0.0;
        if(eq(f, 0.0)) return ret;
        if(max(abs(va), abs(vb)) < c.r + EPS) return f;
        if(distance(Segment(a, b), c.p) > c.r - EPS) return c.r * c.r * arg(vb * conj(va));
        auto u = crosspoint(c, Segment(a, b));
        vector< Point > tot{a, u.first, u.second, b};
        for(int i = 0; i + 1 < tot.size(); i++) {
        ret += cross_area(c, tot[i], tot[i + 1]);
        }
        return ret;
    };
    Real A = 0;
    for(int i = 0; i < p.size(); i++) {
        A += cross_area(c, p[i], p[(i + 1) % p.size()]);
    }
    return A;
}

// http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=CGL_4_B
Real convex_diameter(const Polygon &p) {
    int N = (int) p.size();
    int is = 0, js = 0;
    for(int i = 1; i < N; i++) {
        if(p[i].imag() > p[is].imag()) is = i;
        if(p[i].imag() < p[js].imag()) js = i;
    }
    Real maxdis = norm(p[is] - p[js]);

    int maxi, maxj, i, j;
    i = maxi = is;
    j = maxj = js;
    do {
        if(cross(p[(i + 1) % N] - p[i], p[(j + 1) % N] - p[j]) >= 0) {
        j = (j + 1) % N;
        } else {
        i = (i + 1) % N;
        }
        if(norm(p[i] - p[j]) > maxdis) {
        maxdis = norm(p[i] - p[j]);
        maxi = i;
        maxj = j;
        }
    } while(i != is || j != js);
    return sqrt(maxdis);
}

// http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=CGL_5_A
Real closest_pair(Points ps) {
if(ps.size() <= 1) throw (0);
sort(begin(ps), end(ps));

auto compare_y = [&](const Point &a, const Point &b) {
    return imag(a) < imag(b);
};
vector< Point > beet(ps.size());
const Real INF = 1e18;

function< Real(int, int) > rec = [&](int left, int right) {
    if(right - left <= 1) return INF;
    int mid = (left + right) >> 1;
    auto x = real(ps[mid]);
    auto ret = min(rec(left, mid), rec(mid, right));
    inplace_merge(begin(ps) + left, begin(ps) + mid, begin(ps) + right, compare_y);
    int ptr = 0;
    for(int i = left; i < right; i++) {
    if(abs(real(ps[i]) - x) >= ret) continue;
    for(int j = 0; j < ptr; j++) {
        auto luz = ps[i] - beet[ptr - j - 1];
        if(imag(luz) >= ret) break;
        ret = min(ret, abs(luz));
    }
    beet[ptr++] = ps[i];
    }
    return ret;
};
return rec(0, (int) ps.size());
}


//undebuged
//todo
bool contain(const Circle &c, const Point &p) {
    if(distance(c.p, p) <= c.r) return true;
    return false;
}

// return whether the circle C1 contains the Circle 2
// contain iff the two circles don't have any crosspoint (接する)
bool contain(const Circle &c1, const Circle &c2) {
    if(intersect(c1, c2) == 0 && contain(c1, c2.p + c2.r)) return true;
    return false;
}

//三角形の内接円の中心座標
//in circle
Point triangles_inscribed_circles_center_point(const Polygon &p) {
    assert((int)p.size() == 3);
    Real a = distance(p[0], p[1]), b = distance(p[1], p[2]), c = distance(p[2], p[0]);
    Real divisor = a + b + c;
    return (b * p[0] + c * p[1] + a * p[2]) / divisor;
}
Real triangles_inscribed_circles_radius(const Polygon &p) {
    assert((int)p.size() == 3);
    const Point center = triangles_inscribed_circles_center_point(p);
    return distance(Line(p[0], p[1]), center);
}
Circle triangles_inscribed_circle(const Polygon &p) {
    assert((int)p.size() == 3);
    return Circle(triangles_inscribed_circles_center_point(p), triangles_inscribed_circles_radius(p));
}

//三角形の外接円
//out circle
Point triangles_circumscribed_circles_center_point(const Polygon &p) {
    assert((int)p.size() == 3);
    Real a = p[0].real(), b = p[0].imag();
    Real c = p[1].real(), d = p[1].imag();
    Real e = p[2].real(), f = p[2].imag();
    Real y = ((e - a) * (a * a + b * b - c * c - d * d) - (c - a) * (a * a + b * b - e * e - f * f)) / ((Real)2*(e - a)*(b - d) - (Real)2*(c - a)*(b - f));
    Real x;
    if(c != a) x = (2 * (b - d)*y - a * a - b * b + c * c + d * d) / (2 * (c - a));
    else x = (2 * (b - f)*y - a * a - b * b + e * e + f * f) / (2 * (e - a));
    return Point(x, y);
}

Real triangles_circumscribed_circles_radius(const Polygon &p) {
    assert((int)p.size() == 3);
    return distance(p[0], triangles_circumscribed_circles_center_point(p));
}
Circle triangles_circumscribed_circle(const Polygon &p) {
    assert((int)p.size() == 3);
    return Circle(triangles_circumscribed_circles_center_point(p), triangles_circumscribed_circles_radius(p));
}

//反時計回りにソート(return)
Points sort_counterclockwise(Points &p) {
    int n = (int)p.size();
    int hoge = 0;
    for(int i = 1; i < n; i++) {
        if(p[hoge].imag() > p[i].imag() || (p[hoge].imag() == p[i].imag() && p[hoge].real() > p[i].real())) hoge = i;
    }
    Points ret;
    for(int i = 0; i < n; i++) {
        ret.push_back(p[(hoge + i)%n]);
    }
    return ret;
}
//反時計回りにソート(write)
void print_points_sorted_counterclockwise(Points &p) {
    int n = (int)p.size();
    int hoge = 0;
    for(int i = 1; i < n; i++) {
        if(p[hoge].imag() > p[i].imag() || (p[hoge].imag() == p[i].imag() && p[hoge].real() > p[i].real())) hoge = i;
    }
    for(int i = 0; i < n; i++) {
        cout << p[(hoge + i)%n].real() << " " << p[(hoge + i)%n].imag() << endl;
    }
}

//seed = time(0)
//seed = random_device()()
//最小内包円
//hackありのコンテストでは上記二つを使用すること。特にrandom_device()()が強い
//sqrt(r) が半径
template <class iter>
Circle min_ball(iter left, iter right, int seed = 1333) {
    const int n = right - left;

    assert(n >= 1);
    if (n == 1) {
        return {*left, Real(0)};
    }

    std::mt19937 mt(seed);
    std::shuffle(left, right, mt);
    // std::random_shuffle(left, right); // simple but deprecated

    iter ps = left;

    auto make_circle_3 = [](const Point &a, const Point &b, const Point &c) -> Circle {
        Real A = std::norm(b - c), B = std::norm(c - a), C = std::norm(a - b),
           S = cross(b - a, c - a);
        Point p = (A * (B + C - A) * a + B * (C + A - B) * b + C * (A + B - C) * c) / (4 * S * S);
        Real r2 = std::norm(p - a);
        return {p, r2};
    };

    auto make_circle_2 = [](const Point &a, const Point &b) -> Circle {
        Point c = (a + b) / (Real)2;
        Real r2 = std::norm(a - c);
        return {c, r2};
    };

    auto in_circle = [](const Point &a, const Circle &c) -> bool {
        return std::norm(a - c.p) <= c.r + EPS;
    };

    Circle c = make_circle_2(ps[0], ps[1]);

    // MiniDisc
    for (int i = 2; i < n; ++i) {
        if (!in_circle(ps[i], c)) {
            // MiniDiscWithPoint
            c = make_circle_2(ps[0], ps[i]);
            for (int j = 1; j < i; ++j) {
                if (!in_circle(ps[j], c)) {
                    // MiniDiscWith2Points
                    c = make_circle_2(ps[i], ps[j]);
                    for (int k = 0; k < j; ++k) {
                        if (!in_circle(ps[k], c)) {
                            c = make_circle_3(ps[i], ps[j], ps[k]);
                        }
                    }
                }
            }
        }
    }
    return c;
}