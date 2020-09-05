#include "catch.hpp"

#ifdef __SSE4_2__
#include <immintrin.h>

SCENARIO("strstr simd")
{
    GIVEN("Looking for ':' in a string to get its index")
    {
        const int mode = _SIDD_UBYTE_OPS | _SIDD_CMP_EQUAL_EACH | _SIDD_LEAST_SIGNIFICANT;

        static const unsigned char data[] = "  :             ";
        static const unsigned char delim[] = "::::::::::::::::";

        const __m128i simd_a = _mm_loadu_si128((__m128i *)&data[0]);
        const __m128i simd_b = _mm_loadu_si128((__m128i *)&delim[0]);
        WHEN("Parsed")
        {

            THEN("Expect colon to be found.")
            {
                int result = _mm_cmpestri(simd_b, 16, simd_a , 16, mode);
                REQUIRE(result == 2);
            }
        }
    }

    GIVEN("Looking for ':' in a string anywhere")
    {
        const int mode = _SIDD_UBYTE_OPS | _SIDD_CMP_EQUAL_ORDERED | _SIDD_LEAST_SIGNIFICANT;

        static const unsigned char data[] = "  :             ";
        static const unsigned char delim[] = ":";

        const __m128i simd_a = _mm_loadu_si128((__m128i*)&data[0]);
        const __m128i simd_b = _mm_loadu_si128((__m128i*)&delim[0]);
        WHEN("Parsed")
        {

            THEN("Expect colon to be found.")
            {
                int result = _mm_cmpestri(simd_b, 1, simd_a , 16, mode);
                REQUIRE(result == 2);
            }
        }
    }

    GIVEN("Looking for 'crlf' in a string anywhere")
    {
        const int mode = _SIDD_UBYTE_OPS | _SIDD_CMP_EQUAL_ORDERED | _SIDD_LEAST_SIGNIFICANT;

        static const unsigned char data[] = "    \r\n     \r\n   ";
        static const unsigned char delim[] = "\r\n";

        const __m128i simd_a = _mm_loadu_si128((__m128i*)&data[0]);
        const __m128i simd_b = _mm_loadu_si128((__m128i*)&delim[0]);
        WHEN("Parsed")
        {

            THEN("Expect colon to be found.")
            {
                int result = _mm_cmpestri(simd_b, 2, simd_a , 16, mode);
                REQUIRE(result == 4);
            }
        }
    }
}
#endif