#include<bits/stdc++.h>
using namespace std;

// һ��n�������������Σ�����һ��˳��ѡ������ÿ�����㣬��������ѡ�����ڵĶ��㣬���м���ѡ�񷽷�
// ���: https://oeis.org/A002493

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
