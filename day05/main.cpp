#include "../lib/lib.h"
#include <set>
#include <algorithm>

using namespace std;

int main(int argc, char const *argv[]) {
    lib::timer timer;
    vector<tuple<int, int>> rules;
    vector<vector<int>> updates;

    bool isRules = true;
    lib::read_file(argv[1], [&rules, &updates, &isRules](const string line, const int i)-> void{
        if(line == ""){
            isRules = false;
            return;
        }

        if(isRules){
            auto rule = lib::split(line, "|");

            rules.push_back(make_tuple(stoi(rule[0]), stoi(rule[1])));

            return;
        }

        updates.push_back(lib::map<string, int>(lib::split(line, ","), [](const string element, const int i){ return stoi(element); }));
    });

    set<int> allNumbers;
    int x, y;

    timer.start();

    for (auto ruleTuple : rules)
    {
        tie(x, y) = ruleTuple;
        allNumbers.insert(x);
        allNumbers.insert(y);
    }

    // put all numbers from set into array
    vector<int> allNumbersArray;
    allNumbersArray.reserve(allNumbers.size());
    copy (allNumbers.begin(), allNumbers.end(), std::back_inserter(allNumbersArray));
    
    int N = allNumbersArray.size();

    lib::print_collection(allNumbersArray);


    // swap sort
    bool sorted = false;
    size_t i, j;
    int max_iter = 100;
    size_t iter = 0;
    while (!sorted && iter < max_iter) { //) { // && iter < max_iter) {
        sorted = true;
        // go over all rules
        for (auto ruleTuple : rules) {
            tie(x,y) = ruleTuple;
            
            // find indices
            for (i = 0; i < N; i++) {
                if (allNumbersArray[i] == x) break;
            }
            
            for (j = 0; j < N; j++) {
                if (allNumbersArray[j] == y) break;
            }
            
            if (i > j) {
                // swap i and j
                allNumbersArray[j] = x;
                allNumbersArray[i] = y;
                sorted = false;
            }
        }

        iter++;
        cout << "iter: " << iter << "\n";
        lib::print_collection(allNumbersArray);

    }

    lib::print_collection(allNumbersArray);

    timer.stop();

    cout << "Result: " << '\n';

    return 0;
}