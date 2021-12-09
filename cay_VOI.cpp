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
const int mxn = 1e2 + 5;
const int mxk = 2e4 + 155;
const LL INF = 1e15;
const int lg = 16;
const LL base = 311;
const LL MOD = 1e9 + 7;
vector<int> a[mxk], target[mxk], op[mxk];
LL match[mxn][mxn], win[mxn], max_win[mxn], f[4*mxk], c[4*mxk], global_match;
int d[mxk], n, m, nn, s, t, st;

// match và win là số trận đấu và số trận đã thắng như dữ kiện đề cho
// global_match là tổng số trận đấu sẽ xảy ra
// max_win[i] là số trận nhiều nhất mà i có thể thắng được
// f và c là luồng và cap của cạnh
// d là mảng dùng cho bfs, s, t là đỉnh nguồn và thu
// n là số node ban đầu, nn là số lượng node sau khi tạo ra thêm các đỉnh ảo
// m là số cạnh của đồ thị

// do giới hạn bộ nhớ, thay vì dùng mảng 2 chiều để biểu thị cung (u, v)
// em chuyển sang lưu các cung, mỗi cung xuất phát từ đỉnh u sẽ được lưu trong vector a[u] và có thông tin gồm {v, m}
// với v là điểm kết thúc của cung đó và m là cung ngược của cung hiện tại (phục vụ mục đích tăng giảm luồng)
// v được lưu trong target, còn m được lưu trong op
// a[i] là danh sách các CUNG của i


// cái này để tạo cạnh
void de(int u, int v, LL cap)
{
    ++m;
    a[u].pb(m);
    target[u].pb(v);
    op[u].pb(m + 1);
    c[m] = cap;

    ++m;
    a[v].pb(m);
    target[v].pb(u);
    op[v].pb(m - 1);
    c[m] = 0;
}

// tạo đồ thị
void creat_graph()
{
    s = m = 0;
    nn = n;

    REP(i, 1, n) REP(j, i + 1, n) if (match[i][j])
    {
        ++nn;
        de(nn, j, match[i][j]);
        de(nn, i, match[j][i]);

        ++m;
        a[s].pb(m);
        target[s].pb(nn);
        op[s].pb(-1);
        c[m] = match[i][j];
    }
    t = ++nn;

    st = m + 1;
    REP(i, 1, n)
    {
        ++m;
        a[i].pb(m);
        target[i].pb(t);
        op[i].pb(-1);
        c[m] = -win[i];
    }
}

// bfs dùng cho luồng
bool bfs()
{
    REP(i, 0, nn) d[i] = 0;
    d[s] = 1;
    queue<int> Q;
    Q.push(s);

    while(!Q.empty())
    {
        int u = Q.front();
        Q.pop();

        FOR(i, 0, (int)a[u].size())
        {
            int x = a[u][i], v = target[u][i];
            if (f[x] >= c[x] || d[v]) continue;
            d[v] = d[u] + 1;
            Q.push(v);
        }
    }

    return (d[t] != 0);
}

// dfs dùng cho luồng
LL dfs(int u, LL max_flow)
{
    if (u == t) return max_flow;
    FOR(i, 0, (int)a[u].size())
    {
        int x = a[u][i], v = target[u][i];
        if (f[x] >= c[x] || d[v] != d[u] + 1) continue;
        LL tt = dfs(v, min(max_flow, c[x] - f[x]));
        if (tt)
        {
            f[x] += tt;
            if (op[u][i] != -1) f[op[u][i]] -= tt;
            return tt;
        }
    }
    return 0;
}

// check xem đỉnh u có thỏa mãn không
bool check(int u)
{
    REP(i, st, m) c[i] += max_win[u];
    REP(i, 1, m) f[i] = 0;

    LL res = 0, x;
    while(bfs())
    {
        while(x = dfs(s, INF))
            res += x;
    }

    REP(i, st, m) c[i] -= max_win[u];
    if (res == global_match) return 1;
    return 0;
}

int main()
{
    //freopen("D:\\test.txt", "r", stdin);
    //freopen("D:\\test2.txt", "w", stdout);
    ios::sync_with_stdio(false); cin.tie(NULL);
    int t; cin >> t;

    while(t--)
    {
        // đọc dữ liệu
        cin >> n;
        REP(i, 1, n)
        {
            max_win[i] = 0;
            REP(j, 1, n)
            {
                cin >> match[i][j];
                max_win[i] += match[i][j];
            }
        }

        // mx là số trận nhiều nhất mà 1 người nào nó có thể thắng
        LL mx = global_match = 0;
        REP(i, 1, n)
        {
            cin >> win[i]; mx = max(mx, win[i]);
            global_match += max_win[i];
            max_win[i] += win[i];
        }
        global_match >>= 1;

        // tạo đồ thị
        creat_graph();
        //cout << nn << ' ' << m; return 0;

        vector<int> res;
        REP(i, 1, n) if (max_win[i] >= mx && check(i))
            res.pb(i);

        cout << res.size() << ' ';
        for (int x : res) cout << x << ' '; cout << "\n";

        // reset lại các thông tin này
        REP(i, 0, nn)
        {
            a[i].clear();
            target[i].clear();
            op[i].clear();
        }
    }
}
