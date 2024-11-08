NAME = minishell
CC = cc
# CFLAGS = -Wall -Wextra -Werror -g -lreadline
CFLAGS = -g -lreadline -Wall -Wextra -Werror
# Source files
SRC_DIR = src
LIB = ./libft/libft.a
SRCS = main init tokenizer/lexer exit_handle \
		tokenizer/lexer_utils tokenizer/token_utils \
		main_utils parser/parser parser/parser_utils \
		executor/exec executor/exec_utils redirect/redirect \
		expander env_list executor/pipe_case builtin/chdir_utils \
		builtin/chdir builtin/builtin executor/pipe_utils \
		builtin/exit builtin/echo builtin/export builtin/unset \
		builtin/pwd builtin/env redirect/expander_doc \
		signals get_next_line get_next_line_utils builtin/utils_builtin2 \
		builtin/utils_builtin builtin/export_utils2 executor/exec_utils2 builtin/export_utils \
		helper_function executor/exec_utils3 builtin/exit_utils helper_functions2 \
		parser/parser_utils2 executor/exec_helpers \
		expander3 expander2 executor/copy_mtx_pipe executor/pipe_utils2 \
		tokenizer/lexer_utils2 parser/redirect_parser executor/new_helpers \
		redirect/redirect_helper redirect/redirect_helper2 \
		 builtin/echo_util \
		main2 free_some expander4 \

SRC = $(addprefix $(SRC_DIR)/, $(addsuffix .c, $(SRCS)))
# Object file generation
OBJ_DIR = obj
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
RM = rm -f

# Colors
DEFAULT = \033[0;39m
GREEN = \033[0;92m
RED = \033[0;91m
CIANO = \033[0;96m
YELLOW = \033[0;93m
BLUE = \033[0;94m
MAGENTA = \033[0;95m
GRAY = \033[0;90m
WHITE = \033[0;97m

#$(DEFAULT)

all: lib $(OBJ_DIR) $(NAME)

val: all
	@valgrind --leak-check=full --show-leak-kinds=all --suppressions=readline.supp --quiet ./$(NAME)

lib:
		make -sC libft

$(OBJ_DIR):
		mkdir -p $(OBJ_DIR)

$(NAME): $(SRC)
		@echo "\033[32mCompiling $(NAME) 🚀"
		$(CC) $(CFLAGS) $(SRC) $(LIB) -lreadline -o $(NAME)
		@echo "$(GREEN)$(NAME) compilata con successo❗️ 📁$(DEFAULT)"

# $(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
# 		@mkdir -p $(dir $@)
# 		$(CC) $(CFLAGS) -c $< -o $@

clean:
# $(RM) $(OBJ)
		@echo "$(CIANO)file oggetto di $(NAME) rimossi con successo❗️ 🪦$(DEFAULT)"

fclean: clean
		$(RM) $(NAME)
		$(RM) -r $(OBJ_DIR)
		cd libft && make fclean
		@echo "$(CIANO)directory OBJ rimossa...$(DEFAULT)"
		@echo "$(RED)$(NAME) rimossa con successo❗️ ❌$(DEFAULT)"

re: fclean all
