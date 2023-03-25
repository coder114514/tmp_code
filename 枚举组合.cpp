#include <bits/stdc++.h>
using namespace std;

void deal_with(int n, int combo) {
    for (int i = n - 1; i >= 0; i--) {
        if (combo >> i & 1) cout << '1';
        else cout << '0';
    }
    cout << endl;
}

void enum_combo(int n, int k) {
    int combo = (1 << k) - 1;
    while (combo < (1 << n)) {
        deal_with(n, combo);
        int x = combo & -combo;
        int y = combo + x;
        int z = (combo & ~y);
        combo = z / x;
        combo >>= 1;
        combo |= y;
    }
}

int main() {
    enum_combo(5, 3);
    return 0;
}
