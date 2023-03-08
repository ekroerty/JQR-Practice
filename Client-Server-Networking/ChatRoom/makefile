CFLAGS 	= -Wall -Werror -Wpedantic -Wextra
LDLIBS 	= -lpthread

# Automatic project management
SRCDIR 	= src
OBJDIR	= build
BINDIR  = bin
SRCS	= $(wildcard $(SRCDIR)/*.c)
OBJS	= $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))
BIN    	= $(BINDIR)/server

all: $(BIN)

$(BIN): $(OBJS)
	mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDLIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

debug: CFLAGS += -g
debug: $(BIN)

clean:
	$(RM) $(BIN) $(OBJDIR)/*.o

# server: ./src/server.c ./src/htable.c ./src/tpool.c ./src/queue.c ./src/server_client_helper.c ./src/list.c
# 	gcc -g ./include/server.h ./src/htable.c ./src/server.c ./src/tpool.c ./src/queue.c ./src/list.c ./src/server_client_helper.c -o ./build/server