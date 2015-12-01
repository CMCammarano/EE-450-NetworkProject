README
---------------------------

Name: Colin Cammarano

Email: colincammarano@gmail.com

Overview:
---------------------------

For this assignment, I created a networking simulation in C++ that models client-server TCP communication and client-client UDP communication using the UNIX socket and network libraries. This simulation uses sockets, threading, and file-IO to properly simulate the interactions between users (the patients) and a server that runs as its own process.

Code:
---------------------------

healthcenterserver.cpp:
+ This file is the main source code for the healthcenterserver process. It implements the function and method prototypes defined within the healthcenterserver.h header. Since this project uses C++, this code implementes a class which encapsulates the data and member methods of the health center server process. At the end of the file, an entry point function, main(), is implemented which runs the process until both patients have communicated with the server.

healthcenterserver.h:
+ This file is a header file which defines the healthcenterserver class, its methods, and any data used for the healthcenterserver process.

patient1.cpp:
+ This file is the main source code for the patient1 process. It implements the class, member methods, and funtions defined in the patient1.h header file. Since this also should yield a process, an entry point function, main(), is implemented which runs until the process has completed all three defined phases.

patient1.h:
+ This file is a header file that defines the patient1 class, its methods, and any data used by the patient1 process.

patient2.cpp:
+ This file is the main source code for the patient2 process. It implements the class, member methods, and funtions defined in the patient2.h header file. Since this also should yield a process, an entry point function, main(), is implemented which runs until the process has completed all three defined phases.

patient2.h:
+ This file is a header file that defines the patient2 class, its methods, and any data used by the patient2 process.

doctor.cpp:
+ This file is the main source code for the doctor process(es). Unlike the other classes presented, the doctor uses the fork() function to create two, independent doctor proceses. Hence, this code is multithreaded. The entry point function in this code uses the fork() function to create two doctor processes, doctor1 and doctor2, respectively.

doctor.h:
+ This file is a header file that defines the doctor class, its methods, and any data used by the doctor processes.

Other Files:
---------------------------

README.md:
+ This file. A readme written in the markdown file format.

README.txt:
+ A copy of the markdown readme for portability.

Makefile:
+ A simple UNIX makefile that executes the necessary compilation commands on UNIX systems. Compliles each source with the command "g++ -lsocket -lnsl -o OUTPUT_NAME SOURCE.cpp"

Building the Application:
---------------------------

There are two ways of building the source code for this project:

1: Using the makefile
	+ To use the makefile, simply type the command "make all" without quotes in the terminal. This will compile the source into separate executables for each process on the nunki server.

2: g++
	+ To compile with the g++ command (if the makefile does not work), simply type the command "g++ -lsocket -lnsl -o OUTPUT_NAME SOURCE.cpp" without quotes in the terminal, replacing SOURCE with the name of the source file (healthcenterserver, doctor, patient1, patient2), and OUTPUT_NAME with an appropriate filename for each executable.

Running the Application:
---------------------------

1: Compile and build the application (described above).

2: Begin each process in the order described below using the command "./executable" without quotes, replacing "executable" with the name of the process you wish to run.
	1: healthcenterserver
	2: doctor
	3: patient1
	4: patient2

3: All processes besides the doctor should terminate on their own. For the doctor, use the terminal command "Ctrl + C" to end the process once the simulation is over.

Message Format:
---------------------------

The general message format used throughout the simulation can be found below. Each message adheres to this formula, making parsing on both ends simple. Messages in this application were designed to work with C++ strings, hence a format easily parsible through stringstream was desired.

message = [header] [payload_1]...[payload_n]

EXAMPLE: authenticate username password

Bugs and Failure Conditions:
---------------------------

None found through unit testing.

Reused Code:
---------------------------

None.
