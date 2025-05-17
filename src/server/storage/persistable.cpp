#include "persistable.h"

#include "database.h"
//#include "statement_cache.h"

namespace Sidequest::Server {

	Persistable::Persistable( Database* database ) {
		this->database = database;
	}

	Persistable::~Persistable() = default;
}