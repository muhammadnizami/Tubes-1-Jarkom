all: bin bin/sender bin/receiver

bin: 
	mkdir -p bin

bin/sender: src/msender.cpp src/sender.h src/sender.cpp src/frame.h src/frame.cpp src/checksum.h src/checksum.cpp src/ackframe.h src/ackframe.cpp
	g++ src/msender.cpp src/sender.cpp src/frame.cpp src/ackframe.cpp src/checksum.cpp -o bin/sender -pthread -std=c++11

bin/receiver: src/mreceiver.cpp src/receiver.h src/receiver.cpp src/frame.h src/frame.cpp src/checksum.h src/checksum.cpp src/ackframe.h src/ackframe.cpp
	g++ src/mreceiver.cpp src/receiver.cpp  src/frame.cpp src/ackframe.cpp src/checksum.cpp -o bin/receiver -pthread -std=c++11
