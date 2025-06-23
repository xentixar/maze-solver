compile: main.c
		@mkdir -p build
		gcc main.c -o build/main -lraylib
		
run: compile
	./build/main
