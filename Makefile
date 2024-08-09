NAME = minishell
CC = cc
#CFLAGS = -Wall -Wextra -Werror -g
CFLAGS = -g
# Source files
SRC_DIR = src
LIB = ./libft/libft.a
SRCS = main exit_handle tokenizer states redirection init \
		utils parser\

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

BANNER = \
"$(GREEN)\
\n  __  __          _____ _          _ _ \n\
 |  \/  |        / ____| |        | | |\n\
 | \  / |___  __| (___ | |__   ___| | |\n\
 | |\/| / _ \/ _ \___ \| '_ \ / _ \ | |\n\
 | |  | |__/  /  ____| ) | | | |__/ | |\n\
 |_|  |_|\___|  |_____/|_| |_|\___|_|_|\n\
$(DEFAULT)"

all: banner lib $(OBJ_DIR) $(NAME)

lib:
		make -s -C libft

$(OBJ_DIR):
		mkdir -p $(OBJ_DIR)

$(NAME): $(OBJ)
		@echo "\033[32mCompiling $(NAME) 🚀"
		$(CC) $(CFLAGS) $(OBJ) $(LIB) -lreadline -o $(NAME)
		@echo "$(GREEN)$(NAME) compilata con successo❗️ 📁$(DEFAULT)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
		$(CC) $(CFLAGS) -c $< -o $@

clean:
		$(RM) $(OBJ)
		@echo "$(CIANO)file oggetto di $(NAME) rimossi con successo❗️ 🪦$(DEFAULT)"

fclean: clean
		$(RM) $(NAME)
		$(RM) -r $(OBJ_DIR)
		cd libft && make fclean
		@echo "$(CIANO)directory OBJ rimossa...$(DEFAULT)"
		@echo "$(RED)$(NAME) rimossa con successo❗️ ❌$(DEFAULT)"

re: fclean all

banner:
		@echo $(BANNER)