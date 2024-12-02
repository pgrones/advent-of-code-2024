#include <algorithm>
#include <iostream>
#include <string>
#include "../lib/lib.h"

using namespace std;

void main(int __argc, char* argv[])
{
    timer timer;
    constexpr int n = 1000;
    int left[n];
    int right[n];

    timer.start();

    // read input
    const string delimiter = "   ";
    read_file(argv[1], [&delimiter, &right, &left](const string& line, const int i) -> void
    {
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
}
