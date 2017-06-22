test:test.o Rectange.o myfriend.o
	g++ -o bin/test obj/test.o obj/Rectange.o obj/myfriend.o
test.o:src/other/test.cpp
	g++ -c -o obj/test.o src/other/test.cpp
Rectange.o:src/other/Rectange.cpp
	g++ -c -o obj/Rectange.o src/other/Rectange.cpp
myfriend.o:src/other/myfriend.cpp
	g++ -c -o obj/myfriend.o src/other/myfriend.cpp
clean:
	rm -rf obj/test.o obj/Rectange.o obj/myfriend.o bin/test