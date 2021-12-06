#pragma once

#include <iostream>
#include <string_view>
#include <vector>

namespace snake {

struct Settings {
    std::string_view helpStr = R"_(
snake-ai

arguments:

--no-gui           disable graphics (also disables delay)
--show-path        show the selected path
--show-search      show the full search tree
--delay            delay between frames in ms (0 for as fast as possible)
--help             show this help
--startlen         start length of the snake

)_";

    bool shouldShowSearch = false;
    bool shouldShowPath = false;
    bool hideGui = false;

    int msDelay = 100;
    int startLen = 10;

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
            else if (arg == "--no-gui") {
                hideGui = true;
                msDelay = 0;
            }
            else if (arg == "--startlen") {
                ++i;
                startLen = std::stoi(args.at(i).data());
            }
            else if (arg == "--help") {
                std::cout << helpStr << std::endl;
                std::exit(0);
            }
        }
    }
};

} // namespace snake
