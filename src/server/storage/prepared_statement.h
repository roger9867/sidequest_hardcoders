//
// Created by roger on 5/26/25.
//

#ifndef PREPARED_STATEMENT_H
#define PREPARED_STATEMENT_H
#include <string>
#include <unordered_map>

#include "sqlite3.h"

namespace Sidequest::Server {
    typedef std::unordered_map<std::string, int> ColumnMap;
    typedef sqlite3_stmt PreparedStatement;
}

#endif //PREPARED_STATEMENT_H
