#include "../commands.hpp"
#include "../app.hpp"
#include <dpp/dpp.h>
#include <string>

auto blep_command(app& bot) -> void {
    auto& blep = bot.add_command("blep", "Responds with animal chosen", [](const dpp::slashcommand_t& event) -> void {
        auto animal = std::get<std::string>(event.get_parameter("animal"));

        event.reply(std::string{"You chose "} + animal);
    });

    blep.add_option(dpp::command_option{dpp::co_string, "animal", "Chosen animal", true});
}
