char* sse42_strrchr(const char* s, int c) {

    assert(c >= 0);
    assert(c < 256);

    __m128i* mem = reinterpret_cast<__m128i*>(const_cast<char*>(s));
    const __m128i set = _mm_setr_epi8(c, 0, 0, 0, 0, 0, 0, 0, 
                                      0, 0, 0, 0, 0, 0, 0, 0);

    const uint8_t mode =
        _SIDD_UBYTE_OPS |
        _SIDD_CMP_EQUAL_ANY |
        _SIDD_MOST_SIGNIFICANT;

    char* result = nullptr;

    for (/**/; /**/; mem++) {

        const __m128i chunk = _mm_loadu_si128(mem);

        if (_mm_cmpistrc(set, chunk, mode)) {
            // there is character c in a chunk
            const auto idx = _mm_cmpistri(set, chunk, mode);

            result = reinterpret_cast<char*>(mem) + idx;
        } else if (_mm_cmpistrz(set, chunk, mode)) {
            // there is zero byte in a chunk
            break;
        }
    }

    return result;
}
