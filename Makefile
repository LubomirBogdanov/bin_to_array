#Created on Oct 26, 2023
#by Lubomir Bogdanov

all: outdir ./debug/bin_to_array size

./debug/bin_to_array: main.c
	gcc -Wall -O2 -std=c99 main.c -o ./debug/bin_to_array
	
size:
	size ./debug/bin_to_array	

outdir:
	mkdir -p debug
	
clean:
	rm -rf debug	
	
	