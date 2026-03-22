#pragma once 

#include <stdexcept>

namespace tf {

    class SerializationException : public std::runtime_error {
    public:

        SerializationException(std::string info)
            :	std::runtime_error(info)
        { 
            // TODO: Figure out how to rethrow (not sure how the scope of the nested exception works)
            // We make want to add more specific exceptions in the future
        }

    };

}
