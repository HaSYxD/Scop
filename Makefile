NAME = scope
OBJDIR = build
SRC = src/main.cpp src/Shader.cpp src/RenderEngine.cpp
SRCEXT = .cpp
OBJEXT = .o
OBJ = $(SRC:$(SRCEXT)=$(OBJEXT))
CC = c++
RM = rm -f
INCLUDE = -I./include -I./libs
FLAGS = -Wall -Werror -Wextra -g
LIBS = -lglfw -ldl -lX11

#=====================================
#===**DO NOT EDIT AFTER THIS LINE**===
#=====================================

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(FLAGS) $(INCLUDE) $(LIBS) $(OBJ) libs/glad/glad.c -o $(NAME)

%$(OBJEXT): %$(SRCEXT)
	$(CC) $(FLAGS) $(INCLUDE) -c $< -o $@

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all
