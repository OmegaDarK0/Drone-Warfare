# Détection du système d'exploitation
ifeq ($(OS),Windows_NT)
	PLATFORM = windows
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		PLATFORM = linux
	else ifeq ($(UNAME_S),Darwin)
		PLATFORM = macos
	else
		PLATFORM = unknown
	endif
endif

MAKE := make

# Flags système
ifeq ($(PLATFORM),windows)
	SYS_LFLAGS = -lmingw32 -mwindows
else ifeq ($(PLATFORM),linux)
	SYS_LFLAGS = -ldl
else
	SYS_LFLAGS =
endif

# Répertoires
SRC   = ./src/
INC   = ./include/
OBJ   = ./build/
BIN   = ./bin/
LIB   = ./lib/$(PLATFORM)/

EXEC  = DroneWarfare
CC	= gcc
MKDIR = mkdir -p
RM	= rm -rf

-include Makefile.local

BUILD ?= release

ifeq ($(BUILD),debug)
	CFLAGS = -Wall -Wextra -Wconversion -Wshadow -Wformat=2 -std=c11 -I$(INC) -g -O0 -MMD
	LFLAGS = -lSDL2 -lSDL2_image -lSDL2_mixer -lm $(SYS_LFLAGS)
else ifeq ($(BUILD),sanitize)
	CFLAGS = -Wall -Wextra -Wconversion -Wshadow -Wformat=2 -std=c11 -I$(INC) -g -O1 -fsanitize=address,undefined -MMD
	LFLAGS = -lSDL2 -lSDL2_image -lSDL2_mixer -lm $(SYS_LFLAGS)
else
	CFLAGS = -Wall -Wextra -std=c11 -I$(INC) -O2 -flto -fstack-protector-strong -ffunction-sections -fdata-sections -MMD
	LFLAGS = -Bstatic -lSDL2 -lSDL2_image -lSDL2_mixer -Bdynamic -lm $(SYS_LFLAGS) -Wl,--gc-sections -s
endif

SRCFILES = $(wildcard $(SRC)*.c)
OBJS	 = $(patsubst $(SRC)%.c,$(OBJ)%.o,$(SRCFILES))
DEPS	 = $(OBJS:.o=.d)

.PHONY: all run clean fclean re help upx

all: $(BIN)$(EXEC)

$(OBJ)%.o: $(SRC)%.c
	@$(MKDIR) $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN)$(EXEC): $(OBJS)
	@$(MKDIR) $(BIN)
	$(CC) $(CFLAGS) $(OBJS) -L$(LIB) -o $(BIN)$(EXEC) $(LFLAGS)
	@$(MAKE) postbuild

-include $(DEPS)

run: $(BIN)$(EXEC)
	$(BIN)$(EXEC)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(BIN)

re: fclean all

help:
	@echo "Commandes disponibles :"
	@echo "  make                 : Compile en mode release."
	@echo "  make BUILD=debug     : Compile en mode debug."
	@echo "  make BUILD=sanitize  : Compile avec sanitizers."
	@echo "  make run             : Exécute le binaire."
	@echo "  make clean           : Supprime les objets."
	@echo "  make fclean          : Supprime objets et binaire."
	@echo "  make re              : Reconstruit le projet."
	@echo "  make upx             : Compresse le binaire Linux avec UPX."

ifeq ($(PLATFORM),windows)
postbuild:
	@cp $(LIB)*.dll $(BIN)
else
postbuild:
	@true
endif

