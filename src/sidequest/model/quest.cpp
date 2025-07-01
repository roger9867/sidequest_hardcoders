#include "quest.h"

#include <stdexcept>

namespace Sidequest {
	Quest::Quest() = default;

	Quest::Quest(Id id)
		: id(id) {}

	/*
	Quest::Quest(Id id, Status status, User* owner, std::string title, std::string caption, Quest* parent, std::vector<Quest*> subquests)
		: id(id), owner(owner), editor(owner), caption(caption), parent(parent) {}
	*/

	Quest::Quest(Id id,
			Status status,
			std::string title,
			std::string caption,
			User* owner,
			User* editor,
			Quest* parent,
			std::vector<Quest*> subquests)
				: id(id), status(status), title(title), caption(caption), owner(owner), editor(editor), parent(parent), subquests(subquests) {}

	// Ohne subquests
	Quest::Quest(Id id,
			Status status,
			std::string title,
			std::string caption,
			User* owner,
			User* editor,
			Quest* parent)
				: id(id), status(status), title(title), caption(caption), owner(owner), editor(editor), parent(parent) {}



	// Warum keine id im Konstruktor?
	/*
	Quest::Quest(Status status,
		std::string title,
		std::string caption,
		User* owner,
		User* editor,
		Quest* parent) {}
	*/



	Quest::~Quest() = default;


	void Quest::add_subquest(Quest *quest) {
		subquests.push_back(quest);
	}


	Id Quest::get_id() const {
		return id;
	}
	std::string Quest::get_caption() const {
		return caption;
	}
	Quest* Quest::get_parent() const {
		return parent;
	}
	std::vector<Quest*>  Quest::get_subquests() const {
		return subquests;
	}
	User* Quest::get_owner() const {
		return owner;
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
	void Quest::set_owner(User *owner) {
		this->owner = owner;
	}

	void Quest::set_status(Status status) {
		this->status = status;
	}
	void Quest::set_title(std::string title) {
		this->title = title;
	}

	void Quest::set_editor(User* editor) {
		this->editor = editor;
	}

	User* Quest::get_editor() const {
		return editor;
	}

	std::string Quest::get_title() const {
		return title;
	}

	Quest::Status Quest::get_status() const {
		return status;
	}

	std::string Quest::status_to_string(Status status) {
		switch (status) {
			case initial:  return "initial";
			case done:     return "done";
			case inactive: return "inactive";
		}
		return "Unknown";
	}

	Quest::Status Quest::string_to_status(const std::string& s) {
		if (s == "initial")  return initial;
		if (s == "done")     return done;
		if (s == "inactive") return inactive;
		throw std::invalid_argument("Invalid status string: " + s);
	}


}
