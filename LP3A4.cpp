#include <iostream>
#include <vector>
using namespace std;

int n;
vector<int> col; // col[r] = column of the queen in row r, -1 if the row is empty

bool safe(int r, int c) {
    for (int i = 0; i < n; i++)
        if (i != r && col[i] != -1 && (col[i] == c || abs(col[i] - c) == abs(i - r)))
            return false;
    return true;
}

bool solve(int r) {
    if (r == n) return true;
    if (col[r] != -1) return solve(r + 1); // row of the fixed first queen
    for (int c = 0; c < n; c++) {
        if (!safe(r, c)) continue;
        col[r] = c;
        if (solve(r + 1)) return true;
        col[r] = -1; // backtrack
    }
    return false;
}

int main() {
    int r0, c0;
    cout << "Enter n: ";
    cin >> n;
    cout << "Enter position (row col) of first queen (0-based): ";
    cin >> r0 >> c0;
    if (r0 < 0 || r0 >= n || c0 < 0 || c0 >= n) {
        cout << "Invalid position\n";
        return 1;
    }

    col.assign(n, -1);
    col[r0] = c0;
    if (!solve(0)) {
        cout << "No solution exists with this first queen position\n";
        return 0;
    }

    // n x 2 matrix: each line is the (row, column) of one queen
    for (int i = 0; i < n; i++) cout << i << " " << col[i] << "\n";
}
