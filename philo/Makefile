NAME = philo

OBJ_DIR = obj

UTILS_SRC = ft_atoll.c \
            get_time.c \
            is_not_numeric.c

SRCS = $(addprefix utils/, $(UTILS_SRC)) \
       validate_arguments.c \
       parse_data.c \
       allocate_resources.c \
       init_mutexes_and_philos.c \
       write_status.c \
       philo_routine.c \
       start_simulation.c \
       observer.c \
       cleanup.c \
       main.c

OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

CC = cc
CFLAGS = -Wall -Wextra -Werror
INCLUDE_FLAGS = -I. -I./utils
CFLAGS += -g
# CFLAGS += -g -fsanitize=thread
CFLAGS += $(INCLUDE_FLAGS)

RM = rm -f

all: $(NAME)

$(NAME): $(OBJS)
	@echo "Linking $(NAME)..."
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "\033[32mSuccessfully built $(NAME)!\033[0m"

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(@D)
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c $< -o $@
# 	@$(CC) $(CFLAGS) -fsanitize=thread -c $< -o $@

clean:
	@$(RM) -r $(OBJ_DIR)
	@echo "Cleaned object files."

fclean: clean
	@$(RM) $(NAME)
	@echo "Removed binary: $(NAME)."

re: fclean all

.PHONY: all clean fclean re