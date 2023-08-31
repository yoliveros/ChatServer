#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <boost/asio.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/asio/write.hpp>
#include "ChatUser.h"
#include "ChatRoom.h"

namespace ChatServer {

	class ChatSession :
		public ChatUser,
		public std::enable_shared_from_this<ChatSession>
	{
	public:
		ChatSession(boost::asio::ip::tcp::socket socket, ChatRoom& room);

		void Start();

		void Deliver(const std::string& msg);

	private:
		boost::asio::awaitable<void> Reader();

		boost::asio::awaitable<void> Writer();

		void Stop();

		boost::asio::ip::tcp::socket _socket;
		boost::asio::steady_timer _timer;
		ChatRoom& _room;
		std::deque<std::string> _writeMsgs;
	};
}
