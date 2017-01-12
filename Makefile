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
      example_echo_server_sync\
      example_echo_server_async\
      example_http_server


      


all:${PROGS}


SRC=${shell pwd}/src
OUTPUT=${shell pwd}/bin


MAKE_BIN_DIR := ${shell mkdir -p $(OUTPUT) }
MAKE_EXAMPLE_CHAT_BIN_DIR := ${shell mkdir -p $(OUTPUT)/example_chat }
MAKE_EXAMPLE_ECHO_SYNC_BIN_DIR := ${shell mkdir -p $(OUTPUT)/example_echo_sync }
MAKE_EXAMPLE_ECHO_ASYNC_BIN_DIR := ${shell mkdir -p $(OUTPUT)/example_echo_async }
MAKE_EXAMPLE_HTTP_BIN_DIR := ${shell mkdir -p $(OUTPUT)/example_http }

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

example_echo_server_sync:${SRC}/example_echo_sync/example_echo_server_sync.o
	@${CXX} ${CXXFLAGS}  -o ${OUTPUT}/example_echo_sync/$@   $^  ${LDFLAGS}
example_echo_server_async:${SRC}/example_echo_async/example_echo_server_async.o
	@${CXX} ${CXXFLAGS}  -o ${OUTPUT}/example_echo_async/$@   $^  ${LDFLAGS}

example_http_server:${SRC}/example_http/main.o\
    ${SRC}/example_http/http_server.o\
    ${SRC}/example_http/connection.o\
    ${SRC}/example_http/request_handler.o\
    ${SRC}/example_http/request_parser.o\
    ${SRC}/example_http/reply.o\
    ${SRC}/example_http/connection_manager.o\
    ${SRC}/example_http/mime_types.o
	@${CXX} ${CXXFLAGS}  -o ${OUTPUT}/example_http/$@   $^  ${LDFLAGS}








clean:
	@rm -rf ${OUTPUT} ${SRC}/*.o  ${SRC}/example_chat/*.o ${SRC}/example_echo_sync/*.o \
	    ${SRC}/example_echo_async/*.o\
	    ${SRC}/example_http/*.o


.PHONY: all clean
