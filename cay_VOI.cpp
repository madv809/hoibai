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
const int mxk = 4e5 + 5;
const LL INF = 1e15;
const int lg = 16;
const LL base = 311;
const LL MOD = 1e9 + 7;
vector<int> a[mxn], spec;
vector<LL> c[mxn];
int seg[4*mxn][2], node_dp[mxn], n, m, k, nn;
pLL min_path[mxn];
pLL path[25][25], dp[25][1 << 20];
bool vis[25][1 << 20];

void creat_node_seg(int l, int r, int par2, int par3, int pos)
{
    if (l == r)
    {
        seg[pos][0] = seg[pos][1] = l;
        a[l].pb(par2);
        c[l].pb(0LL);
        a[par3].pb(l);
        c[par3].pb(0);
        return;
    }
    seg[pos][0] = ++nn;
    seg[pos][1] = ++nn;
    a[seg[pos][0]].pb(par2);
    c[seg[pos][0]].pb(0LL);
    a[par3].pb(seg[pos][1]);
    c[par3].pb(0LL);

    int mid = (l + r)>>1;
    creat_node_seg(l, mid, seg[pos][0], seg[pos][1], pos << 1);
    creat_node_seg(mid + 1, r, seg[pos][0], seg[pos][1], pos << 1 | 1);
}

void creat_edge_type2(int l, int r, int L, int R, int pos, int u, LL w)
{
    if (r < L || R < l) return;
    if (L <= l && r <= R)
    {
        a[seg[pos][0]].pb(u);
        c[seg[pos][0]].pb(w);
        return;
    }

    int mid = (l + r)>>1;
    creat_edge_type2(l, mid, L, R, pos << 1, u, w);
    creat_edge_type2(mid + 1, r, L, R, pos << 1 | 1, u, w);
}

void creat_edge_type3(int l, int r, int L, int R, int pos, int u, LL w)
{
    if (r < L || R < l) return;
    if (L <= l && r <= R)
    {
        a[u].pb(seg[pos][1]);
        c[u].pb(w);
        return;
    }

    int mid = (l + r)>>1;
    creat_edge_type3(l, mid, L, R, pos << 1, u, w);
    creat_edge_type3(mid + 1, r, L, R, pos << 1 | 1, u, w);
}

bool mini(pLL &a, pLL b)
{
    if (a.X > b.X) {a = b; return 1;}
    ef (a.X == b.X) (a.Y += b.Y)%=MOD;
    return 0;
}

//type2 là hội tụ
//type3 là phân tán
void creat_graph()
{
    cin >> n >> m >> k;
    REP(i, 1, n) node_dp[i] = -1;
    FOR(i, 0, k)
    {
        int u; cin >> u;
        node_dp[u] = i;
        spec.pb(u);
    }
    if (node_dp[1] == -1) node_dp[1] = k;

    int t, u, v, l, r; LL w;
    nn = n;
    creat_node_seg(1, n, 0, 0, 1);
    REP(i, 1, m)
    {
        cin >> t;
        if (t == 1)
        {
            cin >> u >> v >> w;
            a[u].pb(v);
            c[u].pb(w);
        }
        ef (t == 2)
        {
            cin >> l >> r >> u >> w;
            creat_edge_type2(1, n, l, r, 1, u, w);
        }
        else
        {
            cin >> u >> l >> r >> w;
            creat_edge_type3(1, n, l, r, 1, u, w);
        }
    }

    if (node_dp[1] == k) spec.pb(1);
    for (int u : spec) for (int v : spec) path[node_dp[u]][node_dp[v]] = {INF, INF};
    //tính path[u][v]
    priority_queue<pLi, vector<pLi>, greater<pLi> >heap;
    for (int tt : spec)
    {
        REP(i, 1, nn) min_path[i] = {INF, 0};
        min_path[tt] = {0, 1};
        heap.push({0, tt});

        while(heap.size())
        {
            w = heap.top().X; u = heap.top().Y;
            heap.pop();
            if (w != min_path[u].X) continue;
            for (int i = 0; i < (int)a[u].size(); ++i)
            {
                v = a[u][i]; w = c[u][i];
                if (mini(min_path[v], {min_path[u].X + w, min_path[u].Y}))
                    heap.push({min_path[v].X, v});
            }
        }

        for (int v : spec) if (v != tt)
            path[node_dp[tt]][node_dp[v]] = min_path[v];
        else path[node_dp[tt]][node_dp[v]] = {0, 1};
    }
}

void mini2 (pLL &a, pLL b)
{
    if (a.X > b.X) a = b;
    ef (a.X == b.X) (a.Y += b.Y)%=MOD;
}

pLL solve(int u, int mask)
{
    if (vis[u][mask]) return dp[u][mask];
    vis[u][mask] = 1;
    pLL &ans = dp[u][mask];
    ans = {INF, INF};

    FOR(i, 0, k) if ((mask >> i & 1) && path[u][i].X != INF)
    {
        pLL x = solve(i, mask^(1 << i));
        mini2(ans, {path[u][i].X + x.X, path[u][i].Y*x.Y%MOD});
    }
    return ans;
}

int main()
{
    //freopen("D:\\test.txt", "r", stdin);
    //freopen("D:\\test2.txt", "w", stdout);
    ios::sync_with_stdio(false); cin.tie(NULL);
    creat_graph();
    for (int u : spec)
    {
        vis[node_dp[u]][0] = 1;
        dp[node_dp[u]][0] = path[node_dp[u]][node_dp[1]];
    }
    int mask = (1 << k) - 1;
    if (node_dp[1] != k && node_dp[1] != -1) mask^=(1 << node_dp[1]);
    pLL x = solve(node_dp[1], mask);
    if (x.X == INF) cout << "-1";
    else cout << x.X << ' ' << x.Y;
}
