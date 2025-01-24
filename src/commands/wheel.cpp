#include "../commands.hpp"
#include "../app.hpp"
#include <dpp/dpp.h>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <cstdint>
#include "../gif.hpp"
#include "../canvas.hpp"

auto wheel_command(app& bot) -> void {
    auto& wheel = bot.add_command("wheel", "Spin the wheel", [](const dpp::slashcommand_t& event) -> void {
        auto choices_str = std::get<std::string>(event.get_parameter("choices"));

        std::vector<std::string> choices; {
            std::stringstream ss{choices_str};
            std::string str;
            while (std::getline(ss, str, ','))
                choices.push_back(str);
        }

        if (choices.size() < 3) {
            event.reply("At least three choices are needed.");
            return;
        }

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

        int sectors = choices.size();

        std::vector<std::uint32_t> colors;
        for (int i = 0; i < sectors; ++i) {
            colors.push_back(random_color());
        }

        auto random_float = []() -> float {
            return static_cast<float>(rand()) / RAND_MAX;
        };

        float angle = random_float() * 2.0 * M_PI;
        float sector_angle = (2.0 * M_PI) / sectors;
        float speed = .5f + random_float() * .5f;
        float accel = -.005f - random_float() * .01f;

        Point center{width/2.f, height/2.f};

        for (int i = 0; i < 200; ++i) {
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
                canvas.draw_triangle(Point{width/2.f - width/20.f, 0.f}, Point{width/2.f + width/20.f, 0.f}, Point{width/2.f, height/20.f}, 0xFF000000);
                canvas.draw_text(text.data(), text_pos, 0xFF000000);
            }

            gif.add_frame(canvas.pixels.data());
        }

        gif.end();

        dpp::message msg(event.command.channel_id, "");
        msg.add_file("wheel.gif", std::string_view{static_cast<char*>(gif.result.data), gif.result.dataSize});

        dpp::embed embed;
        embed.set_image("attachment://wheel.gif");

        std::string description;

        for (std::size_t i = 0; i < choices.size(); ++i) {
            std::array<char, 2> text;
            text[0] = 'a' + i;
            text[1] = '\0';

            description.append(text.data());
            description.append(" - ");
            description.append(choices[i]);
            description.append("\n");
        }

        embed.set_description(description);

        msg.add_embed(embed);

        event.reply(msg);
    });

    wheel.add_option(dpp::command_option{dpp::co_string, "choices", "Comma-separated list of choices", true});
}
