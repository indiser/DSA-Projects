#include "common/errors.hpp"

namespace chat {

std::string error_message(ErrorCode ec) {
    switch (ec) {
        case ErrorCode::OK:            return "ok";
        case ErrorCode::NOT_FOUND:     return "not found";
        case ErrorCode::UNAUTHORIZED:  return "unauthorized";
        case ErrorCode::FORBIDDEN:     return "forbidden";
        case ErrorCode::INVALID_INPUT: return "invalid input";
        case ErrorCode::DB_ERROR:      return "database error";
        case ErrorCode::CACHE_ERROR:   return "cache error";
        case ErrorCode::INTERNAL:      return "internal error";
    }
    return "unknown";
}

} // namespace chat
