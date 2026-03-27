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

TEST_SRC = $(wildcard $(TEST_DIR)/main_*.cpp)
TEST_BIN = $(patsubst $(TEST_DIR)/%.cpp, $(TEST_DIR)/%, $(TEST_SRC))

RED = \033[0;31m
GREEN = \033[0;32m
YELLOW = \033[0;33m
RESET = \033[0m

all: $(NAME)

$(NAME): $(OBJ)
	$(AR) rcs $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEP)
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TEST_DIR)/%: $(TEST_DIR)/%.cpp $(NAME)
	$(CXX) $(CXXFLAGS) $< $(NAME) -o $@

test:
	@for src in $(TEST_SRC); do \
		bin="$(TEST_DIR)/$$(basename $$src .cpp)"; \
		needed=0; \
		for h in $$(grep -o '#include "[^"]*\.hpp"' "$$src" | sed -e 's/#include "//' -e 's/"$$//'); do \
			if [ -z "$$h" ]; then continue; fi; \
			if [ -f "$(INC_DIR)/$$h" ]; then \
				needed=1; \
			elif [ -f "$(INC_DIR)/libftpp.hpp" ] && grep -qF "$$h" "$(INC_DIR)/libftpp.hpp"; then \
				needed=1; \
			else \
				echo "$(RED)[libftpp] skip $$bin (missing header $$h)$(RESET)"; \
				needed=2; break; \
			fi; \
		done; \
		if [ $$needed -eq 2 ]; then echo; continue; fi; \
		echo "$(YELLOW)[libftpp] build $$bin$(RESET)"; \
		if $(CXX) $(CXXFLAGS) "$$src" $(NAME) -o "$$bin"; then \
			echo "$(GREEN)[libftpp] run $$bin$(RESET)"; \
			"$$bin"; \
		else \
			echo "$(RED)[libftpp] skip $$bin (build failed)$(RESET)"; \
		fi; \
		echo; \
	done
	rm -f $(TEST_BIN)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all test clean fclean re