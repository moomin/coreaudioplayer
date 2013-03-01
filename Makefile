all: cap

clean:
	rm ./cap

cap: main.c
	cc -o cap -framework AudioUnit -framework CoreFoundation main.c render_callback.c audio_unit.c read_file.c

