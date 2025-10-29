ifeq ($(OS),Windows_NT)
	PLATFORM:= windows
else
	PLATFORM:= linux
endif

MKDIR   := mkdir -p
RM      := rm -rf

SRC	:= src
INC	:= include
OBJ	:= build
BIN	:= bin
LIB	:= lib

CFLAGS  := -std=c11 -Wall -Wextra -I$(INC) -g
CXXFLAGS:= -std=c++11 -Wall -Wextra -I$(INC) -g
LDFLAGS := -Bstatic -L$(LIB)/$(PLATFORM) -lSDL2 -lSDL2_image -lSDL2_mixer

ifeq ($(PLATFORM),windows)
	LDFLAGS	+= -lmingw32 -lsetupapi -lole32 -loleaut32 -limm32 -lversion -lwinmm -lgdi32 -ldxguid -luser32 -lkernel32 -mwindows
	CC	:= x86_64-w64-mingw32-gcc
	CXX	:= x86_64-w64-mingw32-g++
	EXEC	:= DroneWarfare.exe
else
	CC	:= gcc
	CXX	:= g++
	EXEC	:= DroneWarfare.elf
endif

SRCS := $(shell find $(SRC) -type f \( -name "*.c" -o -name "*.cpp" \))
OBJS := $(patsubst $(SRC)/%,$(OBJ)/%,$(SRCS))
OBJS := $(OBJS:.c=.o)
OBJS := $(OBJS:.cpp=.o)

all: $(BIN)/$(EXEC)

$(BIN)/$(EXEC): $(OBJS)
	@$(MKDIR) $(BIN)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(OBJ)/%.o: $(SRC)/%.c
	@$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ)/%.o: $(SRC)/%.cpp
	@$(MKDIR) $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(BIN)/$(EXEC)
	$(BIN)/$(EXEC)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(BIN)

re: fclean all

.PHONY: all run clean fclean re
