#pragma once

#include <iostream>
#include <string>
#include <set>
#include <deque>
#include "ChatUser.h"

typedef std::shared_ptr<ChatServer::ChatUser> ChatUserPtr;
namespace ChatServer {

	class ChatRoom
	{
	public:
		void Join(ChatUserPtr user);

		void Leave(ChatUserPtr user);

		void Deliver(const std::string& msg);

	private:
		std::set<ChatUserPtr> _user;
		enum { MaxRecentMsgs = 100 };
		std::deque<std::string> _recentMsgs;
	};

}
