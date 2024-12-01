#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <bits/stdc++.h>

using namespace std;


int main(int argc, char* argv[]) {

    int N = 1000;
    int left[N];
    int right[N];

    // compute runtime
    auto start = chrono::high_resolution_clock::now();

    // get input stream
    string line;
    string who = argv[1]; // who is either l or j
    string fname = "input-"s + who + ".txt"s;
    cout << fname << '\n';
    ifstream input(fname);
    
    int i = 0;
    if (input.is_open())
    {
        while (getline (input,line))
        {
            string delimiter = "   ";
            right[i] = stoi(line.substr(0, line.find(delimiter)));
            left[i] = (stoi(line.substr(line.find(delimiter) + 1, line.length())));
            i++;
        }
        
        input.close();
    }
    else { 
        cout << "Unable to open file";
        return 1;
    }

    assert(i == N);

    // sort the arrays
    sort(left, left + N);
    sort(right, right + N);
    
    // calculate the result
    int result = 0;
    for (int i = 0; i < N; i++)
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

    for (int i = 0; i < N; i++)
    {
        int count = 0;
        for (int j = 0; j < N; j++)
        {
            count += (left[i] == right[j]);
        }

        result += left[i] * count;
    }

    // compute runtime
    end = chrono::high_resolution_clock::now();
    elapsed = end - start;

    cout << "Result: " << result << '\n';
    cout << "Elapsed time: " << elapsed.count() << " s\n";

    return 0;
}