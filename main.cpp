#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

const int MAXN = 3005;
const int INF = 1e9;

struct Edge {
    int to, cap, rev, orig_cap;
};

vector<Edge> graph[MAXN];
int level[MAXN];
int iter[MAXN];
int level_version[MAXN];
int current_level_version = 0;
int n;

inline void bfs(int s) {
    current_level_version++;
    queue<int> q;
    level[s] = 0;
    level_version[s] = current_level_version;
    q.push(s);
    
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        for (auto& e : graph[v]) {
            if (e.cap > 0 && level_version[e.to] != current_level_version) {
                level[e.to] = level[v] + 1;
                level_version[e.to] = current_level_version;
                q.push(e.to);
            }
        }
    }
}

int dfs(int v, int t, int f) {
    if (v == t) return f;
    for (int& i = iter[v]; i < (int)graph[v].size(); i++) {
        Edge& e = graph[v][i];
        if (e.cap > 0 && level_version[e.to] == current_level_version && level[v] < level[e.to]) {
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

inline void reset_capacities() {
    for (int i = 1; i <= n; i++) {
        for (auto& e : graph[i]) {
            e.cap = e.orig_cap;
        }
    }
}

int max_flow(int s, int t) {
    int flow = 0;
    while (true) {
        bfs(s);
        if (level_version[t] != current_level_version) return flow;
        for (int i = 1; i <= n; i++) iter[i] = 0;
        int f;
        while ((f = dfs(s, t, INF)) > 0) {
            flow += f;
            if (flow >= 3) return flow;
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int m;
    cin >> n >> m;
    
    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        graph[a].push_back({b, 1, (int)graph[b].size(), 1});
        graph[b].push_back({a, 0, (int)graph[a].size() - 1, 0});
        graph[b].push_back({a, 1, (int)graph[a].size(), 1});
        graph[a].push_back({b, 0, (int)graph[b].size() - 1, 0});
    }
    
    long long total = 0;
    
    for (int a = 1; a <= n; a++) {
        for (int b = a + 1; b <= n; b++) {
            reset_capacities();
            total += max_flow(a, b);
        }
    }
    
    cout << total << endl;
    
    return 0;
}
