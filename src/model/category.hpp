#pragma once

#include <string>

#include "core/expect.hpp"

namespace tf {

    class Category {
    public:
        
        Category(std::string_view name) : m_Name(name) {
            tf::expect(name.size() > 0);
        }

    private:
        
        std::string m_Name;

    };

}