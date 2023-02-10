//encoding: GB2312
#include <bits/stdc++.h>
using namespace std;

typedef unsigned long long ull;
ull x, y, k, len;
map<ull, ull> occur;

ull cnt(ull x) {
    ull res = 0;
    while (x > 0) {
        ++res;
        x /= 10;
    }
    return res;
}

void putnum(ull x) {
    if (x <= 0) return;
    putnum(x / k);
    if (x % k >= 10) {
        cout << '(' << x % k << ')';
        len += cnt(x % k) + 2;
    }
    else {
        cout << x % k;
        ++len;
    }
}

int main() {
    cout << "����x,y,k,���x/y��k�����µ�С����ʽ(�����ߵ���ѭ����,��С��ĳλ����9,�ͻ��������)" << endl;
    cin >> x >> y >> k;
    if (x < y) {
        cout << '0';
        len = 1;
    }
    else {
        putnum(x / y);
    }
    x = (x % y) * k;
    if (x == 0) {
        return 0;
    }
    cout << '.', ++len;
    while (!occur[x]) {
        occur[x] = len;
        if (x / y >= 10) {
            cout << '(' << x / y % k << ')';
            len += cnt(x / y % k) + 2;
        }
        else {
            cout << x / y;
            ++len;
        }
        x = (x % y) * k;
    }
    cout << endl;
    for (ull j = 1; j <= len; j++) {
        cout << (j <= occur[x] ? ' ' : '^');
    }
    cout << endl;
    return 0;
}
