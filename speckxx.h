/*
Copyright (c) 2016, Moritz Bitsch

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/
#ifndef SPECK_H
#define SPECK_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <string.h>

/*
 * define speck type to use (one of SPECK_32_64, SPECK_64_128, SPECK_128_256)
 */
#define SPECK_128_256

#ifdef SPECK_32_64
#define SPECK_TYPE uint16_t
#define SPECK_ROUNDS 22
#define SPECK_KEY_LEN 4
#endif

#ifdef SPECK_64_128
#define SPECK_TYPE uint32_t
#define SPECK_ROUNDS 27
#define SPECK_KEY_LEN 4
#endif

#ifdef SPECK_128_256
#define SPECK_TYPE uint64_t
#define SPECK_ROUNDS 34
#define SPECK_KEY_LEN 4
#endif

void speck_expand(SPECK_TYPE const K[SPECK_KEY_LEN], SPECK_TYPE S[SPECK_ROUNDS]);
void speck_encrypt(SPECK_TYPE const pt[2], SPECK_TYPE ct[2], SPECK_TYPE const K[SPECK_ROUNDS]);
void speck_decrypt(SPECK_TYPE const ct[2], SPECK_TYPE pt[2], SPECK_TYPE const K[SPECK_ROUNDS]);

void speck_encrypt_combined(SPECK_TYPE const pt[ 2], SPECK_TYPE ct[ 2], SPECK_TYPE const K[ SPECK_KEY_LEN]);
void speck_decrypt_combined(SPECK_TYPE const ct[ 2], SPECK_TYPE pt[ 2], SPECK_TYPE const K[ SPECK_KEY_LEN]);

#ifdef __cplusplus
}
#endif

#endif