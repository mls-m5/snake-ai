#pragma once
#include <iosfwd>

namespace snake {

struct Logger {
    struct Entry {
        int steps;
        int apples;
        int length;
        long searchTimeMs;
    };

    void logProgress(Entry);

    std::ostream &stream;
};

} // namespace snake
