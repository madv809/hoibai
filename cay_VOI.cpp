//#pragma GCC optimize("O3,unroll-loops")
//#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")
#include <bits/stdc++.h>
#define LL long long
#define RED(i, a, b) for (int i = (a); i >= (b); --i)
#define REP(i, a, b) for (int i = (a); i <= (b); ++i)
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define pii pair<int, int>
#define pLL pair<LL, LL>
#define piL pair<int, LL>
#define pLi pair<LL, int>
#define X first
#define Y second
#define ci const int
#define cLL const LL

#define pb push_back
#define ef else if
//#define int LL
using namespace std;
const int N = 3e5 + 3;
const int K = 2e3 + 3;
const int mxk = 5e5 + 5;
const LL INF = 1e18;
const int lg = 29;
const LL base = 311;
const int S = 350;
const LL inv_base = 787781356;
const LL MOD = 1e9 + 7;
const int MAX = 4e5 + 1;
struct
{
    int v; LL cap, f;
} ed[N];
vector<int> a[K];
int n, m, s, t, ne;
int level[K], st[K];

void calc_something()
{
    cin >> n >> m >> s >> t;
    ne = -1;
    REP(i, 1, m)
    {
        int u, v; LL cap;
        cin >> u >> v >> cap;
        ed[++ne] = {v, cap, 0};
        a[u].pb(ne);

        ed[++ne] = {u, 0, 0};
        a[v].pb(ne);
    }
}

bool bfs()
{
    memset(level, 0, sizeof(level));
    level[s] = 1;
    queue<int> Q;
    Q.push(s);

    while(Q.size())
    {
        int u = Q.front(); Q.pop();
        for (int i : a[u])
        {
            int v = ed[i].v;
            if ((level[v]) || (ed[i].f >= ed[i].cap)) continue;
            level[v] = level[u] + 1;
            Q.push(v);
        }
    }

    return (level[t] != 0);
}

LL dfs(int u, LL mi)
{
    if (u == t) return mi;
    for (int &tt = st[u]; tt < a[u].size(); ++tt)
    {
        int i = a[u][tt];
        int v = ed[i].v;
        if ((level[v] != level[u] + 1) || (ed[i].f >= ed[i].cap)) continue;
        LL x = dfs(v, min(mi, ed[i].cap - ed[i].f));
        if (x)
        {
            ed[i].f += x;
            ed[i^1].f -= x;
            return x;
        }
    }
    return 0;
}

signed main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    //freopen("D:\\test.txt", "r", stdin);
    //freopen("D:\\test2.txt", "w", stdout);
    calc_something();

    LL res = 0;
    while(bfs())
    {
        memset(st, 0, sizeof(st));
        LL x = dfs(s, INF);
        while(x != 0)
        {
            res += x;
            x = dfs(s, INF);
        }
    }

    cout << res << "\n";
    /*REP(i, 0, ne) if (ed[i].cap > 0)
        cout << ed[i].f << "\n";*/
}
