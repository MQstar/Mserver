#include <server.hh>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <cstring>
#include <netinet/in.h>
#include <arpa/inet.h>
namespace server{
	Server::Server():isListeningM(false),socketM(-1)
	{
		printf("con");
	}
	Server::~Server()
	{
		printf("des");
	}
	void Server::close()
	{
		// close(socketM);
	}
	bool Server::setNonblock()
	{
		int flags = fcntl(socketM, F_GETFL, 0);
		if(flags == -1)
		{
			return false;
		}
		flags |= O_NONBLOCK;
		int res = fcntl(socketM, F_SETFL, flags);
		if(res == -1)
		{
			return false;
		}
		return true;
	}
	void Server::start()
	{
		if(isListeningM)
		{
			close();
			return;
		}
		if((socketM = socket(AF_INET6, SOCK_STREAM, 0))<0)
		{
		}
		struct sockaddr_in6 address;
		std::memset(&address, 0, sizeof(address));
		address.sin6_family = AF_INET6;
		address.sin6_port = 80;
		address.sin6_addr = in6addr_any;
		int res;
		if((res = bind(socketM,(struct sockaddr*)&address, sizeof(address)))<0)
		{
			close();
			return;
		}
		int enable = 1;
		setsockopt(socketM, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable));
		unsigned int addresslen = sizeof(address);
		if ((res = getsockname(socketM, (struct sockaddr*) &address, &addresslen)) != 0)//get ip address when bind addr_any
		{
			close();
			return;
		}

		if (setNonblock())
		{
			close();
			return;
		}

		if ((res = listen(socketM, SOMAXCONN)) != 0)//defalut value of SOMAXCONN is 128
		{
			close();
			return;
		}
		isListeningM = true;
		int epollfd = epoll_crate1(0);
		if(epollfd == -1)
		{
			return;
		}
		struct epoll_event event;
		event.data.fd = socketM;
		event.events = EPOLLIN | EPOLLET;


		//TODO
		if (epoll_ctl(epollfd, EPOLL_CTL_ADD, socketfd, &event) == -1)
			if (epollfd == -1)
			{
				std::cerr << "[E] epoll_ctl failed\n";
				return 1;
			}

		std::array<struct epoll_event, ::max_events> events;

		while (true)
		{
			auto n = epoll_wait(epollfd, events.data(), ::max_events, -1);
			for (int i = 0; i < n; ++i)
			{
				if (events[i].events & EPOLLERR ||
						events[i].events & EPOLLHUP ||
						!(events[i].events & EPOLLIN)) // error
				{
					std::cerr << "[E] epoll event error\n";
					close(events[i].data.fd);
				}
				else if (socketfd == events[i].data.fd) // new connection
				{
					while (::accept_connection(socketfd, event, epollfd)) {}
				}
				else // data to read
				{
					auto fd = events[i].data.fd;
					while (::read_data(fd)) {}
				}
			}
		}

		close(socketfd);
	}
}
