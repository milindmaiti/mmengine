# Makefile for mmengine program

# Helpful flags and other variables
CC = g++
CPP_VERSION = -std=c++17
CPPFLAGS = -g -Wall -Wunused -Werror -Wconversion -O2

# Create final executable bitboard
mmengine: bitboard.o boardnotation.o genspecialboards.o
	$(CC) $(CPP_VERSION) $(CPPFLAGS) bitboard.o boardnotation.o genspecialboards.o -o mmengine

# Compile bitboard into object file
bitboard.o: bitboard.cpp
	echo "Compiling the bitboard file"
	$(CC) $(CPP_VERSION) $(CPPFLAGS) -c bitboard.cpp -o bitboard.o

# Compile boardnotation into object file
boardnotation.o: utility/boardnotation.cpp
	echo "Compiling the board notation file"
	$(CC) $(CPP_VERSION) $(CPPFLAGS) -c utility/boardnotation.cpp -o boardnotation.o

# Compile genspecialboards into object file
genspecialboards.o: utility/genspecialboards.cpp
	echo "Compiling the special board macros file"
	$(CC) $(CPP_VERSION) $(CPPFLAGS) -c utility/genspecialboards.cpp -o genspecialboards.o

# Creating the testing executable by linking .o files
current_test: boardnotation.o test_genspecialboards.o
	$(CC) $(CPP_VERSION) $(CPPFLAGS) test_genspecialboards.o boardnotation.o -o current_test 

test_genspecialboards.o: utility/genspecialboards.cpp
	$(CC) $(CPP_VERSION) $(CPPFLAGS) -DTEST_GENSPECIALBOARDS -c utility/genspecialboards.cpp -o test_genspecialboards.o
clean:
	rm -f *.o mmengine current_test
