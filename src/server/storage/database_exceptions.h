//
// Created by roger on 5/15/25.
//

#ifndef DATABASE_EXCEPTIONS_H
#define DATABASE_EXCEPTIONS_H

#include <stdexcept>

namespace Sidequest::Server {

        class DatabaseNotFoundException : public std::runtime_error {
         public:
            explicit DatabaseNotFoundException(const std::string& message);
        };

        class ParameterBindException : public std::runtime_error {
         public:
            explicit ParameterBindException(const std::string& statement, int error_code);
            int error_code;
        };

        class UnableToReadObjectException : public std::runtime_error {
         public:
            explicit UnableToReadObjectException(const std::string& key);
        };

        class UnableToCreateObjectException : public std::runtime_error {
         public:
            explicit UnableToCreateObjectException(const std::string& key);
        };

        class UnableToUpdateObjectException : public std::runtime_error {
         public:
            explicit UnableToUpdateObjectException(const std::string& key);
        };

        class UnableToDeleteObjectException : public std::runtime_error {
         public:
            explicit UnableToDeleteObjectException(const std::string& key);
        };
}

#endif  //DATABASE_EXCEPTIONS_H
