#ifndef DOCTOR_H
#define DOCTOR_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <strings.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>

struct DoctorInsurance {
	std::string insurance;
	std::string price;
};

class Doctor {

	private:
		const int BACKLOG;
		int m_socketFD;
		short m_portNumber;
		std::string m_name;
		std::string m_portID;
		char m_hostname[255];
		char m_ipAddress[255];

		sockaddr_in m_serverAddress;
		sockaddr_in m_clientAddress;
		addrinfo m_hints;
		addrinfo* m_addressInfo;

		std::vector<DoctorInsurance> m_insurances;

	public:
		Doctor();
		virtual ~Doctor();

		void initialize(int number);
		void shutdown();
		bool update();
};

#endif // DOCTOR_H
