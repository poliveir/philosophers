#VARIABLES:

NAME			= philo

SRCS_DIR		= ./source_files

HDRS_DIR		= ./header_files

SRCS			= source_files/philosophers.c source_files/error_handling.c source_files/utils.c source_files/actions.c

OBJS			= ${SRCS:.c=.o}

CC				= gcc

CFLAGS			= -fsanitize=address -Wall -Werror -Wextra -pthread

RM				= rm -rf

#RULES:

.SILENT:
all: ${NAME}

$(NAME): ${OBJS}
	${CC} ${CFLAGS} ${SRCS} -o ${NAME}

clean:
	${RM} *.o ${SRCS_DIR}/*.o

fclean: clean
	${RM} ${NAME}

re: fclean all

.PHONY: all clean fclean re