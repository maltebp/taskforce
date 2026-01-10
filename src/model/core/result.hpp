#pragma once

#include <type_traits>
#include <variant>
#include <string>
#include <utility>
#include <iostream>

namespace tf {

    struct Error {
        Error(std::string&& info) : info(info) { }
        Error(const std::string& info) : info(info) { }
        std::string info;
    };

    template<typename TOk, typename TErr = Error>
    struct Result : std::variant<TOk, TErr> {

        Result(const TOk& ok) : std::variant<TOk, TErr>(ok) { }

        Result(TOk&& ok) : std::variant<TOk, TErr>(std::move(ok)) { }

        Result(const TErr& err) : std::variant<TOk, TErr>(err) { }

        Result(TErr&& err) : std::variant<TOk, TErr>(std::move(err)) { }

        Result(const Result<TOk, TErr>& other) = delete;

        [[nodiscard]] bool is_ok() { return std::holds_alternative<TOk>(*this); }
        [[nodiscard]] TOk& get_ok() { return std::get<TOk>(*this); }

        [[nodiscard]] bool is_err() { return std::holds_alternative<TErr>(*this); }
        [[nodiscard]] TErr& get_err() { return std::get<TErr>(*this); }

        static Result<TOk, TErr> ok(const TOk& ok) { return Result{ok}; }
        static Result<TOk, TErr> ok(TOk&& ok) { return Result{std::move(ok)}; }

        // TODO: We probably want an emplace (using std::forward)
    };

}