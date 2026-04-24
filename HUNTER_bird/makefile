CC = gcc
TARGET = hunter_bird
SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include

CFLAGS = -Wall -Wextra -g -I$(INCLUDE_DIR) `sdl2-config --cflags`
LIBS = `sdl2-config --libs` -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lm

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES))

all: $(BUILD_DIR) $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET): $(OBJECTS)
	$(CC) $^ -o $@ $(LIBS)
	@echo "✅ Compilation réussie ! Lance avec : ./$(TARGET)"

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET)
	@echo "🧹 Nettoyage terminé"

run: all
	./$(TARGET)

rebuild: clean all

.PHONY: all clean run rebuild