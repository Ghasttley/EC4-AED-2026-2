#include<bits/stdc++.h>
using namespace::std;

int maxima(long long n, vector<long long> &N) {
    int res = 0;
    while (N[res] <= n) ++res;
    return res - 1;
}

int minima(long long n) {
    int level = 0;
    long long p = 1;
    while (p < n) {
        n -= p;
        p <<= 1;
        ++level;
    }
    return level;
}

int main() {
    cin.tie(0) -> sync_with_stdio(false);
    vector<long long> N = {1, 2};
    while (N.back() <= 1000000000000000000LL) {
        N.emplace_back(N[N.size() - 2] + N[N.size() - 1] + 1);
    }
    int t;
    cin >> t;
    while (t--) {
        long long n;
        cin >> n;
        cout << minima(n) << " " << maxima(n, N) << '\n';
    }
    return 0;
}