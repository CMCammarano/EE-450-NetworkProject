#include "patient1.h"

Patient1::Patient1() : m_state(IDLE) {}
Patient1::~Patient1() {}

void Patient1::initialize() {

	// Load patient information
	std::ifstream input;
	input.open("Input/patient1.txt");

	std::string data;
	std::getline(input, data);

	std::stringstream stream(data);

	stream >> m_name;
	stream >> m_password;

	// Load insurance
	std::ifstream insurance;
	insurance.open("Input/patient1insurance.txt");

	std::string insuranceData;
	std::getline(insurance, insuranceData);

	std::stringstream insuranceStream(insuranceData);

	insuranceStream >> m_insurance;
}

void Patient1::shutdown() {}

bool Patient1::update() {

	switch (m_state) {
	
		case IDLE: {
			addrinfo hints;
			addrinfo* addressInfo;

			char hostname[256];
			char ipAddress[256];
			gethostname(hostname, 256);

			memset(&hints, 0, sizeof(hints));
			hints.ai_family = AF_UNSPEC;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_flags = AI_PASSIVE;

			getaddrinfo(NULL, "21576", &hints, &addressInfo);

			int socketFD = socket(addressInfo->ai_family, addressInfo->ai_socktype, addressInfo->ai_protocol);
			
			// Display information
			hostent* host = gethostbyname(hostname);
			in_addr** addresses = (in_addr **)host->h_addr_list;

			for (int i = 0; addresses[i] != NULL; i++) {
				strcpy(ipAddress, inet_ntoa(*addresses[i]));
			}

			sockaddr_in* s = (sockaddr_in*)addressInfo->ai_addr;

			std::cout << "Phase 1: Patient 1 has TCP port number " << s->sin_port << " and IP address " << ipAddress << "." << std::endl;
			std::cout << std::endl;

			// Connect
			connect(socketFD, addressInfo->ai_addr, addressInfo->ai_addrlen);

			std::string message = "authenticate ";
			message += m_name;
			message += " ";
			message += m_password;

			int length = strlen(message.c_str());

			std::cout << "Phase 1: Authentication request from Patient 1 with username " << m_name << " and password " << m_password << " has been sent to the Health Center Server." << std::endl;
			std::cout << std::endl;

			send(socketFD, message.c_str(), length, 0);

			char buffer[256];
			recv(socketFD, buffer, 255, 0);

			std::string result(buffer);
			std::cout << "Phase 1: Patient 1 authentication result: " << result << "." << std::endl;
			std::cout << std::endl;

			close(socketFD);
			freeaddrinfo(addressInfo);

			if (result == "failure") {
				m_state = FINISHED;
			}

			else {
				m_state = LOGGING_IN;
			}

			std::cout << "Phase 1: End of Phase 1 for Patient1." << std::endl;
			std::cout << std::endl;
			break;
		}
		
		case LOGGING_IN: {

			addrinfo hints;
			addrinfo* addressInfo;

			char hostname[256];
			char ipAddress[256];
			gethostname(hostname, 256);

			memset(&hints, 0, sizeof(hints));
			hints.ai_family = AF_UNSPEC;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_flags = AI_PASSIVE;

			getaddrinfo(NULL, "21576", &hints, &addressInfo);

			int socketFD = socket(addressInfo->ai_family, addressInfo->ai_socktype, addressInfo->ai_protocol);
			
			// Display information
			hostent* host = gethostbyname(hostname);
			in_addr** addresses = (in_addr **)host->h_addr_list;

			for (int i = 0; addresses[i] != NULL; i++) {
				strcpy(ipAddress, inet_ntoa(*addresses[i]));
			}

			// Connect
			connect(socketFD, addressInfo->ai_addr, addressInfo->ai_addrlen);

			sockaddr_in* s = (sockaddr_in*)addressInfo->ai_addr;
			std::stringstream out;
			out << s->sin_port;
			std::string pt;
			out >> pt;

			std::string message = "available ";
			message += m_name;
			message += " ";
			message += pt;
			int length = strlen(message.c_str());
			send(socketFD, message.c_str(), length, 0);

			// Get available times
			char buffer[256];
			recv(socketFD, buffer, 255, 0);

			std::vector<std::string> times;

			std::string result(buffer);
			std::string temp = "";
			std::string apts = "";
			std::stringstream stream(result);

			int count = 0;

			while (stream >> temp) {
				
				apts += temp;
				apts += " ";
				count++;

				if (count > 2) {
					times.push_back(apts);
					apts = "";
					count = 0;
				}
			}

			std::cout << "Phase 2: The following appointments are available for Patient 1: " << std::endl;
		
			for (unsigned int i = 0; i < times.size(); i++) {
				std::cout << times[i] << std::endl;
			}

			std::string slot = "";
			
			std::cout << std::endl;
			std::cout << "Please enter the preferred appointment index and press enter: ";

			// Appointment selection loop
			do {
				std::cin >> slot;
				
				char* end;
				long num = strtol(slot.c_str(), &end, 10);

				if (end == slot) {
					std::cerr << "Cannot convert string to integer value." << std::endl;
					std::cout << std::endl;
					slot = "";
				}

				else if (num <= 0 || num > (long)times.size()) {
					std::cerr << "Not a valid slot." << std::endl;
					std::cout << std::endl;
					slot = "";
				}
			} while (slot == "");

			std::cout << std::endl;
			std::cout << std::endl;

			// Send selection
			std::string response = "selection ";
			response += slot;
			int len = strlen(response.c_str());
			send(socketFD, response.c_str(), len, 0);

			// Confirmation
			char buf[256];
			recv(socketFD, buf, 255, 0);

			std::string appointment (buf);
			std::string apt_temp = "";
			std::stringstream apt_stream (appointment);			
			
			apt_stream >> apt_temp;
			
			if (apt_temp == "notavailable") {
				std::cout << "Phase 2: The requested appointment from Patient 1 is not available. Exiting..." << std::endl;
				std::cout << std::endl;
				m_state = FINISHED;
				break;
			}

			else {
				m_doctor = apt_temp;
				
				apt_temp = "";
				apt_stream >> apt_temp;

				m_doctorPort = apt_temp;
			}

			close(socketFD);
			freeaddrinfo(addressInfo);

			std::cout << "Phase 2: The requested appointment is available and reserved to Patient1. The assigned doctor port number is " << m_doctorPort << std::endl;
			std::cout << std::endl;

			m_state = SPEAKING_TO_DOCTOR;
			break;
		}
		
		case SPEAKING_TO_DOCTOR: {
			addrinfo hints;
			addrinfo* addressInfo;

			char hostname[256];
			char ipAddress[256];
			gethostname(hostname, 256);

			memset(&hints, 0, sizeof(hints));
			hints.ai_family = AF_UNSPEC;
			hints.ai_socktype = SOCK_DGRAM;
			hints.ai_flags = AI_PASSIVE;

			getaddrinfo(NULL, m_doctorPort.c_str(), &hints, &addressInfo);

			sockaddr_in doctorAddress;
			socklen_t len = sizeof(doctorAddress);
			memset((char*)&doctorAddress, 0, sizeof(doctorAddress));
			doctorAddress.sin_family = AF_INET;
			doctorAddress.sin_port = htons(atoi(m_doctorPort.c_str()));

			int socketFD = socket(addressInfo->ai_family, addressInfo->ai_socktype, addressInfo->ai_protocol);
			
			// Display information
			hostent* host = gethostbyname(hostname);
			in_addr** addresses = (in_addr **)host->h_addr_list;

			for (int i = 0; addresses[i] != NULL; i++) {
				strcpy(ipAddress, inet_ntoa(*addresses[i]));
			}

			sockaddr_in* s = (sockaddr_in*)addressInfo->ai_addr;

			std::cout << "Phase 3: Patient 1 has dynamic UDP port number " << s->sin_port << " and IP address " << ipAddress << "." << std::endl;
			std::cout << std::endl;

			std::stringstream out;
			out << s->sin_port;
			std::string pt;
			out >> pt;

			// Connect
			connect(socketFD, (struct sockaddr*)&doctorAddress, len);

			// Send a request
			std::string insurance = m_insurance;
			insurance += " ";
			insurance += pt;
			std::cout << "Phase 3: The cost estimation request from Patient 1 with insurance plan " << m_insurance << " has been sent to the doctor with port number " << m_doctorPort << " and IP address " << ipAddress << "." << std::endl;
			std::cout << std::endl;

			int length = strlen(insurance.c_str());
			send(socketFD, insurance.c_str(), length, 0);

			// Receive cost
			char buffer[256];
			bzero(buffer, 256);

			if (recv(socketFD, buffer, 255, 0) > 0) {

				std::string cost (buffer);
				std::cout << "Phase 3: Patient 1 receives a $" << cost << " estimation cost from doctor with port number " << m_doctorPort << " and name " << m_doctor << "." << std::endl;
				std::cout << std::endl;
			}

			close (socketFD);
			freeaddrinfo(addressInfo);

			m_state = FINISHED;
			std::cout << "Phase 3: End of Phase 3 for Patient1." << std::endl;
			std::cout << std::endl;
			break;
		}

		case FINISHED: {
			return false;
		}
	}

	return true;
}

int main (int argc, char *argv[]) {

	Patient1 patient;
	patient.initialize();

	bool running = true;

	while (running) {

		running = patient.update();
	}

	patient.shutdown();

	return 0;
}
