#ifndef UTILS_H
#define UTILS_H

#include <map>
#include <random>
#include <string>


template< typename T >
T RollChances(
        const std::map<T, signed>& vars,
        std::default_random_engine& generator)
{
    T result = vars.cbegin()->first;

    signed totalv = 0;
    for (const auto& item: vars)
    {
        totalv += item.second;
    }

    std::uniform_int_distribution<signed>  rrr(1, totalv);
    signed x = rrr(generator);

    signed cx = 0;
    for (const auto& item: vars)
    {
        if ((x > cx) && (x <= (cx + item.second)))
        {
            result = item.first;
            break;
        }

        cx +=  item.second;
    }

    return result;
}

#endif // UTILS_H
