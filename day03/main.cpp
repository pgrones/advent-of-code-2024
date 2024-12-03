#include "../lib/lib.h"
#include <regex>

using namespace std;

int calculateResult(string const memory)
{
    regex const pattern{R"~(mul\(\d{1,3},\d{1,3}\))~"};

    auto mul_begin = sregex_iterator(memory.begin(), memory.end(), pattern);
    auto mul_end = sregex_iterator();

    int result = 0;

    for (sregex_iterator i = mul_begin; i != mul_end; ++i)
    {
        smatch match = *i;
        string match_str = match.str();
        string numbers = match_str.substr(4, match_str.length() - 5);

        int first = stoi(numbers.substr(0, numbers.find(",")));
        int last = stoi(numbers.substr(numbers.find(",") + 1));

        result += first * last;
    }

    return result;
}

int main(int argc, char *argv[])
{
    timer timer;
    string memory = "";

    read_file(argv[1], [&memory](const string &line, const int i) -> void
              { memory += line; });

    timer.start();

    int result = calculateResult(memory);

    timer.end();

    cout << "Result: " << result << '\n';

    // PART II

    regex const disabledPattern{R"~(don't\(\).*?(do\(\)|$))~"};
    result = 0;

    timer.start();

    memory += "do()";
    memory = regex_replace(memory, disabledPattern, "");

    result = calculateResult(memory);

    timer.end();

    cout << "Result: " << result << '\n';

    return 0;
}
