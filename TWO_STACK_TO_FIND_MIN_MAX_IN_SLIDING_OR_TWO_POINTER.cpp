
class Queue {
    stack<pair<int, int>> in, out;

public:
    void push(int x) {
        if (in.empty()) {
            in.push({x, x});
        } else {
            in.push({x, max(x, in.top().second)});
        }
    }

    void pop() {
        if (out.empty()) {
            while (!in.empty()) {
                auto it = in.top();
                in.pop();

                if (out.empty()) {
                    out.push({it.first, it.first});
                } else {
                    out.push({it.first, max(it.first, out.top().second)});
                }
            }
        }

        if (!out.empty()) {
            out.pop();
        }
    }

    int maxxi() {
        int a = (!in.empty()) ? in.top().second : INT_MIN;
        int b = (!out.empty()) ? out.top().second : INT_MIN;

        return max(a, b);
    }
};
