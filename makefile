
INCLUDE_DIRS = -Iinclude  # -I adds include directory of header files.
LIB_DIRS = -Llib/GLFW  # -L looks in directory for library files
LINK_LIBS = -lglfw3dll  # -l links with a library file.
OUTPUT_FILE = target/program.exe

run: 
	g++ -fdiagnostics-color=always -g $(INCLUDE_DIRS) $(LIB_DIRS) ./src/**.cpp ./src/glad.c $(LINK_LIBS) -o $(OUTPUT_FILE)
	./$(OUTPUT_FILE)