int sse42_strcmp(const char* s1, const char* s2) {

    if (s1 == s2) {
        return 0;
    }

    __m128i* ptr1 = reinterpret_cast<__m128i*>(const_cast<char*>(s1));
    __m128i* ptr2 = reinterpret_cast<__m128i*>(const_cast<char*>(s2));

    for (/**/; /**/; ptr1++, ptr2++) {

        const __m128i a = _mm_loadu_si128(ptr1);
        const __m128i b = _mm_loadu_si128(ptr2);

        const uint8_t mode =
            _SIDD_UBYTE_OPS |
            _SIDD_CMP_EQUAL_EACH |
            _SIDD_NEGATIVE_POLARITY |
            _SIDD_LEAST_SIGNIFICANT;
        
        if (_mm_cmpistrc(a, b, mode)) {
            // a & b are different (not counting past-zero bytes)
            const auto idx = _mm_cmpistri(a, b, mode);

            const uint8_t b1 = (reinterpret_cast<char*>(ptr1))[idx];
            const uint8_t b2 = (reinterpret_cast<char*>(ptr2))[idx];

            //printf("%d, %d, %d\n", idx, b1, b2);

            if (b1 < b2) {
                return -1;
            } else if (b1 > b2) {
                return +1;
            } else {
                return 0;
            }
        } else if (_mm_cmpistrz(a, b, mode)) {
            // a & b are same, but b contains a zero byte
            break;
        }
    }

    return 0;
}
