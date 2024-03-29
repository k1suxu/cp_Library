// #pragma GCC target("avx")
// #pragma GCC optimize("O3")
// #pragma GCC optimize("unroll-loops")

#include <bits/stdc++.h>
using namespace std;

template<long long MOD>
struct Modular_Int {
    using Mint_Type = Modular_Int<MOD>;
    long long x;

    Modular_Int() = default;
    Modular_Int(long long x_) : x(x_ >= 0? x_%MOD : (MOD-(-x_)%MOD)%MOD) {}

    long long val() const {
        return (x%MOD+MOD)%MOD;
    }
    static long long get_mod() {
        return MOD;
    }

    Mint_Type& operator^=(long long d)  {
        Mint_Type ret(1);
        long long nx = x;
        while(d) {
            if(d&1) ret *= nx;
            (nx *= nx) %= MOD;
            d >>= 1;
        }
        *this = ret;
        return *this;
    }
    Mint_Type operator^(long long d) const {return Mint_Type(*this) ^= d;}
    Mint_Type pow(long long d) const {return Mint_Type(*this) ^= d;}
    
    //use this basically
    Mint_Type inv() const {
        return Mint_Type(*this) ^ (MOD-2);
    }
    //only if the module number is not prime
    //Don't use. This is broken.
    // Mint_Type inv() const {
    //     long long a = (x%MOD+MOD)%MOD, b = MOD, u = 1, v = 0;
    //     while(b) {
    //         long long t = a/b;
    //         a -= t*b, swap(a, b);
    //         u -= t*v, swap(u, v);
    //     }
    //     return Mint_Type(u);
    // }

    Mint_Type& operator+=(const Mint_Type other) {
        if((x += other.x) >= MOD) x -= MOD;
        return *this;
    }
    Mint_Type& operator-=(const Mint_Type other) {
        if((x -= other.x) < 0) x += MOD;
        return *this;
    }
    Mint_Type& operator*=(const Mint_Type other) {
        long long z = x;
        z *= other.x;
        z %= MOD;
        x = z;
        if(x < 0) x += MOD;
        return *this;
    }
    Mint_Type& operator/=(const Mint_Type other) {
        return *this = *this * other.inv();
    }
    Mint_Type& operator++() {
        x++;
        if (x == MOD) x = 0;
        return *this;
    }
    Mint_Type& operator--() {
        if (x == 0) x = MOD;
        x--;
        return *this;
    }
    
    Mint_Type operator+(const Mint_Type other) const {return Mint_Type(*this) += other;}
    Mint_Type operator-(const Mint_Type other) const {return Mint_Type(*this) -= other;}
    Mint_Type operator*(const Mint_Type other) const {return Mint_Type(*this) *= other;}
    Mint_Type operator/(const Mint_Type other) const {return Mint_Type(*this) /= other;}
    
    Mint_Type& operator+=(const long long other) {Mint_Type other_(other); *this += other_; return *this;}
    Mint_Type& operator-=(const long long other) {Mint_Type other_(other); *this -= other_; return *this;}
    Mint_Type& operator*=(const long long other) {Mint_Type other_(other); *this *= other_; return *this;}
    Mint_Type& operator/=(const long long other) {Mint_Type other_(other); *this /= other_; return *this;}
    Mint_Type operator+(const long long other) const {return Mint_Type(*this) += other;}
    Mint_Type operator-(const long long other) const {return Mint_Type(*this) -= other;}
    Mint_Type operator*(const long long other) const {return Mint_Type(*this) *= other;}
    Mint_Type operator/(const long long other) const {return Mint_Type(*this) /= other;}

    bool operator==(const Mint_Type other) const {return (*this).val() == other.val();}
    bool operator!=(const Mint_Type other) const {return (*this).val() != other.val();}
    bool operator==(const long long other) const {return (*this).val() == other;}
    bool operator!=(const long long other) const {return (*this).val() != other;}

    Mint_Type operator-() const {return Mint_Type(0LL)-Mint_Type(*this);}

    //-1: sqrtが存在しない
    //複数存在する場合どれを返すかは不明
    long long get_sqrt() const {
        long long a = val(), p = get_mod();
        if(a == 0) return 0;
        if(p == 2) return a;
        if(Mint_Type(a).pow((p - 1) >> 1).val() != 1) return -1;
        long long b = 1;
        while(Mint_Type(b).pow((p - 1) >> 1).val() == 1) ++b;
        long long e = 0, m = p - 1;
        while(m % 2 == 0) m >>= 1, ++e;
        long long x = Mint_Type(a).pow((m - 1) >> 1).val();
        long long y = a * (x * x % p) % p;
        (x *= a) %= p;
        long long z = Mint_Type(b).pow(m).val();
        while(y != 1) {
            long long j = 0, t = y;
            while(t != 1) {
                j += 1;
                (t *= t) %= p;
            }
            z = Mint_Type(z).pow((long long)1 << (e - j - 1)).val();
            (x *= z) %= p;
            (z *= z) %= p;
            (y *= z) %= p;
            e = j;
        }
        return x;
    }
};

// const long long MOD_VAL = 1e9+7;
const long long MOD_VAL = 998244353;
using mint = Modular_Int<MOD_VAL>;

istream& operator>>(istream& is, mint& x) {
    long long X;
    is >> X;
    x = X;
    return is;
}
ostream& operator<<(ostream& os, mint& x) {
    os << x.val();
    return os;
}

//mint operator^ は掛け算みたいに優先されないことに注意！！
//基本的にはpowを使うこと！！
//cf: http://www5f.biglobe.ne.jp/~fuku-labo/library/program/cpp/1/008-1.htm

//need to include mod_int structure

//ref: https://ei1333.github.io/library/math/fft/number-theoretic-transform-friendly-mod-int.hpp
template< typename Mint >
struct NumberTheoreticTransformFriendlyModInt {

  static vector< Mint > roots, iroots, rate3, irate3;
  static int max_base;

  NumberTheoreticTransformFriendlyModInt() = default;

  static void init() {
    if(roots.empty()) {
      const unsigned mod = Mint::get_mod();
      assert(mod >= 3 && mod % 2 == 1);
      auto tmp = mod - 1;
      max_base = 0;
      while(tmp % 2 == 0) tmp >>= 1, max_base++;
      Mint root = 2;
      while(root.pow((mod - 1) >> 1) == 1) {
        root += 1;
      }
      assert(root.pow(mod - 1) == 1);

      roots.resize(max_base + 1);
      iroots.resize(max_base + 1);
      rate3.resize(max_base + 1);
      irate3.resize(max_base + 1);

      roots[max_base] = root.pow((mod - 1) >> max_base);
      iroots[max_base] = Mint(1) / roots[max_base];
      for(int i = max_base - 1; i >= 0; i--) {
        roots[i] = roots[i + 1] * roots[i + 1];
        iroots[i] = iroots[i + 1] * iroots[i + 1];
      }
      {
        Mint prod = 1, iprod = 1;
        for(int i = 0; i <= max_base - 3; i++) {
          rate3[i] = roots[i + 3] * prod;
          irate3[i] = iroots[i + 3] * iprod;
          prod *= iroots[i + 3];
          iprod *= roots[i + 3];
        }
      }
    }
  }

  static void ntt(vector< Mint > &a) {
    init();
    const int n = (int) a.size();
    assert((n & (n - 1)) == 0);
    int h = __builtin_ctz(n);
    assert(h <= max_base);
    int len = 0;
    Mint imag = roots[2];
    if(h & 1) {
      int p = 1 << (h - 1);
      Mint rot = 1;
      for(int i = 0; i < p; i++) {
        auto r = a[i + p];
        a[i + p] = a[i] - r;
        a[i] += r;
      }
      len++;
    }
    for(; len + 1 < h; len += 2) {
      int p = 1 << (h - len - 2);
      { // s = 0
        for(int i = 0; i < p; i++) {
          auto a0 = a[i];
          auto a1 = a[i + p];
          auto a2 = a[i + 2 * p];
          auto a3 = a[i + 3 * p];
          auto a1na3imag = (a1 - a3) * imag;
          auto a0a2 = a0 + a2;
          auto a1a3 = a1 + a3;
          auto a0na2 = a0 - a2;
          a[i] = a0a2 + a1a3;
          a[i + 1 * p] = a0a2 - a1a3;
          a[i + 2 * p] = a0na2 + a1na3imag;
          a[i + 3 * p] = a0na2 - a1na3imag;
        }
      }
      Mint rot = rate3[0];
      for(int s = 1; s < (1 << len); s++) {
        int offset = s << (h - len);
        Mint rot2 = rot * rot;
        Mint rot3 = rot2 * rot;
        for(int i = 0; i < p; i++) {
          auto a0 = a[i + offset];
          auto a1 = a[i + offset + p] * rot;
          auto a2 = a[i + offset + 2 * p] * rot2;
          auto a3 = a[i + offset + 3 * p] * rot3;
          auto a1na3imag = (a1 - a3) * imag;
          auto a0a2 = a0 + a2;
          auto a1a3 = a1 + a3;
          auto a0na2 = a0 - a2;
          a[i + offset] = a0a2 + a1a3;
          a[i + offset + 1 * p] = a0a2 - a1a3;
          a[i + offset + 2 * p] = a0na2 + a1na3imag;
          a[i + offset + 3 * p] = a0na2 - a1na3imag;
        }
        rot *= rate3[__builtin_ctz(~s)];
      }
    }
  }

  static void intt(vector< Mint > &a, bool f = true) {
    init();
    const int n = (int) a.size();
    assert((n & (n - 1)) == 0);
    int h = __builtin_ctz(n);
    assert(h <= max_base);
    int len = h;
    Mint iimag = iroots[2];
    for(; len > 1; len -= 2) {
      int p = 1 << (h - len);
      { // s = 0
        for(int i = 0; i < p; i++) {
          auto a0 = a[i];
          auto a1 = a[i + 1 * p];
          auto a2 = a[i + 2 * p];
          auto a3 = a[i + 3 * p];
          auto a2na3iimag = (a2 - a3) * iimag;
          auto a0na1 = a0 - a1;
          auto a0a1 = a0 + a1;
          auto a2a3 = a2 + a3;
          a[i] = a0a1 + a2a3;
          a[i + 1 * p] = (a0na1 + a2na3iimag);
          a[i + 2 * p] = (a0a1 - a2a3);
          a[i + 3 * p] = (a0na1 - a2na3iimag);
        }
      }
      Mint irot = irate3[0];
      for(int s = 1; s < (1 << (len - 2)); s++) {
        int offset = s << (h - len + 2);
        Mint irot2 = irot * irot;
        Mint irot3 = irot2 * irot;
        for(int i = 0; i < p; i++) {
          auto a0 = a[i + offset];
          auto a1 = a[i + offset + 1 * p];
          auto a2 = a[i + offset + 2 * p];
          auto a3 = a[i + offset + 3 * p];
          auto a2na3iimag = (a2 - a3) * iimag;
          auto a0na1 = a0 - a1;
          auto a0a1 = a0 + a1;
          auto a2a3 = a2 + a3;
          a[i + offset] = a0a1 + a2a3;
          a[i + offset + 1 * p] = (a0na1 + a2na3iimag) * irot;
          a[i + offset + 2 * p] = (a0a1 - a2a3) * irot2;
          a[i + offset + 3 * p] = (a0na1 - a2na3iimag) * irot3;
        }
        irot *= irate3[__builtin_ctz(~s)];
      }
    }
    if(len >= 1) {
      int p = 1 << (h - 1);
      for(int i = 0; i < p; i++) {
        auto ajp = a[i] - a[i + p];
        a[i] += a[i + p];
        a[i + p] = ajp;
      }
    }
    if(f) {
      Mint inv_sz = Mint(1) / n;
      for(int i = 0; i < n; i++) a[i] *= inv_sz;
    }
  }

  static vector< Mint > multiply(vector< Mint > a, vector< Mint > b) {
    int need = a.size() + b.size() - 1;
    int nbase = 1;
    while((1 << nbase) < need) nbase++;
    int sz = 1 << nbase;
    a.resize(sz, 0);
    b.resize(sz, 0);
    ntt(a);
    ntt(b);
    Mint inv_sz = Mint(1) / sz;
    for(int i = 0; i < sz; i++) a[i] *= b[i] * inv_sz;
    intt(a, false);
    a.resize(need);
    return a;
  }
};

template< typename Mint >
vector< Mint > NumberTheoreticTransformFriendlyModInt< Mint >::roots = vector< Mint >();
template< typename Mint >
vector< Mint > NumberTheoreticTransformFriendlyModInt< Mint >::iroots = vector< Mint >();
template< typename Mint >
vector< Mint > NumberTheoreticTransformFriendlyModInt< Mint >::rate3 = vector< Mint >();
template< typename Mint >
vector< Mint > NumberTheoreticTransformFriendlyModInt< Mint >::irate3 = vector< Mint >();
template< typename Mint >
int NumberTheoreticTransformFriendlyModInt< Mint >::max_base = 0;

//ref: https://ei1333.github.io/library/math/fps/formal-power-series-friendly-ntt.hpp
template< typename T >
struct FormalPowerSeriesFriendlyNTT : vector< T > {
    using vector< T >::vector;
    using P = FormalPowerSeriesFriendlyNTT;
    using NTT = NumberTheoreticTransformFriendlyModInt< T >;

    // prefix
    P pre(int deg) const {
        return P(begin(*this), begin(*this) + min((int) this->size(), deg));
    }

    P rev(int deg = -1) const {
        P ret(*this);
        if(deg != -1) ret.resize(deg, T(0));
        reverse(begin(ret), end(ret));
        return ret;
    }

    void shrink() {
        while(this->size() && this->back() == T(0)) this->pop_back();
    }

    P operator+(const P &r) const { return P(*this) += r; }

    P operator+(const T &v) const { return P(*this) += v; }

    P operator-(const P &r) const { return P(*this) -= r; }

    P operator-(const T &v) const { return P(*this) -= v; }

    P operator*(const P &r) const { return P(*this) *= r; }

    P operator*(const T &v) const { return P(*this) *= v; }

    P operator/(const P &r) const { return P(*this) /= r; }

    P operator%(const P &r) const { return P(*this) %= r; }

    P &operator+=(const P &r) {
        if(r.size() > this->size()) this->resize(r.size());
        for(int i = 0; i < (int) r.size(); i++) (*this)[i] += r[i];
        return *this;
    }

    P &operator-=(const P &r) {
        if(r.size() > this->size()) this->resize(r.size());
        for(int i = 0; i < (int) r.size(); i++) (*this)[i] -= r[i];
        return *this;
    }

    // https://judge.yosupo.jp/problem/convolution_mod
    P &operator*=(const P &r) {
        if(this->empty() || r.empty()) {
            this->clear();
            return *this;
        }
        auto ret = NTT::multiply(*this, r);
        return *this = {begin(ret), end(ret)};
    }

    P &operator/=(const P &r) {
        if(this->size() < r.size()) {
            this->clear();
            return *this;
        }
        int n = this->size() - r.size() + 1;
        return *this = (rev().pre(n) * r.rev().inv(n)).pre(n).rev(n);
    }

    P &operator%=(const P &r) {
        *this -= *this / r * r;
        shrink();
        return *this;
    }

    // https://judge.yosupo.jp/problem/division_of_polynomials
    pair< P, P > div_mod(const P &r) {
        P q = *this / r;
        P x = *this - q * r;
        x.shrink();
        return make_pair(q, x);
    }

    P operator-() const {
        P ret(this->size());
        for(int i = 0; i < (int) this->size(); i++) ret[i] = -(*this)[i];
        return ret;
    }

    P &operator+=(const T &r) {
        if(this->empty()) this->resize(1);
        (*this)[0] += r;
        return *this;
    }

    P &operator-=(const T &r) {
        if(this->empty()) this->resize(1);
        (*this)[0] -= r;
        return *this;
    }

    P &operator*=(const T &v) {
        for(int i = 0; i < (int) this->size(); i++) (*this)[i] *= v;
        return *this;
    }

    P dot(P r) const {
        P ret(min(this->size(), r.size()));
        for(int i = 0; i < (int) ret.size(); i++) ret[i] = (*this)[i] * r[i];
        return ret;
    }

    P operator>>(int sz) const {
        if((int) this->size() <= sz) return {};
        P ret(*this);
        ret.erase(ret.begin(), ret.begin() + sz);
        return ret;
    }

    P operator<<(int sz) const {
        P ret(*this);
        ret.insert(ret.begin(), sz, T(0));
        return ret;
    }

    T operator()(T x) const {
        T r = 0, w = 1;
        for(auto &v : *this) {
            r += w * v;
            w *= x;
        }
        return r;
    }

    // differential (微分)
    P diff() const {
        const int n = (int) this->size();
        P ret(max((int)0, n-1));
        for(int i = 1; i < n; i++) ret[i - 1] = (*this)[i] * T(i);
        return ret;
    }

    P integral() const {
        const int n = (int) this->size();
        P ret(n + 1);
        ret[0] = T(0);
        for(int i = 0; i < n; i++) ret[i + 1] = (*this)[i] / T(i + 1);
        return ret;
    }

    // https://judge.yosupo.jp/problem/inv_of_formal_power_series
    // F(0) must not be 0
    P inv(int deg = -1) const {
        assert(((*this)[0]) != T(0));
        const int n = (int) this->size();
        if(deg == -1) deg = n;
        P res(deg);
        res[0] = {T(1) / (*this)[0]};
        for(int d = 1; d < deg; d <<= 1) {
            P f(2 * d), g(2 * d);
            for(int j = 0; j < min(n, 2 * d); j++) f[j] = (*this)[j];
            for(int j = 0; j < d; j++) g[j] = res[j];
            NTT::ntt(f);
            NTT::ntt(g);
            f = f.dot(g);
            NTT::intt(f);
            for(int j = 0; j < d; j++) f[j] = 0;
            NTT::ntt(f);
            for(int j = 0; j < 2 * d; j++) f[j] *= g[j];
            NTT::intt(f);
            for(int j = d; j < min(2 * d, deg); j++) res[j] = -f[j];
        }
        return res;
    }

    // https://judge.yosupo.jp/problem/log_of_formal_power_series
    // F(0) must be 1
    P log(int deg = -1) const {
        assert((*this)[0] == T(1));
        const int n = (int) this->size();
        if(deg == -1) deg = n;
        return (this->diff() * this->inv(deg)).pre(deg - 1).integral();
    }

    // https://judge.yosupo.jp/problem/sqrt_of_formal_power_series
    P sqrt(int deg = -1) const {
        const int n = (int) this->size();
        if(deg == -1) deg = n;
        if((*this)[0] == T(0)) {
            for(int i = 1; i < n; i++) {
                if((*this)[i] != T(0)) {
                    if(i & 1) return {};
                    if(deg - i / 2 <= 0) break;
                    auto ret = (*this >> i).sqrt(deg - i / 2);
                    if(ret.empty()) return {};
                    ret = ret << (i / 2);
                    if((int) ret.size() < deg) ret.resize(deg, T(0));
                    return ret;
                }
            }
            return P(deg, 0);
        }
        auto sqr = T((*this)[0].get_sqrt());
        if(sqr * sqr != (*this)[0]) return {};
        P ret{sqr};
        T inv2 = T(1) / T(2);
        for(int i = 1; i < deg; i <<= 1) {
            ret = (ret + pre(i << 1) * ret.inv(i << 1)) * inv2;
        }
        return ret.pre(deg);
    }

    // https://judge.yosupo.jp/problem/exp_of_formal_power_series
    // F(0) must be 0
    P exp(int deg = -1) const {
        if(deg == -1) deg = this->size();
        assert((*this)[0] == T(0));

        P inv;
        inv.reserve(deg + 1);
        inv.push_back(T(0));
        inv.push_back(T(1));

        auto inplace_integral = [&](P &F) -> void {
            const int n = (int) F.size();
            auto mod = T::get_mod();
            while((int) inv.size() <= n) {
                int i = inv.size();
                inv.push_back((-inv[mod % i]) * (mod / i));
            }
            F.insert(begin(F), T(0));
            for(int i = 1; i <= n; i++) F[i] *= inv[i];
        };

        auto inplace_diff = [](P &F) -> void {
            if(F.empty()) return;
            F.erase(begin(F));
            T coeff = 1, one = 1;
            for(int i = 0; i < (int) F.size(); i++) {
                F[i] *= coeff;
                coeff += one;
            }
        };

        P b{1, 1 < (int) this->size() ? (*this)[1] : 0}, c{1}, z1, z2{1, 1};
        for(int m = 2; m < deg; m *= 2) {
            auto y = b;
            y.resize(2 * m);
            NTT::ntt(y);
            z1 = z2;
            P z(m);
            for(int i = 0; i < m; ++i) z[i] = y[i] * z1[i];
            NTT::intt(z);
            fill(begin(z), begin(z) + m / 2, T(0));
            NTT::ntt(z);
            for(int i = 0; i < m; ++i) z[i] *= -z1[i];
            NTT::intt(z);
            c.insert(end(c), begin(z) + m / 2, end(z));
            z2 = c;
            z2.resize(2 * m);
            NTT::ntt(z2);
            P x(begin(*this), begin(*this) + min< int >(this->size(), m));
            inplace_diff(x);
            x.push_back(T(0));
            NTT::ntt(x);
            for(int i = 0; i < m; ++i) x[i] *= y[i];
            NTT::intt(x);
            x -= b.diff();
            x.resize(2 * m);
            for(int i = 0; i < m - 1; ++i) x[m + i] = x[i], x[i] = T(0);
            NTT::ntt(x);
            for(int i = 0; i < 2 * m; ++i) x[i] *= z2[i];
            NTT::intt(x);
            x.pop_back();
            inplace_integral(x);
            for(int i = m; i < min< int >(this->size(), 2 * m); ++i) x[i] += (*this)[i];
            fill(begin(x), begin(x) + m, T(0));
            NTT::ntt(x);
            for(int i = 0; i < 2 * m; ++i) x[i] *= y[i];
            NTT::intt(x);
            b.insert(end(b), begin(x) + m, end(x));
        }
        return P{begin(b), begin(b) + deg};
    }

    // https://judge.yosupo.jp/problem/pow_of_formal_power_series
    P pow(int64_t k, int deg = -1) const {
        const int n = (int) this->size();
        if(deg == -1) deg = n;
        if(k == 0) {
            P ret(deg, T(0));
            ret[0] = T(1);
            return ret;
        }
        for(int i = 0; i < n; i++) {
            if(i * k > deg) return P(deg, T(0));
            if((*this)[i] != T(0)) {
                T rev = T(1) / (*this)[i];
                P ret = (((*this * rev) >> i).log(deg) * k).exp(deg) * ((*this)[i].pow(k));
                ret = (ret << (i * k)).pre(deg);
                if((int) ret.size() < deg) ret.resize(deg, T(0));
                return ret;
            }
        }
        return *this;
    }

    P mod_pow(int64_t k, P g) const {
        P modinv = g.rev().inv();
        auto get_div = [&](P base) {
            if(base.size() < g.size()) {
                base.clear();
                return base;
            }
            int n = base.size() - g.size() + 1;
            return (base.rev().pre(n) * modinv.pre(n)).pre(n).rev(n);
        };
        P x(*this), ret{1};
        while(k > 0) {
            if(k & 1) {
                ret *= x;
                ret -= get_div(ret) * g;
                ret.shrink();
            }
            x *= x;
            x -= get_div(x) * g;
            x.shrink();
            k >>= 1;
        }
        return ret;
    }

    // https://judge.yosupo.jp/problem/polynomial_taylor_shift
    P taylor_shift(T c) const {
        int n = (int) this->size();
        vector< T > fact(n), rfact(n);
        fact[0] = rfact[0] = T(1);
        for(int i = 1; i < n; i++) fact[i] = fact[i - 1] * T(i);
        rfact[n - 1] = T(1) / fact[n - 1];
        for(int i = n - 1; i > 1; i--) rfact[i - 1] = rfact[i] * T(i);
        P p(*this);
        for(int i = 0; i < n; i++) p[i] *= fact[i];
        p = p.rev();
        P bs(n, T(1));
        for(int i = 1; i < n; i++) bs[i] = bs[i - 1] * c * rfact[i] * fact[i - 1];
        p = (p * bs).pre(n);
        p = p.rev();
        for(int i = 0; i < n; i++) p[i] *= rfact[i];
        return p;
    }
};

//FPSの挙動 refer: https://ei1333.github.io/library/math/fps/formal-power-series-friendly-ntt.hpp

template< typename Mint >
using FPS = FormalPowerSeriesFriendlyNTT< Mint >;
using fps = FPS<mint>;

// Π(1 + a_ix)みたいなmulti linear productionも計算できる
// calc Π poly[i] with `devide and conquor algorithm`
// 次数の総和をNとしたときにO(N\log^2{N})
template<template<typename> class Inner_FPS, typename Mint>
Inner_FPS<Mint> many_polynomials_production(vector<Inner_FPS<Mint>> poly) {
    const int N = (int)poly.size();
    if(N == 0) return {1};
    auto rec = [&](auto self, int l, int r) -> Inner_FPS<Mint> {
        if(l == r) return poly[l];
        int mid = (l+r)/2;
        return move(self(self, l, mid)) * move(self(self, mid+1, r));
    };
    return rec(rec, 0, N-1);
}

// インスタンスは自動解釈される
// ref: https://37zigen.com/multipoint-evaluation/
// used for `multipoint_evaluation` and `polynomial_interpolation` as a `sub` function
template<template<typename> class Inner_FPS, typename Mint>
vector<Inner_FPS<Mint>> subproduct_tree(const Inner_FPS<Mint> &xs) {
    const int n = (int)xs.size();
    int k = 1;
    while(k < n) k <<= 1;
    vector<Inner_FPS<Mint>> g(2*k, {1});
    for(int i = 0; i < n; i++) g[k+i] = {-xs[i], Mint(1)};
    for(int i = k; i-- > 1;) g[i] = g[i<<1] * g[i<<1 | 1];
    return g;
}
// インスタンスは自動解釈される
// N次多項式f, M個の評価点x1,x2,...,xmについてf(x1), f(x2), ..., f(xm)
// をO(N\log{N} + M\log^2{M})で求める
template<template<typename> class Inner_FPS, typename Mint>
Inner_FPS<Mint> multipoint_evaluation(const Inner_FPS<Mint> &f, const Inner_FPS<Mint> &xs) {
    vector<Inner_FPS<Mint>> g = subproduct_tree(xs);
    int m = (int)xs.size(), k = (int)g.size() / 2;
    g[1] = f % g[1];
    for(int i = 2; i < k+m; i++) g[i] = g[i>>1] % g[i];
    Inner_FPS<Mint> ys(m);
    for(int i = 0; i < m; i++)  ys[i] = (g[k+i].empty()? Mint(0) : g[k+i][0]);
    return ys;
}
// インスタンスは自動解釈される
// for all i(0,2,...,N-1) f(xi)≡f(yi) をみたすN-1次多項式を求める(これは一意に定まる)
template<template<typename> class Inner_FPS, typename Mint>
Inner_FPS<Mint> polynomial_interpolation(const Inner_FPS<Mint> &xs, const Inner_FPS<Mint> &ys) {
    assert(xs.size() == ys.size());
    vector<Inner_FPS<Mint>> mul = subproduct_tree(xs);
    int n = (int)xs.size(), k = (int)mul.size() / 2;
    vector<Inner_FPS<Mint>> g(2*k);
    g[1] = mul[1].diff() % mul[1];
    for(int i = 2; i < k+n; i++) g[i] = g[i>>1]%mul[i];
    for(int i = 0; i < n; i++) g[k+i] = {ys[i] / g[k+i][0]};
    for(int i = k; i-- > 1;) g[i] = g[i<<1] * mul[i<<1 | 1] + g[i<<1 | 1] * mul[i<<1];
    return g[1];
}

void solve() {
    
}

int main() {
    cin.tie(nullptr);
    ios::sync_with_stdio(false);
    solve();
    return 0;
}
