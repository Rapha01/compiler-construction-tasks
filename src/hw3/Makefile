# Compiler flags
CFLAGS := -w -O3 -m64 -D'main(a,b)=main(int argc, char** argv)' -Duint64_t='unsigned long long'
CC := gcc

# Compile selfie.c into selfie executable
selfie: selfie.c
	$(CC) $(CFLAGS) $< -o $@

# Consider these targets as targets, not files
.PHONY : test sat all clean

# Test self-compilation, self-execution, and self-hosting
test: selfie
	./selfie -c selfie.c -o selfie1.m -s selfie1.s -m 2 -c selfie.c -o selfie2.m -s selfie2.s
	diff -q selfie1.m selfie2.m
	diff -q selfie1.s selfie2.s
	./selfie -c selfie.c -m 2
	./selfie -c arrayTest.c -m 2

# Test SAT solver
sat: selfie
	./selfie -sat manuscript/cnfs/rivest.cnf
	./selfie -c selfie.c -m 1 -sat manuscript/cnfs/rivest.cnf

# Test everything
all: test sat

# Clean up
clean:
	rm -rf *.m
	rm -rf *.s
	rm -rf selfie
	rm -rf selfie.exe
