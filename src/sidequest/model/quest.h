#pragma once

#include <string>
#include <vector>

namespace Sidequest
{

	class Quest {
	public:
		typedef unsigned long Id;

		Quest();
		~Quest();

		Id id;
		std::string caption;
		Quest* parent;
		std::vector<Quest*> subquests;
	};
}