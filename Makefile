NAME		=	webserv

SRCDIR		=	./src/
UTILDIR		=	$(SRCDIR)util/
SOCKETDIR	=	$(SRCDIR)socket/
REQUESTDIR	=	$(SRCDIR)request/
BINDIR		=	./bin/
INCLUDEDIR	=	./include/

SRCFILES	=	webserv.cpp		\
			log.cpp			\
			ip.cpp			\
			socket.cpp		\
			parse_head.cpp		\
			parse_request.cpp	\

BINFILES	=	$(notdir $(SRCFILES:.cpp=.o))

SRCS		=	$(addprefix $(SRCDIR), $(SRCFILES))
BINS		=	$(addprefix $(BINDIR), $(BINFILES))

CC		=	c++
CFLAGS		=	-Wall -Wextra -Werror
RM		=	rm -rf

all:		$(NAME)

$(NAME):	$(BINS)
			$(CC) $(CFLAGS) $(BINS) -o $(NAME)

$(BINDIR)%.o:	$(SRCDIR)%.cpp
			@mkdir -p $(BINDIR)
			$(CC) $(CFLAGS) -I $(INCLUDEDIR) -c $< -o $@

$(BINDIR)%.o:	$(UTILDIR)%.cpp
			@mkdir -p $(BINDIR)
			$(CC) $(CFLAGS) -I $(INCLUDEDIR) -c $< -o $@

$(BINDIR)%.o:	$(SOCKETDIR)%.cpp
			@mkdir -p $(BINDIR)
			$(CC) $(CFLAGS) -I $(INCLUDEDIR) -c $< -o $@

$(BINDIR)%.o:	$(REQUESTDIR)%.cpp
			@mkdir -p $(BINDIR)
			$(CC) $(CFLAGS) -I $(INCLUDEDIR) -c $< -o $@

clean:
			$(RM) $(NAME)

fclean:		clean
			$(RM) $(BINDIR)

re:		fclean all

.PHONY:		all clean fclean re
