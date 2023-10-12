debug: test
	@builds/MazeGenerator
test: src/*.cpp
	@g++ src/*.cpp -o builds/MazeGenerator -lraylib
clean:
	rm builds/* -r
	rm temp -r