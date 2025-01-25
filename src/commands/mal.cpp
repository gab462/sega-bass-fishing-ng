#include "../commands.hpp"
#include "../app.hpp"
#include <dpp/dpp.h>
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <httplib.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

auto character_command(app& bot) -> void {
    bot.add_command("character", "Random character from MAL", [](const dpp::slashcommand_t& event) -> void {
        httplib::SSLClient client{"api.jikan.moe"};

        json character_data;

        if (auto res = client.Get("/v4/random/characters")) {
            if (res->status == httplib::StatusCode::OK_200) {
                character_data = json::parse(res->body);
            } else {
                event.reply(std::format("API error: {}", res->status));
            }
        } else {
            event.reply(std::format("HTTP error: {}", static_cast<int>(res.error())));
        }

        auto character = character_data["data"];

        dpp::message msg(event.command.channel_id, "");

        dpp::embed embed;
        embed.set_url(character["url"].template get<std::string>());
        embed.set_title(character["name"].template get<std::string>());
        embed.set_image(character["images"]["webp"]["image_url"].template get<std::string>());
        embed.add_field("Favorites", std::format("{}", character["favorites"].template get<int>()));

        if (character.contains("about") && character["about"].is_string())
            embed.set_description(character["about"].template get<std::string>());

        msg.add_embed(embed);

        event.reply(msg);
    });
}
