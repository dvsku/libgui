#pragma once

#include <imgui/imgui.h>
#include <cstdint>
#include <vector>
#include <string>

namespace libgui {
    using font_id = std::string;

    struct font_range {
        ImWchar from;
        ImWchar to;
    };

    struct font_config {
        int    oversample_h         = 2;
        int    oversample_v         = 1;
        ImVec2 extra_spacing        = { 0.0f, 0.0f };
        ImVec2 offset               = { 0.0f, 0.0f };
        float  min_advance_x        = 0;
        float  max_advance_x        = FLT_MAX;
        bool   advance_x_match_size = false;
    };

    class font_source {
    public:
        uint8_t*                data     = nullptr;
        size_t                  size     = 0U;
        std::string             filename = "";
        std::vector<font_range> range    = {};
        font_config             config   = {};

    public:
        font_source() = delete;
        
        /*
            Create from stream
        */
        font_source(const void* data, size_t size, const std::vector<font_range>& range = {}, const font_config& config = {});

        /*
            Create from file
        */
        font_source(const std::string& ttf_file, const std::vector<font_range>& range = {}, const font_config& config = {});

    private:
        void set_range(const std::vector<font_range>& range);
    };
}
