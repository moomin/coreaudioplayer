all: cap

cap: main.c
	cc -o cap -framework AudioUnit main.c

