#include<bits/stdc++.h>
#define LL long long
#define REP(i, a, b) for(int i = (a); i <= (b); ++i)
#define FOR(i, a, b) for(int i = (a); i <= (b - 1); ++i)
#define pii pair<int, int>
#define pLL pair<LL, LL>
#define X first
#define Y second
#define ci const int
#define cLL const LL

#define pb push_back
#define ef else if
using namespace std;
const int mxn = 1e3 + 5;
const int mxk = 2500 + 5;
const int INF = 1e8;
const LL base = 311;
const LL MOD = 1e9 + 7;
int a[mxn][mxn], b[mxn][mxn], vis[mxn], open[mxn], n, t;
LL pow_base[mxn], Hash[mxn][mxn];
char sz;
string s[mxn];

bool dfs_find_cycle(char u)
{
    open[u] = 1;
    for (char ch = 'a'; ch <= sz; ++ch) if (a[u][ch] == 1)
    {
        if (open[ch] == 0) if (dfs_find_cycle(ch)) return 1;
        ef (open[ch] == 1) return 1;
    }
    open[u] = 2;
    return 0;
}

void dfs_creat_mat(char u, char p)
{
    vis[u] = t;
    for (char ch = 'a'; ch <= sz; ++ch) if (a[u][ch] == 1)
    {
        if (vis[ch] == t) continue;
        b[p][ch] = 1; b[ch][p] = -1;
        dfs_creat_mat(ch, p);
    }
}

int main()
{
    //freopen("D:\\test.txt", "r", stdin);
    //freopen("D:\\test2.txt", "w", stdout);
    cin >> sz >> n;
    pow_base[0] = 1; REP(i, 1, mxn) pow_base[i] = base*pow_base[i - 1]%MOD;
    REP(i, 1, n)
    {
        cin >> s[i];
        for (int j = 1; j <= (int)(s[i].size()); ++j)
            Hash[i][j] = (Hash[i][j - 1] + (LL)s[i][j - 1]*pow_base[j]%MOD)%MOD;
    }

    FOR(i, 1, n) REP(j, i + 1, n)
    {
        int l = 0, r = (int)min(s[i].size(), s[j].size());
        while(l < r)
        {
            int mid = (l + r + 1)>>1;
            if (Hash[i][mid] == Hash[j][mid]) l = mid;
            else r = mid - 1;
        }
        if (l == (int)min(s[i].size(), s[j].size())) continue;
        char u = s[i][l], v = s[j][l];
        if (a[v][u] == 1) {cout << "IMPOSSIBLE"; return 0;}
        a[u][v] = 1; a[v][u] = -1;
    }

    for (char ch = 'a'; ch <= sz; ++ch) if (open[ch] == 0)
        if (dfs_find_cycle(ch)) {cout << "IMPOSSIBLE"; return 0;}

    for (char ch = 'a'; ch <= sz; ++ch)
    {
        ++t;
        dfs_creat_mat(ch, ch);
    }

    /*for (char ch1 = 'a'; ch1 <= sz; ++ch1)
    {
        for (char ch2 = 'a'; ch2 <= sz; ++ch2) cout << b[ch1][ch2] << ' ';
        cout << endl;
    }
    return 0;*/


    for (char ch1 = 'a'; ch1 <= sz; ++ch1)
        for (char ch2 = 'a'; ch2 <= sz; ++ch2) if (ch1 != ch2 && (b[ch1][ch2] == 0 || b[ch2][ch1] == 0))
        {cout << "AMBIGUOUS"; return 0;}

    for (int i = (int)(sz - 'a'); i >= 0; --i)
    {
        for (char ch = 'a'; ch <= 'z'; ++ch)
        {
            int cnt = 0;
            for (char x = 'a'; x <= sz; ++x) if (b[ch][x] == 1) ++cnt;
            if (cnt != i) continue;
            cout << ch;
            break;
        }
    }
}
