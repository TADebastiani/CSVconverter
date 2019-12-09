all:
	@gcc main.c -Wall -o CSVconverter
install: all
	@sudo mv CSVconverter /usr/bin
	@sync
