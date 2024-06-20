CC = gcc

SOURCES = src/main.c src/init/init.c src/structs/structs.c src/snake/snake.c src/cui/cui.c lib/clientAPI/clientAPI.c lib/snakeAPI/snakeAPI.c
OBJS = ${SOURCES:.c=.o}

all: build clean run

build: $(OBJS)
	@$(CC) -o dist/snake $(OBJS) -lpthread

%.o: %.c
	@$(CC) -c $< -o $@ -lpthread

clean: ${OBJS}
	@rm -rf ${OBJS}

run: ./dist/snake
	@./dist/snake

runLeak: dist/snake
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-out.txt ./dist/snake