# Makefile pour Windows (MinGW)

CC = gcc
TARGET = hunter_bird.exe
SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include

# Chemins SDL2 (à ajuster selon votre installation)
SDL2_INC = -Iinclude -Ilibs/SDL2/include -Ilibs/SDL2_image/include -Ilibs/SDL2_ttf/include -Ilibs/SDL2_mixer/include
SDL2_LIB = -Llibs/SDL2/lib -Llibs/SDL2_image/lib -Llibs/SDL2_ttf/lib -Llibs/SDL2_mixer/lib
LIBS = -lSDL2 -lSDL2main -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lm

CFLAGS = -Wall -Wextra -g $(SDL2_INC)
LDFLAGS = $(SDL2_LIB) $(LIBS)

SOURCES = $(SRC_DIR)/main.c $(SRC_DIR)/game.c $(SRC_DIR)/bird.c $(SRC_DIR)/player.c $(SRC_DIR)/level.c $(SRC_DIR)/hud.c $(SRC_DIR)/menu.c $(SRC_DIR)/sound.c
OBJECTS = $(BUILD_DIR)/main.o $(BUILD_DIR)/game.o $(BUILD_DIR)/bird.o $(BUILD_DIR)/player.o $(BUILD_DIR)/level.o $(BUILD_DIR)/hud.o $(BUILD_DIR)/menu.o $(BUILD_DIR)/sound.o

all: $(BUILD_DIR) $(TARGET)

$(BUILD_DIR):
	if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)

$(TARGET): $(OBJECTS)
	$(CC) $^ -o $@ $(LDFLAGS)
	@echo Compilation terminee !

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	if exist $(BUILD_DIR) rmdir /s /q $(BUILD_DIR)
	if exist $(TARGET) del $(TARGET)
	@echo Nettoyage termine

run: all
	$(TARGET)

rebuild: clean all

.PHONY: all clean run rebuild