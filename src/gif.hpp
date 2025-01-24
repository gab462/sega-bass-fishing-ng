#ifndef INCLUDE_GIF_HPP
#define INCLUDE_GIF_HPP

#include <string_view>
#include <cstdlib>
#include <msf_gif.h>

struct Gif {
    MsfGifState state;
    MsfGifResult result;
    int dur_cs;
    int bit_depth;
    int pitch_bytes;

    Gif(std::size_t width, std::size_t height): state{}, dur_cs{4}, bit_depth{16}, pitch_bytes{static_cast<int>(width) * 4} {
        msf_gif_begin(&state, width, height);
    }

    auto add_frame(std::uint32_t *data) -> void {
        msf_gif_frame(&state, reinterpret_cast<std::uint8_t*>(data), dur_cs, bit_depth, pitch_bytes);
    }

    auto end() -> void {
        result = msf_gif_end(&state);
    }

    ~Gif() {
        msf_gif_free(result);
    }
};

#endif
