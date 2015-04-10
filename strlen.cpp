size_t sse41_strlen(const char* s) {

    size_t result = 0;

    const __m128i zeros = _mm_setzero_si128();
    __m128i* mem = reinterpret_cast<__m128i*>(const_cast<char*>(s));

    for (/**/; /**/; mem++, result += 16) {

        const __m128i data = _mm_loadu_si128(mem);
        const __m128i cmp  = _mm_cmpeq_epi8(data, zeros);

        if (!_mm_testc_si128(zeros, cmp)) {

            const auto mask = _mm_movemask_epi8(cmp);

            return result + bits::get_first_bit_set(mask);
        }
    }

    assert(false);
    return 0;
}


size_t sse42_strlen(const char* s) {

    size_t result = 0;

    __m128i* mem = reinterpret_cast<__m128i*>(const_cast<char*>(s));
    const __m128i zeros = _mm_setzero_si128();

    for (/**/; /**/; mem++, result += 16) {

        const __m128i data = _mm_loadu_si128(mem);

        const uint8_t mode =
            _SIDD_UBYTE_OPS |
            _SIDD_CMP_EQUAL_EACH |
            _SIDD_LEAST_SIGNIFICANT;

        // Note: pcmpstri return mask/index and set ALU flags. Intrinsics
        //       functions can return just single value (mask, particular
        //       flag), so multiple intrinsics functions have to be called.
        //
        //       The good news: GCC and MSVC merges multiple _mm_cmpXstrX
        //       invocations with the same arguments to the single pcmpstri
        //       instruction.
        //
        if (_mm_cmpistrc(data, zeros, mode)) {
            const auto idx = _mm_cmpistri(data, zeros, mode);

            return result + idx;
        }
    }
}
