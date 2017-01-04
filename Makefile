PROGS=clienttest dnstest server buffertest udptest asyncudptest asyncudptest2


all:${PROGS}


SRC=${shell pwd}/src
OUTPUT=${shell pwd}/bin


MAKE_BIN_DIR := ${shell mkdir -p $(OUTPUT) }

CXXFLAGS+=-g -std=c++11
LDFLAGS+=-lboost_system -lboost_thread -lpthread

clienttest:${SRC}/clienttest.o
	@${CXX} ${CXXFLAGS}  -o ${OUTPUT}/$@   $^  ${LDFLAGS}

dnstest:${SRC}/dnstest.o
	@${CXX} ${CXXFLAGS}  -o ${OUTPUT}/$@   $^  ${LDFLAGS}

server:${SRC}/server.o
	@${CXX} ${CXXFLAGS}  -o ${OUTPUT}/$@   $^  ${LDFLAGS}
buffertest:${SRC}/buffertest.o
	@${CXX} ${CXXFLAGS}  -o ${OUTPUT}/$@   $^  ${LDFLAGS}
udptest:${SRC}/udptest.o
	@${CXX} ${CXXFLAGS}  -o ${OUTPUT}/$@   $^  ${LDFLAGS}

asyncudptest:${SRC}/asyncudptest.o
	@${CXX} ${CXXFLAGS}  -o ${OUTPUT}/$@   $^  ${LDFLAGS}

asyncudptest2:${SRC}/asyncudptest2.o
	@${CXX} ${CXXFLAGS}  -o ${OUTPUT}/$@   $^  ${LDFLAGS}






clean:
	@rm -rf ${OUTPUT} ${SRC}/*.o 


.PHONY: all clean
