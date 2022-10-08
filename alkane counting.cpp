#include<bits/stdc++.h>
using namespace std;

//n��ͬ���칹����Ŀ 

typedef long long ll;

const ll MOD=998244353;

const ll N=2010;
ll n;

ll C(ll n,ll m)
{
    ll res=1; 
    for(ll i=n;i>=n-m+1;i--)res*=i;
    for(ll i=1;i<=m;i++)res/=i;
    return res;
}

ll f[(N-1)/2][N][5];
// f[size][i][j]=������С������size,i����,���Ķ���Ϊj�ķ����� 
// f[size][i][j]=f[size-1][i][j]+��k f[size-1][i-k*size][j-k]*C(sum+k-1,k)   ��sum=��l=0-3  f[size-1][size][l]
//                ��<size                         ��=size��
// ans=��k=0-4, f[(n-1)/2][n][k] + [n%2==0] C(��k=0-3 f[(n-1)/2][n/2][k]+1,2)
void solve1(ll n)
{
    if(n>2000)
    {
        cout<<"no"<<endl;
        return;
    }
    for(ll size=0;size<=(n-1)/2;size++)f[size][1][0]=1;
    for(ll size=1;size<=(n-1)/2;size++) // size<=(n-1)/2Ҫ��֤�������� 
    {
        ll sum=0;
        for(ll l=0;l<=3;l++)sum+=f[size-1][size][l];
        for(ll i=1;i<=n;i++)
        {
            for(ll j=1;j<=4;j++)
            {
                f[size][i][j]=f[size-1][i][j];
                for(ll k=1;k*size<i&&k<=j;k++)
                    f[size][i][j]+=f[size-1][i-k*size][j-k]*C(sum+k-1,k);
            }
        }
    }
    ll ans=0;
    for(ll k=0;k<=4;k++)ans+=f[(n-1)/2][n][k];
    if(n%2==0)
    {
        ll sum=0;
        for(ll k=0;k<=3;k++)sum+=f[(n-1)/2][n/2][k];
        ans+=sum*(sum+1)/2;
    }
    cout<<ans<<endl;
}

// �����Ż�
ll g[N][5];
void solve2(ll n)
{
    if(n>2000)
    {
        cout<<"no"<<endl;
        return;
    }
    g[1][0]=1;
    for(ll size=1;size<=(n-1)/2;size++) // size<=(n-1)/2Ҫ��֤�������� 
    {
        ll sum=0;
        for(ll l=0;l<=3;l++)sum+=g[size][l];
        for(ll i=n;i>=1;i--)
        {
            for(ll j=1;j<=4;j++)
            {
                g[i][j]=g[i][j];
                for(ll k=1;k*size<i&&k<=j;k++)
                    g[i][j]+=g[i-k*size][j-k]*C(sum+k-1,k);
            }
        }
    }
    ll ans=0;
    for(ll k=0;k<=4;k++)ans+=g[n][k];
    if(n%2==0)
    {
        ll sum=0;
        for(ll k=0;k<=3;k++)sum+=g[n/2][k];
        ans+=sum*(sum+1)/2;
    }
    cout<<ans<<endl;
}

int main()
{
    cin>>n;
    solve2(n);
    return 0;
}
