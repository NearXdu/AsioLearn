PROGS=clienttest \
      dnstest\
      server\
      buffertest\
      udptest\
      asyncudptest\
      asyncudptest2\
      tutorial_timer1\
      tutorial_timer2\
      tutorial_timer3\
      tutorial_timer4\
      tutorial_timer5\
      tutorial_daytime1\
      tutorial_daytime2\
      tutorial_daytime3\
      tutorial_daytime4\
      tutorial_daytime5\
      tutorial_daytime6\
      example_allocation\
      example_buffer\
      example_chat_client\
      example_chat_server\


      


all:${PROGS}


SRC=${shell pwd}/src
OUTPUT=${shell pwd}/bin


MAKE_BIN_DIR := ${shell mkdir -p $(OUTPUT) }
MAKE_EXAMPLE_BIN_DIR := ${shell mkdir -p $(OUTPUT)/example_chat }

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

tutorial_timer1:${SRC}/tutorial_timer1.o
	@${CXX} ${CXXFLAGS}  -o ${OUTPUT}/$@   $^  ${LDFLAGS}

tutorial_timer2:${SRC}/tutorial_timer2.o
	@${CXX} ${CXXFLAGS}  -o ${OUTPUT}/$@   $^  ${LDFLAGS}

tutorial_timer3:${SRC}/tutorial_timer3.o
	@${CXX} ${CXXFLAGS}  -o ${OUTPUT}/$@   $^  ${LDFLAGS}
tutorial_timer4:${SRC}/tutorial_timer4.o
	@${CXX} ${CXXFLAGS}  -o ${OUTPUT}/$@   $^  ${LDFLAGS}
tutorial_timer5:${SRC}/tutorial_timer5.o
	@${CXX} ${CXXFLAGS}  -o ${OUTPUT}/$@   $^  ${LDFLAGS}

tutorial_daytime1:${SRC}/tutorial_daytime1.o
	@${CXX} ${CXXFLAGS}  -o ${OUTPUT}/$@   $^  ${LDFLAGS}
tutorial_daytime2:${SRC}/tutorial_daytime2.o
	@${CXX} ${CXXFLAGS}  -o ${OUTPUT}/$@   $^  ${LDFLAGS}
tutorial_daytime3:${SRC}/tutorial_daytime3.o
	@${CXX} ${CXXFLAGS}  -o ${OUTPUT}/$@   $^  ${LDFLAGS}
tutorial_daytime4:${SRC}/tutorial_daytime4.o
	@${CXX} ${CXXFLAGS}  -o ${OUTPUT}/$@   $^  ${LDFLAGS}
tutorial_daytime5:${SRC}/tutorial_daytime5.o
	@${CXX} ${CXXFLAGS}  -o ${OUTPUT}/$@   $^  ${LDFLAGS}
tutorial_daytime6:${SRC}/tutorial_daytime6.o
	@${CXX} ${CXXFLAGS}  -o ${OUTPUT}/$@   $^  ${LDFLAGS}
example_allocation:${SRC}/example_allocation.o
	@${CXX} ${CXXFLAGS}  -o ${OUTPUT}/$@   $^  ${LDFLAGS}
example_buffer:${SRC}/example_buffer.o
	@${CXX} ${CXXFLAGS}  -o ${OUTPUT}/$@   $^  ${LDFLAGS}

example_chat_server:${SRC}/example_chat/chat_server.o
	@${CXX} ${CXXFLAGS}  -o ${OUTPUT}/example_chat/$@   $^  ${LDFLAGS}
example_chat_client:${SRC}/example_chat/chat_client.o
	@${CXX} ${CXXFLAGS}  -o ${OUTPUT}/example_chat/$@   $^  ${LDFLAGS}











clean:
	@rm -rf ${OUTPUT} ${SRC}/*.o 


.PHONY: all clean
