# Generic Segment Trees - Deep Study Guide

This guide explains the PDF "Generic Segment Trees & Problem Solving" in a way that helps you solve new questions, not just memorize code.

## 1. What Is A Segment Tree?

A segment tree is a binary tree built over an array.

Each node represents a segment/range of the array:

- Root represents the whole array: `[0, n - 1]`
- Left child represents the left half
- Right child represents the right half
- Leaf node represents one index: `[i, i]`

Example:

```text
Array indices: 0 1 2 3 4 5 6 7

Root: [0, 7]
  Left:  [0, 3]
  Right: [4, 7]

[0, 3] splits into [0, 1] and [2, 3]
[4, 7] splits into [4, 5] and [6, 7]
...
Leaves: [0,0], [1,1], [2,2], ...
```

Segment trees are useful when you need:

- Many range queries
- Many point updates
- Fast recomputation after updates

Common queries:

- Range sum
- Range minimum
- Range maximum
- Range xor
- Range gcd
- Count something in a range
- Find first/last index satisfying a condition

## 2. Why Segment Trees Are Fast

If you answer each range query by looping over the array, one query can take `O(n)`.

If there are `q` queries, total time can become:

```text
O(n * q)
```

That is too slow for large constraints like:

```text
n, q <= 2e5
```

A segment tree preprocesses the array so that:

- Build: `O(n)`
- Point update: `O(log n)`
- Range query: `O(log n)`
- Memory: about `4 * n`

The reason is simple: every time we go down the tree, the segment size halves.

Tree height:

```text
log2(n)
```

So update/search paths are logarithmic.

## 3. What Makes A Segment Tree "Generic"?

A normal segment tree is usually hardcoded for one operation, like range sum.

Example:

```cpp
tree[index] = tree[2 * index] + tree[2 * index + 1];
```

But if the next problem asks range min, you rewrite:

```cpp
tree[index] = min(tree[2 * index], tree[2 * index + 1]);
```

If the next problem asks gcd:

```cpp
tree[index] = gcd(tree[2 * index], tree[2 * index + 1]);
```

A generic segment tree separates:

1. Static part: tree recursion logic that almost never changes
2. Dynamic part: what each node stores and how two nodes merge

The PDF's main idea:

> Keep the segment tree traversal fixed. Change only the node, update, and merge logic.

That is powerful because most segment tree problems differ only in:

- What information a node stores
- What the identity/default value is
- How two children combine
- How an update changes a leaf

## 4. The Two Main Custom Pieces

The generic template has two important structs:

```cpp
struct Node1 { ... };
struct Update1 { ... };
```

You can rename them, but conceptually:

- `Node` = what information is stored for a segment
- `Update` = what happens when an array position is updated

## 5. The Node Struct

For range sum, each node stores one value:

```cpp
struct Node {
    long long val;

    Node() {
        val = 0;
    }

    Node(long long x) {
        val = x;
    }

    void merge(Node &left, Node &right) {
        val = left.val + right.val;
    }
};
```

### Meaning Of Each Part

#### `val`

This is the answer for the segment represented by this node.

If the node represents `[l, r]`, then for range sum:

```text
val = a[l] + a[l+1] + ... + a[r]
```

For range minimum:

```text
val = min(a[l], a[l+1], ..., a[r])
```

For range gcd:

```text
val = gcd(a[l], a[l+1], ..., a[r])
```

#### Default Constructor

```cpp
Node() {
    val = 0;
}
```

This is the identity value used when there is no overlap in a query.

Identity means:

```text
merge(real_answer, identity) = real_answer
```

Examples:

| Query Type | Merge Operation | Identity |
|---|---:|---:|
| Sum | `a + b` | `0` |
| Min | `min(a, b)` | `INF` |
| Max | `max(a, b)` | `-INF` |
| GCD | `gcd(a, b)` | `0` |
| XOR | `a ^ b` | `0` |

Why `INF` for min?

```cpp
min(real_value, INF) = real_value
```

Why `0` for gcd?

```cpp
gcd(real_value, 0) = real_value
```

#### Parameterized Constructor

```cpp
Node(long long x) {
    val = x;
}
```

This builds a leaf node from the original array value.

If the segment is `[i, i]`, its answer is based only on `a[i]`.

#### Merge Function

```cpp
void merge(Node &left, Node &right) {
    val = left.val + right.val;
}
```

This combines two child segments.

If:

```text
left = answer for [l, mid]
right = answer for [mid+1, r]
```

Then:

```text
current = answer for [l, r]
```

For sum:

```text
current.sum = left.sum + right.sum
```

For min:

```text
current.min = min(left.min, right.min)
```

For xor:

```text
current.xor = left.xor ^ right.xor
```

## 6. The Generic Segment Tree Constructor

The tree stores:

```cpp
vector<Node> tree;
vector<long long> arr;
int n;
int s;
```

Typical constructor:

```cpp
SegTree(int n, vector<long long> &a) {
    arr = a;
    this->n = n;

    s = 1;
    while (s < 2 * n) {
        s <<= 1;
    }

    tree.resize(s);
    fill(tree.begin(), tree.end(), Node());
    build(0, n - 1, 1);
}
```

### Why Use Around `4n` Memory?

A segment tree over `n` elements has fewer than `4n` nodes.

The exact number depends on whether `n` is a power of 2.

Safe rule:

```cpp
tree.resize(4 * n);
```

The PDF's code finds a power-of-two-sized vector:

```cpp
s = 1;
while (s < 2 * n) s <<= 1;
```

Both are fine in competitive programming.

## 7. Build Function

```cpp
void build(int start, int end, int index) {
    if (start == end) {
        tree[index] = Node(arr[start]);
        return;
    }

    int mid = (start + end) / 2;

    build(start, mid, 2 * index);
    build(mid + 1, end, 2 * index + 1);

    tree[index].merge(tree[2 * index], tree[2 * index + 1]);
}
```

### What Build Does

Build is a postorder traversal:

1. Go to left child
2. Go to right child
3. Merge both children into current node

Leaf node:

```cpp
tree[index] = Node(arr[start]);
```

Internal node:

```cpp
tree[index].merge(left_child, right_child);
```

### Example

For sum:

```text
a = [2, 5, 1, 7]
```

Leaves:

```text
[0,0] = 2
[1,1] = 5
[2,2] = 1
[3,3] = 7
```

Parents:

```text
[0,1] = 2 + 5 = 7
[2,3] = 1 + 7 = 8
```

Root:

```text
[0,3] = 7 + 8 = 15
```

## 8. Update Struct

For point update, the update object stores the information needed to update a leaf.

Example: set `a[i] = x`.

```cpp
struct Update {
    long long val;

    Update(long long x) {
        val = x;
    }

    void apply(Node &node) {
        node.val = val;
    }
};
```

### Why Have A Separate Update Struct?

Because different problems update differently.

Examples:

Set value:

```cpp
node.val = x;
```

Add value:

```cpp
node.val += x;
```

Toggle bit:

```cpp
node.val ^= 1;
```

Replace with pair/state:

```cpp
node.minVal = x;
node.index = i;
```

The segment tree recursion is the same. Only `apply` changes.

## 9. Point Update Function

```cpp
void update(int start, int end, int index, int query_index, Update &u) {
    if (start == end) {
        u.apply(tree[index]);
        return;
    }

    int mid = (start + end) / 2;

    if (query_index <= mid) {
        update(start, mid, 2 * index, query_index, u);
    } else {
        update(mid + 1, end, 2 * index + 1, query_index, u);
    }

    tree[index].merge(tree[2 * index], tree[2 * index + 1]);
}
```

Wrapper:

```cpp
void make_update(int idx, long long val) {
    Update new_update(val);
    update(0, n - 1, 1, idx, new_update);
}
```

### Why Merge After Updating?

Suppose only `a[3]` changes.

Only nodes whose range contains index `3` become outdated.

So after updating the leaf:

```text
[3,3]
```

we recompute its ancestors:

```text
[2,3]
[0,3]
[0,7]
...
```

That path has length `O(log n)`.

## 10. Query Function

```cpp
Node query(int start, int end, int index, int left, int right) {
    if (start > right || end < left) {
        return Node();
    }

    if (start >= left && end <= right) {
        return tree[index];
    }

    int mid = (start + end) / 2;

    Node l = query(start, mid, 2 * index, left, right);
    Node r = query(mid + 1, end, 2 * index + 1, left, right);

    Node ans;
    ans.merge(l, r);
    return ans;
}
```

Wrapper:

```cpp
Node make_query(int left, int right) {
    return query(0, n - 1, 1, left, right);
}
```

### The 3 Query Cases

#### 1. No Overlap

```cpp
if (start > right || end < left)
```

Example:

```text
node segment = [0, 2]
query range  = [5, 7]
```

They do not intersect.

Return identity:

```cpp
return Node();
```

#### 2. Complete Overlap

```cpp
if (start >= left && end <= right)
```

Example:

```text
node segment = [2, 4]
query range  = [1, 6]
```

The node segment is fully inside the query range.

Return the stored answer directly:

```cpp
return tree[index];
```

#### 3. Partial Overlap

Otherwise, split:

```cpp
left_answer = query(left child)
right_answer = query(right child)
answer = merge(left_answer, right_answer)
```

## 11. How To Convert Any Simple Range Query

Ask these four questions:

### Question 1: What should each segment store?

Examples:

- Range sum: store sum
- Range min: store minimum
- Range max: store maximum
- Range gcd: store gcd
- Range xor: store xor

### Question 2: What is the identity value?

Examples:

- Sum: `0`
- Min: `INF`
- Max: `-INF`
- GCD: `0`
- XOR: `0`

### Question 3: How do I merge left and right?

Examples:

```cpp
sum = left.sum + right.sum;
mn = min(left.mn, right.mn);
mx = max(left.mx, right.mx);
g = gcd(left.g, right.g);
xr = left.xr ^ right.xr;
```

### Question 4: How does a point update affect a leaf?

For assignment:

```cpp
node.value = new_value;
```

For addition:

```cpp
node.value += add_value;
```

Then ancestors are recomputed automatically by `merge`.

## 12. Examples Of Node For Common Problems

### Range Minimum

```cpp
const long long INF = 4'000'000'000'000'000'000LL;

struct Node {
    long long mn;

    Node() {
        mn = INF;
    }

    Node(long long x) {
        mn = x;
    }

    void merge(Node &left, Node &right) {
        mn = min(left.mn, right.mn);
    }
};
```

### Range XOR

```cpp
struct Node {
    long long xr;

    Node() {
        xr = 0;
    }

    Node(long long x) {
        xr = x;
    }

    void merge(Node &left, Node &right) {
        xr = left.xr ^ right.xr;
    }
};
```

### Range GCD

```cpp
struct Node {
    long long g;

    Node() {
        g = 0;
    }

    Node(long long x) {
        g = x;
    }

    void merge(Node &left, Node &right) {
        g = std::gcd(left.g, right.g);
    }
};
```

## 13. Range Second Minimum

This is the first advanced pattern in the PDF.

Problem:

> Given a range `[l, r]`, find the second minimum value in that range.

For every segment, storing only the minimum is not enough.

Example:

```text
left segment:  minimum = 1
right segment: minimum = 2
```

To find the second minimum of the combined segment, you need more information.

Store:

```cpp
first_min
second_min
```

### Merge Idea

If left stores:

```text
left.first_min
left.second_min
```

and right stores:

```text
right.first_min
right.second_min
```

Then combined segment's two smallest values are the two smallest among:

```text
left.first_min
left.second_min
right.first_min
right.second_min
```

### Node Code

If duplicates count separately, for array `[1, 1, 2]`, second minimum is `1`.

```cpp
const long long INF = 4'000'000'000'000'000'000LL;

struct Node {
    long long mn1, mn2;

    Node() {
        mn1 = INF;
        mn2 = INF;
    }

    Node(long long x) {
        mn1 = x;
        mn2 = INF;
    }

    void merge(Node &left, Node &right) {
        vector<long long> candidates = {
            left.mn1, left.mn2,
            right.mn1, right.mn2
        };

        sort(candidates.begin(), candidates.end());

        mn1 = candidates[0];
        mn2 = candidates[1];
    }
};
```

If the problem asks second distinct minimum, then skip duplicates:

```cpp
void merge(Node &left, Node &right) {
    vector<long long> candidates = {
        left.mn1, left.mn2,
        right.mn1, right.mn2
    };

    sort(candidates.begin(), candidates.end());

    mn1 = candidates[0];
    mn2 = INF;

    for (long long x : candidates) {
        if (x > mn1) {
            mn2 = x;
            break;
        }
    }
}
```

### Important Exam Trap

Always check whether the problem wants:

- Second minimum with duplicates counted
- Second distinct minimum

These produce different answers:

```text
Array: [1, 1, 2]

second minimum counting duplicates = 1
second distinct minimum = 2
```

## 14. Range Minimum Index

Problem:

> Return the index of the minimum value in `[l, r]`.

If there are multiple minimum values, usually return the leftmost index unless stated otherwise.

You must store:

```cpp
value
index
```

### Node Code

```cpp
const long long INF = 4'000'000'000'000'000'000LL;

struct Node {
    long long val;
    int idx;

    Node() {
        val = INF;
        idx = -1;
    }

    Node(long long x, int i = -1) {
        val = x;
        idx = i;
    }

    void merge(Node &left, Node &right) {
        if (left.val < right.val) {
            val = left.val;
            idx = left.idx;
        } else if (right.val < left.val) {
            val = right.val;
            idx = right.idx;
        } else {
            val = left.val;
            idx = min(left.idx, right.idx);
        }
    }
};
```

The generic build shown in the PDF calls:

```cpp
Node(arr[start])
```

For this problem, the leaf also needs the index. So either:

1. Modify build to call `Node(arr[start], start)`, or
2. Store index another way inside the node/update logic

The clean version is:

```cpp
tree[index] = Node(arr[start], start);
```

### Merge Rule

For leftmost min:

```text
if left.val <= right.val, choose left
else choose right
```

For rightmost min:

```text
if left.val < right.val, choose left
else choose right
```

Small tie-breaking changes matter a lot.

## 15. First Element Greater Than X In Range

This is the most important advanced pattern in the PDF.

Problem:

> Given `[l, r]` and `x`, find the first index `i` in `[l, r]` such that `a[i] > x`.

Naive method:

```text
loop i from l to r
return first i with a[i] > x
```

Time:

```text
O(r - l + 1)
```

Segment tree method:

Store maximum at every node.

Why maximum?

If a segment's maximum is `<= x`, then no element in that segment can be greater than `x`.

So we can skip the whole segment.

### Node For This Problem

```cpp
const long long NEG_INF = -4'000'000'000'000'000'000LL;

struct Node {
    long long mx;

    Node() {
        mx = NEG_INF;
    }

    Node(long long x) {
        mx = x;
    }

    void merge(Node &left, Node &right) {
        mx = max(left.mx, right.mx);
    }
};
```

### Search Logic

We do not simply use `make_query(l, r)`.

We need a custom descent.

```cpp
int find_first_greater(
    int start,
    int end,
    int index,
    int left,
    int right,
    long long x
) {
    if (start > right || end < left) {
        return -1;
    }

    if (tree[index].mx <= x) {
        return -1;
    }

    if (start == end) {
        return start;
    }

    int mid = (start + end) / 2;

    int ans = find_first_greater(start, mid, 2 * index, left, right, x);

    if (ans != -1) {
        return ans;
    }

    return find_first_greater(mid + 1, end, 2 * index + 1, left, right, x);
}
```

Wrapper:

```cpp
int first_greater(int left, int right, long long x) {
    return find_first_greater(0, n - 1, 1, left, right, x);
}
```

### Why Check Left Child First?

Because we need the first index.

The first valid index is always found by trying the left side before the right side.

If the left side has some value `> x`, answer lies there.

Only if left side cannot contain an answer do we try the right side.

### Example

```text
a = [2, 1, 7, 3, 5, 8, 9, 10]
x = 4
range = [0, 7]
```

First element greater than `4` is:

```text
a[2] = 7
answer = 2
```

Why segment tree helps:

- Check root max: `10 > 4`, possible
- Go left half `[0, 3]`, max is `7 > 4`, possible
- Go left child `[0, 1]`, max is `2 <= 4`, impossible
- Go right child `[2, 3]`, max is `7 > 4`
- Go leaf `[2, 2]`, answer `2`

### Key Inequality

To skip a segment:

```cpp
if (segment_max <= x) return -1;
```

To continue searching:

```cpp
segment_max > x
```

The PDF illustrates this with maximum values stored at every node.

## 16. How The Prefix Maximum Idea Appears

The PDF's last slide shows an array like:

```text
Original: 100, 2, 10, 20, 5, 50, 40, 20, 60, 100
Prefix max inside range: 10, 20, 20, 50, 50, 50, 60
```

If you are trying to find the first value greater than `x`, the prefix maximum helps conceptually:

```text
first element > x
= first position where prefix maximum becomes > x
```

For example:

```text
range values: 10, 20, 5, 50, 40, 20, 60
x = 19
```

First value greater than `19`:

```text
20
```

Answer index corresponds to the first prefix max greater than `19`.

For `x = 55`:

```text
first value greater than 55 = 60
```

This is why storing maximum lets us binary-search/descent on the segment tree.

## 17. Generic Template Skeleton

Here is a clean generic version based on the PDF's structure.

```cpp
template <typename Node, typename Update>
struct SegTree {
    int n;
    vector<long long> arr;
    vector<Node> tree;

    SegTree(int n, vector<long long> &a) {
        this->n = n;
        arr = a;
        tree.assign(4 * n + 5, Node());
        build(0, n - 1, 1);
    }

    void build(int start, int end, int index) {
        if (start == end) {
            tree[index] = Node(arr[start]);
            return;
        }

        int mid = (start + end) / 2;
        build(start, mid, 2 * index);
        build(mid + 1, end, 2 * index + 1);

        tree[index].merge(tree[2 * index], tree[2 * index + 1]);
    }

    void update(int start, int end, int index, int query_index, Update &u) {
        if (start == end) {
            u.apply(tree[index]);
            return;
        }

        int mid = (start + end) / 2;

        if (query_index <= mid) {
            update(start, mid, 2 * index, query_index, u);
        } else {
            update(mid + 1, end, 2 * index + 1, query_index, u);
        }

        tree[index].merge(tree[2 * index], tree[2 * index + 1]);
    }

    void make_update(int index, long long value) {
        Update u(value);
        update(0, n - 1, 1, index, u);
    }

    Node query(int start, int end, int index, int left, int right) {
        if (start > right || end < left) {
            return Node();
        }

        if (start >= left && end <= right) {
            return tree[index];
        }

        int mid = (start + end) / 2;

        Node l = query(start, mid, 2 * index, left, right);
        Node r = query(mid + 1, end, 2 * index + 1, left, right);

        Node ans;
        ans.merge(l, r);
        return ans;
    }

    Node make_query(int left, int right) {
        return query(0, n - 1, 1, left, right);
    }
};
```

## 18. Full Example: Range Minimum With Point Assignment

```cpp
#include <bits/stdc++.h>
using namespace std;

const long long INF = 4'000'000'000'000'000'000LL;

struct Node {
    long long mn;

    Node() {
        mn = INF;
    }

    Node(long long x) {
        mn = x;
    }

    void merge(Node &left, Node &right) {
        mn = min(left.mn, right.mn);
    }
};

struct Update {
    long long val;

    Update(long long x) {
        val = x;
    }

    void apply(Node &node) {
        node.mn = val;
    }
};

template <typename Node, typename Update>
struct SegTree {
    int n;
    vector<long long> arr;
    vector<Node> tree;

    SegTree(int n, vector<long long> &a) {
        this->n = n;
        arr = a;
        tree.assign(4 * n + 5, Node());
        build(0, n - 1, 1);
    }

    void build(int start, int end, int index) {
        if (start == end) {
            tree[index] = Node(arr[start]);
            return;
        }

        int mid = (start + end) / 2;
        build(start, mid, 2 * index);
        build(mid + 1, end, 2 * index + 1);
        tree[index].merge(tree[2 * index], tree[2 * index + 1]);
    }

    void update(int start, int end, int index, int query_index, Update &u) {
        if (start == end) {
            u.apply(tree[index]);
            return;
        }

        int mid = (start + end) / 2;

        if (query_index <= mid) {
            update(start, mid, 2 * index, query_index, u);
        } else {
            update(mid + 1, end, 2 * index + 1, query_index, u);
        }

        tree[index].merge(tree[2 * index], tree[2 * index + 1]);
    }

    void make_update(int index, long long value) {
        Update u(value);
        update(0, n - 1, 1, index, u);
    }

    Node query(int start, int end, int index, int left, int right) {
        if (start > right || end < left) {
            return Node();
        }

        if (start >= left && end <= right) {
            return tree[index];
        }

        int mid = (start + end) / 2;

        Node l = query(start, mid, 2 * index, left, right);
        Node r = query(mid + 1, end, 2 * index + 1, left, right);

        Node ans;
        ans.merge(l, r);
        return ans;
    }

    Node make_query(int left, int right) {
        return query(0, n - 1, 1, left, right);
    }
};

int main() {
    vector<long long> a = {5, 2, 7, 1, 9};
    int n = a.size();

    SegTree<Node, Update> st(n, a);

    cout << st.make_query(1, 3).mn << "\n"; // min of [2,7,1] = 1

    st.make_update(3, 10); // a[3] = 10

    cout << st.make_query(1, 3).mn << "\n"; // min of [2,7,10] = 2
}
```

## 19. How To Identify Segment Tree Problems

Think segment tree if the problem has:

1. An array
2. Many queries
3. Range queries or range-dependent answers
4. Updates
5. Operation can be merged from left half and right half

The magic word is merge.

If answer for `[l, r]` can be formed from:

```text
answer([l, mid]) and answer([mid+1, r])
```

then segment tree is likely possible.

## 20. The Merge Test

Before writing code, ask:

> Can I combine two adjacent segment answers to get the bigger segment answer?

Examples:

### Range Sum

```text
sum(left + right) = sum(left) + sum(right)
```

Works.

### Range Minimum

```text
min(left + right) = min(min(left), min(right))
```

Works.

### Range GCD

```text
gcd(left + right) = gcd(gcd(left), gcd(right))
```

Works.

### Range Median

If you only store median of left and median of right, you cannot get median of combined range.

So plain segment tree with one value does not work.

But if you store sorted vectors in each node, it becomes merge sort tree.

## 21. Decision Table

| Problem asks | Store in Node | Merge |
|---|---|---|
| Range sum | `sum` | `left.sum + right.sum` |
| Range min | `mn` | `min(left.mn, right.mn)` |
| Range max | `mx` | `max(left.mx, right.mx)` |
| Range xor | `xr` | `left.xr ^ right.xr` |
| Range gcd | `g` | `gcd(left.g, right.g)` |
| Range second min | `mn1, mn2` | two smallest of four candidates |
| Min index | `val, idx` | smaller value, tie by index |
| First `> x` | `mx` | max, then custom descent |

## 22. Common Mistakes

### Mistake 1: Wrong Identity

For min query, if you return `0` on no overlap, answer becomes wrong.

Example:

```text
a = [5, 6, 7]
query [1, 2]
```

If a no-overlap segment returns `0`, then:

```text
min(0, 6, 7) = 0
```

Wrong.

Correct no-overlap value for min:

```text
INF
```

### Mistake 2: Forgetting To Merge After Update

If you update the leaf but do not recompute ancestors, future queries still see old values.

Always do:

```cpp
tree[index].merge(tree[2 * index], tree[2 * index + 1]);
```

after recursive update.

### Mistake 3: Bad Tie-Breaking

For min index, decide:

- leftmost minimum?
- rightmost minimum?
- any minimum?

Then write merge accordingly.

### Mistake 4: Using Query When You Need Search

For "first index greater than x", a normal query gives maximum value, not the first index.

You need custom descent using maximum values.

### Mistake 5: Mixing 0-Based And 1-Based Indexing

The template uses 0-based array indices:

```cpp
build(0, n - 1, 1)
```

Tree node index starts at `1`.

Array index starts at `0`.

## 23. Problem-Solving Formula

Whenever you see a new segment tree question, fill this:

```text
1. Query type:
2. Update type:
3. Node stores:
4. Identity value:
5. Leaf constructor:
6. Merge rule:
7. Update apply rule:
8. Is normal range query enough, or do I need custom descent?
```

Example:

```text
Problem: range gcd, point assignment

1. Query type: gcd of [l, r]
2. Update type: set a[i] = x
3. Node stores: gcd
4. Identity value: 0
5. Leaf constructor: g = a[i]
6. Merge rule: gcd(left.g, right.g)
7. Update apply: node.g = x
8. Normal range query is enough
```

Example:

```text
Problem: first element > x in [l, r], point assignment

1. Query type: first index satisfying condition
2. Update type: set a[i] = x
3. Node stores: maximum
4. Identity value: -INF
5. Leaf constructor: mx = a[i]
6. Merge rule: max(left.mx, right.mx)
7. Update apply: node.mx = x
8. Need custom descent, not normal range query
```

## 24. Practice Questions

Try to solve these by filling the formula above.

### Easy

1. Range sum with point assignment
2. Range minimum with point assignment
3. Range maximum with point addition
4. Range xor with point assignment
5. Range gcd with point assignment

### Medium

1. Return count of zeroes in `[l, r]`
2. Return index of maximum in `[l, r]`
3. Return minimum value and its frequency in `[l, r]`
4. Return maximum subarray sum in `[l, r]`
5. Find first index in `[l, r]` where `a[i] >= x`

### Harder

1. Find first index where prefix sum becomes at least `x`
2. Find kth zero in the array
3. Range second maximum
4. Range maximum subarray sum with updates
5. Segment tree with lazy propagation for range updates

## 25. Quick Mental Model

A segment tree node is a compressed summary of a range.

The entire art is choosing a summary that:

1. Is enough to answer the query
2. Can be merged from two children
3. Can be updated at a leaf

If you can design that summary, the generic template does the rest.

That is the whole lesson.

## 26. Practice Problems - Thinking Approach

This section explains how to think for each practice problem.

For every segment tree problem, use this mental checklist:

```text
1. Query kya maang rahi hai?
2. Ek segment/node me kya store karna enough hoga?
3. Do child nodes ko merge kaise karenge?
4. No-overlap ke liye identity value kya hogi?
5. Update leaf ko kaise change karega?
6. Kya normal range query enough hai, ya custom descent/search chahiye?
```

### Easy 1. Range Sum With Point Assignment

Problem:

```text
Update: a[i] = x
Query: sum of a[l...r]
```

Thinking:

For any range, sum is easy to split:

```text
sum([l, r]) = sum(left half) + sum(right half)
```

So each node only needs to store:

```cpp
sum
```

Identity:

```text
0
```

because:

```text
real_sum + 0 = real_sum
```

Merge:

```cpp
sum = left.sum + right.sum;
```

Point assignment:

```cpp
node.sum = x;
```

Normal range query is enough.

Final thought:

```text
This is the most basic segment tree problem.
If range answer can be added from children, store sum.
```

### Easy 2. Range Minimum With Point Assignment

Problem:

```text
Update: a[i] = x
Query: minimum of a[l...r]
```

Thinking:

Minimum of a bigger segment comes from minimum of its two halves:

```text
min([l, r]) = min(min(left), min(right))
```

So each node stores:

```cpp
mn
```

Identity:

```text
INF
```

because no-overlap should not affect answer:

```text
min(real_value, INF) = real_value
```

Merge:

```cpp
mn = min(left.mn, right.mn);
```

Point assignment:

```cpp
node.mn = x;
```

Normal range query is enough.

Common mistake:

Never use `0` as identity for minimum unless all values are negative and the problem specially allows it. Usually `INF` is correct.

### Easy 3. Range Maximum With Point Addition

Problem:

```text
Update: a[i] += x
Query: maximum of a[l...r]
```

Thinking:

Maximum of a bigger segment comes from max of child maximums:

```text
max([l, r]) = max(max(left), max(right))
```

So each node stores:

```cpp
mx
```

Identity:

```text
-INF
```

because:

```text
max(real_value, -INF) = real_value
```

Merge:

```cpp
mx = max(left.mx, right.mx);
```

Point addition update:

```cpp
node.mx += x;
```

Why this works:

At a leaf, the segment contains only one value, so adding `x` to the array element means adding `x` to that leaf's maximum.

Then ancestors recompute automatically using merge.

Normal range query is enough.

### Easy 4. Range XOR With Point Assignment

Problem:

```text
Update: a[i] = x
Query: xor of a[l...r]
```

Thinking:

XOR can be merged:

```text
xor([l, r]) = xor(left) ^ xor(right)
```

So each node stores:

```cpp
xr
```

Identity:

```text
0
```

because:

```text
real_xor ^ 0 = real_xor
```

Merge:

```cpp
xr = left.xr ^ right.xr;
```

Point assignment:

```cpp
node.xr = x;
```

Normal range query is enough.

Extra intuition:

XOR behaves like addition without carry. It is associative, so segment tree works nicely.

### Easy 5. Range GCD With Point Assignment

Problem:

```text
Update: a[i] = x
Query: gcd of a[l...r]
```

Thinking:

GCD of a big range can be merged from child GCDs:

```text
gcd([l, r]) = gcd(gcd(left), gcd(right))
```

So each node stores:

```cpp
g
```

Identity:

```text
0
```

because:

```text
gcd(real_value, 0) = real_value
```

Merge:

```cpp
g = gcd(left.g, right.g);
```

Point assignment:

```cpp
node.g = x;
```

Normal range query is enough.

Common mistake:

For GCD, identity is not `1`. If you return `1` for no overlap, every answer may become `1`.

### Medium 1. Return Count Of Zeroes In `[l, r]`

Problem:

```text
Update: usually a[i] = x
Query: number of zeroes in a[l...r]
```

Thinking:

For every segment, you do not need all values. You only need:

```text
How many zeroes are inside this segment?
```

So each node stores:

```cpp
cntZero
```

Leaf:

```cpp
if (a[i] == 0) cntZero = 1;
else cntZero = 0;
```

Identity:

```text
0
```

because no-overlap contributes no zeroes.

Merge:

```cpp
cntZero = left.cntZero + right.cntZero;
```

Point assignment:

```cpp
node.cntZero = (x == 0);
```

Normal range query is enough.

Final thought:

```text
Whenever query asks "count of something", node usually stores count.
```

### Medium 2. Return Index Of Maximum In `[l, r]`

Problem:

```text
Query: index of maximum value in range
Update: usually a[i] = x
```

Thinking:

If you store only maximum value, you know the value but not where it occurs.

So store:

```cpp
mx, idx
```

Leaf:

```cpp
mx = a[i]
idx = i
```

Identity:

```cpp
mx = -INF
idx = -1
```

Merge:

```text
If left.mx > right.mx, choose left.
If right.mx > left.mx, choose right.
If equal, choose based on question:
    leftmost max  -> smaller index
    rightmost max -> larger index
```

For leftmost maximum:

```cpp
if (left.mx >= right.mx) {
    mx = left.mx;
    idx = left.idx;
} else {
    mx = right.mx;
    idx = right.idx;
}
```

Point assignment:

```cpp
node.mx = x;
node.idx = i;
```

Important:

The leaf constructor needs index also. So build may need:

```cpp
tree[index] = Node(arr[start], start);
```

Normal range query is enough.

### Medium 3. Return Minimum Value And Its Frequency In `[l, r]`

Problem:

```text
Query: minimum value in range and how many times it appears
```

Example:

```text
a = [3, 1, 2, 1, 5]
query [0, 4]
answer = min value 1, frequency 2
```

Thinking:

Storing only min value is not enough because frequency is also needed.

Store:

```cpp
mn, freq
```

Leaf:

```cpp
mn = a[i]
freq = 1
```

Identity:

```cpp
mn = INF
freq = 0
```

Merge logic:

```text
If left.mn < right.mn:
    answer min = left.mn
    frequency = left.freq

If right.mn < left.mn:
    answer min = right.mn
    frequency = right.freq

If both equal:
    answer min = left.mn
    frequency = left.freq + right.freq
```

Code:

```cpp
if (left.mn < right.mn) {
    mn = left.mn;
    freq = left.freq;
} else if (right.mn < left.mn) {
    mn = right.mn;
    freq = right.freq;
} else {
    mn = left.mn;
    freq = left.freq + right.freq;
}
```

Point assignment:

```cpp
node.mn = x;
node.freq = 1;
```

Normal range query is enough.

Final thought:

```text
When answer needs "best value + count", store both.
```

### Medium 4. Return Maximum Subarray Sum In `[l, r]`

Problem:

```text
Query: maximum subarray sum inside range [l, r]
Update: usually point assignment
```

This is more interesting.

For a range, maximum subarray can be:

```text
1. Completely inside left child
2. Completely inside right child
3. Crosses from left child to right child
```

To handle crossing case, each node stores four values:

```cpp
sum   = total sum of segment
pref  = maximum prefix sum
suff  = maximum suffix sum
best  = maximum subarray sum
```

Leaf:

```cpp
sum = x;
pref = x;
suff = x;
best = x;
```

Merge:

```cpp
sum = left.sum + right.sum;
pref = max(left.pref, left.sum + right.pref);
suff = max(right.suff, right.sum + left.suff);
best = max({left.best, right.best, left.suff + right.pref});
```

Identity:

For no-overlap, be careful. This problem is easier if query avoids merging invalid nodes, or uses:

```cpp
sum = 0
pref = suff = best = -INF
```

Point assignment:

```cpp
sum = x;
pref = x;
suff = x;
best = x;
```

Normal range query is enough if your identity is handled correctly.

Final thought:

```text
When answer may cross the middle, store prefix and suffix information.
```

### Medium 5. Find First Index In `[l, r]` Where `a[i] >= x`

Problem:

```text
Find smallest i in [l, r] such that a[i] >= x
```

Thinking:

Normal range query can tell maximum value, but not first index directly.

To know whether a segment can contain answer, store:

```cpp
mx
```

If:

```cpp
segment.mx < x
```

then this whole segment cannot contain an answer.

If:

```cpp
segment.mx >= x
```

then this segment may contain an answer.

Node:

```cpp
mx
```

Merge:

```cpp
mx = max(left.mx, right.mx);
```

Identity:

```text
-INF
```

Search:

```text
1. If segment outside [l, r], return -1.
2. If segment maximum < x, return -1.
3. If leaf, return index.
4. Search left child first.
5. If not found, search right child.
```

Why left first?

Because we need first/smallest index.

This needs custom descent, not normal query.

### Harder 1. Find First Index Where Prefix Sum Becomes At Least `x`

Problem:

```text
Find first index i such that:
a[0] + a[1] + ... + a[i] >= x
```

Usually assumes all values are non-negative.

Thinking:

If all values are non-negative, prefix sum only increases.

Store:

```cpp
sum
```

At root, total sum tells whether answer exists:

```cpp
if (tree[1].sum < x) return -1;
```

Then descend:

```text
At node:
    If left.sum >= x:
        answer is in left child
    Else:
        answer is in right child
        x -= left.sum
```

Why subtract?

If left half total sum is smaller than `x`, then the prefix must pass through the entire left half. So when going right, we only need the remaining sum:

```text
new_x = x - left.sum
```

Node:

```cpp
sum
```

Merge:

```cpp
sum = left.sum + right.sum;
```

Update:

```cpp
node.sum = x;
```

Custom descent is needed.

Important condition:

This simple logic works when array values are non-negative. If negative values exist, prefix sum is not monotonic, so this approach breaks.

### Harder 2. Find Kth Zero In The Array

Problem:

```text
Find index of kth zero
```

Example:

```text
a = [1, 0, 5, 0, 0]
1st zero -> index 1
2nd zero -> index 3
3rd zero -> index 4
```

Thinking:

This is like prefix count search.

Store:

```cpp
cntZero
```

At root:

```cpp
if (tree[1].cntZero < k) return -1;
```

Descent:

```text
If left.cntZero >= k:
    kth zero is in left child
Else:
    kth zero is in right child
    k -= left.cntZero
```

Why subtract?

If left has fewer than `k` zeroes, then all zeroes in left come before the answer. So in right child, we search for:

```text
k - left.cntZero
```

Merge:

```cpp
cntZero = left.cntZero + right.cntZero;
```

Leaf:

```cpp
cntZero = (a[i] == 0);
```

Update:

```cpp
node.cntZero = (x == 0);
```

Custom descent is needed.

Final thought:

```text
Kth something usually means store count and descend by counts.
```

### Harder 3. Range Second Maximum

Problem:

```text
Query: second maximum value in [l, r]
```

Thinking:

Like second minimum, but reversed.

Storing only maximum is not enough. Store top two maximums:

```cpp
mx1, mx2
```

Leaf:

```cpp
mx1 = a[i]
mx2 = -INF
```

Identity:

```cpp
mx1 = -INF
mx2 = -INF
```

Merge:

Take four candidates:

```text
left.mx1
left.mx2
right.mx1
right.mx2
```

The largest becomes `mx1`, second largest becomes `mx2`.

If duplicates count separately:

```cpp
vector<long long> v = {left.mx1, left.mx2, right.mx1, right.mx2};
sort(v.rbegin(), v.rend());
mx1 = v[0];
mx2 = v[1];
```

If second distinct maximum is required, skip equal values.

Point assignment:

```cpp
mx1 = x;
mx2 = -INF;
```

Normal range query is enough.

Important:

Always read statement carefully:

```text
second maximum vs second distinct maximum
```

They are different.

### Harder 4. Range Maximum Subarray Sum With Updates

This is the same structure as Medium 4, but with point updates included.

Problem:

```text
Update: a[i] = x
Query: maximum subarray sum in [l, r]
```

Thinking:

Again, maximum subarray can be:

```text
inside left
inside right
crossing middle
```

So store:

```cpp
sum, pref, suff, best
```

Merge:

```cpp
sum = left.sum + right.sum;
pref = max(left.pref, left.sum + right.pref);
suff = max(right.suff, right.sum + left.suff);
best = max({left.best, right.best, left.suff + right.pref});
```

Update:

At leaf:

```cpp
sum = x;
pref = x;
suff = x;
best = x;
```

Then merge ancestors.

Normal range query is enough.

Important variation:

Some problems allow empty subarray. Then leaf becomes:

```cpp
pref = suff = best = max(0LL, x);
```

Some problems do not allow empty subarray. Then leaf is:

```cpp
pref = suff = best = x;
```

This tiny detail changes the answer for all-negative arrays.

### Harder 5. Segment Tree With Lazy Propagation For Range Updates

Problem type:

```text
Range update: add x to every a[i] in [l, r]
Range query: sum/min/max/etc. in [l, r]
```

Thinking:

Normal segment tree point update changes one leaf and recomputes ancestors.

But range update may affect many leaves.

If you update every leaf one by one:

```text
O(length * log n)
```

Too slow.

Lazy propagation idea:

```text
If a whole segment is covered by update range,
update that node directly and store pending update in lazy array.
Do not immediately go to children.
```

This is called lazy because we delay pushing updates to children until needed.

### Example: Range Add + Range Sum

Node stores:

```cpp
sum
```

Lazy stores:

```cpp
pendingAdd
```

If adding `x` to every element in segment `[start, end]`, segment length is:

```cpp
len = end - start + 1
```

Then:

```cpp
tree[index].sum += x * len;
lazy[index] += x;
```

Why multiply by length?

Because every element increases by `x`.

If there are `len` elements, total sum increases by:

```text
x * len
```

### Push Function

When going down from a node, push pending update to children:

```cpp
void push(int index, int start, int end) {
    if (lazy[index] == 0 || start == end) return;

    int mid = (start + end) / 2;
    int leftLen = mid - start + 1;
    int rightLen = end - mid;

    tree[2 * index].sum += lazy[index] * leftLen;
    lazy[2 * index] += lazy[index];

    tree[2 * index + 1].sum += lazy[index] * rightLen;
    lazy[2 * index + 1] += lazy[index];

    lazy[index] = 0;
}
```

### Range Update Cases

Same three overlap cases:

```text
1. No overlap:
   return

2. Complete overlap:
   update current node and lazy
   return

3. Partial overlap:
   push current lazy
   update left and right
   merge children
```

### Range Query Cases

Again:

```text
1. No overlap:
   return identity

2. Complete overlap:
   return tree[index]

3. Partial overlap:
   push current lazy
   query left and right
   merge answers
```

Final thought:

```text
Lazy propagation is used when updates affect a whole range.
Point update does not need lazy.
```

## 27. One-Line Recognition Tricks

Use these shortcuts during contests:

```text
Range sum/min/max/gcd/xor:
    Store same thing, merge same operation.

Need value + where:
    Store value and index.

Need value + how many:
    Store value and frequency.

Need second best:
    Store top two values.

Need first position satisfying condition:
    Store enough info to reject a whole segment, then descend left first.

Need kth object:
    Store count, descend using left count.

Need max subarray:
    Store sum, prefix, suffix, best.

Need range updates:
    Add lazy propagation.
```
