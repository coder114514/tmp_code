#include <bits/stdc++.h>
using namespace std;

int bits[114514], nB;

int main() {
    int n, base;
    cin >> n >> base;
    while (n) {
        if (n < 0 && base < 0) {
            int x1 = (n + base + 1) / base;
            bits[++nB] = n - x1 * base;
            n = x1;
        } else {
            bits[++nB] = n % base;
            n /= base;
        }
    }
    for (int i = nB; i >= 1; i--) {
        if (bits[i] >= 10) {
            cout << (char)(bits[i] - 10 + 'A');
        } else {
            cout << bits[i];
        }
    }
    cout << endl;
    return 0;
}
