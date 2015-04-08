CC=g++
FLAGS=-O2 -Wall -Wextra -pedantic -std=c++11 -msse4
DEPS=strlen.cpp
ALL=unittests

unittests: unittests.cpp $(DEPS)
	$(CC) $(FLAGS) unittests.cpp -o unittests

clean:
	rm -f $(ALL)

