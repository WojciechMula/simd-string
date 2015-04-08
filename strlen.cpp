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
