#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <exception>
#include <iomanip>
#include <sstream>

class FrequencyRatio
{
    char delim = ';';
    std::string path;
    std::vector<double> first_frequency;
    std::vector<double> second_frequency;
    std::vector<double> times;
    std::vector<double> first_derivatives;
    std::vector<double> second_derivatives;

    void SplitString(std::string line, bool with_time=false)
    {
        auto first_delim_index = line.find_first_of(delim);
        auto second_delim_index = line.find_last_of(delim);
        double first = std::stod(line.substr(0, first_delim_index));
        double second = std::stod(line.substr(first_delim_index + 1, second_delim_index));
        first_frequency.push_back(first);
        second_frequency.push_back(second);
        if (with_time)
        {
            double time = std::stod(line.substr(second_delim_index + 1, line.size() - second_delim_index));
            times.push_back(time);
        }
    }
public:
    FrequencyRatio(std::string path)
    {
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
    void FindDerivatives(bool with_time=false)
    {
        for (long i = 1; i < first_frequency.size(); i++)
        {
            first_derivatives.push_back(
                (first_frequency[i] - first_frequency[i - 1])/
                ( with_time ? times[i] - times[i - 1] : 1.)
            );
            second_derivatives.push_back(
                (second_frequency[i] - second_frequency[i - 1]) /
                (with_time ? times[i] - times[i - 1] : 1.)
            );
        }

        for (auto x : first_derivatives)
        {
            std::cout << x << std::endl;
        }
    }
};

int main()
{
    std::string path = "C:/Users/VEX1T/Documents/test1.csv";
    FrequencyRatio test1(path);
    test1.FindDerivatives();
}

