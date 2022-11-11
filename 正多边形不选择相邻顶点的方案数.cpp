#include<bits/stdc++.h>
using namespace std;

// 一个n个顶点的正多边形，按照一定顺序选择它的每个顶点，不能连续选择相邻的顶点，问有几种选择方法
// 结果: https://oeis.org/A002493

int n,ans;
bool vis[110];
void dfs(int i,int last) {
    if(i==n+1) {
        ans++;
        return;
    }
    for(int j=0; j<n; j++) {
        if(vis[j])continue;
        if(last!=-1&&(last==(j+1)%n||(last+1)%n==j))continue;
        vis[j]=1;
        dfs(i+1,j);
        vis[j]=0;
    }
}

int main() {
    cin>>n;
    dfs(1,-1);
    cout<<ans<<endl;
    return 0;
}
