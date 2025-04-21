# Répertoires
SRC = ./src/
INC = ./include/
OBJ = ./build/
BIN = ./bin/
LIB = ./lib/

# Nom de l'exécutable
EXEC = DroneWarfare

# Compilateur et flags
CC = gcc
CFLAGS = -Wall -std=c11 -I$(INC)
LFLAGS = -Bstatic -lSDL2 -lSDL2_image -lSDL2_mixer -Bdynamic -lm -lmingw32

# Liste des fichiers objets (ajustez selon vos modules)
OBJS = \
	$(OBJ)main.o \
	$(OBJ)engine.o \
	$(OBJ)graphics.o \
	$(OBJ)entity.o \
	$(OBJ)entity_list.o \
	$(OBJ)resource_pack.o \
	$(OBJ)hud.o \
	$(OBJ)glyph.o \
	$(OBJ)ui.o \
	$(OBJ)utils.o \
	$(OBJ)score.o \
	$(OBJ)audio.o \
	$(OBJ)images.o
# Ajoutez $(OBJ)debug.o si nécessaire.

.PHONY: all run clean setup

all: setup $(BIN)$(EXEC)

# Création des dossiers cibles
setup:
	mkdir -p $(BIN)
	mkdir -p $(OBJ)

# Règle de liaison
$(BIN)$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -L$(LIB) -o $(BIN)$(EXEC) $(LFLAGS)

# Règles de compilation
$(OBJ)main.o: $(SRC)main.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ)engine.o: $(SRC)engine.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ)entity.o: $(SRC)entity.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ)graphics.o: $(SRC)graphics.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ)entity_list.o: $(SRC)entity_list.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ)resource_pack.o: $(SRC)resource_pack.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ)hud.o: $(SRC)hud.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ)glyph.o: $(SRC)glyph.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ)ui.o: $(SRC)ui.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ)utils.o: $(SRC)utils.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ)score.o: $(SRC)score.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ)audio.o: $(SRC)audio.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ)images.o: $(SRC)images.c
	$(CC) $(CFLAGS) -c $< -o $@

# Optionnel : si vous disposez d'un module debug
$(OBJ)debug.o: $(SRC)debug.c
	$(CC) $(CFLAGS) -c $< -o $@

# Règle pour lancer l'exécutable
run: $(BIN)$(EXEC)
	$(BIN)$(EXEC)

# Nettoyage des dossiers de build et de binaires
clean:
	rm -rf $(OBJ) $(BIN)

