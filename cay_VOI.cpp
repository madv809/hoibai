#include<bits/stdc++.h>
#define LL long long
#define RED(i, a, b) for (int i = (a); i >= (b); --i)
#define REP(i, a, b) for(int i = (a); i <= (b); ++i)
#define FOR(i, a, b) for(int i = (a); i <= (b - 1); ++i)
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
using namespace std;
const int mxn = 2e5 + 5;
const int mxk = 1e7;
const int S = 200;
const long double eps = 1e-10;
const int INF = 1e9;
const int lg = 16;
const LL base = 311;
const LL MOD = 1e9 + 7;
vector<int> a[mxn], sta;
vector<pii> bridge;
int par[lg + 1][mxn], st[mxn], ed[mxn], bit[mxn], num[mxn], low[mxn], d[mxn], comp[mxn], ti, t, n, m;

void up(int x, int val)
{
    while(x <= n)
    {
        bit[x] += val;
        x += x&-x;
    }
}

void update(int l, int r, int val)
{
    up(l, val);
    up(r + 1, -val);
}

int get(int x)
{
    int res = 0;

    while(x > 0)
    {
        res += bit[x];
        x -= x&-x;
    }
    return res;
}

void dfs_make_comp(int u, int p)
{
    num[u] = low[u] = ++ti;
    for (int v : a[u]) if (v != p)
    {
        if (num[v]) low[u] = min(low[u], num[v]);
        else
        {
            dfs_make_comp(v, u);
            low[u] = min(low[u], low[v]);
        }
    }
    sta.pb(u);

    if (num[u] == low[u])
    {
        ++n;
        bridge.pb({p, u});

        while(sta.size())
        {
            comp[sta.back()] = n;
            sta.pop_back();
        }
    }
}

void dfs(int u, int p)
{
    st[u] = ++t;
    for (int v : a[u]) if (v != p)
    {
        par[0][v] = u;
        d[v] = d[u] + 1;
        dfs(v, u);
    }

    ed[u] = t;
}

void calc_something()
{
    cin >> n >> m;
    int u, v;
    REP(i, 1, m)
    {
        cin >> u >> v;
        a[u].pb(v); a[v].pb(u);
    }

    n = 0;
    dfs_make_comp(1, 0);
    REP(u, 1, n) a[u].clear();
    FOR(i, 0, (int)bridge.size())
    {
        u = comp[bridge[i].X]; v = comp[bridge[i].Y];
        if (u == 0) continue;
        a[u].pb(v); a[v].pb(u);
    }

    dfs(1, 0);
    REP(i, 1, lg) REP(u, 1, n)
        par[i][u] = par[i - 1][par[i - 1][u]];
    st[0] = -INF; ed[0] = INF;

    REP(u, 1, n) update(st[u], st[u], d[u]);
}

bool is_par(int u, int v)
{
    return (st[u] <= st[v] && ed[u] >= ed[v]);
}

int lca(int u, int v)
{
    if (is_par(u, v)) return u;
    if (is_par(v, u)) return v;

    RED(i, lg, 0) if (!is_par(par[i][u], v))
        u = par[i][u];

    return par[0][u];
}

int find_next(int u)
{
    int x = get(st[u]);
    RED(i, lg, 0) if (get(st[par[i][u]]) == x)
        u = par[i][u];

    if (u == 1) return 0;
    return u;
}

void query(int u, int x)
{
    while(1)
    {
        u = find_next(u);
        if (is_par(u, x)) return;
        update(st[u], ed[u], -1);
    }
}

int main()
{
    //freopen("D:\\test.txt", "r", stdin);
    //freopen("D:\\test2.txt", "w", stdout);
    ios::sync_with_stdio(false); cin.tie(NULL);
    calc_something();

    int u, v, t, x;
    while(cin >> t)
    {
        if (t == 0) break;
        cin >> u >> v;
        u = comp[u]; v = comp[v];
        x = lca(u, v);
        if (t == 2)
        {
            cout << (get(st[u]) + get(st[v]) - 2*get(st[x])) << "\n";
            continue;
        }

        query(u, x);
        query(v, x);
    }
}
