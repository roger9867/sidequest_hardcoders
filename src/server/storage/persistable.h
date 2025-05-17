#ifndef PERSISTABLE_H
#define PERSISTABLE_H

#include <string>
#include <vector>
#include <sqlite3.h>

/* Base class of all persistent domain model objects on the server
*  that implement CRUDS operations */
namespace Sidequest::Server {

		class Database;

		/* All persistent classes should derive from Persistable */
		class Persistable {
		 protected:
			Database* database;

		 public:
			explicit Persistable(Database* database);
			virtual ~Persistable();

			virtual void create_on_database() = 0;
			virtual void read_on_database() = 0;
			/*
			virtual void update_on_database() = 0;
			virtual void delete_on_database() = 0;

			virtual std::string class_id() = 0;
			*/
		};
};

#endif  //PERSISTABLE_H
