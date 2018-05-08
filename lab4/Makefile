# $Id: Makefile,v 1.4 2016-01-20 14:30:32-08 - - $

GPP  = g++ -std=gnu++14 -g -O0 -Wall -Wextra 
LIBS = -lglut -lGLU -lGL -lX11 -lrt -lm

all : hello-gl 
    

hello-gl : hello-gl.cpp bonjour-monde.cpp
	${GPP} hello-gl.cpp -o hello-gl ${LIBS}
	${GPP} bonjour-monde.cpp -o bonjour-monde ${LIBS}


ci : Makefile hello-gl.cpp bonjour-monde.cpp
	cpplint.py.perl hello-gl.cpp
	checksource Makefile hello-gl.cpp
	cid Makefile hello-gl.cpp

test : all
	hello-gl &

