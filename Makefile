NAME = libftpp.a

CXX = g++
AR = ar
CXXFLAGS = -std=c++20 -Wall -Wextra -Werror -I$(INC_DIR) -I$(TEMP_DIR)

SRC_DIR = src
OBJ_DIR = obj
INC_DIR = include
TEMP_DIR = template
TEST_DIR = tests

SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC))
DEP = $(wildcard $(INC_DIR)/*.hpp) $(wildcard $(TEMP_DIR)/*.tpp)

TEST_SRC = $(wildcard $(TEST_DIR)/*.cpp)
TEST_BIN = test_exec

all: $(NAME)

$(NAME): $(OBJ)
	$(AR) rcs $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEP)
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TEST_BIN): $(TEST_SRC) $(NAME)
	$(CXX) $(CXXFLAGS) $(TEST_SRC) $(NAME) -o $@

test: $(TEST_BIN)

run: $(TEST_BIN)
	./$(TEST_BIN)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)
	rm -f $(TEST_BIN)

re: fclean all

.PHONY: all test run clean fclean re