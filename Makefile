ifeq ($(OS),Windows_NT)
	HOST_OS := windows
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		HOST_OS := linux
	else
		$(error Unsupported host OS: $(UNAME_S))
	endif
endif

PLATFORM?= $(HOST_OS)

SRC	:= src
INC	:= include
OBJ	:= build
BIN	:= bin
LIB	:= lib

CC	?= gcc
CXX	?= g++

CFLAGS  := -std=c11
CXXFLAGS:= -std=c++11
CPPFLAGS:= -Wall -Wextra -I$(INC)
LDFLAGS := -L$(LIB)/$(PLATFORM)

ifeq ($(PLATFORM),windows)
	CC	:= x86_64-w64-mingw32-gcc
	CXX	:= x86_64-w64-mingw32-g++
	CPPFLAGS+= -Dmain=SDL_main
	LDFLAGS += -lmingw32 -lSDL2main -lSDL2 -mwindows -lSDL2_image -lSDL2_mixer
	LDFLAGS += -Wl,--dynamicbase -Wl,--nxcompat -Wl,--high-entropy-va -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lsetupapi -lversion -luuid
else ifeq ($(PLATFORM),linux)
	CPPFLAGS+= -D_REENTRANT
	LDFLAGS	+= -Wl,--enable-new-dtags -lSDL2 -lSDL2_image -lSDL2_mixer
	LDFLAGS += -pthread -lm
else
	$(error Unsupported target platform: $(PLATFORM))
endif

BUILD	?= release

ifeq ($(BUILD),debug)
	CFLAGS	+= -g -Og
	CXXFLAGS+= -g -Og
else ifeq ($(BUILD),release)
	ifneq ($(shell command -v dpkg-buildflags 2>/dev/null),)
		CFLAGS	+= $(shell dpkg-buildflags --get CFLAGS)
		CXXFLAGS+= $(shell dpkg-buildflags --get CXXFLAGS)
		CPPFLAGS+= $(shell dpkg-buildflags --get CPPFLAGS)
		ifneq ($(PLATFORM),windows)
			LDFLAGS	+= $(shell dpkg-buildflags --get LDFLAGS)
		endif
	else
		CFLAGS   += -g -O2 -Werror=implicit-function-declaration -fstack-protector-strong -fstack-clash-protection -Wformat -Werror=format-security -fcf-protection
		CXXFLAGS += -g -O2 -fstack-protector-strong -fstack-clash-protection -Wformat -Werror=format-security -fcf-protection
		CPPFLAGS += -Wdate-time -D_FORTIFY_SOURCE=2
		ifneq ($(PLATFORM),windows)
			LDFLAGS	+= -Wl,-z,relro
		endif
	endif
else
	$(error Unknown build type: $(BUILD))
endif

SRCS	:= $(shell find $(SRC) -type f \( -name "*.c" -o -name "*.cpp" \))
OBJS	:= $(patsubst $(SRC)/%,$(OBJ)/%,$(SRCS))
OBJS	:= $(OBJS:.c=.o)
OBJS	:= $(OBJS:.cpp=.o)

MKDIR	:= mkdir -p
RM	:= rm -rf

EXEC	:= DroneWarfare

all: $(BIN)/$(EXEC)

$(BIN)/$(EXEC): $(OBJS)
	@$(MKDIR) $(BIN)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(OBJ)/%.o: $(SRC)/%.c
	@$(MKDIR) $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(OBJ)/%.o: $(SRC)/%.cpp
	@$(MKDIR) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

run: $(BIN)/$(EXEC)
	$(BIN)/$(EXEC)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(BIN)

re: fclean all

.PHONY: all run clean fclean re
