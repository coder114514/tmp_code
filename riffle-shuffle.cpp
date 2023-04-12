#include <bits/stdc++.h>
using namespace std;

void shuffle(vector<int>& a) {
    vector<int> b[2];
    for (int i = 0; i < a.size(); i++) {
        if (2 * i < a.size()) {
            b[0].push_back(a[i]);
        }
        else {
            b[1].push_back(a[i]);
        }
    }
    int j = 0, idx[2] = {};
    for (int i = 0; i < a.size(); i++) {
        if (idx[j] < b[j].size()) {
            a[i] = b[j][idx[j]];
            ++idx[j];
        }
        j ^= 1;
    }
}

bool order(const vector<int>& a) {
    for (int i = 0; i < a.size(); i++) {
        if (a[i] != i + 1) return false;
    }
    return true;
}

int cycle(int n) {
    vector<int> a(n);
    for (int i = 1; i <= n; i++) {
        a[i-1] = i;
    }
    int cnt = 0;
    do
    {
        ++cnt;
        shuffle(a);
    } while (!order(a));
    return cnt;
}

int main() {
    freopen("riffle-shuffle-cycle.txt", "w", stdout);
    for (int i = 1; i <= 54; i++) {
        cout << i << ": " << cycle(i) << endl;
    }
    return 0;
}
