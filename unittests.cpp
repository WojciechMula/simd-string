#include <cstdlib>
#include <cstring>
#include <vector>
#include <memory>

// --------------------------------------------------

#include "headers.h"
#include "bits.cpp"
#include "strlen.cpp"
#include "strcmp.cpp"
#include "strchr.cpp"
#include "strrchr.cpp"
#include "is_xdigit.cpp"
#include "strtolower.cpp"
#include "memcmp.cpp"

// --------------------------------------------------


class UnitTests final {

public:
    bool run_all() {

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

        printf("test strrchr SSE4.2: ");
        fflush(stdout);

        if (test_strrchr()) {
            print_ok();
        } else {
            print_fail();
            return false;
        }

        printf("test is_xidigt SSE4.2: ");
        fflush(stdout);

        if (test_is_xdigit()) {
            print_ok();
        } else {
            print_fail();
            return false;
        }

        printf("test change_case SSE4.2: ");
        fflush(stdout);

        if (test_change_case()) {
            print_ok();
        } else {
            print_fail();
            return false;
        }

        printf("test memcmp SSE4.2: ");
        fflush(stdout);

        if (test_memcmp()) {
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


    bool test_is_xdigit() {

        struct fixture {
            std::string string;
            bool        expected;

            const char* c_str() const {
                return string.c_str();
            }
        };

        std::vector<fixture> fixtures = {
            {"",                            false},
            {"cat",                         false},
            {"42",                          true},
            {"badcafe",                     true},
            {"0x80",                        false},
            {"0123456789abcdefABCDEF",      true},
            {"nobody expects 42",           false},
        };

        for (const auto& item: fixtures) {
            if (sse42_is_xdigit(item.c_str()) != item.expected) {
                printf("failed for '%s'\n", item.c_str());
                return false;
            }
        }

        return true;
    }


    bool test_change_case() {

        struct fixture {
            std::string str;
            std::string lower;
            std::string upper;
            std::string swapped;
        };

        std::vector<fixture> fixtures = {
            {"01234", "01234", "01234", "01234"},
            {"HaXoR", "haxor", "HAXOR", "hAxOr"},
            {"The number is 42!", "the number is 42!", "THE NUMBER IS 42!", "tHE NUMBER IS 42!"},
        };

        char buffer[256];

        for (const auto& item: fixtures) {
            
            memset(buffer, 'x', sizeof(buffer));
            memcpy(buffer, item.str.c_str(), item.str.size() + 1);

            sse4_strcase(buffer, strcase_mode::to_lower);
            if (std::string(buffer) != item.lower) {
                printf("failed for to_lower '%s'\n", buffer);
                return false;
            }
            
            memset(buffer, 'x', sizeof(buffer));
            memcpy(buffer, item.str.c_str(), item.str.size() + 1);

            sse4_strcase(buffer, strcase_mode::to_upper);
            if (std::string(buffer) != item.upper) {
                printf("failed for to_upper '%s'\n", buffer);
                return false;
            }
            
            memset(buffer, 'x', sizeof(buffer));
            memcpy(buffer, item.str.c_str(), item.str.size() + 1);

            sse4_strcase(buffer, strcase_mode::swap_case);
            if (std::string(buffer) != item.swapped) {
                printf("failed for swap_case '%s'\n", buffer);
                return false;
            }
        }

        return true;
    }


    bool test_memcmp() {

        const size_t size = 1024;

        char a[size];
        char b[size];

        memset(a, 'x', sizeof(a));

        auto same_result = [](int a, int b) -> bool {
            if (a == 0 && b == 0) {
                return true;
            }

            if ((a > 0 && b > 0) || (a < 0 && b < 0)) {
                return true;
            }

            return false;
        };


        for (size_t i=0; i < size; i++) {
            
            memcpy(b, a, size);
            b[i] = 'y';

            for (size_t j=0; j < size; j++) {

                const auto expected = memcmp(a, b, j);
                const auto actual   = sse42_memcmp(a, b, j);

                if (!same_result(expected, actual)) {
                    return false;
                }
            }
        }


        memset(a, 'x', size);
        memset(b, 'x', size);

        for (size_t i=0; i < size; i++) {
            
            const auto expected = memcmp(a, b, i);
            const auto actual   = sse42_memcmp(a, b, i);

            if (!same_result(expected, actual)) {
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

