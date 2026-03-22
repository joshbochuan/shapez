//
// Created by joshb on 2026/3/22.
//

#ifndef REPLACE_WITH_YOUR_PROJECT_NAME_BIGNUMSTR_HPP
#define REPLACE_WITH_YOUR_PROJECT_NAME_BIGNUMSTR_HPP
#include <string>

// thanks chatgpt
std::string BigNumStr(int x) {
    const char* suffixes[] = {"", "k", "m", "b", "t", "qd", "qt", "sx", "sp", "o", "n", "d"};
    int suffixIndex = 0;

    long long value = x;

    // Find appropriate suffix
    while (value >= 1000 && suffixIndex < 4) {
        value /= 1000;
        suffixIndex++;
    }

    // Base value (integer part)
    long long base = x;
    for (int i = 0; i < suffixIndex; i++) {
        base /= 1000;
    }

    // If base >= 10 → no decimal
    if (base >= 10) {
        return std::to_string(base) + suffixes[suffixIndex];
    }

    // Otherwise, get one decimal digit
    long long divisor = 1;
    for (int i = 0; i < suffixIndex; i++) {
        divisor *= 1000;
    }

    long long remainder = x % divisor;
    long long decimal = (remainder * 10) / divisor;

    // Avoid trailing ".0"
    if (decimal == 0) {
        return std::to_string(base) + suffixes[suffixIndex];
    }

    return std::to_string(base) + "." + std::to_string(decimal) + suffixes[suffixIndex];
}

#endif //REPLACE_WITH_YOUR_PROJECT_NAME_BIGNUMSTR_HPP