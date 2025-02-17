CC = g++
CFLAGS = -Wall -Wextra -Wpedantic -Wshadow -Wformat=2 -fsanitize=address,undefined -g
MKDIR = mkdir -p

all: acl

acl:
	${MKDIR} ./bin/
	${CC} ${CFLAGS} ./acl.cpp -o ./bin/acl
	
clean:
	rm -rf bin