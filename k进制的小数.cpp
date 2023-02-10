//encoding: GB2312
#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
ll x, y, k, len;
map<ll, ll> occur;

ll quot(ll n, ll k) {
    if (k > 0) {
        return (n > 0 ? n / k : (n-k+1) / k);
    }
    else {
        return (n > 0 ? n / k : (n+k+1) / k);
    }
}

ll mod(ll n, ll k) {
    if (k > 0) {
        ll q = (n > 0 ? n / k : (n-k+1) / k);
        return n - q * k;
    }
    else {
        ll q = (n > 0 ? n / k : (n+k+1) / k);
        return n - q * k;
    }
}

ll cnt(ll x) {
    ll res = 0;
    while (x > 0) {
        ++res;
        x /= 10;
    }
    return res;
}

void putnum(ll x) {
    if (x == 0) return;
    putnum(quot(x, k));
    if (mod(x, k) >= 10) {
        cout << '(' << mod(x, k) << ')';
        len += cnt(mod(x, k)) + 2;
    }
    else {
        cout << mod(x, k);
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
    cout << '.';
    ++len;
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
    if (x == 0) return 0;
    cout << endl;
    for (ll j = 1; j <= len; j++) {
        cout << (j <= occur[x] ? ' ' : '^');
    }
    cout << endl;
    return 0;
}
