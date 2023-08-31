#pragma once

#include <iostream>
#include <string>

namespace ChatServer {

	class ChatUser
	{
	public:
		virtual ~ChatUser() {}
		virtual void Deliver(const std::string& msg) = 0;
	};
}