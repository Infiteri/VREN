CUR_DIR := $(notdir $(CURDIR))
SRC_DIR ?= Source

rwildcard = $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))
CPP_FILES := $(call rwildcard,$(SRC_DIR)/,*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(BIN_OBJ)/$(CUR_DIR)/$(SRC_DIR)/%.o,$(CPP_FILES))
OBJ_DIRS := $(sort $(dir $(OBJ_FILES)))

.PHONY: all scaffold link

all: scaffold $(OBJ_FILES) link

scaffold:
	@mkdir -p $(OBJ_DIRS)

$(BIN_OBJ)/$(CUR_DIR)/$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	g++ -std=c++2a -c -g $< -o $@ -ISource -IVendor/glad/include

link:
	@g++ -o $(TARGET_LIB) $(OBJ_FILES) -L$(BIN) -shared -lglad