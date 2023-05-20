#include <bits/stdc++.h>
using namespace std;

const int N = 1000010;

int nTests;
int a[N];

namespace Lumuto {

int part(int, int);
void quicksort(int l, int r) {
    if (l >= r) return;
    int p = part(l, r);
    quicksort(l, p - 1);
    quicksort(p + 1, r);
}

int part(int l, int r) {
    int pivot = a[r];
    int i = l;
    for (int j = l; j < r; j++) {
        if (a[j] <= pivot) {
            swap(a[i++], a[j]);
        }
    }
    swap(a[i], a[r]);
    return i;
}

} // namespace Lumuto


namespace Hoare {

int part(int, int);
void quicksort(int l, int r) {
    if (l >= r) return;
    int p = part(l, r);
    quicksort(l, p);
    quicksort(p + 1, r);
}

int part(int l, int r) {
    int pivot = a[l];
    int i = l - 1, j = r + 1;
    while (true) {
        do ++i; while (a[i] < pivot);
        do --j; while (a[j] > pivot);
        if (i >= j) return j;
        swap(a[i], a[j]);
    }
}

} // namespace Hoare


namespace threeway {

void part(int, int, int&, int&);
void quicksort(int l, int r) {
    if (l >= r) return;
    int left, right;
    part(l, r, left, right);
    quicksort(l, left-1);
    quicksort(right+1, r);
}

void part(int l, int r, int& left, int& right) {
    int pivot = a[l];
    int i = l;
    int j = l;
    int k = r;
    // [l,i) <    [i,j) =     [j,k] not sorted   (k,r] >
    while (j <= k) {
        if (a[j] < pivot) {
            swap(a[j++], a[i++]);
        }
        else if (a[j] > pivot) {
            swap(a[j], a[k--]);
        }
        else {
            j++;
        }
    }
    left = i;
    right = j-1;
}

} // namespace threeway


double test(int n, void (*algo)(int, int)) {
    clock_t tot = 0;
    for (int iT = 1; iT <= nTests; ++iT) {
        for (int i = 1; i <= n; i++) a[i] = rand() % 100;
        clock_t start = clock();
        algo(1, n);
        clock_t end = clock();
        for (int i = 1; i < n; i++) {
            assert(a[i] <= a[i + 1]);
        }
        tot += end - start;
    }
    double msecs = tot * 1000.0 / CLOCKS_PER_SEC;
    msecs /= nTests;
    return msecs;
}

int main() {
    ios::sync_with_stdio(false);
    int n;
    cin >> n >> nTests;
    if (n >= N) {
        cout << "too big" << endl;
        return -1;
    }
    double msecs;
    msecs = test(n, Lumuto::quicksort);
    cout << msecs << "ms" << endl;
    msecs = test(n, Hoare::quicksort);
    cout << msecs << "ms" << endl;
    msecs = test(n, threeway::quicksort);
    cout << msecs << "ms" << endl;
    return 0;
}
