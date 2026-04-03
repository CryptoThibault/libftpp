NAME = libftpp.a

CXX = g++
AR = ar
CXXFLAGS = -std=c++20 -Wall -Wextra -Werror -I$(INC_DIR) -I$(TEMP_DIR)

SRC_DIR = src
OBJ_DIR = obj
INC_DIR = include
TEMP_DIR = template
TEST_DIR = test

SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC))
DEP = $(wildcard $(INC_DIR)/*.hpp) $(wildcard $(TEMP_DIR)/*.tpp)

TEST_SRC = $(wildcard $(TEST_DIR)/main_*.cpp)
TEST_SRC := $(filter-out $(TEST_DIR)/main_client.cpp $(TEST_DIR)/main_server.cpp, $(TEST_SRC))
TEST_BIN = $(patsubst $(TEST_DIR)/%.cpp, $(TEST_DIR)/%, $(TEST_SRC))
VERBOSE ?= 0
HIDE_OUTPUT := $(TEST_DIR)/main_worker_pool.cpp $(TEST_DIR)/main_persistent_worker.cpp

RED = \033[0;31m
GREEN = \033[0;32m
BROWN = \033[0;33m
BLUE = \033[0;34m
GRAY = \033[0;90m
RESET = \033[0m

all: $(NAME)

$(NAME): $(OBJ)
	$(AR) rcs $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEP)
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TEST_DIR)/%: $(TEST_DIR)/%.cpp $(NAME)
	$(CXX) $(CXXFLAGS) $< $(NAME) -o $@

run: $(NAME)
	@name=$(filter-out $@,$(MAKECMDGOALS)); \
	if [ -z "$$name" ]; then \
		echo "Usage: make run <test_name>"; \
		exit 1; \
	fi; \
	src="$(TEST_DIR)/main_$$name.cpp"; \
	bin="$(TEST_DIR)/main_$$name"; \
	if [ ! -f "$$src" ]; then \
		echo "Test $$src not found"; \
		exit 1; \
	fi; \
	$(CXX) $(CXXFLAGS) "$$src" $(NAME) -o "$$bin" && "$$bin" \
	"$$bin"; \
	rm -f "$$bin"


test: $(NAME)
	@for src in $(TEST_SRC); do \
		bin="$(TEST_DIR)/$$(basename $$src .cpp)"; \
		bin_name="$$(basename $$bin)"; \
		echo "$(BLUE)[libftpp] build $$bin_name$(RESET)"; \
		if $(CXX) $(CXXFLAGS) "$$src" $(NAME) -o "$$bin"; then \
			echo "$(BROWN)[libftpp] run $$bin_name$(RESET)"; \
			# Déterminer si la sortie doit être masquée \
			hide=0; \
			if [ "$(VERBOSE)" -eq 0 ]; then \
				hide=1; \
			elif echo "$(HIDE_OUTPUT)" | grep -qw "$$bin_name"; then \
				hide=1; \
			fi; \
			if [ $$hide -eq 1 ]; then \
				echo "$(GRAY)[libftpp] output for $$bin_name hidden$(RESET)"; \
				"$$bin" > /dev/null 2>&1; \
			else \
				"$$bin"; \
			fi; \
			status=$$?; \
			if [ $$status -eq 0 ]; then \
				echo "$(GREEN)[libftpp] test $$bin_name passed$(RESET)"; \
			else \
				echo "$(RED)[libftpp] test $$bin_name failed$(RESET)"; \
			fi; \
		else \
			echo "$(RED)[libftpp] build $$bin_name failed$(RESET)"; \
		fi; \
		echo; \
	done
	rm -f $(TEST_BIN)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

%:
	@:

.PHONY: all run test clean fclean re