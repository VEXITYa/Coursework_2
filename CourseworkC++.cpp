#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <exception>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <algorithm>

class FrequencyRatio
{
    char delim = ' ';
    std::string path;
    std::vector<long double> first_frequency;
    std::vector<long double> second_frequency;
    std::vector<long double> times;
    std::vector<long double> first_derivatives;
    std::vector<long double> second_derivatives;
    std::vector<long double> first_phases;
    std::vector<long double> second_phases;
    bool with_time = false;

    void SplitString(std::string line)
    {
        auto first_delim_index = line.find_first_of(delim);
        auto second_delim_index = line.find_last_of(delim);
        long double time = std::stod(line.substr(0, first_delim_index));
        long double first = std::stod(line.substr(first_delim_index + 1, second_delim_index));
        times.push_back(time);
        first_frequency.push_back(first);
        if (with_time)
        {
            long double second = std::stod(
                line.substr(second_delim_index + 1, line.size() - second_delim_index)
            );
            second_frequency.push_back(second);
        }
    }

    void Output(std::vector<long double> a, std::vector<long double> b, std::string output_path, int howMuchStrings=-1)
    {
        std::ofstream file_out((output_path).c_str());
        for (std::size_t i = 0; i < (howMuchStrings > 0 ? (std::size_t)howMuchStrings : a.size()); i++)
        {
            file_out << std::fixed << std::setprecision(30) << a[i] << ' ' << b[i] << std::endl;
        }
        file_out.close();
    }
public:
    FrequencyRatio(std::string path, bool with_time=false)
    {
        this->with_time = with_time;
        std::ifstream file_in(path.c_str());
        if (file_in.bad())
        {
            throw std::exception("Does not exist");
        }

        this->path = path;
        if (file_in.is_open())
        {
            std::string line;
            while (std::getline(file_in, line))
            {
                SplitString(line);
            }
        }
        file_in.close();
    }
    void FindDerivatives()
    {
        for (long i = 1; i < first_frequency.size(); i++)
        {
            first_derivatives.push_back(
                (first_frequency[i] - first_frequency[i - 1]) /
                (with_time ? times[i] - times[i - 1] : (long double)1)
            );
            second_derivatives.push_back(
                (second_frequency[i] - second_frequency[i - 1]) /
                (with_time ? times[i] - times[i - 1] : (long double)1)
            );

        }
    }

    void CalculatePhases()
    {
        for (long i = 0; i < first_derivatives.size(); i++)
        {
            first_phases.push_back(atan2(first_derivatives[i], first_frequency[i + 1]));
            second_phases.push_back(atan2(second_derivatives[i], second_frequency[i + 1]));
        }
    }

    void PhasesOutput(std::string output_path, int howMuchStrings=-1)
    {
        Output(first_phases, second_phases, output_path, howMuchStrings);
    }

    void DerivativesOutput(std::string output_path, int howMuchStrings = -1)
    {
        Output(first_derivatives, second_derivatives, output_path, howMuchStrings);
    }

    void AllOutput(std::string output_path, int howMuchStrings = -1)
    {
        std::ofstream file_out((output_path).c_str());
        for (std::size_t i = 0; i < (howMuchStrings > 0 ? (std::size_t)howMuchStrings : first_derivatives.size()); i++)
        {
            file_out << std::fixed << std::setprecision(30) << 
                    times[i] << ' ' <<
                    first_frequency[i] << ' ' <<
                    second_frequency[i] << std::endl;
        }
        file_out.close();
    }
};

int main()
{
    std::string path = "C:/Users/VEX1T/Documents/data.txt";
    std::string output_path = "C:/Users/VEX1T/Documents/data_out.txt";
    FrequencyRatio test1(path, true);
    test1.FindDerivatives();
    test1.CalculatePhases();
    test1.PhasesOutput(output_path);
}

