CC = cc
CFLAGS = -Wall -Wextra -Werror
NAME = philo
SOURCES = $(wildcard src/*.c)
OBJECTS = $(patsubst src/%.c, objs/%.o, $(SOURCES))
$(info OBJECTS = $(OBJECTS))
all : objs $(NAME)
$(NAME) : $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(NAME)
objs :
	mkdir -p objs
objs/%.o : src/%.c
	$(CC) $(CFLAGS) -c $< -o $@
clean :
	rm -f $(OBJECTS)
	rm -rf objs
fclean : clean
	rm -f $(NAME)
re : fclean all