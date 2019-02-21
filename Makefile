CC=g++
CC_FLAGS=-Wall --std=c++0x
EXE_NAME=ppm_steg

.SILENT:

init:
	@echo Compiling executable $(EXE_NAME) ...
	$(CC) $(CC_FLAGS) main.c -o $(EXE_NAME)
