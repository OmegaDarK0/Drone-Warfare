SRC = ./src/
INC = ./include/
OBJ = ./build/
BIN = ./bin/
LIB = ./lib/

CC = gcc
CFLAGS = -Wall -std=c11
LFLAGS = -Bstatic -lSDL2 -lSDL2_image -lSDL2_mixer -Bdynamic -lm -ldl
EXEC = DroneWarfare

.PHONY: all run clean

all: setup $(EXEC)

setup:
	mkdir -p $(BIN)
	mkdir -p $(OBJ)

run: $(EXEC)
	$(BIN)$<

$(EXEC): main.o engine.o entity.o entity_list.o resource.o hud.o glyph.o utils.o globals.o constants.o debug.o
	$(CC) $(CFLAGS) $(OBJ)* -I $(INC) -L $(LIB) -o $(BIN)$@ $(LFLAGS)

main.o: $(SRC)main.c
	$(CC) $(CFLAGS) $< -c -I $(INC) -o $(OBJ)$@

engine.o: $(SRC)engine.c
	$(CC) $(CFLAGS) $< -c -I $(INC) -o $(OBJ)$@

entity.o: $(SRC)entity.c
	$(CC) $(CFLAGS) $< -c -I $(INC) -o $(OBJ)$@

entity_list.o: $(SRC)entity_list.c
	$(CC) $(CFLAGS) $< -c -I $(INC) -o $(OBJ)$@

resource.o: $(SRC)resource.c
	$(CC) $(CFLAGS) $< -c -I $(INC) -o $(OBJ)$@

hud.o: $(SRC)hud.c
	$(CC) $(CFLAGS) $< -c -I $(INC) -o $(OBJ)$@

glyph.o: $(SRC)glyph.c
	$(CC) $(CFLAGS) $< -c -I $(INC) -o $(OBJ)$@

utils.o: $(SRC)utils.c
	$(CC) $(CFLAGS) $< -c -I $(INC) -o $(OBJ)$@

globals.o: $(SRC)globals.c
	$(CC) $(CFLAGS) $< -c -I $(INC) -o $(OBJ)$@

constants.o: $(SRC)constants.c
	$(CC) $(CFLAGS) $< -c -I $(INC) -o $(OBJ)$@

debug.o: $(SRC)debug.c
	$(CC) $(CFLAGS) $< -c -I $(INC) -o $(OBJ)$@

clean:
	rm -rf $(BIN) $(OBJ)
