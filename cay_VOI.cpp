//#pragma GCC optimize("O3,unroll-loops")
//#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")
#include<bits/stdc++.h>
#define LL long long
#define RED(i, a, b) for (int i = (a); i >= (b); --i)
#define REP(i, a, b) for(int i = (a); i <= (b); ++i)
#define FOR(i, a, b) for(int i = (a); i < (b); ++i)
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
const int N = 1e2 + 51;
const int K = 1e2 + 5;
const int mxk = 5e5 + 5;
const LL INF = 1e18;
const int lg = 40;
const LL base = 311;
const LL inv_base = 787781356;
//const LL MOD = 1e3;
const int MAX = 1e6;
vector<int> tt[N];
int m, n, MOD, S;
int dp[N][N][N][41], nxt[41][N], child[41][N], f[41], par[41], node, end_state;
bool ok[41];
string grid[N], xau[41], xuoi, nguoc;

void calc_something()
{
    cin >> m >> n >> MOD >> xuoi;
    REP(i, 1, m)
    {
        cin >> grid[i];
        grid[i] = '#' + grid[i];
    }
    S = xuoi.size();
    nguoc = xuoi;

    reverse(nguoc.begin(), nguoc.end());
    xuoi = '#' + xuoi;
    nguoc = '#' + nguoc;
    xau[0] = '#';

    REP(i, 1, S)
    {
        par[i] = i - 1;
        child[i - 1][xuoi[i]] = i;
        xau[i] = xau[i - 1] + xuoi[i];
        tt[i].pb(i);
    }

    node = S;
    REP(i, 1, S) if (xuoi[i] != nguoc[i])
    {
        ++node;
        par[node] = i - 1;
        child[i - 1][nguoc[i]] = node;
        xau[node] = xau[i - 1] + nguoc[i];
        tt[i].pb(node);

        REP(t, i + 1, S)
        {
            ++node;
            par[node] = node - 1;
            child[node - 1][nguoc[t]] = node;
            xau[node] = xau[node - 1] + nguoc[t];
            tt[t].pb(node);
        }
        break;
    }

    REP(i, 1, S) for (int x : tt[i])
    {
        if (par[x] == 0) continue;
        int t = f[par[x]];
        while(t > 0 && child[t][xau[x][i]] == 0)
            t = f[t];
        f[x] = child[t][xau[x][i]];
    }

    REP(i, 0, node) for (char ch = 'a'; ch <= 'z'; ++ch)
    {
        int t = i;
        while(t > 0 && child[t][ch] == 0)
            t = f[t];
        nxt[i][ch] = child[t][ch];
    }
    int ed1 = 0, ed2 = 0;
    ed1 = S;
    ed2 = node;

    for (char ch = 'a'; ch <= 'z'; ++ch)
    {
        nxt[ed1][ch] = ed1;
        nxt[ed2][ch] = ed2;
    }

    REP(t, 1, node) for(int i : tt[t])
    {
        ok[i] = ok[f[i]];
        if (ok[i]) continue;
        string t = xau[i];
        reverse(xau[i].begin(), xau[i].end());
        t += xau[i];
        while(t.size() > xuoi.size())
            t.erase((t.size()) - 1, 1);

        if (t == xuoi || t == nguoc)
            ok[i] = 1;
    }
    ok[ed1] = ok[ed2] = 1;

    end_state = (m + n)/2;
}

bool valid(int x, int y, int u, int v)
{
    if (x > u || y > v) return 0;
    if (x < 1 || x > m || y < 1 || y > n) return 0;
    if (u < 1 || u > m || v < 1 || v > n) return 0;
    if ((grid[x][y] != grid[u][v]) || (grid[x][y] == '#')) return 0;
    return 1;
}

// dp[len][trai][phai][f]
int solve(int len, int x, int y, int u, int v, int match)
{
    if (len == end_state)
        return ok[match];
    int &ans = dp[len][x][u][match];
    if (ans != -1) return ans;
    ans = 0;

    if (valid(x, y + 1, u, v - 1))
        ans += solve(len + 1, x, y + 1, u, v - 1, nxt[match][grid[x][y + 1]]);

    if (valid(x, y + 1, u - 1, v))
        (ans += solve(len + 1, x, y + 1, u - 1, v, nxt[match][grid[x][y + 1]]))%=MOD;

    if (valid(x + 1, y, u, v - 1))
        (ans += solve(len + 1, x + 1, y, u, v - 1, nxt[match][grid[x + 1][y]]))%=MOD;

    if (valid(x + 1, y, u - 1, v))
        (ans += solve(len + 1, x + 1, y, u - 1, v, nxt[match][grid[x + 1][y]]))%=MOD;

    return ans;
}

signed main()
{
    ios::sync_with_stdio(false); cin.tie(NULL);
    //freopen("D:\\test.txt", "r", stdin);
    //freopen("D:\\test2.txt", "w", stdout);
    calc_something();
    if (grid[1][1] != grid[m][n])
        cout << "0";
    else
    {
        memset(dp, -1, sizeof(dp));
        int t = 0;
        if (xuoi[1] == nguoc[1])
        {
            if (grid[1][1] == xuoi[1])
                t = 1;
            else
                t = 0;
        }
        else
        {
            if (grid[1][1] == xuoi[1])
                t = 1;
            ef (grid[1][1] == nguoc[1])
                t = S + 1;
            else
                t = 0;
        }
        cout << solve(1, 1, 1, m, n, t);
    }
}
