CC = cc
FLAGS = -Wall -Werror -Wextra -g -pthread

SRC_DIR = src/
SRCS = main.c utils.c init.c simulation_utils.c monitoring.c process_utils.c str_utils.c

SRC = $(addprefix $(SRC_DIR), $(SRCS))
OBJS = $(SRC:.c=.o)

HEADER_PATH = include

NAME = philo_bonus

# Default target
all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -I$(HEADER_PATH) -o $(NAME)

# Rule to compile .o files from .c files
$(SRC_DIR)%.o: $(SRC_DIR)%.c
	$(CC) $(FLAGS) -I$(HEADER_PATH) -c $< -o $@

# Clean object files
clean:
	rm -f $(OBJS)

# Clean all build artifacts
fclean: clean
	rm -f $(NAME)

# Rebuild everything
re: fclean all

.PHONY: all clean fclean re
