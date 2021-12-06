#include "logger.h"
#include <iostream>

namespace snake {

void Logger::logProgress(Entry entry) {
    stream << entry.steps << "\t" << entry.apples << "\t" << entry.length
           << "\t" << entry.searchTimeMs << "\n";
    stream.flush();
}

} // namespace snake
