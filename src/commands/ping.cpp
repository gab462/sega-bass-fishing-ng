#include "../commands.hpp"
#include "../app.hpp"
#include <dpp/dpp.h>

auto ping_command(app& bot) -> void {
    bot.add_command("ping", "Responds with pong", [](const dpp::slashcommand_t& event) -> void {
        event.reply("pong");
    });
}
