#include<bits/stdc++.h>
using namespace std;

int bits[1010],nB;

void change(int n) {
    nB=0;
    while(n) {
        int m=(n>0?n/-10:(-n+9)/10);
        bits[++nB]=n-m*-10;
        n=m;
    }
    if(!nB)bits[++nB]=0;
}

int main() {
    for(int i=-100; i<=100; i++) {
        change(i);
        printf("%6d   ",i);
        change(i);
        for(int j=nB; j>=1; j--) {
            printf("%d",bits[j]);
        }
        printf("\n");
    }
    int n;
    cin>>n;
    change(n);
    for(int j=nB; j>=1; j--) {
        printf("%d",bits[j]);
    }
    printf("\n");
    return 0;
}
