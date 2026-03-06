/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

#ifdef __aarch64__

#include <stdio.h>
#include <stdint.h>
#include <arm_sve.h>

int utf8_naive(const unsigned char *data, int len);

/*
 * Map high nibble of "First Byte" to legal character length minus 1
 * 0x00 ~ 0xBF --> 0
 * 0xC0 ~ 0xDF --> 1
 * 0xE0 ~ 0xEF --> 2
 * 0xF0 ~ 0xFF --> 3
 */
static const uint8_t _first_len_tbl[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 3,
};

/* Map "First Byte" to 8-th item of range table (0xC2 ~ 0xF4) */
static const uint8_t _first_range_tbl[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 8, 8, 8,
};

/*
 * Range table, map range index to min and max values
 * Index 0    : 00 ~ 7F (First Byte, ascii)
 * Index 1,2,3: 80 ~ BF (Second, Third, Fourth Byte)
 * Index 4    : A0 ~ BF (Second Byte after E0)
 * Index 5    : 80 ~ 9F (Second Byte after ED)
 * Index 6    : 90 ~ BF (Second Byte after F0)
 * Index 7    : 80 ~ 8F (Second Byte after F4)
 * Index 8    : C2 ~ F4 (First Byte, non ascii)
 * Index 9~15 : illegal: u >= 255 && u <= 0
 */
static const uint8_t _range_min_tbl[] = {
    0x00, 0x80, 0x80, 0x80, 0xA0, 0x80, 0x90, 0x80,
    0xC2, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
};
static const uint8_t _range_max_tbl[] = {
    0x7F, 0xBF, 0xBF, 0xBF, 0xBF, 0x9F, 0xBF, 0x8F,
    0xF4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

/*
 * Tables for fast handling of four special First Bytes(E0,ED,F0,F4), after
 * which the Second Byte are not 80~BF. It contains "range index adjustment".
 * +------------+---------------+------------------+----------------+
 * | First Byte | original range| range adjustment | adjusted range |
 * +------------+---------------+------------------+----------------+
 * | E0         | 2             | 2                | 4              |
 * +------------+---------------+------------------+----------------+
 * | ED         | 2             | 3                | 5              |
 * +------------+---------------+------------------+----------------+
 * | F0         | 3             | 3                | 6              |
 * +------------+---------------+------------------+----------------+
 * | F4         | 4             | 4                | 8              |
 * +------------+---------------+------------------+----------------+
 */
/* index1 -> E0, index14 -> ED */
static const uint8_t _range_adjust_tbl[] = {
    0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0,
    0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0,
};

static const uint8_t _range_adjust_tb2[] = {
    0, 3, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 3, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

int utf8_range(const unsigned char *data, int len)
{
    if (len >= 32) {
        svbool_t pg = svptrue_b8();
        svuint8_t prev_input = svdup_u8(0);
        svuint8_t prev_first_len = svdup_u8(0);
        const svuint8_t first_len_tbl = svld1_u8(pg, _first_len_tbl);
        const svuint8_t first_range_tbl = svld1_u8(pg, _first_range_tbl);
        const svuint8_t range_min_tbl = svld1_u8(pg, _range_min_tbl);
        const svuint8_t range_max_tbl = svld1_u8(pg, _range_max_tbl);
        const svuint8_t range_adjust_tbl = svld1_u8(pg, _range_adjust_tbl);
        const svuint8_t range_adjust_tb2 = svld1_u8(pg, _range_adjust_tb2);
        const svuint8_t const_1 = svdup_u8(1);
        const svuint8_t const_2 = svdup_u8(2);
        const svuint8_t const_ef = svdup_u8(0xEF);

	    svuint8_t error1 = svdup_u8(0);
        svuint8_t error2 = svdup_u8(0);

        while (len >= 32) {
	        svuint8_t input_a = svld1_u8(pg, data);
            printf()

	        svuint8_t high_nibbles_a = svlsr_n_u8_z(pg, input_a, 4);

	        svuint8_t first_len_a = svtbl_u8(first_len_tbl, high_nibbles_a);

	        svuint8_t range_a = svtbl_u8(first_range_tbl, high_nibbles_a);

            range_a = svorr_u8_z(pg, range_a, svext(prev_first_len, first_len_a, 31));

            svuint8_t tmp1_a = svext_u8(prev_first_len, first_len_a, 30);
            tmp1_a = svsub_u8_z(pg, tmp1_a, const_1);
            range_a = svorr_u8_z(pg, range_a, tmp1_a);

            svuint8_t tmp2_a = svext_u8(prev_first_len, first_len_a, 29);
            tmp2_a = svsub_u8_z(pg, tmp2_a, const_2);
            range_a = svorr_u8_z(pg, range_a, tmp2_a);

            svuint8_t shift1_a = svext_u8(prev_input, input_a, 31);
            svuint8_t pos_a = svsub_u8_z(pg, shift1_a, const_ef);

            svuint8_t range_adjust_tbl_0 = svget2(range_adjust_tbl, 0);
            svuint8_t range_adjust_tbl_1 = svget2(range_adjust_tbl, 1);
            svuint8_t result_0 = svtbl_u8(range_adjust_tbl_0, pos_a);
            svuint8_t result_1 = svtbl_u8(range_adjust_tbl_1, pos_a);
            svuint8_t table_result = svorr_u8_z(pg, result_0, result_1);
            range_a = svadd_u8_z(pg, range_a, table_result);

            svuint8_t minv_a = svtbl_u8(range_min_tbl, range_a);
            svuint8_t maxv_a = svtbl_u8(range_max_tbl, range_a);

            svuint8_t zero_vec = svdup_u8(0);
            error1 = svorr_u8_z(pg, error1, svsel_u8(svcmplt(pg, input_a, minv_a),svdup_u8(0xFF),zero_vec));
            error2 = svorr_u8_z(pg, error2, svsel_u8(svcmpgt(pg, input_a, maxv_a),svdup_u8(0xFF),zero_vec));

            prev_input = input_a;
            prev_first_len = first_len_a;
            data += 32;
            len -= 32;
        }
        error1 = svorr_u8_z(pg, error1, error2);
        if (svmaxv_u8(pg, error1))
            return 1;

        uint32_t token4;
        svuint32_t temp = svreinterpret_u32(prev_input);
        uint32_t lane_value = svlastb(svptrue_b32(), temp);
       *(&token4) = lane_value;
        const int8_t *token = (const int8_t *)&token4;
        int lookahead = 0;
        if (token[3] > (int8_t)0xBF)
            lookahead = 1;
        else if (token[2] > (int8_t)0xBF)
            lookahead = 2;
        else if (token[1] > (int8_t)0xBF)
            lookahead = 3;
        data -= lookahead;
        len += lookahead;
    }
    return utf8_tail(data, len);
}

#endif