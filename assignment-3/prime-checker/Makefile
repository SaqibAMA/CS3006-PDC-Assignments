target:
	/opt/homebrew/opt/gcc/bin/gcc-11 -fopenmp main.c -o main
	./main

test-stdout-100:
	/opt/homebrew/opt/gcc/bin/gcc-11 -fopenmp main.c -o main
	./main 10 100 out/testfile-100.txt

clean:
	rm -f main