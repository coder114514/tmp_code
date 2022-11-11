#include <bits/stdc++.h>
using namespace std;

int n;
double a[109];

const double eps = 0.001;
void printPoly(int n, double a[]) {
    cout << fixed << setprecision(3);
    while (n >= 0 && fabs(a[n]) < eps) n--;
    if (n < 0) {
        cout << 0;
        return;
    }
    for (int i = n; i >= 0; i--) {
        if (fabs(a[i]) < eps) continue;
        if (a[i] > 0 && i < n) cout << "+";
        if (1-eps > fabs(a[i]) || fabs(a[i]) > 1+eps || i == 0) cout << a[i];
        else if (-1-eps < a[i] && a[i] < -1+eps) cout << "-";
        if (i > 1) cout << "x^" << i;
        else if (i == 1) cout << "x";
    }
}

int main() {
    cin >> n;
    for (int i = 1; i <= n+1; i++) cin >> a[n-i+1];
    printPoly(n, a);
    return 0;
}
