all: compile run

compile:
	g++ src/*.c src/*.cpp -o bin/sandbox.exe -I C:/Libraries/include -L C:/Libraries/lib/GLFW -lglfw3 -lgdi32 -lopengl32 -luser32

run:
	.\bin\sandbox.exe