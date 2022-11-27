#include <bits/stdc++.h>
using namespace std;

int t, n;
map<int, int> f;
int a[20020];

void ins(int x, int y) {
	auto it = prev(f.lower_bound(x));
	if (it->second >= y)
	{
		return;
	}
	it++;
    while (it != f.end() && it->second < y) {
        f.erase(it++);
    }
    f[x] = y;
}

int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &a[i]);
    }
    f.clear();
    f[-1e9] = 0;
    for (int i = 0; i < n; i++) {
        ins(a[i], (--f.lower_bound(a[i]))->second + 1);
    }
    printf("%d\n", f.rbegin()->second);
    return 0;
}

