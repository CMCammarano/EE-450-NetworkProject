#include "healthcenterserver.h"

HealthCenterServer::HealthCenterServer() : BACKLOG(20), m_count(0), m_addressInfo(NULL) {}
HealthCenterServer::~HealthCenterServer() {}

void HealthCenterServer::initialize() {

	// Load patient info, schedule, and other settings
	std::ifstream schedule;
	schedule.open("Input/availabilities.txt");

	std::string scheduleString;
	std::string tempScheduleString;

	int count = 0;

	while (std::getline(schedule, tempScheduleString)) {
		scheduleString += tempScheduleString;
		scheduleString += " ";
		count++;
	}

	std::stringstream scheduleStream (scheduleString);

	for (int i = 0; i < count; i++) {

		HealthCenterData data;
		data.available = true;

		scheduleStream >> data.slot;
		scheduleStream >> data.date;
		scheduleStream >> data.time;
		scheduleStream >> data.doctor;
		scheduleStream >> data.port;

		m_availabilities.push_back(data);
	}

	// Load users
	std::ifstream users;
	users.open("Input/users.txt");

	std::string userString = "";
	std::string tempUserString = "";

	count = 0;

	while (std::getline(users, tempUserString)) {
		userString += tempUserString;
		userString += " ";
		count++;
	}

	std::stringstream userStream(userString);

	for (int i = 0; i < count; i++) {
		std::string patName;
		std::string patPwd;

		userStream >> patName;
		userStream >> patPwd;

		m_users.insert(std::make_pair(patName, patPwd));
	}

	// Setup port number, and IP Address
	this->m_portID = "21576";
	this->m_portNumber = 21576;

	gethostname(m_hostname, 255);
	hostent* host = gethostbyname(m_hostname);
	in_addr** addresses = (in_addr **)host->h_addr_list;

	for (int i = 0; addresses[i] != NULL; i++) {
		strcpy(m_ipAddress, inet_ntoa(*addresses[i]));
	}

	std::cout << "Phase 1: The Health Center Server has port number " << m_portID << " and IP address " << m_ipAddress << std::endl;
	std::cout << std::endl;

	// Set up sockets on the system
	m_socketFD = socket(AF_INET, SOCK_STREAM, 0);

	bzero((char*)&m_serverAddress, sizeof(m_serverAddress));
	m_serverAddress.sin_family = AF_INET;
	m_serverAddress.sin_port = htons(m_portNumber);

	if (bind(m_socketFD, (struct sockaddr*) &m_serverAddress, sizeof(m_serverAddress)) < 0) {
		std::cerr << "Error: Socket could not be bound." << std::endl;
		std::cout << std::endl;
		exit(EXIT_FAILURE);
	}

	if (listen(m_socketFD, BACKLOG) < 0) {
		std::cerr << "Error: Could not listen on socket." << std::endl;
		std::cout << std::endl;
		exit(EXIT_FAILURE);
	}
}

void HealthCenterServer::shutdown() {

	freeaddrinfo(m_addressInfo);
	close(m_socketFD);
}

bool HealthCenterServer::update() {

	// Accept incoming connections and block.
	socklen_t socketLength = sizeof(socketLength);
	int client = accept(m_socketFD, (struct sockaddr*)&m_clientAddress, &socketLength);

	if (client < 0) {
		std::cerr << "Error: Can't establish a connection." << std::endl;
		std::cout << std::endl;
		exit(EXIT_FAILURE);
	}

	char buffer[256];
	bzero(buffer, 256);

	// Proceed if a valid response comes in
	if (recv(client, buffer, 255, 0) > 0) {

		std::string message(buffer);
		std::stringstream stream(message);

		std::string header;

		stream >> header;

		// Look for authentication requests
		if (header == "authenticate") {

			std::string username;
			std::string password;

			stream >> username;
			stream >> password;

			std::cout << "Phase 1: The Health Center Server has received a request from a patient with username " << username << " and password " << password << "." << std::endl;
			std::cout << std::endl;

			std::string response;

			if (password == m_users[username]) {

				response = "success";
			}

			else {

				response = "failure";
				m_count++;
			}

			std::cout << "Phase 1: The Health Center Server sends the response " << response << " to patient with username " << username << "." << std::endl;
			std::cout << std::endl;

			int length = strlen(response.c_str());
			send(client, response.c_str(), length, 0);
		}

		// Checking for appointments
		if (header == "available") {

			// Get client name
			std::string user;
			std::string port;
			stream >> user;
			stream >> port;

			// Request for available slots
			std::cout << "Phase 2: The Health Center Server, receives a request for available time slots from patients with port number " << port << " and IP address " << m_ipAddress << "." << std::endl;
			std::cout << std::endl;

			std::string slots = "";
			
			for (unsigned int i = 0; i < m_availabilities.size(); i++) {
				slots += m_availabilities[i].slot;
				slots += " ";
				slots += m_availabilities[i].date;
				slots += " ";
				slots += m_availabilities[i].time;
				slots += " ";
			}

			// Collect available slots
			std::cout << "Phase 2: The Health Center Server sends available time slot to patient with username " << user << "." << std::endl;
			std::cout << std::endl;

			int length = strlen(slots.c_str());
			send(client, slots.c_str(), length, 0);

			char buf[256];
			bzero(buf, 256);

			recv(client, buf, 256, 0);

			std::string selection (buf);
			std::string sel_temp;
			std::string slot;
			std::stringstream selection_stream(selection);

			selection_stream >> sel_temp;
			selection_stream >> slot;

			// Request for a single slot
			std::cout << "Phase 2: The Health Center Server receives a request for appointment " << slot << " from patient with port number " << port << " and username " << user << "." << std::endl;
			std::cout << std::endl;

			for (unsigned int i = 0; i < m_availabilities.size(); i++) {

				if (m_availabilities[i].slot == slot) {

					std::string response;

					if (m_availabilities[i].available) {

						m_availabilities[i].available = false;
						std::cout << "Phase 2: The Health Center Server confirms the following appointment " << slot << " to patient with username " << user << "." << std::endl;
						std::cout << std::endl;

						response += m_availabilities[i].doctor;
						response += " ";
						response += m_availabilities[i].port;

						int length = strlen(response.c_str());
						send(client, response.c_str(), length, 0);
					}

					else {

						std::cout << "Phase 2: The Health Center Server rejects the following appointment " << slot << " to patient with username " << user << "." << std::endl;
						std::cout << std::endl;
						
						response = "notavailable";
						int length = strlen(response.c_str());
						send(client, response.c_str(), length, 0);
					}
				}
			}

			close(client);

			// Exit automatically when both patients have finished.
			m_count++;

			if (m_count < 2) {
				return true;
			}

			else {
				return false;
			}
		}
	}

	close(client);
	return true;
}

// Entry point for health center server process.
int main (int argc, char *argv[]) {

	HealthCenterServer healthCenter;
	healthCenter.initialize();

	bool running = true;

	while (running) {

		running = healthCenter.update();
	}

	healthCenter.shutdown();

	return 0;
}
