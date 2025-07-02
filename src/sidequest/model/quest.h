#ifndef _SIDEQUEST_H
#define _SIDEQUEST_H

#include <string>
#include <vector>

#include "id.h"
#include "user.h"

namespace Sidequest {

	class Quest  {
	 public:
		enum Status {
			initial,
			done,
			inactive
		};

		//typedef std::string Id;

		Quest();
		explicit Quest(Id id);
		Quest(Id id,
			Status status,
			std::string title,
			std::string caption,
			User* owner,
			User* editor,
			Quest* parent,
			std::vector<Quest*> subquests);

		Quest(Id id,
			Status status,
			std::string title,
			std::string caption,
			User* owner,
			User* editor,
			Quest* parent);

		//?? subquests
		//Quest(Status status = initial, std::string title  = "Empty.", std::string caption  = "Empty.", User* owner, User* editor, Quest* parent);

		virtual ~Quest();

		void add_subquest(Quest* quest);

		Id get_id() const;
		std::string get_caption() const;
		Quest* get_parent() const;
		std::vector<Quest*>  get_subquests() const;
		User* get_owner() const;
		std::string get_title() const;
		User* get_editor() const;
		Status get_status() const;


		void set_id(Id id);
		void set_caption(std::string caption);
		void set_parent(Quest* parent);
		void set_subquests(std::vector<Quest*> subquests);
		void set_owner(User* owner);
		void set_status(Status status);
		void set_title(std::string title);
		void set_editor(User* editor);

		std::string status_to_string(Status status);
		Status string_to_status(const std::string& s);



	protected:
		Id id;
		Status status = initial;

		std::string caption = "Empty.";
		std::string title = "Empty.";

		Quest* parent = nullptr;
		std::vector<Quest*> subquests = std::vector<Quest*>();

		User* owner = nullptr;
		User* editor = nullptr;
	};
}

#endif
