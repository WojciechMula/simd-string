bool sse42_is_xdigit(const char* s) {

    if (s == nullptr) {
        return false;
    }

    // 3 ranges 
    const __m128i ranges = _mm_setr_epi8(
        '0', '9', 'a', 'f', 'A', 'F', 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0
    );

    __m128i* mem = reinterpret_cast<__m128i*>(const_cast<char*>(s));

    const uint8_t mode =
        _SIDD_UBYTE_OPS |
        _SIDD_CMP_RANGES |
        _SIDD_MASKED_NEGATIVE_POLARITY |
        _SIDD_LEAST_SIGNIFICANT |
        _SIDD_BIT_MASK;

    for (/**/; /**/; mem++) {

        const __m128i chunk = _mm_loadu_si128(mem);

        if (_mm_cmpistrc(ranges, chunk, mode)) {
            // there are some characters outside the given ranges in a chunk
            return false;
        } else if (_mm_cmpistrz(ranges, chunk, mode)) {
            // there is zero byte in a chunk
            if (*s == 0) {
                // empty string
                return false;
            } else {
                return true;
            }
        }
    }

    assert(false && "impossible happend");
    return false;
}
