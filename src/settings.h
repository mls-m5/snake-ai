#pragma once

#include <iostream>
#include <string_view>
#include <vector>

struct Settings {
    std::string_view helpStr = R"_(
snake-ai

arguments:

--show-path        show the selected path
--show-search      show the full search tree
--delay            delay between frames in ms (0 for as fast as possible)
--help             show this help

)_";

    bool shouldShowSearch = false;
    bool shouldShowPath = false;

    int msDelay = 100;

    Settings(int argc, char **argv) {
        auto args = std::vector<std::string_view>{argv + 1, argv + argc};

        for (size_t i = 0; i < args.size(); ++i) {
            auto arg = args.at(i);

            if (arg == "--show-search") {
                shouldShowSearch = true;
            }
            else if (arg == "--show-path") {
                shouldShowPath = true;
            }
            else if (arg == "--delay") {
                ++i;
                msDelay = std::stoi(args.at(i).data());
            }
            else if (arg == "--help") {
                std::cout << helpStr << std::endl;
                std::exit(0);
            }
        }
    }
};
