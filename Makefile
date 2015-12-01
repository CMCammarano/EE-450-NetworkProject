CC = g++
CFLAGS = -lsocket -lnsl
LDFLAGS =

all: hcs doc pat1 pat2

hcs:
	$(CC) $(CFLAGS) -o hcs.out healthcenterserver.cpp

doc:
	$(CC) $(CFLAGS) -o doc.out doctor.cpp

pat1:
	$(CC) $(CFLAGS) -o pat1.out patient1.cpp

pat2:
	$(CC) $(CFLAGS) -o pat2.out patient2.cpp
