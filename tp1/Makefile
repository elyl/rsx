NAME=	tcpchat
SRC=	main.c
OBJ=	$(SRC:.c=.o)
CFLAGS=	-W -Wall -lpthread -D_REENTRANT
CC=	gcc
RM=	@rm -fv

$(NAME):	$(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ)

clean:
	$(RM) $(OBJ)

fclean:
	$(RM) $(NAME)

re:	clean fclean $(NAME)
