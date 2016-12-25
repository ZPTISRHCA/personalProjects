vpath %.cpp ./test
vpath %.cpp ./src
vpath %.h ./include

objs = testThreadsDownload.o logger.o analysisURL.o conWebServer.o httpPacket.o ioHandler.o showMsg.o downloadTool.o 
target = a.out

$(target):$(objs) 
	g++ $^ -pthread
testThreadsDownload.o: testThreadsDownload.cpp analysisURL.h downloadTool.h
	g++ -c $< 
logger.o: logger.cpp logger.h
	g++ -c $<  
analysisURL.o: analysisURL.cpp analysisURL.h logger.h
	g++ -c $< 
conWebServer.o: conWebServer.cpp conWebServer.h logger.h analysisURL.h
	g++ -c $<
httpPacket.o: httpPacket.cpp httpPacket.h logger.h analysisURL.h
	g++ -c $<
ioHandler.o: ioHandler.cpp ioHandler.h logger.h httpPacket.h sem.h
	g++ -c $<
showMsg.o: showMsg.cpp showMsg.h logger.h
	g++ -c $<
downloadTool.o: downloadTool.cpp downloadTool.h analysisURL.h logger.h conWebServer.h httpPacket.h ioHandler.h sem.h showMsg.h
	g++ -c $< -pthread
.PHONY: clean
clean:
	rm -rf ./*.o
