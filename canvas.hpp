#include <cstdlib>
#define OLIVECDEF
#include "olive.c"

struct Point {
    float x, y;

    auto rotate(float angle, Point center) -> void {
        float dx = x - center.x, dy = y - center.y;
        float mag = std::sqrtf(dx * dx + dy * dy);
        float dir = std::atan2f(dy, dx) + angle;

        x = std::cosf(dir) * mag + center.x;
        y = std::sinf(dir) * mag + center.y;
    }

    auto operator+(Point other) -> Point {
        return Point{x + other.x, y + other.y};
    }

    auto operator-(Point other) -> Point {
        return Point{x - other.x, y - other.y};
    }

    auto operator/(int n) -> Point {
        return Point{x / n, y / n};
    }
};

template <std::size_t W, std::size_t H>
struct Canvas {
    std::array<std::uint32_t, W * H> pixels;
    Olivec_Canvas oc;
    std::size_t text_size;

    Canvas(): pixels{} {
        oc = olivec_canvas(pixels.data(), W, H, W);
    }

    auto font_size() -> Point {
        return Point{
            olivec_default_font.height * static_cast<float>(text_size),
            olivec_default_font.width * static_cast<float>(text_size)
        };
    }

    auto fill(std::uint32_t color) -> void {
        olivec_fill(oc, color);
    }

    auto draw_triangle(Point a, Point b, Point c, std::uint32_t color) -> void {
        olivec_triangle(oc, a.x, a.y, b.x, b.y, c.x, c.y, color);
    }

    auto draw_text(const char *text, Point pos, std::uint32_t color) -> void {
        olivec_text(oc, text, pos.x, pos.y, olivec_default_font, text_size, color);
    }

    auto draw_rect(Point pos, Point size, std::uint32_t color) -> void {
        olivec_rect(oc, pos.x, pos.y, size.x, size.y, color);
    }
};
