APP = uchat_client

CC = clang
CFLAGS = -g -std=c11 $(addprefix -W, all extra pedantic) #Add error
DFLAGS = -fsanitize=address -fsanitize=undefined
GLIB = `pkg-config --cflags --libs glib-2.0 --libs gio-2.0 --libs gtk+-3.0`

all:
	@$(CC) $(CFLAGS) $(DFLAGS) $(GLIB) src/*.c src/screens/*.c -I inc -o $(APP)