#include <vector>
#include <dpp/dpp.h>
#include "app.hpp"
#include "commands.hpp"

auto main() -> int {
    auto token = std::getenv("TOKEN");
    auto guilds_str = std::getenv("GUILDS");

    std::vector<unsigned long long int> guilds; {
        std::stringstream ss{guilds_str};
        std::string str;
        while (std::getline(ss, str, ','))
            guilds.push_back(std::stoull(str));
    }

    app bot{token, std::move(guilds)};

    ping_command(bot);
    blep_command(bot);
    wheel_command(bot);

    bot.run();

    return 0;
}
