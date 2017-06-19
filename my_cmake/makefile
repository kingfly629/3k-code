test:test.o Rectange.o
	g++ -o bin/test obj/test.o obj/Rectange.o
test.o:src/other/test.cpp
	g++ -c -o obj/test.o src/other/test.cpp
Rectange.o:src/other/Rectange.cpp
	g++ -c -o obj/Rectange.o src/other/Rectange.cpp
clean:
	rm -rf obj/test.o obj/Rectange.o bin/test