#include "../commands.hpp"
#include "../app.hpp"
#include <dpp/dpp.h>

auto character_command(app& bot) -> void {
    auto& character = bot.add_command("character", "Random character from MAL", [](const dpp::slashcommand_t& event) -> void {
        event.reply("TODO");
    });
}
