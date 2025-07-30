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

# Définition d'un flag de liaison spécifique au système
ifeq ($(PLATFORM),windows)
	SYS_LFLAGS = -lmingw32
else ifeq ($(PLATFORM),linux)
	SYS_LFLAGS = -ldl
else
	SYS_LFLAGS =
endif

# ----------------------------------------------------------------
# Répertoires
SRC   = ./src/
INC   = ./include/
OBJ   = ./build/
BIN   = ./bin/
LIB   = ./lib/$(PLATFORM)/

# Nom de l'exécutable
EXEC  = DroneWarfare

# Compilateur
CC	= gcc

# Commandes système
MKDIR = mkdir -p
RM	= rm -rf

# Inclusion optionnelle de variables locales
-include Makefile.local

# Configuration de build : release par défaut, autres modes via BUILD=debug ou BUILD=sanitize
BUILD ?= release

ifeq ($(BUILD),debug)
	CFLAGS = -Wall -Wextra -Wconversion -Wshadow -Wformat=2 -std=c11 -I$(INC) -g -O0 -MMD
	LFLAGS = -Bstatic -lSDL2 -lSDL2_image -lSDL2_mixer -Bdynamic -lm $(SYS_LFLAGS)
else ifeq ($(BUILD),sanitize)
	CFLAGS = -Wall -Wextra -Wconversion -Wshadow -Wformat=2 -std=c11 -I$(INC) -g -O1 -fsanitize=address,undefined -MMD
	LFLAGS = -flto -fsanitize=address,undefined -Bstatic -lSDL2 -lSDL2_image -lSDL2_mixer -Bdynamic -lm $(SYS_LFLAGS)
else
	CFLAGS = -Wall -Wextra -std=c11 -I$(INC) -O2 -flto -fstack-protector-strong -MMD
	LFLAGS = -Bstatic -lSDL2 -lSDL2_image -lSDL2_mixer -Bdynamic -lm $(SYS_LFLAGS)
endif

# Détection automatique des fichiers sources et génération des objets correspondants
SRCFILES = $(wildcard $(SRC)*.c)
OBJS	 = $(patsubst $(SRC)%.c,$(OBJ)%.o,$(SRCFILES))
DEPS	 = $(OBJS:.o=.d)

.PHONY: all run clean fclean re help install test package

all: $(BIN)$(EXEC)

# Règle générique pour la compilation des fichiers .c
$(OBJ)%.o: $(SRC)%.c
	@$(MKDIR) $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN)$(EXEC): $(OBJS)
	@$(MKDIR) $(BIN)
	$(CC) $(CFLAGS) $(OBJS) -L$(LIB) -o $(BIN)$(EXEC) $(LFLAGS)
	@$(MAKE) postbuild

# Inclusion automatique des dépendances générées
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
	@echo "  make			: Compile le projet en mode release par défaut."
	@echo "  make BUILD=debug	: Compile en mode debug avec warnings étendus."
	@echo "  make BUILD=sanitize	: Compile avec address/undefined sanitizers."
	@echo "  make run		: Exécute le binaire."
#	@echo "  make test		: Lance les tests (voir cible 'test')."
#	@echo "  make package		: Crée une archive du projet (tar.gz)."
	@echo "  make clean		: Supprime uniquement les fichiers objets."
	@echo "  make fclean		: Supprime les fichiers objets et le binaire."
	@echo "  make re		: Reconstruit entièrement le projet."
	@echo "  make help		: Affiche ce message d'aide."
	@echo "Utilise 'make -jN' pour compiler en parallèle (N = nombre de jobs simultanés)."

#install: $(BIN)$(EXEC)
#   @echo "Installation de $(EXEC) dans /usr/local/bin ..."
#   install -Dm755 $(BIN)$(EXEC) /usr/local/bin/$(EXEC)

#test: $(BIN)$(EXEC)
#   @echo "Lancement des tests..."
#   @./tests/run_tests.sh

#package: $(BIN)$(EXEC)
#   @echo "Création de l'archive du projet..."
#   @tar czvf $(EXEC).tar.gz $(BIN) $(OBJ) $(SRC) $(INC) Makefile

# Règle conditionnelle pour copier les DLL sous Windows après la compilation
ifeq ($(PLATFORM),windows)
postbuild:
#	@echo "Copie des DLLs Windows dans bin/"
	@cp $(LIB)*.dll $(BIN)
else
postbuild:
	@true
endif

