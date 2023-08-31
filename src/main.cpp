#include "ChatRoom.h"
#include "ChatSession.h"


boost::asio::awaitable<void> listener(boost::asio::ip::tcp::acceptor acceptor)
{
	ChatServer::ChatRoom room;

	for (;;)
	{
		std::make_shared<ChatServer::ChatSession>(
			co_await acceptor.async_accept(boost::asio::use_awaitable),
			room
		)->Start();
	}
}

int main(int argc, char* argv[])
{
	try
	{
		if (argc < 2)
		{
			std::cerr << "Usage: ChatServer <port> [<port> ..]\n";
			exit(EXIT_FAILURE);
		}

		boost::asio::io_context io(1);

		for (int i = 1; i < argc; i++)
		{
		using boost::asio::ip::tcp;
		unsigned short port = 9191;
		boost::asio::co_spawn(io,
			listener(tcp::acceptor(io, { tcp::v4(), port })),
			boost::asio::detached
		);
		}

		boost::asio::signal_set signals(io, SIGINT, SIGTERM);
		signals.async_wait([&](auto, auto) { io.stop(); });

		io.run();
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << "\n";
		exit(EXIT_FAILURE);
	}
}