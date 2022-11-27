#include <bits/stdc++.h>
using namespace std;

typedef pair<int,int> pii;

int t, n;
map<int, pii> f;
int a[20020];

void ins(int x, int y,int cnt) {
	auto it = prev(f.lower_bound(x));
	if (it->second.first >= y)
	{
		return;
	}
	it++;
    while (it != f.end() && it->second.first < y) {
        f.erase(it++);
    }
    f[x] = make_pair(y,cnt);
}

int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &a[i]);
    }
    f.clear();
    f[-1e9] = make_pair(0,1);
    for (int i = 0; i < n; i++) {
        auto it=f.lower_bound(a[i]);
        int len=prev(it)->second.first,cnt=0;
        do
        {
            --it;
            if(it->second.first!=len)break;
            cnt+=it->second.second;
        }while(it!=f.begin());
        ins(a[i],len + 1,cnt);
    }
    printf("%d %d\n", f.rbegin()->second.first,f.rbegin()->second.second);
    return 0;
}

