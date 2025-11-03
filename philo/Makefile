NAME = philo

OBJ_DIR = obj

PHILO_UTILS = 
PHILO_SRC = 

SRCS = $(addprefix pipex_utils/, $(PIPEX_UTILS)) \
       $(addprefix parse/, $(PARSER_SRC)) \
       main.c

OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

CC = cc
CFLAGS = -Wall -Wextra -Werror
INCLUDE_FLAGS = -I. -I./parse -I./pipex_utils
CFLAGS += -g -g3
CFLAGS += $(INCLUDE_FLAGS)

RM = rm -f

all: $(NAME)

$(NAME): $(OBJS)
	@echo "Linking $(NAME)..."
	@$(CC) $(OBJS) -o $(NAME)
	@echo "\033[32mSuccessfully built $(NAME)!\033[0m"

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(@D)
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@$(RM) -r $(OBJ_DIR)
	@echo "Cleaned object files."

fclean: clean
	@$(RM) $(NAME)
	@echo "Removed binary: $(NAME)."

re: fclean all

.PHONY: all clean fclean re