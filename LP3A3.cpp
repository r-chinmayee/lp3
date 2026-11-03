#include <bits/stdc++.h>
using namespace std;

struct Item { int w, v; };

int main() {
    int n, W;
    cout << "Enter number of items and capacity: ";
    cin >> n >> W;
    vector<Item> a(n);
    cout << "Enter weight and value of each item:\n";
    for (auto& it : a) cin >> it.w >> it.v;

    vector<vector<int>> dp(n + 1, vector<int>(W + 1, 0));
    for (int i = 1; i <= n; i++) {
        for (int c = 0; c <= W; c++) {
            dp[i][c] = dp[i - 1][c];
            if (a[i - 1].w <= c)      
                dp[i][c] = max(dp[i][c], dp[i - 1][c - a[i - 1].w] + a[i - 1].v);
        }
    }
    cout << "Max value: " << dp[n][W] << "\nSelected items (weight, value):\n";

    for (int i = n, c = W; i > 0; i--)
        if (dp[i][c] != dp[i - 1][c]) {
            cout << "Item " << i << ": (" << a[i - 1].w << ", " << a[i - 1].v << ")\n";
            c -= a[i - 1].w;
        }
}
