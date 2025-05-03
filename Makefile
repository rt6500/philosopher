CC = cc
CFLAGS = -Wall -Wextra -Werror -g
NAME = philo

SOURCES = src/main.c src/dinner.c src/getters_setters.c src/init.c \
src/sync_utilis.c src/utilis_dinner.c src/utilis_free_malloc.c \
src/utilis_thread_mutex.c src/utilis_timekeeper.c src/validate_input.c \
src/write.c src/monitor.c

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