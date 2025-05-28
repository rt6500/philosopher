CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread -g
NAME = philo

SOURCES = \
src/main.c \
src/create_and_join_threads.c \
src/dinner.c \
src/philo_actions.c \
src/philo_actions_eat_utilis.c \
src/getters_setters.c \
src/init.c \
src/monitor.c \
src/sync_utilis.c \
src/thread_utilis.c \
src/utilis_thread_mutex.c \
src/utilis_timekeeper.c \
src/utilis.c \
src/validate_input.c \
src/write.c  

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