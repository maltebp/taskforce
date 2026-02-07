#pragma once

#include <variant>
#include <string>
#include <utility>
#include <optional>

#include "core/result.fwd.hpp"

namespace tf {

    struct Error {
        
        explicit Error(std::string&& info) : info(info) { }
        explicit Error(const std::string& info) : info(info) { }
        std::string info;
    };

    template<typename TOk, typename TErr>
    struct Result : std::variant<TOk, TErr> {

        Result(const TOk& ok) : std::variant<TOk, TErr>(ok) { }

        Result(TOk&& ok) : std::variant<TOk, TErr>(std::move(ok)) { }

        Result(const TErr& err) : std::variant<TOk, TErr>(err) { }

        Result(TErr&& err) : std::variant<TOk, TErr>(std::move(err)) { }

        Result(const Result<TOk, TErr>& other) = delete;

        [[nodiscard]] bool is_ok() const { return std::holds_alternative<TOk>(*this); }
        [[nodiscard]] TOk& get_ok() { return std::get<TOk>(*this); }

        [[nodiscard]] bool is_err() const { return std::holds_alternative<TErr>(*this); }
        [[nodiscard]] TErr& get_err() { return std::get<TErr>(*this); }

        static Result<TOk, TErr> ok(const TOk& ok) { return Result{ok}; }
        static Result<TOk, TErr> ok(TOk&& ok) { return Result{std::move(ok)}; }

        static Result<TOk, TErr> err(const TErr&& error) { return Result{std::move(error)}; }
        static Result<TOk, TErr> err(TErr&& error) { return Result{std::move(error)}; }

        // TODO: We probably want an emplace (using std::forward)
    };

    template<typename TErr>
    struct Result<void, TErr> : private std::optional<TErr> {
        
        Result() : std::optional<TErr>() { }

        Result(const TErr& err) : std::optional<TErr>(err) { }

        Result(TErr&& err) : std::optional<TErr>(std::move(err)) { }

        Result(const Result<void, TErr>& other) = delete;

        [[nodiscard]] bool is_ok() { return !std::optional<TErr>::has_value(); }

        [[nodiscard]] bool is_err() { return std::optional<TErr>::has_value(); }
        [[nodiscard]] TErr& get_err() { return std::optional<TErr>::value(); }

        static Result<void, TErr> ok() { return Result{}; }

        static Result<void, TErr> err(const TErr& error) { return Result{error}; }
        static Result<void, TErr> err(const TErr&& error) { return Result{std::move(error)}; }
        static Result<void, TErr> err(TErr&& error) { return Result{std::move(error)}; }
    };
}