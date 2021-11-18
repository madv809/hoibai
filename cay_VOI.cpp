#include<bits/stdc++.h>
#define LL long long
#define REP(i, a, b) for(int i = (a); i <= (b); ++i)
#define FOR(i, a, b) for(int i = (a); i <= (b - 1); ++i)
#define pii pair<int, int>
#define pLL pair<LL, LL>
#define X first
#define Y second

#define pb push_back
#define ef else if
using namespace std;
const int mxn = 1e6 + 5;
const int mxk = 2e3 + 5;
const int INF = 1e9 + 7;
const LL base = 311;
const LL MOD = 1e9 + 7;
int n, q;
LL Q[mxn], P[mxn], pow_base[mxn], deg[mxn], a[mxn], b[mxn], sz[mxn], global, ans4;
map<LL, LL> QP;

// cái mảng sz[] luôn là số âm đấy ạ
// với cả deg[mxn], a[mxn], b[mxn], sz[mxn], global luôn <= n
// còn riêng ans4 luôn <= n^2 nên em nghĩ nó không tràn chỗ mấy thằng này;

int uni(const int &u, const int &v)
{
    if (sz[u] > sz[v]) return uni(v, u);
    sz[u] += sz[v];
    sz[v] = u;
    (P[u] += P[v])%=MOD;
    (Q[u] += Q[v])%=MOD;
    return u;
}

int findd(const int &u)
{
    if (sz[u] < 0) return u;
    return (sz[u] = findd(sz[u]));
}

void Query2(int u, int v)
{
    // tính lại global và ans4
    int x = findd(u), y = findd(v);
    if (x == y) return;
    if (Q[x] != P[x])
    {
        --global;
        ans4 += sz[x]*QP[(P[x] - Q[x] + MOD)%MOD];
        QP[(Q[x] - P[x] + MOD)%MOD] += sz[x];
    }
    if (Q[y] != P[y])
    {
        --global;
        ans4 += sz[y]*QP[(P[y] - Q[y] + MOD)%MOD];
        QP[(Q[y] - P[y] + MOD)%MOD] += sz[y];
    }

    int par = uni(x, y);
    if (Q[par] != P[par])
    {
        ++global;
        ans4 -= sz[par]*QP[(P[par] - Q[par] + MOD)%MOD];
        QP[(Q[par] - P[par] + MOD)%MOD] -= sz[par];
    }
}

void Query1(int u, int v)
{
    int x = findd(u), y = findd(v);
    //cout << u << ' ' << v << endl;
    //cout << x << ' ' << y; exit(0);
    if (x == y) {swap(a[u], a[v]); return;}

    if (Q[x] != P[x])
    {
        --global;
        ans4 += sz[x]*QP[(P[x] - Q[x] + MOD)%MOD];
        QP[(Q[x] - P[x] + MOD)%MOD] += sz[x];
    }
    if (Q[y] != P[y])
    {
        --global;
        ans4 += sz[y]*QP[(P[y] - Q[y] + MOD)%MOD];
        QP[(Q[y] - P[y] + MOD)%MOD] += sz[y];
    }

    P[x] -= pow_base[deg[a[u]]];
    (P[x] += pow_base[deg[a[v]]] + MOD)%=MOD;
    P[y] -= pow_base[deg[a[v]]];
    (P[y] += pow_base[deg[a[u]]] + MOD)%=MOD;

    if (P[x] != Q[x])
    {
        ++global;
        ans4 -= sz[x]*QP[(P[x] - Q[x] + MOD)%MOD];
        QP[(Q[x] - P[x] + MOD)%MOD] -= sz[x];
    }

    if (P[y] != Q[y])
    {
        ++global;
        ans4 -= sz[y]*QP[(P[y] - Q[y] + MOD)%MOD];
        QP[(Q[y] - P[y] + MOD)%MOD] -= sz[y];
    }

    swap(a[u], a[v]);
}

int main()
{
    //freopen("D:\\test.txt", "r", stdin);
    //freopen("D:\\test2.txt", "w", stdout);
    cin >> n >> q;
    REP(i, 1, n)
    {
        scanf("%lli", &a[i]);
        b[i] = a[i];
    }
    int t = 0;
    sort(b + 1, b + n + 1);
    REP(i, 1, n)
    {
        if (b[i] != b[i - 1]) ++t;
        deg[b[i]] = t;
    }
    pow_base[0] = 1; REP(i, 1, n) pow_base[i] = (LL)base*pow_base[i - 1]%MOD;
    REP(i, 1, n)
    {
        P[i] = pow_base[deg[a[i]]];
        Q[i] = pow_base[deg[b[i]]];
        if (P[i] != Q[i])
        {
            ++global;
            ans4 += QP[(P[i] - Q[i] + MOD)%MOD];
            ++QP[(Q[i] - P[i] + MOD)%MOD];
        }
    }

    REP(i, 1, n) sz[i] = -1;
    int u, v;
    REP(i, 1, q)
    {
        scanf("%d", &t);
        if (t <= 2)
        {
            scanf("%d%d", &u, &v);
            if (t == 1) Query1(u, v);
            else Query2(u, v);
        }
        ef (t == 3)
        {
            if (global == 0) printf("DA\n");
            else printf("NE\n");
        }
        else printf("%lli\n", ans4);
    //if (i == 3) {cout << ans4; return 0;}
    }
}
