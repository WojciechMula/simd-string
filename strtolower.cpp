enum class strcase_mode {

    to_lower,
    to_upper,
    swap_case
};

void sse4_strcase(const char* s, strcase_mode m) {

    __m128i ranges;

    switch (m) {
        case strcase_mode::to_lower:
            ranges = _mm_setr_epi8('A', 'Z', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
            break;

        case strcase_mode::to_upper:
            ranges = _mm_setr_epi8('a', 'z', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
            break;

        case strcase_mode::swap_case:
            ranges = _mm_setr_epi8('a', 'z', 'A', 'Z', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
            break;

        default:
            assert(false && "invalid mode value");
            return;
    }

    __m128i* mem = reinterpret_cast<__m128i*>(const_cast<char*>(s));
    const __m128i diff = _mm_set1_epi8(0x20);

    const uint8_t mode =
        _SIDD_UBYTE_OPS |
        _SIDD_CMP_RANGES |
        _SIDD_UNIT_MASK;
    
    for (/**/; /**/; mem++) {
        
        const __m128i chunk = _mm_loadu_si128(mem);
        if (_mm_cmpistrc(ranges, chunk, mode)) {

            const __m128i tmp1 = _mm_cmpistrm(ranges, chunk, mode);
            const __m128i mask = _mm_and_si128(tmp1, diff);

            _mm_storeu_si128(mem, _mm_xor_si128(chunk, mask));
        }

        if (_mm_cmpistrz(ranges, chunk, mode)) {
            break;
        }
    }
}
