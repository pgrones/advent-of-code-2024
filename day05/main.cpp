#include "../lib/lib.h"
#include <set>
#include <algorithm>

using namespace std;

tuple<int, vector<vector<int>>> get_invalid_updates(vector<tuple<int, int>> &rules, vector<vector<int>> &updates){
    int result = 0;
    int x, y;
    vector<vector<int>> invalidUpdates;

    for(auto update : updates){
        bool isValid = true;

        for(size_t i = 0; i < update.size(); i++) {
            int page = update[i];

            for(auto rule : rules) {
                tie(x, y) = rule;

                if(x != page && y != page) continue;

                if(x == page) {
                    for(size_t j = 0; j < update.size(); j++) {
                        if(update[j] == y){
                            if(j < i) isValid = false;
                            break;
                        }
                    }
                }
            
                if(y == page) {
                    for(size_t j = 0; j < update.size(); j++) {
                        if(update[j] == x){
                            if(j > i) isValid = false;
                            break;
                        }
                    }
                }

                if(!isValid) break;
            }
        }

        if(isValid) {
            result += update[update.size() / 2];
        } else {
            invalidUpdates.push_back(update);
        }
    }

    return make_tuple(result, invalidUpdates);
}

int order_invalid_updates(vector<tuple<int, int>> &rules, vector<vector<int>> &invalidUpdates){
    int result = 0;

    for(auto update : invalidUpdates) {
        vector<int> validUpdate = update;

        sort(validUpdate.begin(), validUpdate.end(), [rules](int a, int b) {
            int x, y;

            for(auto rule : rules){
                tie(x, y) = rule;

                if(x == a && y == b){
                    return true;
                }

                if(x == b && y == a){
                    return false;
                }
            }

            return false;
        });
        
        result += validUpdate[validUpdate.size() / 2];
    }

    return result;
}

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

    int result;
    vector<vector<int>> invalidUpdates;

    timer.start();

    tie(result, invalidUpdates) = get_invalid_updates(rules, updates);

    timer.stop();

    cout << "Result: " << result << '\n';

    // PART II

    timer.start();

    result = order_invalid_updates(rules, invalidUpdates);

    timer.stop();

    cout << "Result: " << result << '\n';

    return 0;
}