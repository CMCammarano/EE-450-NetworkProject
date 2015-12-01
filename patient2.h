#ifndef Patient2_H
#define Patient2_H

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

// A state machine enum to handle transitions between communication states
enum Patient2State {

	IDLE,
	LOGGING_IN,
	SPEAKING_TO_DOCTOR,
	FINISHED
};

// Patient 2 class
class Patient2 {

	private:
		std::string m_name;
		std::string m_password;
		std::string m_port;
		std::string m_insurance;
		std::string m_doctor;
		std::string m_doctorPort;		

		Patient2State m_state;

	public:
		Patient2();
		virtual ~Patient2();

		// Scheduler
		bool update();

		// Initialization and shutdown
		void initialize();
		void shutdown();
};

#endif // Patient2_H
