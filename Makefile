CC  = g++
CXX = $(CC)

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

OBJ := Grid.o BoundingBox.o ProgramConfig.o
OBJ := $(OBJ:%.o=$(OBJ_DIR)/%.o)

TEST_OBJ := grid_test.o
TEST_OBJ := $(TEST_OBJ:%.o=$(OBJ_DIR)/%.o)

BIN :=
BIN := $(BIN:%=$(BIN_DIR)/%)

TEST := grid_test
TEST := $(TEST:%=$(BIN_DIR)/%)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp dep/%.d
	mkdir -p $(OBJ_DIR)
	$(CXX) $(CFLAGS) -c -o $@ $<

dep/%.d: $(SRC_DIR)/%.cpp
	mkdir -p $(@D)
	$(CXX) -M $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

.PHONY: all clean test

all: $(BIN)
test: $(TEST)

$(BIN_DIR)/grid_test: $(OBJ) $(OBJ_DIR)/grid_test.o
	mkdir -p $(BIN_DIR)
	$(CXX) $^ -o $@

clean:
	rm -fr $(BIN_DIR) $(OBJ_DIR) dep
	find . -iname "*~"|xargs rm

-include $(OBJ:%.o=dep/%.d)
-include $(TEST_OBJ:%.o=dep/%.d)
