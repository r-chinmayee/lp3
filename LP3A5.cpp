#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
using namespace std;

long long comparisons = 0;

int partition(vector<int>& a, int low, int high, bool randomized) {
    if (randomized)                                   // random pivot moved to end
        swap(a[low + rand() % (high - low + 1)], a[high]);
    int pivot = a[high], i = low - 1;
    for (int j = low; j < high; j++) {
        comparisons++;
        if (a[j] <= pivot) swap(a[++i], a[j]);
    }
    swap(a[i + 1], a[high]);
    return i + 1;
}

void quickSort(vector<int>& a, int low, int high, bool randomized) {
    if (low < high) {
        int p = partition(a, low, high, randomized);
        quickSort(a, low, p - 1, randomized);
        quickSort(a, p + 1, high, randomized);
    }
}

vector<int> analyze(vector<int> a, bool randomized) {
    comparisons = 0;
    auto start = chrono::high_resolution_clock::now();
    quickSort(a, 0, a.size() - 1, randomized);
    auto end = chrono::high_resolution_clock::now();
    cout << (randomized ? "Randomized    " : "Deterministic ")
         << "| Comparisons: " << comparisons
         << " | Time: " << chrono::duration<double, milli>(end - start).count() << " ms\n";
    return a;
}

void print(string label, const vector<int>& a) {
    cout << label;
    for (int x : a) cout << x << " ";
    cout << "\n";
}

int main() {
    srand(time(0));
    int n;
    cout << "Enter number of elements: ";
    cin >> n;

    vector<int> arr(n);
    for (int& x : arr) x = rand() % 100;

    print("Before sorting: ", arr);
    analyze(arr, false);
    vector<int> sorted = analyze(arr, true);
    print("After sorting:  ", sorted);
    return 0;
}