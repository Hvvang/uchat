APP = uchat_server

CC = clang
CFLAGS = -g -std=c11 $(addprefix -W, all extra pedantic) #Add error
DFLAGS = -fsanitize=address -fsanitize=undefined
GLIB = `pkg-config --cflags --libs glib-2.0  --libs gio-2.0 --libs gtk+-3.0 --libs json-glib-1.0`
cJSON = -I ../json/inc src/*.c 

all:
		@$(CC) $(CFLAGS) $(DFLAGS) $(GLIB) $(cJSON) -lsqlite3 ../json/src/*.c -I inc -o  $(APP)
