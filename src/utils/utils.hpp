
// builtin
#include <chrono>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>



class TimeMeasurer
{
public:

    enum TimeUnit
    {
        MICROSECOND,
        MILLISECOND,
        SECOND
    };

private:

    // TODO: tornar static
    const std::unordered_map<TimeUnit, std::string> unit_strings{
        {MICROSECOND, "us"},
        {MILLISECOND, "ms"},
        {SECOND, "s"},
    };

    std::string message;
    TimeUnit time_unit;
    std::chrono::system_clock::time_point start_time;

public:

    TimeMeasurer(std::string&& message = "", const TimeUnit time_unit = TimeUnit::MICROSECOND):
        message{message}, time_unit{time_unit}, start_time{std::chrono::high_resolution_clock::now()}
    {
    }

public:

    [[nodiscard]] auto get_time() const
    {
        const auto current_time = std::chrono::high_resolution_clock::now();
        const auto elapsed_time = current_time - this->start_time;
        switch (this->time_unit)
        {
            case MICROSECOND:
                return std::chrono::duration_cast<std::chrono::microseconds>(elapsed_time).count();
            case MILLISECOND:
                return std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_time).count();
            case SECOND:
                return std::chrono::duration_cast<std::chrono::seconds>(elapsed_time).count();
            default:
                return std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_time).count();
        }
    }

    void restart_time()
    {
        this->start_time = std::chrono::high_resolution_clock::now();
    }

    [[nodiscard]] auto get_decimal_time() const
    {
        const auto current_time = std::chrono::high_resolution_clock::now();
        const auto elapsed_time = current_time - this->start_time;
        return std::chrono::duration_cast<std::chrono::microseconds>(elapsed_time).count();
        switch (this->time_unit)
        {
            case MICROSECOND:
                return std::chrono::duration_cast<std::chrono::microseconds>(elapsed_time).count();
            case MILLISECOND:
                return std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_time).count();
            case SECOND:
                return std::chrono::duration_cast<std::chrono::seconds>(elapsed_time).count();
            default:
                return std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_time).count();
        }
    }

    void print_time() const
    {
        const auto elapsed_time = this->get_time();
        std::cout << message << " in " << elapsed_time << this->unit_strings.at(this->time_unit) << std::endl;
    }
};

template <class ElementT>
inline std::ostream& operator<<(std::ostream& ostream, const std::vector<ElementT>& vector)
{
    ostream << "[ ";
    for (size_t i = 0; i < vector.size() - 1; i++)
    {
        ostream << vector[i] << ", ";
    }
    ostream << vector[vector.size() - 1] << " ]\n";

    return ostream;
}