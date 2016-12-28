object = main.o analysisCommand.o common.o
myls: $(object) 
	g++ -g $^ -o myls
main.o: main.cpp analysisCommand.h common.h
	g++ -c $<
analysisCommand.o: analysisCommand.cpp analysisCommand.h common.h
	g++ -c $<
common.o: common.cpp common.h
	g++ -c $<
.PHONY: clean
clean:
	-rm -f *.o 
