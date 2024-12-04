#include "lib.h"

using namespace std;

void lib::read_file(const char *who, const function<void(string, int)> &lambda)
{
    const string file_name = string("input-") + who + string(".txt");
    ifstream input(file_name);

    if (input.is_open())
    {
        int i = 0;
        string line;

        while (getline(input, line))
        {
            lambda(line, i++);
        }

        input.close();
    }
    else
    {
        throw runtime_error("Unable to open file");
    }
}

vector<string> lib::split(const string s, const string delimiter)
{
    vector<string> result;
    size_t from = 0, to = 0;

    while (string::npos != (to = s.find(delimiter, from)))
    {
        result.push_back(s.substr(from, to - from));
        from = to + delimiter.length();
    }

    result.push_back(s.substr(from, to));

    return result;
}

void lib::timer::start()
{
    start_ = chrono::high_resolution_clock::now();
}

void lib::timer::end() const
{
    const chrono::duration<double> elapsed = chrono::high_resolution_clock::now() - start_;
    cout << "Elapsed time: " << elapsed.count() << " s\n";
}
