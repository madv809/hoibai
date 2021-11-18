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
int  a[mxn], b[mxn], n, q;
LL Q[mxn], P[mxn], pow_base[mxn], deg[mxn], sz[mxn], global, ans4;
map<LL, LL> QP;

// deg[mxn], sz[mxn], global, luôn < = n
// riêng ans4 <= n^2 nên em nghĩ mấy cái này sẽ không tràn số được

// P[i] là mã hash của các số trên dãy ban đầu
// Q[i] là mã hash của các số trên dãy đã sort
//global là số lượng tập node mà có Q[i] != P[i]
//truy vấn 3 sẽ ra DA nếu global = 0 và ngược lại
// ans4 là đáp án cho truy vấn 4

// hợp 2 tập lại
int uni(const int &u, const int &v)
{
    if (sz[u] > sz[v]) return uni(v, u);
    sz[u] += sz[v];
    sz[v] = u;
    (P[u] += P[v])%=MOD;
    (Q[u] += Q[v])%=MOD;
    return u;
}

//tìm node đại diện của tập mà node u đang ở trong
int findd(const int &u)
{
    if (sz[u] < 0) return u;
    return (sz[u] = findd(sz[u]));
}

// update lại
void de(const int &i, const LL &val)
{
    if (P[i] == Q[i]) return;
    LL x = P[i] - Q[i];
    global += val;
    ans4 += -sz[i]*QP[(x + MOD)%MOD]*val;
    QP[(MOD - x)%MOD] += -sz[i]*val;
}

// Thao tác 1
void Query1(int u, int v)
{
    int x = findd(u), y = findd(v);
    if (x == y) {swap(a[u], a[v]); return;}

    de(x, -1); de(y, -1);

    P[x] -= pow_base[deg[a[u]]];
    (P[x] += pow_base[deg[a[v]]] + MOD)%=MOD;
    P[y] -= pow_base[deg[a[v]]];
    (P[y] += pow_base[deg[a[u]]] + MOD)%=MOD;

    de(x, 1); de(y, 1);
    swap(a[u], a[v]); return;
}

// Thao tác 2
void Query2(int u, int v)
{
    int x = findd(u), y = findd(v);
    if (x == y) return;

    de(x, -1); de(y, -1);
    int par = uni(x, y);
    de(par, 1); return;
}

int main()
{
    //freopen("D:\\test.txt", "r", stdin);
    //freopen("D:\\test2.txt", "w", stdout);
    cin >> n >> q;
    REP(i, 1, n)
    {
        scanf("%d", &a[i]);
        b[i] = a[i];
    }
    sort(b + 1, b + n + 1);
    int t = 0;
    REP(i, 1, n)
    {
        if (b[i] != b[i - 1]) ++t;
        deg[b[i]] = t;
    }
    pow_base[0] = 1;
    REP(i, 1, n) pow_base[i] = base*pow_base[i - 1]%MOD;

    // tính toán cho đáp án cho dãy ban đầu
    LL x;
    REP(i, 1, n)
    {
        P[i] = pow_base[deg[a[i]]];
        Q[i] = pow_base[deg[b[i]]];
        if (P[i] != Q[i])
        {
            ++global; x = P[i] - Q[i];
            ans4 += QP[(x + MOD)%MOD];
            ++QP[(MOD - x)%MOD];
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
    }
}
