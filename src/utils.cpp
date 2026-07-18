#include <sstream>
#include <iomanip>
#include <string>
#include <stdint.h>
#include "../include/utils.h"

using std::string;
using std::ostringstream;
using std::fixed;
using std::setprecision;
using std::to_string;

string SizeFormatter::format_Bytes(uint64_t bytes)
{
    const char *size[] = {"B", "KB", "MB", "GB", "TB"};
    double len = static_cast<double>(bytes);
    int order = 0;

    while(len >= 1024 && order < 4)
    {
        len /= 1024.0;
        ++order;
    }

    ostringstream oss;
    oss << fixed << setprecision(2);
    oss << len <<" "<< size[order];

    return oss.str();
}

uint64_t Math::intPow(uint64_t base, uint64_t power)
{
    uint64_t result = 1;
    
    for(uint64_t i = 0; i < power; i++)
    {
        result = result * base;
    }

    return result;
}

uint64_t Math::intPow_Ologn(uint64_t base, uint64_t power)
{
    uint64_t result = 1;

    while (power)
    {
        if(power % 2 == 1) result *= base;
        base *= base;
        power /= 2;
    }

    return result;
}

string Time::seconds_converter(uint64_t totalSeconds)
{
    uint64_t unitSeconds[] = {
        365ULL * 24 * 60 * 60,  // year
        30ULL  * 24 * 60 * 60,  // month
        7ULL   * 24 * 60 * 60,  // week
        24ULL  * 60 * 60,       // day
        60ULL  * 60,            // hour
        60ULL,                  // minute
        1ULL                    // second
    };

    string unitNames[] = {
        "year", "month", "week", "day", 
        "hour", "minute", "second"
    };

    string str = "";
    uint64_t count;

    for(int i = 0; i < 7; i++)
    {
        count = totalSeconds / unitSeconds[i] ;
        totalSeconds = totalSeconds % unitSeconds[i];

        if(count > 0)
        {
            str += to_string(count) + "" + unitNames[i][0]; 

            if(count > 1)
            {
                // str += "s";
            }
            str += " ";
        }
    }

    if(str.empty())
    {
        str = "0 seconds";
    }

    if (!str.empty() && str.back() == ' ') {
        str.pop_back();
    }
    
    return str;
}

string Time::format_miliseconds(uint64_t totalMilliseconds)
{
    uint64_t unitMs[] = {
        24ULL * 60 * 60 * 1000,  // day
        60ULL * 60 * 1000,       // hour
        60ULL * 1000,            // minute
        1000ULL,                 // second
        1ULL                     // millisecond
    };

    string unitNames[] = {
        "d", "h", "m", "s", "ms"
    };

    string result = "";
    uint64_t count;

    for (size_t i = 0; i < 5; ++i)
    {
        count = totalMilliseconds / unitMs[i];
        totalMilliseconds %= unitMs[i];

        if (count > 0)
        {
            result += to_string(count) + unitNames[i] + " ";
        }
    }

    if (result.empty())
        result = "0 ms";

    // Remove trailing space
    if (!result.empty() && result.back() == ' ')
        result.pop_back();

    return result;
}
