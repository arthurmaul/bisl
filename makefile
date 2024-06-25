CC = ccache distcc c++
CFLAGS = -Wall -Wextra -pedantic -ggdb -std=c++20 -o
OBJECT = bin/bisl
SRC = $(shell find src/ -type f -name *.cpp)
INSTALLATION_TARGET = /usr/bin/

default: $(SRC)
	@mkdir -p bin
	@$(CC) $(CFLAGS) $(OBJECT) $(SRC)

clean: $(OBJECT)
	@rm $(OBJECt)

install: $(INSTALLATION_TARGET)
	@make default
	@sudo mv $(OBJECT) $(INSTALLATION_TARGET)

uninstall: $(INSTALLATION_TARGET)
	@sudo rm $(INSTALLATION_TARGET)
