#include <iostream>
#include <chrono>
using namespace std;
using namespace std::chrono;

int rec_fibno(int n){
    if(n == 0)
        return 0;
    if(n == 1)
        return 1;
    return rec_fibno(n - 1) + rec_fibno(n - 2);
}

int itr_fibno(int n){
    int a = 0, b = 1, c;
    if(n == 0)
        return a;
    for(int i = 1; i < n; i++){
        c = a + b;
        a = b;
        b = c;
    }
    return b;
}

int main() {
    int n;
    while(true) {
        cout << "Enter the number which you want to find Fibonacci for (or -1 to exit): ";
        cin >> n;
        if(n == -1)
            break;

        auto start = high_resolution_clock::now();
        int recResult = rec_fibno(n);
        auto end = high_resolution_clock::now();
        long long recTime = duration_cast<nanoseconds>(end - start).count();

        start = high_resolution_clock::now();
        int itrResult = itr_fibno(n);
        end = high_resolution_clock::now();
        long long itrTime = duration_cast<nanoseconds>(end - start).count();

        cout << "Recursive Fibonacci of " << n << ": " << recResult << "  (Time: " << recTime << " ns)" << endl;
        cout << "Iterative Fibonacci of " << n << ": " << itrResult << "  (Time: " << itrTime << " ns)" << endl;
    }
    return 0;
}

/*
Enter the number which you want to find Fibonacci for (or -1 to exit): 5
Recursive Fibonacci of 5: 5  (Time: 400 ns)
Iterative Fibonacci of 5: 5  (Time: 120 ns)
Enter the number which you want to find Fibonacci for (or -1 to exit): 10
Recursive Fibonacci of 10: 55  (Time: 1192 ns)
Iterative Fibonacci of 10: 55  (Time: 120 ns)
Enter the number which you want to find Fibonacci for (or -1 to exit): 25
Recursive Fibonacci of 25: 75025  (Time: 794522 ns)
Iterative Fibonacci of 25: 75025  (Time: 150 ns)
Enter the number which you want to find Fibonacci for (or -1 to exit): 50
Recursive Fibonacci of 50: -298632863  (Time: 51501606262 ns)
Iterative Fibonacci of 50: -298632863  (Time: 100 ns)
Enter the number which you want to find Fibonacci for (or -1 to exit): -1
*/