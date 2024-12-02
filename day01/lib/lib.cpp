#include "lib.h"

using namespace std;

void read_file(const char* who, const function<void(string, int)>& lambda)
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


void ::timer::start()
{
    start_ = chrono::high_resolution_clock::now();
}

void ::timer::end() const
{
    const chrono::duration<double> elapsed = chrono::high_resolution_clock::now() - start_;
    cout << "Elapsed time: " << elapsed.count() << " s\n";
}
