#pragma once

#include <string_view>
#include <cstdint>

#include "core/result.fwd.hpp"

namespace tf {

    class Version {
    public:

        Version(uint32_t major, uint32_t minor, uint32_t patch) 
            :   m_Major(major),
                m_Minor(minor),
                m_Patch(patch)
        { }

        [[nodiscard]] Result<Version> parse_from_string(std::string_view string_to_parse) noexcept;

        static const Version FIRST;

    private:

        uint32_t m_Major;

        uint32_t m_Minor;

        uint32_t m_Patch;
    };

}