#ifndef _SIDEQUEST_H
#define _SIDEQUEST_H

#include <string>
#include <vector>

#include "user.h"

namespace Sidequest {
	class Quest  {
	public:
		typedef std::string Id;

		Quest();
		Quest(Id id);
		Quest(Id id, User* owner, std::string caption, Quest* parent, std::vector<Quest*> subquests);
		~Quest();

		void add_subquest(Quest *quest);

		Id get_id() const;
		std::string get_caption() const;
		Quest* get_parent() const;
		std::vector<Quest*>  get_subquests() const;
		User* get_owner() const;


		void set_id(Id id);
		void set_caption(std::string caption);
		void set_parent(Quest* parent);
		void set_subquests(std::vector<Quest*> subquests);
		void set_owner(User* owner);

	protected:
		Id id;
		std::string caption = "Empty.";
		Quest* parent = nullptr;
		std::vector<Quest*> subquests = std::vector<Quest*>();
		User* owner = nullptr;
	};
}

#endif
