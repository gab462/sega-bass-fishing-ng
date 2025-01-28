#ifndef INCLUDE_APP_HPP
#define INCLUDE_APP_HPP

#include <functional>
#include <map>
#include <print>
#include <string_view>
#include <utility>
#include <vector>
#include <dpp/dpp.h>

struct app {
    dpp::cluster bot;
    std::vector<dpp::slashcommand> configs;
    std::vector<unsigned long long int> guilds;
    std::map<std::string_view, std::function<void(const dpp::slashcommand_t&)>> callbacks;

    app(std::string token, std::vector<unsigned long long int> guild_ids): bot{token}, guilds{std::move(guild_ids)} {
        bot.on_log(dpp::utility::cout_logger());

        bot.on_slashcommand([this](const dpp::slashcommand_t& event) -> void {
            std::string name = event.command.get_command_name();

            if (this->callbacks.contains(name))
                this->callbacks[name](event);
            else
                std::println("Command {} requested in guild {} but not configured.", name, event.command.get_guild().name);
        });

        bot.on_ready([this](const dpp::ready_t&) -> void {
            if (dpp::run_once<struct register_bot_commands>()) {
                for (auto guild_id: this->guilds) {
                    std::println("Adding {} commands to guild {}...", this->configs.size(), guild_id);

                    // this->bot.guild_bulk_command_delete(guild_id);
                    this->bot.guild_bulk_command_create(this->configs, guild_id);
                }
            }

            std::println("Bot {} started successfully with {} commands!", bot.me.username, callbacks.size());
        });
    }

    auto add_command(std::string_view name, std::string_view description, std::function<void(const dpp::slashcommand_t&)> callback) -> dpp::slashcommand& {
        callbacks[name] = callback;
        return configs.emplace_back(std::string{name}, std::string{description}, bot.me.id);
    }

    auto run() -> void {
        bot.start(dpp::st_wait);
    }
};

#endif
