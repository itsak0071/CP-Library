#include<bits/stdc++.h>
using namespace std;
using namespace chrono;
#define int long long

#define fastio() ios_base::sync_with_stdio(false);cin.tie(NULL);cout.tie(NULL)
const int N = 1e6;

/* --- SEGMENT TREE TEMPLATE START --- */

// 1. NODE STRUCTURE: Yahan hum define karte hain ki ek node kya store karega
struct Node {
    int val;
    
    // Default Constructor: Yeh 'Identity Element' hota hai. 
    // Iski value aisi honi chahiye jo answer pe asar na dale.
    // Example: Sum ke liye 0, Min ke liye INF, Max ke liye -INF, GCD ke liye 0
    Node() {
        val = 0;
    }
    
    // Parameterized Constructor: Jab leaf node par direct array ki value dalni ho
    Node(int p) {
        val = p;
    }
};

Node seg[4 * N + 5];
int arr[N];

// 2. COMBINE FUNCTION: Sirf ye logic change hoga har naye question ke liye!
Node combine(Node a, Node b) {
    Node res;
    res.val = a.val + b.val; // Yahan Sum ho raha hai. Isko min(a.val, b.val) ya gcd() bhi kar sakte ho.
    return res;
}

// 3. BUILD FUNCTION: Tree ko initialze karne ke liye (O(N) time)
void build(int s, int e, int idx) {
    // Base Case: Agar leaf node hai, toh array ki value daal do
    if (s == e) {
        seg[idx] = Node(arr[s]);
        return; 
    }
    int mid = (s + e) / 2;
    // Left child build karo
    build(s, mid, 2 * idx);
    // Right child build karo
    build(mid + 1, e, 2 * idx + 1);
    
    // Wapas aate waqt current node ko update karo left aur right merge karke
    seg[idx] = combine(seg[2 * idx], seg[2 * idx + 1]);
}

// 4. POINT UPDATE FUNCTION: Kisi ek index ki value change karne ke liye (O(log N) time)
void update(int s, int e, int idx, int update_index, int value) {
    // Base Case: Sahi index mil gaya, leaf node update karo
    if (s == e) {
        arr[s] = value;
        seg[idx] = Node(arr[s]);
        return;
    }
    int mid = (s + e) / 2;
    
    // Agar required index left side hai, toh left jao
    if (update_index <= mid) {
        update(s, mid, 2 * idx, update_index, value);
    } 
    // Warna right side jao
    else {
        update(mid + 1, e, 2 * idx + 1, update_index, value);
    }
    
    // Niche se update hokar aane ke baad parent ko bhi update karo
    seg[idx] = combine(seg[2 * idx], seg[2 * idx + 1]);
}

// 5. QUERY FUNCTION: Range ka answer nikalne ke liye (O(log N) time)
Node query(int s, int e, int idx, int l, int r) {
    // Case 1: No Overlap (Range bilkul bahar hai) -> Default 'Identity' Node bhej do
    if (s > r || e < l) {
        return Node(); 
    }
    // Case 2: Complete Overlap (Current segment poori tarah query range ke andar hai)
    if (s >= l && e <= r) {
        return seg[idx];
    }
    
    // Case 3: Partial Overlap -> Dono side jao aur unka result combine karo
    int mid = (s + e) / 2;
    Node left = query(s, mid, 2 * idx, l, r);
    Node right = query(mid + 1, e, 2 * idx + 1, l, r);
    return combine(left, right);
}

/* --- SEGMENT TREE TEMPLATE END --- */

// Tumhara main logic yahan likha jayega
void solve() {
    // Example:
    // int n; cin >> n;
    // for(int i = 0; i < n; i++) cin >> arr[i];
    // build(0, n - 1, 1);
}

int32_t main() {
    fastio();
#ifndef ONLINE_JUDGE
    freopen("input1.txt", "r", stdin);
    freopen("output1.txt", "w", stdout);
#endif
    
    int t;
    cin >> t; 
    while (t--) {
        solve();
    }
    
    return 0;
}
