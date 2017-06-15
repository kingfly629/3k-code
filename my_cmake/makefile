test:test.o tes.o
	g++ -o bin/test obj/test.o obj/tes.o
test.o:src/template/newmain.cpp
	g++ -c -o obj/test.o src/template/newmain.cpp
tes.o:src/template/templateClass.cpp
	g++ -c -o obj/tes.o src/template/templateClass.cpp
clean:
	rm -rf obj/test.o obj/tes.o bin/test
