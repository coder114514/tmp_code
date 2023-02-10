#include <bits/stdc++.h>
using namespace std;

int bits[1010], nB;

void change(int n) {
    nB = 0;
    while (n) {
        int q = ceil(n / -10.0);
        bits[++nB] = n - q * -10;
        n = q;
    }
    if (!nB) bits[++nB] = 0;
}

int main() {
    for (int i = -100; i <= 100; i++) {
        change(i);
        printf("%6d   ", i);
        change(i);
        for (int j = nB; j >= 1; j--) {
            printf("%d", bits[j]);
        }
        printf("\n");
    }
    int n;
    cin >> n;
    change(n);
    for (int j = nB; j >= 1; j--) {
        printf("%d", bits[j]);
    }
    printf("\n");
    return 0;
}
