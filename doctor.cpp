#include "doctor.h"

Doctor::Doctor() : BACKLOG(20), m_addressInfo(NULL) {}
Doctor::~Doctor() {}

void Doctor::initialize(int number) {

	std::ifstream file;

	if (number == 1) {

		this->m_name = "Doctor 1";
		this->m_portID = "41576";
		this->m_portNumber = 41576;

		file.open("Input/doc1.txt");
	}

	else if (number == 2) {

		this->m_name = "Doctor 2";
		this->m_portID = "42576";
		this->m_portNumber = 42576;

		file.open("Input/doc2.txt");
	}

	// Setup insurances
	std::string temp = "";
	std::string doctor = "";

	int count = 0;

	while (std::getline(file, temp)) {
		doctor += temp;
		doctor += " ";
		count++;
	}

	std::stringstream stream(doctor);

	for (int i = 0; i < count; i++) {

		DoctorInsurance insurance;

		stream >> insurance.insurance;
		stream >> insurance.price;

		m_insurances.push_back(insurance);
	}

	// Setup port number, and IP Address
	gethostname(m_hostname, 255);
	hostent* host = gethostbyname(m_hostname);
	in_addr** addresses = (in_addr **)host->h_addr_list;

	for (int i = 0; addresses[i] != NULL; i++) {
		strcpy(m_ipAddress, inet_ntoa(*addresses[i]));
	}

	// Set up sockets
	m_socketFD = socket(AF_INET, SOCK_DGRAM, 0);

	bzero((char*)&m_serverAddress, sizeof(m_serverAddress));
	m_serverAddress.sin_family = AF_INET;
	m_serverAddress.sin_port = htons(m_portNumber);

	if (bind(m_socketFD, (struct sockaddr*) &m_serverAddress, sizeof(m_serverAddress)) < 0) {
		std::cerr << "Error: Socket could not be bound." << std::endl;
		std::cout << std::endl;
		exit(EXIT_FAILURE);
	}
}

void Doctor::shutdown() {

	freeaddrinfo(m_addressInfo);
}

bool Doctor::update() {

	socklen_t socketLength = sizeof(m_clientAddress);

	char buffer[256];
	bzero(buffer, 256);

	// If a message from a patient is received
	if (recvfrom(m_socketFD, buffer, 255, 0, (struct sockaddr*)&m_clientAddress, &socketLength) > 0) {

		std::cout << "Phase 3: " << m_name << " has a static UDP port " << m_portID << " and IP address " << m_ipAddress << std::endl;
		std::cout << std::endl;

		std::string message(buffer);
		std::string insurance;
		std::string port;

		std::stringstream stream(message);
		stream >> insurance;
		stream >> port;

		connect(m_socketFD, (struct sockaddr*)&m_clientAddress, socketLength);

		for (int i = 0; i < m_insurances.size(); i++) {

			if (insurance == m_insurances[i].insurance) {

				std::cout << "Phase 3: " << m_name << " receives the request from the patient with port number " << port << " and the insurance plan " << insurance << "." << std::endl;
				std::cout << std::endl;

				std::string price = m_insurances[i].price;
				int length = strlen(price.c_str());
				
				if (send(m_socketFD, price.c_str(), length, 0) > 0) {
					std::cout << "Phase 3: " << m_name << " has sent estimated price $" << price << " to patient with port number " << port << "." << std::endl;
					std::cout << std::endl;
				}
			}
		}
	}

	close(m_socketFD);
	return true;
}

int main (int argc, char *argv[]) {

	pid_t pid;
	pid = fork();

	// Child process -- doctor 2
	if (pid == 0) {

		Doctor doctor2;
		doctor2.initialize(2);

		bool running = true;

		while (running) {

			running = doctor2.update();
		}
		
		doctor2.shutdown();
	}

	// Main process -- doctor 1
	else {

		Doctor doctor1;
		doctor1.initialize(1);

		bool running = true;

		while (running) {

			running = doctor1.update();
		}
		
		doctor1.shutdown();
	}

	return 0;
}
