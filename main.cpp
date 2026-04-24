#include <iostream>
#include <vector>
#include <queue>
#include <cstring>
#include <algorithm>

using namespace std;

const int MAXN = 3005;
const int INF = 1e9;

struct Edge {
    int to, cap, rev, orig_cap;
};

class MaxFlow {
private:
    vector<Edge> graph[MAXN];
    int level[MAXN];
    int iter[MAXN];
    int n;

    void bfs(int s) {
        memset(level, -1, sizeof(level));
        queue<int> q;
        level[s] = 0;
        q.push(s);
        
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (auto& e : graph[v]) {
                if (e.cap > 0 && level[e.to] < 0) {
                    level[e.to] = level[v] + 1;
                    q.push(e.to);
                }
            }
        }
    }

    int dfs(int v, int t, int f) {
        if (v == t) return f;
        for (int& i = iter[v]; i < (int)graph[v].size(); i++) {
            Edge& e = graph[v][i];
            if (e.cap > 0 && level[v] < level[e.to]) {
                int d = dfs(e.to, t, min(f, e.cap));
                if (d > 0) {
                    e.cap -= d;
                    graph[e.to][e.rev].cap += d;
                    return d;
                }
            }
        }
        return 0;
    }

public:
    void init(int n) {
        this->n = n;
        for (int i = 0; i <= n; i++) {
            graph[i].clear();
        }
    }

    void add_edge(int from, int to, int cap) {
        graph[from].push_back({to, cap, (int)graph[to].size(), cap});
        graph[to].push_back({from, 0, (int)graph[from].size() - 1, 0});
    }

    void reset_capacities() {
        for (int i = 0; i <= n; i++) {
            for (auto& e : graph[i]) {
                e.cap = e.orig_cap;
            }
        }
    }

    int max_flow(int s, int t) {
        int flow = 0;
        while (true) {
            bfs(s);
            if (level[t] < 0) return flow;
            memset(iter, 0, sizeof(iter));
            int f;
            while ((f = dfs(s, t, INF)) > 0) {
                flow += f;
                if (flow >= 3) return flow; // Early termination: max degree is 3
            }
        }
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m;
    cin >> n >> m;
    
    MaxFlow mf;
    mf.init(n);
    
    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        mf.add_edge(a, b, 1);
        mf.add_edge(b, a, 1);
    }
    
    long long total = 0;
    
    for (int a = 1; a <= n; a++) {
        for (int b = a + 1; b <= n; b++) {
            mf.reset_capacities();
            int flow = mf.max_flow(a, b);
            total += flow;
        }
    }
    
    cout << total << endl;
    
    return 0;
}
