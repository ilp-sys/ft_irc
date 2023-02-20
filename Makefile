NAME			:= ircserv

CXX 			:= c++
CXXFLAGS  		:= -std=c++98 -Wall -Wextra -Werror

RM				:= rm
RMFLAGS     	:= -rf

SRCDIR			:= ./srcs/
SRCNAME 		:= main.cpp Server.cpp User.cpp CommandInvoker.cpp Nick.cpp Command.cpp
SRCS			:= $(addprefix $(SRCDIR), $(SRCNAME))

OBJDIR			:= ./obj/
OBJNAME			:= $(SRCNAME:.cpp=.o)
OBJS			:= $(addprefix $(OBJDIR), $(OBJNAME))

INCDIR			:= includes/

.PHONY			: all
all				: $(NAME)

$(NAME) 		: $(OBJS)
				$(CXX) -I$(INCDIR) $^ -o $@

.PHONY			: clean
clean			:
				$(RM) $(RMFLAGS) $(OBJS)
				$(RM) $(RMFLAGS) $(OBJDIR)

.PHONY			: fclean
fclean			: clean
				$(RM) $(NAME)

.PHONY			: re
re				: fclean all

$(OBJDIR)%.o:$(SRCDIR)%.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) -c $< -o $@