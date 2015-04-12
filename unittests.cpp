#include <cstdlib>
#include <cstring>
#include <memory>

// --------------------------------------------------

#include "headers.h"
#include "bits.cpp"
#include "strlen.cpp"
#include "strcmp.cpp"
#include "strchr.cpp"
#include "strrchr.cpp"

// --------------------------------------------------


class UnitTests final {

public:
    bool run_all() {

#if 0
        printf("test strlen SSE4.1: ");
        fflush(stdout);

        if (test_strlen(sse41_strlen)) {
            print_ok();
        } else {
            print_fail();
            return false;
        }

        printf("test strlen SSE4.2: ");
        fflush(stdout);

        if (test_strlen(sse42_strlen)) {
            print_ok();
        } else {
            print_fail();
            return false;
        }

        printf("test strcmp SSE4.2: ");
        fflush(stdout);

        if (test_strcmp(sse42_strcmp)) {
            print_ok();
        } else {
            print_fail();
            return false;
        }

        printf("test strchr SSE4.2: ");
        fflush(stdout);

        if (test_strchr()) {
            print_ok();
        } else {
            print_fail();
            return false;
        }
#endif

        printf("test strrchr SSE4.2: ");
        fflush(stdout);

        if (test_strrchr()) {
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
    template <typename T>
    bool test_strlen(T simd_strlen) {

        const size_t size = 1024;

        std::unique_ptr<char> b(new char[size+1]);
        char* buffer = b.get();

        for (size_t i=0; i < size; i++) {
            buffer[i] = 'x';
        }

        for (int i = size - 1; i >= 0; i--) {

            buffer[i] = 0;

            const auto expected = strlen(buffer);
            const auto actual   = simd_strlen(buffer);

            if (expected != actual) {
                printf("failed: %lu != %lu\n", expected, actual);
                return false;
            }
        }

        return true;
    }

    template <typename T>
    bool test_strcmp(T simd_strcmp) {

        const size_t size = 1024;

        std::unique_ptr<char> A(new char[size+1]);
        std::unique_ptr<char> B(new char[size+1]);
        char* a = A.get();
        char* b = B.get();

        for (size_t i=0; i < size; i++) {
            a[i] = 'x';
            b[i] = 'x';
        }

        a[size] = 0;

        auto same_result = [](int a, int b) -> bool {
            if (a == 0 && b == 0) {
                return true;
            }

            if ((a > 0 && b > 0) || (a < 0 && b < 0)) {
                return true;
            }

            return false;
        };

        // 1. different strings

        for (unsigned i = 0; i < size; i++) {
            for (unsigned j = i + 1; j < size; j++) {

                memcpy(a, b, size);

                a[i] = 'y';
                a[j] = 0;

                const auto expected1 = strcmp(a, b);
                const auto actual1   = simd_strcmp(a, b);

                if (!same_result(expected1, actual1)) {
                    printf("failed: %d != %d\n", expected1, actual1);
                    return false;
                }

                const auto expected2 = strcmp(b, a);
                const auto actual2   = simd_strcmp(b, a);

                if (!same_result(expected2, actual2)) {
                    printf("failed: %d != %d\n", expected2, actual2);
                    return false;
                }
            }
        }

        // 2. same strings

        for (unsigned i = 0; i < size; i++) {

            if (i > 0) {
                b[i - 1] = 'x';
            }

            b[i] = 0;

            memcpy(a, b, i+1);

            const auto expected = strcmp(a, b);
            const auto actual   = simd_strcmp(a, b);

            if (!same_result(expected, actual)) {
                printf("failed: %d != %d\n", expected, actual);
                return false;
            }
        }

        return true;
    }


    bool test_strchr() {

        const size_t size = 1024;

        std::unique_ptr<char> b(new char[size+1]);
        char* buffer = b.get();

        for (size_t i=0; i < size; i++) {
            for (size_t j=i; j < size; j++) {

                memset(buffer, 'x', size);
                buffer[i] = 'y';
                buffer[j] = 0;

                {
                    const auto expected = strchr(buffer, 'y');
                    const auto actual   = sse42_strchr(buffer, 'y');

                    if (expected != actual) {
                        printf("failed2: %p != %p\n", expected, actual);
                        return false;
                    }
                }

                {
                    const auto expected = strchr(buffer, '!');
                    const auto actual   = sse42_strchr(buffer, '!');

                    if (expected != actual) {
                        printf("failed1: %p != %p\n", expected, actual);
                        return false;
                    }
                }
            }
        }

        return true;
    }


    bool test_strrchr() {

        const size_t size = 1024;

        std::unique_ptr<char> b(new char[size+1]);
        char* buffer = b.get();

        for (size_t i=0; i < size; i++) {
            for (size_t j=i; j < size - 1; j++) {

                memset(buffer, 'x', size);
                buffer[i] = 'y';
                buffer[j] = 'y';
                buffer[j + 1] = 0;

                {
                    const auto expected = strrchr(buffer, 'y');
                    const auto actual   = sse42_strrchr(buffer, 'y');

                    if (expected != actual) {
                        printf("failed2: %p != %p\n", expected, actual);
                        return false;
                    }
                }

                {
                    const auto expected = strrchr(buffer, '!');
                    const auto actual   = sse42_strrchr(buffer, '!');

                    if (expected != actual) {
                        printf("failed1: %p != %p\n", expected, actual);
                        return false;
                    }
                }
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

