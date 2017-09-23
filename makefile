all:
	gcc -o audioNote source/search.c source/json.c source/create.c source/main.c

clean: 
	rm audioNote