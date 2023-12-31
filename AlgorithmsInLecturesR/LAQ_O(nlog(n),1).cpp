// github.com/andy489

#include <iostream>
#include <vector>
#include <list>

using namespace std;

#define ios ios::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr)
#define pb push_back

int n, leaves;
vector<list<int>> adj, AL; // adjacency list, All Leaves
vector<int> par, dep, SL, mark, indexP, flog; // parent, depth, Sorted leaves, max/long-Path index, floor log
vector<vector<int>> P, L, jump; // P = max/long-paths, L = ladders
vector<int> traversal; // shortest path from root to current node

void pre() {
    flog.resize(n);
    for (int i = 2; i <= n; ++i)
        flog[i] = flog[i / 2] + 1;
}

void init() { // read tree from file
    cin >> n;
    adj.resize(n + 1);
    int m, c;
    for (int i = 1; i <= n; ++i) {
        cin >> m;
        while (m--) {
            cin >> c;
            adj[i].pb(c);
            adj[c].pb(i);
        }
    }
    par.resize(n + 1), dep.resize(n + 1, -1);
    AL.resize(n), mark.resize(n + 1), indexP.resize(n + 1);
    traversal.reserve(n); // making traversal vector efficient for a stack (no resizing)
    jump.resize(n + 1);
    pre();
}

void fillJumps(int u) {
    if (u == 1) return;
    int l = (int)traversal.size() - 1;
    jump[u].resize(flog[l] + 1);
    int k = 0, j = 1;
    while (j <= l) {
        jump[u][k++] = traversal[l - j];
        j = (j << 1);
    }
}

void dfs(int u = 1, int p = 0) {
    par[u] = p; // parent function fill
    dep[u] = dep[p] + 1; // depth function fill
    traversal.pb(u); // traversal path fill
    if (adj[u].size() == 1) AL[dep[u]].pb(u), leaves++, fillJumps(u);; // All Leaves array of lists
    for (const int& child : adj[u]) {
        if (child == p) continue;
        dfs(child, u);
        traversal.pop_back();
    }
}

void counting() {
    SL.resize(leaves); // Sorted Leaves (decreasing)
    int k = 0;
    for (int d = n - 1; d >= 0; --d) {
        while (!AL[d].empty()) {
            auto it = --AL[d].end();
            int l = *it;
            AL[d].erase(it), SL[k++] = l;
        }
    }
}

void maxPathsDecomposition() {
    for (int l = 0; l <= leaves - 1; ++l) {
        vector<int> currMaxPath;
        currMaxPath.reserve(20);
        int v = SL[l];
        while (v && !mark[v]) {
            currMaxPath.pb(v);
            indexP[v] = l;
            mark[v] = true;
            v = par[v];
        }
        P.pb(currMaxPath); // P = Max Paths Decomposition
    }
}

void ladders() {
    L.resize(leaves);
    // we start from the second path, the first contains the root
    L[0] = P[0];
    for (int i = 1; i < leaves; ++i) {
        int SIZE = (int)P[i].size();
        L[i].reserve(2 * SIZE + 1);
        L[i].resize(SIZE);
        int j = 0;
        for (; j < SIZE; ++j) {
            L[i][j] = P[i][j];
        }
        int v = par[P[i].back()];
        L[i].pb(v);
        while (SIZE-- && par[v]) {
            v = par[v];
            L[i].pb(v);
        }
    }
}

int LAQ_leaf(int l, int d) {
    int k = flog[d];
    int u = jump[l][flog[d]]; // jump to a vertex with height at least 2^k
    int ind = indexP[u]; // index of long-path with at least 2^k nodes
    int height = dep[L[ind][0]] - dep[u]; // height of node u in ladder
    int searched = height + d - (1 << k); // we are sure that it is in that ladder
    return L[ind][searched];
}

int LAQ_const(int v, int d) {
    if (d == 0) return v;
    if (d > dep[v]) return -1;
    if (d == 1) return par[v];
    int i = indexP[v]; // index to long-path, where v is
    int l = L[indexP[v]][0]; // leaf node (first node) in ladder/long-path
    int height = dep[l] - dep[v]; // height of node v in ladder
    if (height + d < L[i].size()) return L[i][height + d]; // return the searched ancestor if in the ladder
    return LAQ_leaf(l, height + d); // query a leaf (1 jump and 1 ladder)
}

void solve() {
    int q, v, d;
    cout << "Enter number of queries of the form \"v d\": ";
    cin >> q;
    cout << "Enter query \"v d\":\n";
    while (q--) {
        cin >> v >> d;
        int ans = LAQ_const(v, d);
        ans == -1 ? cout << "no such ancestor\n" : cout << ans << '\n';
    }
}

int main() {
    ios;
    freopen("input.txt", "r", stdin);
    init();
    dfs();
    counting();
    maxPathsDecomposition();
    ladders();
    solve();
    return 0;
}