#include "quest.h"

namespace Sidequest {
	Quest::Quest() = default;

	Quest::Quest(Id id)
		: id(id) {}

	Quest::Quest(Id id, std::string caption, Quest *parent, std::vector<Quest *> *subquests)
		: id(id), caption(caption), parent(parent) {}

	Quest::~Quest() = default;



	void Quest::add_subquest(Quest *quest) {
		subquests.push_back(quest);
	}


	Quest::Id Quest::get_id() const {
		return id;
	}
	std::string Quest::get_caption() const {
		return caption;
	}
	Quest* Quest::get_parent() const {
		return parent;
	}
	std::vector<Quest*> Quest::get_subquests() const {
		return subquests;
	}

	void Quest::set_id(Id id) {
		this->id = id;
	}
	void Quest::set_caption(std::string caption) {
		this->caption = caption;
	}
	void Quest::set_parent(Quest* parent) {
		this->parent = parent;
	}
	void Quest::set_subquests(std::vector<Quest*> subquests) {
		this->subquests = subquests;
	}
}