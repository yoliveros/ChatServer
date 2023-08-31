#include "ChatRoom.h"

namespace ChatServer {

	void ChatRoom::Join(ChatUserPtr user)
	{
		_user.insert(user);
		for (auto& msg : _recentMsgs)
			user->Deliver(msg);
	}

	void ChatRoom::Leave(ChatUserPtr user)
	{
		_user.erase(user);
	}

	void ChatRoom::Deliver(const std::string& msg)
	{
		_recentMsgs.push_back(msg);
		while (_recentMsgs.size() > MaxRecentMsgs)
			_recentMsgs.pop_front();

		for (auto& user : _user)
			user->Deliver(msg);
	}
}
