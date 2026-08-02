#include <iostream>
#include <set>
using namespace std;

// mergesort, but written in an iterative way
template <typename T>
void mergesort(T* a, size_t len, T* buf)
{
    for (size_t w = 1; w < len; w *= 2)
    {
        for (size_t i = 0; i + w < len; i += w+w)
        {
            // merge
            size_t j1 = i;
            size_t j2 = i + w;
            size_t p = 0;
            while (j1 < i + w && j2 < i + w+w && j2 < len)
                if (a[j1] < a[j2])
                    buf[p++] = a[j1++];
                else
                    buf[p++] = a[j2++];
            while (j1 < i + w)
                buf[p++] = a[j1++];
            while (j2 < i + w*w && j2 < len)
                buf[p++] = a[j2++];
            // copy back to the original array
            for (size_t j = 0; j < p; j++)
                a[i+j] = buf[j];
        }
    }
}

int main()
{
    constexpr size_t MAXLEN = 100;
    pair<int,int> a[MAXLEN], buf[MAXLEN];
    set<int> st;
    cout << "Enter array length: ";
    size_t len;
    cin >> len;
    if (len > MAXLEN)
    {
        cout << "length too big" << endl;
        return 1;
    }
    cout << "Enter array: ";
    for (int i = 0; i < len; i++)
    {
        int x;
        cin >> x;
        a[i].first = x;
        a[i].second = st.count(x);
        st.insert(x);
    }
    cout << "Before sort: ";
    for (int i = 0; i < len; i++)
    {
        if (i > 0) cout << " ";
        cout << "(" << a[i].first << "," << a[i].second << ")";
    }
    mergesort(a, len, buf);
    cout << endl << "After sort: ";
    for (int i = 0; i < len; i++)
    {
        if (i > 0) cout << " ";
        cout << "(" << a[i].first << "," << a[i].second << ")";
    }
    cout << endl;
    return 0;
}
