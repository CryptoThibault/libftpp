NAME = libftpp
ARCHIVE = $(NAME).a

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
TEST_SRC := $(filter-out $(TEST_DIR)/main_thread_safe_iostream.cpp $(TEST_DIR)/main_client.cpp $(TEST_DIR)/main_server.cpp, $(TEST_SRC))
HIDE_OUTPUT := $(TEST_DIR)/main_worker_pool.cpp $(TEST_DIR)/main_persistent_worker.cpp
VERBOSE ?= 1
VALGRIND ?= 0

RED = \033[0;31m
GREEN = \033[0;32m
BROWN = \033[0;33m
BLUE = \033[0;34m
GRAY = \033[0;90m
RESET = \033[0m

all: $(ARCHIVE)

$(ARCHIVE): $(OBJ)
	$(AR) rcs $(ARCHIVE) $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEP)
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(ARCHIVE)
	@name=$(filter-out $@,$(MAKECMDGOALS)); \
	if [ -z "$$name" ]; then \
		echo "$(RED)[${NAME}] Usage: make run <test_name>$(RESET)"; \
		exit 1; \
	fi; \
	src="$(TEST_DIR)/main_$$name.cpp"; \
	bin="$(TEST_DIR)/main_$$name"; \
	if [ ! -f "$$src" ]; then \
		echo "$(RED)[${NAME}] file $$src not found$(RESET)"; \
		exit 1; \
	fi; \
	if ! $(CXX) $(CXXFLAGS) "$$src" $(ARCHIVE) -o "$$bin"; then \
		echo "$(RED)[${NAME}] build $$bin failed$(RESET)"; \
		exit 1; \
	fi; \
	hide=0; \
	if [ "$(VERBOSE)" = "0" ]; then hide=1; fi; \
	if [ "$(VALGRIND)" = "1" ]; then \
		if [ $$hide -eq 1 ]; then \
			valgrind --leak-check=full "$$bin" > /dev/null; \
		else \
			valgrind --leak-check=full "$$bin"; \
		fi; \
	else \
		if [ $$hide -eq 1 ]; then \
			"$$bin" > /dev/null 2>&1; \
		else \
			"$$bin"; \
		fi; \
	fi; \
	rm -f "$$bin"

test: $(ARCHIVE)
	@for src in $(TEST_SRC); do \
		bin="$(TEST_DIR)/$$(basename $$src .cpp)"; \
		bin_name="$$(basename $$bin)"; \
		echo "$(BLUE)[${NAME}] build $$bin_name$(RESET)"; \
		if $(CXX) $(CXXFLAGS) "$$src" $(ARCHIVE) -o "$$bin"; then \
			echo "$(BROWN)[${NAME}] run $$bin_name$(RESET)"; \
			hide=0; \
			if [ "$(VERBOSE)" -eq 0 ] || echo "$(HIDE_OUTPUT)" | grep -qw "$$bin_name"; then \
				hide=1; \
			fi; \
			if [ "$(VALGRIND)" = "1" ]; then \
				if [ $$hide -eq 1 ]; then \
					valgrind --leak-check=full "$$bin" > /dev/null; \
				else \
					valgrind --leak-check=full "$$bin"; \
				fi; \
			else \
				if [ $$hide -eq 1 ]; then \
					echo "$(GRAY)[${NAME}] output for $$bin_name hidden$(RESET)"; \
					"$$bin" > /dev/null 2>&1; \
				else \
					"$$bin"; \
				fi; \
			fi; \
			status=$$?; \
			if [ $$status -eq 0 ]; then \
				echo "$(GREEN)[${NAME}] test $$bin_name passed$(RESET)"; \
			else \
				echo "$(RED)[${NAME}] test $$bin_name failed$(RESET)"; \
			fi; \
		else \
			echo "$(RED)[${NAME}] build $$bin_name failed$(RESET)"; \
		fi; \
		rm -f "$$bin" 2>/dev/null; \
		echo; \
	done

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(ARCHIVE)
	rm -f log.txt

re: fclean all

%:
	@:

.PHONY: all run test clean fclean re