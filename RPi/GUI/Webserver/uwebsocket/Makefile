# Quick and ok!
SRC=src/Extensions.cpp src/Group.cpp src/Networking.cpp src/Hub.cpp src/Node.cpp src/WebSocket.cpp src/HTTPSocket.cpp src/Socket.cpp src/Epoll.cpp
OBJS=$(addprefix ${OBJ_DEST_DIR}/,$(notdir ${SRC:%.cpp=%.o}))
DEPS=$(addprefix ${OBJ_DEST_DIR}/,$(notdir ${SRC:%.cpp=%.d}))

# Which target? set default
ifndef TARGET
TARGET=host
endif

# Debug build? debug is default
ifdef DEBUG
CXXFLAGS=-g -I . -Isrc -std=c++14 -DUWS_THREADSAFE
MODE=debug
else
CXXFLAGS=-O2 -I . -Isrc -std=c++14 -DUWS_THREADSAFE
MODE=release
endif
DEST_DIR=lib/${TARGET}/${MODE}
LIB_NAME=libuWS.so
OBJ_DEST_DIR=build/${TARGET}/${MODE}


ifeq (${TARGET},host)
# Include host specific stuff
include compiler_setup.host
endif

ifeq (${TARGET},rpi)
# Include target specific stuff
include compiler_setup.rpi
endif

# Setting compiler flags
CXXFLAGS+= -Wall -Iinc  
VPATH=src

all: info ${DEST_DIR}/${LIB_NAME}

info:
	@echo "Compiling for '${TARGET}' in '${MODE}' mode..."

${OBJ_DEST_DIR}/%.o: %.cpp
	@echo "Compiling "$<
	@${CXX} -c $< -o $@ ${CXXFLAGS}

${OBJ_DEST_DIR}/%.d: %.cpp 
	@echo "Generating dependency for $<"
	@mkdir -p ${OBJ_DEST_DIR}
	@$(CXX) -MT${OBJ_DEST_DIR}/$(@:.d=.o) -MM $(CXXFLAGS) $^ > $@

${DEST_DIR}:
	@mkdir -p ${DEST_DIR}

${OBJ_DEST_DIR}:
	@mkdir -p ${OBJ_DEST_DIR}

${DEST_DIR}/${LIB_NAME}: ${OBJ_DEST_DIR} ${DEST_DIR} $(DEPS) ${OBJS} 
	@echo "Linking "$@
	@${AR} cr $@ ${OBJS}


clean:
	@echo "Cleaning up..."
	@rm -f ${DEST_DIR}/${TARGET} $(OBJS) $(DEPS)

ifneq ($(MAKECMDGOALS),clean)
-include $(DEPS)
endif

