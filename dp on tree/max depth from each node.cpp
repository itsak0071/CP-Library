#include <bits/stdc++.h>
using namespace std;

void dfs(int node, int parent, const vector<vector<int>> &adj,
         vector<multiset<int>> &depths) {
  depths[node].insert(0);
  for (int neighbor : adj[node]) {
    if (neighbor == parent) continue;
    dfs(neighbor, node, adj, depths);
    depths[node].insert(*depths[neighbor].rbegin() + 1);
  }
}

void changeRoot(int oldRoot, int newRoot, vector<multiset<int>> &depths) {
  int val = *depths[newRoot].rbegin() + 1;
  auto it = depths[oldRoot].find(val);          // should exist
  depths[oldRoot].erase(it);                    // erase one occurrence
  depths[newRoot].insert(*depths[oldRoot].rbegin() + 1);
}

void reroot(int node, int parent, const vector<vector<int>> &adj,
            vector<multiset<int>> &depths, vector<int> &ans) {
  ans[node] = *depths[node].rbegin();
  for (int neighbor : adj[node]) {
    if (neighbor == parent) continue;
    changeRoot(node, neighbor, depths);
    reroot(neighbor, node, adj, depths, ans);
    changeRoot(neighbor, node, depths);
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n; cin >> n;
  vector<vector<int>> adj(n + 1);
  for (int i = 0; i < n - 1; i++) {
    int u, v; cin >> u >> v;
    adj[u].push_back(v);
    adj[v].push_back(u);
  }

  vector<multiset<int>> depths(n + 1);
  vector<int> ans(n + 1);

  dfs(1, 0, adj, depths);
  reroot(1, 0, adj, depths, ans);

  for (int i = 1; i <= n; i++) cout << ans[i] << (i == n ? '\n' : ' ');
}
