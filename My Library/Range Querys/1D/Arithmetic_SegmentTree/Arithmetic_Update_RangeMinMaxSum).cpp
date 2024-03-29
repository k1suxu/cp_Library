#include"atcoder/all"

const ll LINF = (ll)4e18;
const int IINF = (int)1e9+10;
struct S {
    ll min, max, sum;
    int l, r;
    S() = default;
    S(ll min, ll max, ll sum, int l, int r) : min(min), max(max), sum(sum), l(l), r(r) {}
};
struct F {
    ll a, b;
    F() = default;
    F(ll a, ll b) : a(a), b(b) {}
};
S op(S s, S t) {
    return {min(s.min, t.min), max(s.max, t.max), s.sum + t.sum, min(s.l, t.l), max(s.r, t.r)};
}
S e() { return {LINF, -LINF, 0, IINF, -IINF}; }
S mapping(F f, S s) {
    if (f.a == LINF) return s;
    if (f.a >= 0) return {f.a * s.l + f.b, f.a * (s.r - 1) + f.b, (f.a * (s.l + s.r - 1) + f.b * 2) * (s.r - s.l) / 2, s.l, s.r};
    return {f.a * (s.r - 1) + f.b, f.a * s.l + f.b, (f.a * (s.l + s.r - 1) + f.b * 2) * (s.r - s.l) / 2, s.l, s.r};
}
F composition(F f, F g) { return f.a == LINF ? g : f; }
F id() { return {LINF, LINF}; }

using lazy_segtype = atcoder::lazy_segtree<S, op, e, F, mapping, composition, id>;

struct lazy_segtree_arithmetic : lazy_segtype {
    using lazy_segtype::lazy_segtype;
    lazy_segtree_arithmetic(int n) {
        vector<S> sv(n);
        for (int i = 0; i < n; i++) sv[i] = {0, 0, 0, i, i + 1};
        (*this) = lazy_segtree_arithmetic(sv);
    }
    lazy_segtree_arithmetic(vector<ll> v) {
        vector<S> sv(v.size());
        for (int i = 0; i < v.size(); i++) sv[i] = {v[i], v[i], v[i], i, i + 1};
        (*this) = lazy_segtree_arithmetic(sv);
    }
    // f = (a, b)
    // [l, r) �� b, a+b, 2a+b, ... �ōX�V
    void apply(int l, int r, F f) {
        lazy_segtype::apply(l, r, {f.a, f.b - f.a * get(l).l});
    }
    void apply(int p, F f) { apply(p, {f.a, f.b - f.a * get(p).l}); }
};

//be careful the data types(int, long long)
//