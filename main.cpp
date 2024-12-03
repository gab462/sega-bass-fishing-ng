#include <vector>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <cstdint>
#include <dpp/dpp.h>
#include "app.hpp"
#include "gif.hpp"
#include "canvas.hpp"

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

    bot.add_command("wheel", "Spin the wheel", [](const dpp::slashcommand_t& event) -> void {
        static constexpr std::size_t width = 400;
        static constexpr std::size_t height = 400;

        Gif gif{width, height};
        Canvas<width, height> canvas;

        canvas.text_size = width / 100;

        std::srand(std::time(NULL));

        auto random_color = []() -> std::uint32_t {
            return (0xFF << (3 * 8))
                 | (((rand() % 0x7F) + 0x7F) << (2 * 8))
                 | (((rand() % 0x7F) + 0x7F) << 8)
                 | ((rand() % 0x7F) + 0x7F);
        };

        int sectors = 8;

        std::vector<std::uint32_t> colors;
        for (int i = 0; i < sectors; ++i) {
            colors.push_back(random_color());
        }

        float angle = 0.f;
        float sector_angle = (2.0 * M_PI) / sectors;
        float speed = .4f;
        float accel = -.008f;

        Point center{width/2.f, height/2.f};

        for (int i = 0; i < 100; ++i) {
            canvas.fill(0xFFFFFFFF);

            angle += speed;
            speed += accel;

            if (speed <= 0.f)
                speed = 0.f;

            for (int i = 0; i < sectors; ++i) {
                Point b{width, height/2.f};
                Point c{width, height/2.f};

                b.rotate(angle + sector_angle * i, center);
                c.rotate(angle + sector_angle * (i + 1), center);

                Point text_pos = b + (c - b) / 2.f - canvas.font_size() / 2.f;

                // Closer to center
                text_pos = text_pos - (text_pos - center) / 8;

                std::array<char, 2> text;
                text[0] = 'a' + i;
                text[1] = '\0';

                canvas.draw_triangle(center, b, c, colors[i]);
                canvas.draw_text(text.data(), text_pos, 0xFF000000);
                canvas.draw_rect(Point{0, height/2 - height/80}, Point{width/20, height/40}, 0xFF000000);
            }

            gif.add_frame(canvas.pixels.data());
        }

        gif.end();

        dpp::message msg(event.command.channel_id, "");
        msg.add_file("wheel.gif", std::string_view{static_cast<char*>(gif.result.data), gif.result.dataSize});

        dpp::embed embed;
        embed.set_image("attachment://wheel.gif");

        msg.add_embed(embed);

        event.reply(msg);
    });

    bot.run();

    return 0;
}
