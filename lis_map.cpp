#include <bits/stdc++.h>
using namespace std;

//https://wwwwodddd.com/public/lis.html

typedef pair<int, int> pii;

int t, n;
map<int, pii> f;
int a[20020];

//f[v]: 表示以v结尾的最长上升子序列 first是长度 second是数量

//维护f满足单调性：v1<v2, f[v1].first<=f[v2].first
void ins(int x, int y, int cnt) {
    auto it = prev(f.lower_bound(x));
    if (it->second.first >= y) {
        return;
    }
    it++;
    while (it != f.end() && it->second.first < y) {  //结尾数字>=x但是长度<y的子序列都比当前的差，所以全部删掉
        f.erase(it++);
    }
    f[x] = make_pair(y, cnt);
}

int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &a[i]);
    }
    f.clear();
    f[-1e9] = make_pair(0, 1);
    for (int i = 0; i < n; i++) {
        auto it = f.lower_bound(a[i]);
        int len = prev(it)->second.first, cnt = 0;
        do {
            --it;
            if (it->second.first != len)
                break;
            cnt += it->second.second;
        } while (it != f.begin());
        ins(a[i], len + 1, cnt);
    }
    printf("%d %d\n", f.rbegin()->second.first, f.rbegin()->second.second);
    return 0;
}
