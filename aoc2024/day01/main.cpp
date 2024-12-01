#include <algorithm>
#include <chrono>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int __argc, char* argv[])
{
    constexpr int n = 1000;
    int left[n];
    int right[n];

    // compute runtime
    auto start = chrono::high_resolution_clock::now();

    // get input stream
    string who = argv[1]; // who is either l or j
    string file_name = "input-"s + who + ".txt"s;
    ifstream input("day01/" + file_name);

    if (input.is_open())
    {
        int i = 0;
        string line;
        
        while (getline(input, line))
        {
            string delimiter = "   ";
            right[i] = stoi(line.substr(0, line.find(delimiter)));
            left[i] = (stoi(line.substr(line.find(delimiter) + 1, line.length())));
            i++;
        }

        input.close();
    }
    else
    {
        cout << "Unable to open file";
        return 1;
    }
    
    // sort the arrays
    sort(left, left + n);
    sort(right, right + n);

    // calculate the result
    int result = 0;
    for (int i = 0; i < n; i++)
    {
        result += abs(left[i] - right[i]);
    }

    // compute runtime
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;

    cout << "Result: " << result << '\n';
    cout << "Elapsed time: " << elapsed.count() << " s\n";

    // PART II
    // compute runtime
    start = chrono::high_resolution_clock::now();

    // calculate the result
    result = 0;

    for (int i : left)
    {
        int count = 0;
        for (int j : right)
        {
            count += i == j;
        }

        result += i * count;
    }

    // compute runtime
    end = chrono::high_resolution_clock::now();
    elapsed = end - start;

    cout << "Result: " << result << '\n';
    cout << "Elapsed time: " << elapsed.count() << " s\n";

    return 0;
}
