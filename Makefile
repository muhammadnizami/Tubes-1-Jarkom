all: obj bin bin/sender bin/receiver  bin/sender-SENGAJASALAH

bin: 
	mkdir -p bin

obj:
	mkdir -p obj

bin/sender: src/msender.cpp src/sender.h src/sender.cpp obj/frame.o obj/checksum.o obj/ackframe.o obj/escape.o src/dcomm.h
	g++ src/msender.cpp src/sender.cpp obj/frame.o obj/checksum.o obj/ackframe.o obj/escape.o -o bin/sender -pthread -std=c++11

bin/receiver: src/mreceiver.cpp src/receiver.h src/receiver.cpp obj/frame.o obj/checksum.o obj/ackframe.o obj/escape.o src/dcomm.h
	g++ src/mreceiver.cpp src/receiver.cpp obj/frame.o obj/checksum.o obj/ackframe.o obj/escape.o -o bin/receiver -pthread -std=c++11

obj/frame.o: src/frame.h src/frame.cpp src/dcomm.h
	g++ -c src/frame.cpp -o obj/frame.o

obj/checksum.o: src/checksum.h src/checksum.cpp
	g++ -c src/checksum.cpp -o obj/checksum.o

obj/ackframe.o: src/ackframe.h src/ackframe.cpp src/dcomm.h
	g++ -c src/ackframe.cpp -o obj/ackframe.o

obj/escape.o: src/escape.h src/escape.cpp src/dcomm.h
	g++ -c src/escape.cpp -o obj/escape.o

bin/sender-SENGAJASALAH: src/msender.cpp src/sender.h src/sender-SENGAJASALAH.cpp obj/frame.o obj/checksum.o obj/ackframe.o obj/escape.o src/dcomm.h
	g++ src/msender.cpp src/sender-SENGAJASALAH.cpp obj/frame.o obj/checksum.o obj/ackframe.o obj/escape.o -o bin/sender-SENGAJASALAH -pthread -std=c++11
