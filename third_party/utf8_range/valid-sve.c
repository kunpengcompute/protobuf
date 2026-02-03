/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

#ifdef __aarch64__
#include <arm_sve.h>

#define prefer(x) __builtin_expect(!!(x), 1)
#define BLOCK_SIZE 32
#define BLOCK_NUM 4
#define STEP_SIZE BLOCK_SIZE * BLOCK_NUM

// Bit 0 = Too Short (lead byte/ASCII followed by lead byte/ASCII)
// Bit 1 = Too Long (ASCII followed by continuation)
// Bit 2 = Overlong 3-byte
// Bit 4 = Surrogate
// Bit 5 = Overlong 2-byte
// Bit 7 = Two Continuations
const uint8_t TOO_SHORT = 1 << 0;  // 11______ 0_______
const uint8_t TOO_LONG = 1 << 1;   // 0_______ 10______
const uint8_t OVERLONG_3 = 1 << 2; // 11100000 100_____
const uint8_t SURROGATE = 1 << 4;  // 11101101 101_____
const uint8_t OVERLONG_2 = 1 << 5; // 1100000_ 10______
const uint8_t TWO_CONTS = 1 << 7;  // 10______ 10______
const uint8_t TOO_LARGE = 1 << 3;  // 11110100 1001____
const uint8_t TOO_LARGE_1000 = 1 << 6;
// 11110101 1000____
// 1111011_ 1000____
// 11111___ 1000____
const uint8_t OVERLONG_4 = 1 << 6; // 11110000 1000____
const uint8_t CARRY = TOO_SHORT | TOO_LONG | TWO_CONTS;

const uint8_t data1[BLOCK_SIZE] = {
    // 0_______ ________ <ASCII in byte 1>
    TOO_LONG, TOO_LONG, TOO_LONG, TOO_LONG, TOO_LONG, TOO_LONG, TOO_LONG,
    TOO_LONG,
    // 10______ ________ <continuation in byte 1>
    TWO_CONTS, TWO_CONTS, TWO_CONTS, TWO_CONTS,
    // 1100____ ________ <two byte lead in byte 1>
    TOO_SHORT | OVERLONG_2,
    // 1101____ ________ <two byte lead in byte 1>
    TOO_SHORT,
    // 1110____ ________ <three byte lead in byte 1>
    TOO_SHORT | OVERLONG_3 | SURROGATE,
    // 1111____ ________ <four+ byte lead in byte 1>
    TOO_SHORT | TOO_LARGE | TOO_LARGE_1000 | OVERLONG_4,
    // others all 0
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
const uint8_t data2[BLOCK_SIZE] = {
    // ____0000 ________
    CARRY | OVERLONG_3 | OVERLONG_2 | OVERLONG_4,
    // ____0001 ________
    CARRY | OVERLONG_2,
    // ____001_ ________
    CARRY, CARRY,
    // ____0100 ________
    CARRY | TOO_LARGE,
    // ____0101 ________
    CARRY | TOO_LARGE | TOO_LARGE_1000,
    // ____011_ ________
    CARRY | TOO_LARGE | TOO_LARGE_1000,
    CARRY | TOO_LARGE | TOO_LARGE_1000,
    // ____1___ ________
    CARRY | TOO_LARGE | TOO_LARGE_1000,
    CARRY | TOO_LARGE | TOO_LARGE_1000,
    CARRY | TOO_LARGE | TOO_LARGE_1000,
    CARRY | TOO_LARGE | TOO_LARGE_1000,
    CARRY | TOO_LARGE | TOO_LARGE_1000,
    // ____1101 ________
    CARRY | TOO_LARGE | TOO_LARGE_1000 | SURROGATE,
    CARRY | TOO_LARGE | TOO_LARGE_1000,
    CARRY | TOO_LARGE | TOO_LARGE_1000,
    // others all 0
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
const uint8_t data3[BLOCK_SIZE] = {
    // ________ 0_______ <ASCII in byte 2>
    TOO_SHORT, TOO_SHORT, TOO_SHORT, TOO_SHORT, TOO_SHORT, TOO_SHORT,
    TOO_SHORT, TOO_SHORT,
    // ________ 1000____
    TOO_LONG | OVERLONG_2 | TWO_CONTS | OVERLONG_3 | TOO_LARGE_1000 |
        OVERLONG_4,
    // ________ 1001____
    TOO_LONG | OVERLONG_2 | TWO_CONTS | OVERLONG_3 | TOO_LARGE,
    // ________ 101_____
    TOO_LONG | OVERLONG_2 | TWO_CONTS | SURROGATE | TOO_LARGE,
    TOO_LONG | OVERLONG_2 | TWO_CONTS | SURROGATE | TOO_LARGE,
    // ________ 11______
    TOO_SHORT, TOO_SHORT, TOO_SHORT, TOO_SHORT,
    // others all 0
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

inline __attribute__((always_inline)) int is_ascii(svuint8_t vec0, svuint8_t vec1, svuint8_t vec2, svuint8_t vec3) {
    svuint8_t or1 = svorr_u8_z(svptrue_b8(), vec0, vec1);
    svuint8_t or2 = svorr_u8_z(svptrue_b8(), vec2, vec3);
    svuint8_t bits = svorr_u8_z(svptrue_b8(), or1, or2);
    uint8_t max_val = svmaxv_u8(svptrue_b8(), bits);
    return max_val < 0b10000000u ? 1 : 0;
}

inline __attribute__((always_inline)) svuint8_t check_special_cases(const svuint8_t input, const svuint8_t prev1, const svuint8_t tb1,
    const svuint8_t tb2, const svuint8_t tb3) {
    const svuint8_t right4 = svlsr_n_u8_z(svptrue_b8(), prev1, 4);
    const svuint8_t byte_1_high = svtbl_u8(tb1, right4);
    const svuint8_t last4 = svdup_n_u8(0x0F);
    const svuint8_t prev_and = svand_u8_z(svptrue_b8(), prev1, last4);
    const svuint8_t byte_1_low = svtbl_u8(tb2, prev_and);
    const svuint8_t input_right4 = svlsr_n_u8_z(svptrue_b8(), input, 4);
    const svuint8_t byte_2_high = svtbl_u8(tb3, input_right4);
    return svand_u8_z(svptrue_b8(), byte_2_high, svand_u8_z(svptrue_b8(), byte_1_high, byte_1_low));
}

inline __attribute__((always_inline)) svuint8_t must_be_2_3_continuation(svuint8_t prev2, svuint8_t prev3) {
    svuint8_t first3 = svdup_n_u8(0b11100000u);
    svuint8_t first4 = svdup_n_u8(0b11110000u);
    svbool_t is_third_byte = svcmpge_u8(svptrue_b8(), prev2, first3);
    svbool_t is_forth_byte = svcmpge_u8(svptrue_b8(), prev3, first4);
    return sveor_u8_z(svptrue_b8(), svdup_u8_z(is_third_byte, 0xFF), svdup_u8_z(is_forth_byte, 0XFF));
}

inline __attribute__((always_inline)) svuint8_t check_multibyte_lengths(const svuint8_t input, const svuint8_t prev2,
    const svuint8_t prev3, const svuint8_t sc) {
    svuint8_t must23 = must_be_2_3_continuation(prev2, prev3);
    svuint8_t byte80 = svdup_n_u8(0x80);
    svuint8_t must23_80 = svand_u8_z(svptrue_b8(), must23, byte80);
    svuint8_t result = sveor_u8_z(svptrue_b8(), must23_80, sc);
    return result;
}

inline __attribute__((always_inline)) svuint8_t check_utf8_bytes(const svuint8_t input, const svuint8_t prev_input,
    const svuint8_t error, const svuint8_t tb1, const svuint8_t tb2, const svuint8_t tb3) {
    svuint8_t prev1 = svext_u8(prev_input, input, 32 - 1);
    svuint8_t prev2 = svext_u8(prev_input, input, 32 - 2);
    svuint8_t prev3 = svext_u8(prev_input, input, 32 - 3);
    svuint8_t sc = check_special_cases(input, prev1, tb1, tb2, tb3);
    svuint8_t temp_result = check_multibyte_lengths(input, prev2, prev3, sc);
    return svorr_u8_z(svptrue_b8(), error, temp_result);
}

inline __attribute__((always_inline)) svuint8_t is_incomplete(svuint8_t input) {
    svuint8_t ff = svdup_n_u8(0xFF);
    return svdup_u8_z(svcmpgt_u8(svptrue_b8(), input, ff), 0xFF);
}

inline __attribute__((always_inline)) int generic_validate_utf8(const uint8_t *data, int length) {
    svuint8_t error = svdup_n_u8(0);
    svuint8_t prev_incomplete = svdup_n_u8(0);
    svuint8_t prev_input_block = svdup_n_u8(0);
    const svuint8_t tb1 = svld1_u8(svptrue_b8(), data1);
    const svuint8_t tb2 = svld1_u8(svptrue_b8(), data2);
    const svuint8_t tb3 = svld1_u8(svptrue_b8(), data3);
    svuint8_t vec0, vec1, vec2, vec3;
    while (length >= STEP_SIZE) {
        vec0 = svld1_u8(svptrue_b8(), data);
        vec1 = svld1_u8(svptrue_b8(), data + BLOCK_SIZE);
        vec2 = svld1_u8(svptrue_b8(), data + BLOCK_SIZE * 2);
        vec3 = svld1_u8(svptrue_b8(), data + BLOCK_SIZE * 3);

        if (prefer(is_ascii(vec0, vec1, vec2, vec3))) {
            error = svorr_u8_z(svptrue_b8(), error, prev_incomplete);
        } else {
            error = check_utf8_bytes(vec0, prev_input_block, error, tb1, tb2, tb3);
            error = check_utf8_bytes(vec1, vec0, error, tb1, tb2, tb3);
            error = check_utf8_bytes(vec2, vec1, error, tb1, tb2, tb3);
            error = check_utf8_bytes(vec3, vec2, error, tb1, tb2, tb3);
            prev_incomplete = is_incomplete(vec3);
            prev_input_block = vec3;
        }
        data += STEP_SIZE;
        length -= STEP_SIZE;
    }
    if (length != 0) {
        uint8_t remain[STEP_SIZE];
        memset(remain, 0x20, sizeof(remain));
        memcpy(remain, data, length);
        vec0 = svld1_u8(svptrue_b8(), remain);
        vec1 = svld1_u8(svptrue_b8(), remain + BLOCK_SIZE);
        vec2 = svld1_u8(svptrue_b8(), remain + BLOCK_SIZE * 2);
        vec3 = svld1_u8(svptrue_b8(), remain + BLOCK_SIZE * 3);
        if (prefer(is_ascii(vec0, vec1, vec2, vec3))) {
            error = svorr_u8_z(svptrue_b8(), error, prev_incomplete);
        } else {
            error = check_utf8_bytes(vec0, prev_input_block, error, tb1, tb2, tb3);
            error = check_utf8_bytes(vec1, vec0, error, tb1, tb2, tb3);
            error = check_utf8_bytes(vec2, vec1, error, tb1, tb2, tb3);
            error = check_utf8_bytes(vec3, vec2, error, tb1, tb2, tb3);
            prev_incomplete = is_incomplete(vec3);
            prev_input_block = vec3;
        }
    }
    error = svorr_u8_z(svptrue_b8(), error, prev_incomplete);
    return svmaxv_u8(svptrue_b8(), error) > 0 ? 1 : 0;
}

#endif