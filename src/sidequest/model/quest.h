#ifndef _SIDEQUEST_H
#define _SIDEQUEST_H

#include <string>
#include <vector>

namespace Sidequest
{

	class Quest {
	public:
		typedef unsigned long Id;

		Quest();
		~Quest();

		void create_on_database();

		Id id;
		std::string caption;
		Quest* parent;
		std::vector<Quest*> subquests;
	};
}

#endif
