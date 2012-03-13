all: cap

cap: main.c
	cc -o cap -framework AudioUnit -framework CoreFoundation main.c

