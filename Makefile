CXX = g++

SRC_DIR = src
BUILD_DIR = build
INC_DIR = include

SDL_CFLAGS  = $(shell pkg-config --cflags sdl2 SDL2_image SDL2_ttf)
SDL_LDFLAGS    = $(shell pkg-config --libs sdl2 SDL2_image SDL2_ttf)

CXXFLAGS = -Wall -O2 -std=c++20 -I$(INC_DIR) $(SDL_CFLAGS)
CXXFLAGS += -Werror=return-type

SRC = $(wildcard $(SRC_DIR)/*.cpp)

OBJS = $(SRC:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

TARGET = game

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(SDL_LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET)