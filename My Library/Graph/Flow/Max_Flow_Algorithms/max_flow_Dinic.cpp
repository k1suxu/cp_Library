template<typename T>
struct Edge {
    int to;
    T cap;
    int rev;

    Edge() = default;
    Edge(int to, T cap, int rev) : to(to), cap(cap), rev(rev) {}
};

template<typename T>
struct Dinic {
    const T e = numeric_limits<T>::max();
    const T zero = 0;

    int n;
    vector<vector<Edge<T>>> g;
    vector<int> level, iter;

    Dinic(int n) : n(n), g(n) {}

    void add_edge(int from, int to, T cap) {
        Edge<T> f(to, cap, g[to].size());
        Edge<T> t(from, zero, g[from].size());

        g[from].push_back(f);
        g[to].push_back(t);
    }

    bool bfs(int s, int t) {
        level.assign(n, -1);
        level[s] = 0;
        queue<int> que;
        que.push(s);

        while(que.size()) {
            int v = que.front();
            que.pop();

            for(auto e : g[v]) {
                if(e.cap > zero && level[e.to] < 0) {
                    level[e.to] = level[v] + 1;
                    que.push(e.to);
                }
            }
        }

        return level[t] != -1;
    }

    T dfs(int v, int t, T f) {
        if(v == t) return f;

        for(int& i = iter[v]; i < g[v].size(); i++) {
            Edge<T>& e = g[v][i];

            if(e.cap > zero && level[v] < level[e.to]) {
                T d = dfs(e.to, t, min(f, e.cap));

                if(d > zero) {
                    e.cap -= d;
                    g[e.to][e.rev].cap += d;
                    return d;
                }
            }
        }

        return zero;
    }

    T flow(int s, int t) {
        T f = zero;

        while(bfs(s, t)) {
            iter.assign(n, 0);
            T d;

            while((d = dfs(s, t, e)) > zero) f += d;
        }

        return f;
    }
};

//bipartite matching 
//TODO: make faster Dinic using dynamic tree structure and current edge structure
//Rโท฿้ref: https://qiita.com/ningenMe/items/69ed7ce43c9cd0a2de38