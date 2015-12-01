#ifndef PATIENT1_H
#define PATIENT1_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
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

enum Patient1State {

	IDLE,
	LOGGING_IN,
	SPEAKING_TO_DOCTOR,
	FINISHED
};

class Patient1 {

	private:
		std::string m_name;
		std::string m_password;
		std::string m_port;
		std::string m_insurance;
		std::string m_doctor;
		std::string m_doctorPort;		

		Patient1State m_state;

	public:
		Patient1();
		virtual ~Patient1();

		// Scheduler
		bool update();

		// Initialization and shutdown
		void initialize();
		void shutdown();
};

#endif // PATIENT1_H
