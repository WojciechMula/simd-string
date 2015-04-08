#include <cstdlib>
#include <cstring>
#include <memory>

// --------------------------------------------------

#include "headers.h"
#include "bits.cpp"
#include "strlen.cpp"

// --------------------------------------------------


class UnitTests final {

public:
    bool run_all() {

        printf("test_strlen: ");
        fflush(stdout);

        if (test_strlen()) {
            print_ok();
        } else {
            print_fail();
            return false;
        }

        return true;
    }


private:
    void print_ok() {
        print_ansi("OK", 32);
    }

    void print_fail() {
        print_ansi("fail", 31);
    }

    void print_ansi(const char* str, int color) {
        printf("\033[%dm%s\033[0m\n", color, str);
    }

private:
    bool test_strlen() {
        
        const size_t size = 1024;

        std::unique_ptr<char> b(new char[1023]);
        char* buffer = b.get();

        for (size_t i=0; i < size; i++) {
            buffer[i] = 'x';
        }

        for (int i = size - 1; i >= 0; i--) {

            buffer[i] = 0;

            const auto expected = strlen(buffer);
            const auto actual   = sse41_strlen(buffer);

            if (expected != actual) {
                printf("failed: %lu != %lu\n", expected, actual);
                return false;
            }
        }

        return true;
    }

};


// --------------------------------------------------


int main() {

    UnitTests tests;

    return tests.run_all() ? EXIT_SUCCESS : EXIT_FAILURE;
}

