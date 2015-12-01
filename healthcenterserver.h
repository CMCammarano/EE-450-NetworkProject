#ifndef HEALTHCENTERSERVER_H
#define HEALTHCENTERSERVER_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <stdlib.h>
#include <strings.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>

// A struct to hold appointment data
struct HealthCenterData {
	bool available;
	std::string slot;
	std::string date;
	std::string time;
	std::string doctor;
	std::string port;
};

class HealthCenterServer {

	private:
		const int BACKLOG;
		int m_count;
		int m_socketFD;
		int m_portNumber;

		char m_hostname[255];
		char m_ipAddress[255];

		std::string m_portID;

		sockaddr_in m_serverAddress;
		sockaddr_in m_clientAddress;
		addrinfo m_hints;
		addrinfo* m_addressInfo;

		std::map<std::string, std::string>  m_users;
		std::vector<HealthCenterData> m_availabilities;

	public:
		HealthCenterServer();
		virtual ~HealthCenterServer();

		void initialize();
		void shutdown();

		bool update();
};

#endif // HEALTHCENTERSERVER_H
