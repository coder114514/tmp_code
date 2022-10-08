#include<bits/stdc++.h>
using namespace std;

int N=100000,gate=3,nsim=10;

double simulateOnce(){
    srand(time(0));
    int win=0;
    for(int i=1;i<=N;i++){
        // c车,x选,k开,h换
        int c=rand()%gate,x=rand()%gate,k,h;
        vector<int> pos; // posibilities所有可能性
        for(k=0;k<gate;k++)if(k!=c&&k!=x)pos.push_back(k);
        k=pos[rand()%pos.size()];
        for(h=0;h<gate;h++)if(h!=k&&h!=x)break;
        if(h==c)win++;
    }
    return win*1.0/N;
}

int main(){
    double res=0;
    for(int i=1;i<=nsim;i++){
        res+=simulateOnce();
    }
    cout<<res/nsim<<endl;
    return 0;
}