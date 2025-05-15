//
// Created by roger on 5/15/25.
//
#include "database_exceptions.h"

namespace Sidequest::Server {

    DatabaseNotFoundException::DatabaseNotFoundException(const std::string& key) : std::runtime_error(key) {}

    ParameterBindException::ParameterBindException(const std::string& statement, int error_code)
        : std::runtime_error(statement), error_code(error_code) {}

    UnableToReadObjectException::UnableToReadObjectException(const std::string& key)
        : std::runtime_error(key) {}

    UnableToCreateObjectException::UnableToCreateObjectException(const std::string& key)
        : std::runtime_error(key) {}

    UnableToUpdateObjectException::UnableToUpdateObjectException(const std::string& key)
        : std::runtime_error(key) {}

    UnableToDeleteObjectException::UnableToDeleteObjectException(const std::string& key)
        : std::runtime_error(key) {}

}