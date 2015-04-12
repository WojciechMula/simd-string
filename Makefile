CC=g++
FLAGS=-O2 -Wall -Wextra -pedantic -std=c++11 -msse4
DEPS=strlen.cpp strcmp.cpp strchr.cpp strrchr.cpp is_xdigit.cpp strtolower.cpp
ALL=unittests

unittests: unittests.cpp $(DEPS)
	$(CC) $(FLAGS) $< -o $@

clean:
	rm -f $(ALL)

