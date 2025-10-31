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

MKDIR	:= mkdir -p
RM	:= rm -rf

SRC	:= src
INC	:= include
OBJ	:= build
BIN	:= bin
LIB	:= lib

EXEC	:= DroneWarfare

CC	?= gcc
CXX	?= g++
CPPFLAGS:= -Wall -Wextra -I$(INC)
CFLAGS  := -std=c11
CXXFLAGS:= -std=c++11
LDFLAGS := -Bstatic -L$(LIB)/$(PLATFORM) -lSDL2 -lSDL2_image -lSDL2_mixer

ifeq ($(PLATFORM),windows)
	CC	:= x86_64-w64-mingw32-gcc
	CXX	:= x86_64-w64-mingw32-g++
	LDFLAGS += -lmingw32 -lsetupapi -lole32 -loleaut32 -limm32 \
	-lversion -lwinmm -lgdi32 -ldxguid -luser32 -lkernel32 -mwindows
else ifeq ($(PLATFORM),linux)
else
	$(error Unsupported target platform: $(PLATFORM))
endif

SRCS	:= $(shell find $(SRC) -type f \( -name "*.c" -o -name "*.cpp" \))
OBJS	:= $(patsubst $(SRC)/%,$(OBJ)/%,$(SRCS))
OBJS	:= $(OBJS:.c=.o)
OBJS	:= $(OBJS:.cpp=.o)

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
