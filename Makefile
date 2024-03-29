# assumes a desktop with the raylib library installed and the emscripten sdk
# i.e., `brew install raylib` for desktop
# for web, `brew install emscripten` and then download the wasm build of raylib into dep:
# curl -sL https://github.com/raysan5/raylib/releases/download/5.0/raylib-5.0_webassembly.zip | bsdtar -x -C ./dep
# then run `TARGET=desktop make` or `TARGET=wasm make` as needed

# shared variables
BUILD_DIR := ./build/$(TARGET)
SRC_DIRS := ./src
BIN_DIR := ./bin
ASSETS_DIR := ./assets
SRCS := $(shell find $(SRC_DIRS) -name '*.cpp')
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
HTML_SRCS := $(shell find $(SRC_DIRS) -name '*.html')
JS_SRCS := $(shell find $(SRC_DIRS) -name '*.js')

# platform-specific variables
DESKTOP_CXX := clang++
DESKTOP_PREFIX := /opt/homebrew
DESKTOP_CXXFLAGS := -I $(DESKTOP_PREFIX)/include -std=c++20
DESKTOP_LDFLAGS := -L $(DESKTOP_PREFIX)/lib -lraylib -lm
DESKTOP_TARGET_EXEC := app.desktop.bin

WASM_CXX := emcc
WASM_PREFIX := ./dep
WASM_RAYLIB_PATH := $(WASM_PREFIX)/raylib-5.0_webassembly
WASM_CXXFLAGS := -I $(WASM_RAYLIB_PATH)/include
WASM_LDFLAGS := -L $(WASM_RAYLIB_PATH)/lib \
	-lraylib \
	-lm \
	-s USE_GLFW=3 \
	-s GL_ENABLE_GET_PROC_ADDRESS \
	-s WASM=1 \
	-s MODULARIZE=1 \
	-s EXPORT_ES6=1 \
	-s ENVIRONMENT=web \
	-s EXPORTED_RUNTIME_METHODS=['FS'] \
	--embed-file ./assets \
	-O3
WASM_TARGET_EXEC := index.js
WASM_PARCEL_FLAGS := --no-source-maps --no-cache --dist-dir $(BUILD_DIR) 

ifeq ($(TARGET),wasm)
	CXX=$(WASM_CXX)
	CXXFLAGS=$(WASM_CXXFLAGS)
	LDFLAGS=$(WASM_LDFLAGS)
	TARGET_EXEC=$(WASM_TARGET_EXEC)
else
	CXX=$(DESKTOP_CXX)
	CXXFLAGS=$(DESKTOP_CXXFLAGS)
	LDFLAGS=$(DESKTOP_LDFLAGS)
	TARGET_EXEC=$(DESKTOP_TARGET_EXEC)
endif

ifeq ($(TARGET),wasm)
web: parcel
	cp $(shell find ./build/wasm -name '*.*.js' -o -name '*.html' -o -name '*.wasm') bin

parcel: $(HTML_SRCS) $(JS_SRCS) $(BUILD_DIR)/$(TARGET_EXEC)
	mkdir -p $(BUILD_DIR)
	npx parcel build $(WASM_PARCEL_FLAGS) $(HTML_SRCS)

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CXX) $(OBJS) -o $(BUILD_DIR)/$(TARGET_EXEC) $(LDFLAGS)
else
bin/$(TARGET_EXEC): $(OBJS)
	mkdir -p $(dir $@)
	$(CXX) $(OBJS) -o bin/$(TARGET_EXEC) $(LDFLAGS)
	cp -r $(ASSETS_DIR) bin
endif

$(BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)/*
	rm -rf $(BIN_DIR)/*