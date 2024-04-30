NAME		=	webserv

SRCDIR		=	src/
UTILDIR		=	util/
SOCKETDIR	=	socket/
REQUESTDIR	=	request/
SERVERDIR	=	server/
RESPONSEDIR	=	response/
VHOSTDIR	=	vhost/
INITDIR		=	initialization/
BINDIR		=	./bin/
INCLUDEDIR	=	./include/

## Reset
NC		:= \033[0m# 	Text Reset

# Regular Colors
BLACK	:= \033[0;30m#	Black
RED		:= \033[3;31m#	Red
GREEN	:= \033[3;32m#	Green
BLD_GRN	:= \033[1;32m#	Green but bold
YELLOW	:= \033[3;33m#	Yellow
BLUE	:= \033[1;34m#	Blue
PURPLE	:= \033[3;35m#	Purple
BLD_PUR	:= \033[1;35m#	Purple but bold
CYAN	:= \033[3;36m#	Cyan
WHITE	:= \033[1;37m#	White Reset
NC		:= \033[0m# 	Text Reset

# Regular Colors
BLACK	:= \033[0;30m#	Black
RED		:= \033[3;31m#	Red
GREEN	:= \033[3;32m#	Green
BLD_GRN	:= \033[1;32m#	Green but bold
YELLOW	:= \033[3;33m#	Yellow
BLUE	:= \033[1;34m#	Blue
PURPLE	:= \033[3;35m#	Purple
BLD_PUR	:= \033[1;35m#	Purple but bold
CYAN	:= \033[3;36m#	Cyan
WHITE	:= \033[1;37m#	White

SRCFILES	=	webserv.cpp

UTILFILES	=	log.cpp				\
				read_file.cpp		\

SOCKETFILES	=	socket.cpp

REQFILES	=	parse_head.cpp		\
				parse_request.cpp	\

SERVERFILES	=	Server.cpp			\
				server_util.cpp		\

RESPONFILES	=	response.cpp		\
				response_util.cpp	\

VHOSTFILES	=	VHost.cpp			\
				process_request.cpp	\

INITFILES	=	initConfig.cpp		\
				initUtils.cpp		\
				getFunctions.cpp	\
				fillFunctions.cpp	\
				printFunctions.cpp	\
				parsing/parsing.cpp	\

SRCFILES	+=	$(addprefix $(UTILDIR), $(UTILFILES))
SRCFILES	+=	$(addprefix $(SOCKETDIR), $(SOCKETFILES))
SRCFILES	+=	$(addprefix $(REQUESTDIR), $(REQFILES))
SRCFILES	+=	$(addprefix $(SERVERDIR), $(SERVERFILES))
SRCFILES	+=	$(addprefix $(RESPONSEDIR), $(RESPONFILES))
SRCFILES	+=	$(addprefix $(VHOSTDIR), $(VHOSTFILES))
SRCFILES	+=	$(addprefix $(INITDIR), $(INITFILES))
BINFILES	=	$(SRCFILES:.cpp=.o)

BINS		=	$(addprefix $(BINDIR), $(BINFILES))

CC			=	c++
CFLAGS		=	-Wall -Wextra -Werror -std=c++23
RM			=	rm -rf

all:	$(NAME)

$(NAME):	$(BINS)
	@echo "$(BLD_GRN)Building... 🏗$(NC)"
	@$(CC) $(CFLAGS) $(BINS) -o $(NAME)
	@echo "$(BLD_GRN)Done! 🏠$(NC)"

$(BINDIR)%.o:	$(SRCDIR)%.cpp | $(BINDIR)
	@echo "$(PURPLE)Compiling... $(NC)🧱 $(PURPLE) $< $(NC)"
	@$(CC) $(CFLAGS) -I $(INCLUDEDIR) -c $< -o $@

$(BINDIR):
	@echo "$(YELLOW)Creating folders... $(NC)🗂"
	@mkdir -p $@
	@mkdir -p $@$(UTILDIR)
	@mkdir -p $@$(SOCKETDIR)
	@mkdir -p $@$(REQUESTDIR)
	@mkdir -p $@$(SERVERDIR)
	@mkdir -p $@$(RESPONSEDIR)
	@mkdir -p $@$(VHOSTDIR)
	@mkdir -p $@$(INITDIR)parsing/

clean:
	@echo "$(CYAN)Cleaning...$(NC) 🧹"
	@$(RM) $(NAME)

fclean:		clean
	@echo "$(RED)Destroying...$(NC) 💥"
	@$(RM) $(BINDIR)

re:		fclean all

.PHONY:		all clean fclean re
