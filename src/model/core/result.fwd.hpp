#pragma once

namespace tf {

    struct Error;

    template<typename TOk, typename TErr = Error>
    struct Result;

    template<typename TErr>
    struct Result<void, TErr>;

}