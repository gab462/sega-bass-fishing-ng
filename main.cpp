#include <cstdlib>
#include <dpp/dpp.h>
#include "app.hpp"

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

    bot.add_command("ping", "Responds with pong", [](const dpp::slashcommand_t& event) -> void {
        event.reply("pong");
    });

    auto& blep = bot.add_command("blep", "Responds with animal chosen", [](const dpp::slashcommand_t& event) -> void {
        auto animal = std::get<std::string>(event.get_parameter("animal"));

        event.reply(std::string{"You chose "} + animal);
    });

    blep.add_option(dpp::command_option{dpp::co_string, "animal", "Chosen animal", true});

    bot.run();

    return 0;
}
