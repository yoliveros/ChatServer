#include "ChatSession.h"

namespace ChatServer {

	ChatSession::ChatSession(boost::asio::ip::tcp::socket socket, ChatRoom& room) :
		_socket(std::move(socket)),
		_timer(_socket.get_executor()),
		_room(room)
	{
		_timer.expires_at(std::chrono::steady_clock::time_point::max());
	}

	void ChatSession::Start()
	{
		_room.Join(shared_from_this());

		boost::asio::co_spawn(_socket.get_executor(),
			[self = shared_from_this()] {return self->Reader(); },
			boost::asio::detached);
	}

	void ChatSession::Deliver(const std::string& msg)
	{
		_writeMsgs.push_back(msg);
		_timer.cancel_one();
	}

	boost::asio::awaitable<void> ChatSession::Reader()
	{
		try
		{
			for (std::string readMsg;;)
			{
				std::size_t n = co_await boost::asio::async_read_until(_socket,
					boost::asio::dynamic_buffer(readMsg, 1024), "\n", boost::asio::use_awaitable);

				_room.Deliver(readMsg.substr(0, n));
				readMsg.erase(0, n);
			}
		}
		catch (const std::exception&)
		{
			Stop();
		}
	}

	boost::asio::awaitable<void> ChatSession::Writer()
	{
		try
		{
			while (_socket.is_open())
			{
				if (_writeMsgs.empty())
				{
					boost::system::error_code ec;
					co_await _timer.async_wait(boost::asio::redirect_error(boost::asio::use_awaitable, ec));
				}
				else
				{
					co_await boost::asio::async_write(_socket,
						boost::asio::buffer(_writeMsgs.front()), boost::asio::use_awaitable);
					_writeMsgs.pop_front();
				}
			}
		}
		catch (const std::exception&)
		{
			Stop();
		}
	}

	void ChatSession::Stop()
	{
		_room.Leave(shared_from_this());
		_socket.close();
		_timer.cancel();
	}
}
