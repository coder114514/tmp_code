#include<bits/stdc++.h>
using namespace std;

// n个顶点的多边形，按照一定顺序排列它的所有顶点，两个相邻的顶点不能排在一起，问有几种顺序（旋转之后的算作不同方案）
// https://oeis.org/A002493

int n, ans;
bool vis[110];
void dfs(int i, int last) {
    if (i == n+1) {
        ans++;
        return;
    }
    for (int j = 0; j < n; j++) {
        if (vis[j]) continue;
        if (last != -1 && (last == (j+1)%n || (last+1)%n == j)) continue;
        vis[j] = true;
        dfs(i+1, j);
        vis[j] = false;
    }
}

int main() {
    cin >> n;
    dfs(0, -1);
    cout << ans << endl;
    return 0;
}
