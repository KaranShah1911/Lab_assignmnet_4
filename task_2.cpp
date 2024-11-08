#include <bits/stdc++.h>
using namespace std;

long long bruteForceMultiply(long long x, long long y) 
{
    return x * y;
}

long long karatsuba(long long x, long long y) 
{
    if (x < 10 || y < 10) 
    {
        return x * y;
    }

    int n = max(to_string(x).length(), to_string(y).length());
    int m = n / 2;

    long long highX = x / pow(10, m);
    long long lowX = x % static_cast<long long>(pow(10, m));
    long long highY = y / pow(10, m);
    long long lowY = y % static_cast<long long>(pow(10, m));

    long long z0 = karatsuba(lowX, lowY);                     
    long long z1 = karatsuba((lowX + highX), (lowY + highY)); 
    long long z2 = karatsuba(highX, highY);  

    return (z2 * pow(10, 2 * m)) + ((z1 - z2 - z0) * pow(10, m)) + z0;
}

void runTestCases() {
    vector<pair<long long, long long>> testCases = 
    {
        {12, 34},
        {123, 456},
        {789, 123},
        {12345, 67890},
        {987654321, 123456789},
        {-12, 34},
        {123, -456},
        {-789, -123},
        {-12345, 67890},
        {-987654321, -123456789}
    };

    cout << "Brute Force Multiplication Results:" << endl;
    for (auto& test : testCases) 
    {
        cout << test.first << " * " << test.second << " = " << bruteForceMultiply(test.first, test.second) << endl;
    }

    cout << "\nKaratsuba Multiplication Results:" << endl;
    for (auto& test : testCases) 
    {
        cout << test.first << " * " << test.second << " = " << karatsuba(test.first, test.second) << endl;
    }
}

int main() 
{
    runTestCases();
    return 0;
}
