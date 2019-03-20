CFLAGS=-g
CFLAGS+=`pkg-config --cflags libavformat`
LOADLIBES=`pkg-config --libs libavformat libavcodec libavutil libswscale`
main:main.o pic_op.o

clean:
	rm -rf *.o main
