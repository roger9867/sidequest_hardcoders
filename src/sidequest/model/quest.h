#ifndef _SIDEQUEST_H
#define _SIDEQUEST_H

#include <string>
#include <vector>

#include "storage/Query.h"

namespace Sidequest
{

	class Quest  {
	public:
		typedef std::string Id;

		Id id;
		std::string caption = "null";
		Quest* parent = nullptr;
		std::vector<Quest*> subquests = std::vector<Quest*>();

		Quest();
		Quest(Id id);
		Quest(Id id, std::string caption, Quest* parent, std::vector<Quest*>* subquests);
		~Quest();

		void add_subquest(Quest *quest);

		Id get_id() const;
		std::string get_caption() const;
		Quest* get_parent() const;
		std::vector<Quest*> get_subquests() const;

		void set_id(Id id);
		void set_caption(std::string caption);
		void set_parent(Quest* parent);
		void set_subquests(std::vector<Quest*> subquests);
	};
}

#endif
