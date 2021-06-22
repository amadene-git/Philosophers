NAME 	=	philo

SRCS	=	srcs/main.c\
			srcs/time.c\

OBJS	=	${SRCS:.c=.o}

FLAG	=	-Wall -Wextra -Werror

HEADER	=	-I includes

CC		=	clang

########################################################

.c.o	:	
				${CC} ${FLAG} ${HEADER} -c $< -o ${<:.c=.o}

all		:	${NAME}

${NAME}	:	${OBJS}
				${CC}  ${FLAG} -o ${NAME} ${OBJS}


clean	:
				rm -rf ${OBJS}
fclean	:	clean
				rm -rf ${NAME}
re		:	fclean
				@make

.PHONY	:	clean fclean re make