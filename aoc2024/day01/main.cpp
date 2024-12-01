#include <algorithm>
#include <iostream>
#include <string>
#include "../lib/lib.h"

using namespace std;

int main(int __argc, char* argv[])
{
    timer timer;
    constexpr int n = 1000;
    int left[n];
    int right[n];
    
    timer.start();

    // read input
    const auto who = argv[1]; // who is either l or j
    read_file(who, [&right, &left](const string& line, const int i) -> void
    {
        const string delimiter = "   ";
        right[i] = stoi(line.substr(0, line.find(delimiter)));
        left[i] = stoi(line.substr(line.find(delimiter) + 1, line.length()));
    });

    // sort the arrays
    sort(left, left + n);
    sort(right, right + n);

    // calculate the result
    int result = 0;
    for (int i = 0; i < n; i++)
    {
        result += abs(left[i] - right[i]);
    }
    
    timer.end();
    cout << "Result: " << result << '\n';

    // PART II
    timer.start();
    
    result = 0;
    for (const int i : left)
    {
        int count = 0;
        for (const int j : right)
        {
            count += i == j;
        }

        result += i * count;
    }
    
    timer.end();
    cout << "Result: " << result << '\n';

    return 0;
}
