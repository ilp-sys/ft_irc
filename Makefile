NAME			:= ircserv

CXX 			:= c++
CXXFLAGS  		:= #-std=c++98 -Wall -Wextra -Werror #-v

RM				:= rm
RMFLAGS     	:= -rf

SRCDIR			:= ./srcs/
<<<<<<< HEAD

SRCNAME 		:= Client.cpp Command.cpp CommandInvoker.cpp Join.cpp Pass.cpp\
					Kick.cpp Nick.cpp Notice.cpp Part.cpp Privmsg.cpp \
					Quit.cpp Server.cpp User.cpp main.cpp Channel.cpp Join.cpp Part.cpp

=======

SRCNAME 		:= Client.cpp Command.cpp CommandInvoker.cpp Join.cpp Kick.cpp Nick.cpp Notice.cpp Part.cpp Privmsg.cpp Quit.cpp Server.cpp User.cpp main.cpp Channel.cpp Join.cpp Part.cpp

>>>>>>> 4abb5d70130c821d52d43f04c95f3753cb32d50e
SRCS			:= $(addprefix $(SRCDIR), $(SRCNAME))

OBJDIR			:= ./obj/
OBJNAME			:= $(SRCNAME:.cpp=.o)
OBJS			:= $(addprefix $(OBJDIR), $(OBJNAME))

INCDIR			:= includes/

.PHONY			: all
all				: $(NAME)

$(NAME) 		: $(OBJS)
				$(CXX) $(CXXFLAGS) -I$(INCDIR) $^ -o $@

.PHONY		: clean
clean			:
				$(RM) $(RMFLAGS) $(OBJS)
				$(RM) $(RMFLAGS) $(OBJDIR)

.PHONY			: fclean
fclean			: clean
				$(RM) $(RMFLAGS) $(NAME)

.PHONY			: re
re				: fclean all

$(OBJDIR)%.o:$(SRCDIR)%.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@
