#ifndef ERRORS_H
#define ERRORS_H

#include <iostream>
#include <string>
#include <variant>
#include <stdexcept>

namespace kvdb {

enum class ErrorType {
    NoError,
    FailedOpenDataFile,
    FailedReadDataFile,
    FailedWriteDataFile,
};

template <typename T>
class Result {
public:
    static Result<T> Ok(T&& val) {
        return Result<T>(std::move(val));
    }

    static Result<T> Ok(T val) {
        return Result<T>(std::move(val));
    }

    static Result<T> Err(ErrorType error_type, std::string&& message) {
        return Result<T>(error_type, std::move(message));
    }

    static Result<T> Err(ErrorType error_type) {
        return Result<T>(error_type, "");
    }

    bool is_ok() const {
        return std::holds_alternative<T>(content);
    }

    bool is_err() const {
        return std::holds_alternative<ErrorType>(content);
    }

    T unwrap() const {
        if (is_ok()) {
            return std::get<T>(content);
        } else {
            throw std::runtime_error("Called unwrap on an Err object");
        }
    }

    ErrorType unwrap_err() const {
        if (is_err()) {
            return std::get<ErrorType>(content);
        } else {
            throw std::runtime_error("Called unwrap_err on an Ok object");
        }
    }

    std::string get_message() const {
        return message;
    }

private:
    std::variant<T, ErrorType> content;
    std::string message;

private:
    Result(T&& value) : content(std::move(value)) {}
    Result(ErrorType error, std::string&& message) : content(error), message(std::move(message)) {}
};

};

#endif
