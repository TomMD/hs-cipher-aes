/*
 *	Copyright (C) 2008 Vincent Hanquez <tab@snarc.org>
 *
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the author nor the names of his contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * AES implementation
 */
#ifndef AES_H
#define AES_H

#include <stdint.h>
#include "block128.h"

typedef block128 aes_block;

typedef struct {
	uint8_t nbr; /* number of rounds: 10 (128), 12 (192), 14 (256) */
	uint8_t _padding[7];
	uint8_t data[16*14*2];
} aes_key;

/* size = 4*16+2*8+aes_key=456 = 536 */
typedef struct {
	aes_block tag;
	aes_block h;
	aes_block iv;
	aes_block civ;
	uint64_t length_aad;
	uint64_t length_input;
	aes_key key;
} aes_gcm;

/* in bytes: either 16,24,32 */
void aes_initkey(aes_key *ctx, uint8_t *key, uint8_t size);

void aes_encrypt(aes_block *output, aes_key *key, aes_block *input);
void aes_decrypt(aes_block *output, aes_key *key, aes_block *input);

void aes_encrypt_ecb(uint8_t *output, aes_key *key, uint8_t *input, uint32_t nb_blocks);
void aes_decrypt_ecb(uint8_t *output, aes_key *key, uint8_t *input, uint32_t nb_blocks);

void aes_encrypt_cbc(uint8_t *output, aes_key *key, aes_block *iv, uint8_t *input, uint32_t nb_blocks);
void aes_decrypt_cbc(uint8_t *output, aes_key *key, aes_block *iv, uint8_t *input, uint32_t nb_blocks);

void aes_gen_ctr(uint8_t *output, aes_key *key, aes_block *iv, uint32_t nb_blocks);

void aes_encrypt_xts(uint8_t *output, aes_key *key, aes_key *key2, aes_block *sector,
                     uint32_t spoint, uint8_t *input, uint32_t nb_blocks);
void aes_decrypt_xts(uint8_t *output, aes_key *key, aes_key *key2, aes_block *sector,
                     uint32_t spoint, uint8_t *input, uint32_t nb_blocks);

void aes_gcm_init(aes_gcm *gcm, aes_key *key, uint8_t *iv, uint32_t len);
void aes_gcm_aad(aes_gcm *gcm, uint8_t *input, uint32_t length);
void aes_gcm_encrypt(uint8_t *output, aes_gcm *gcm, uint8_t *input, uint32_t length);
void aes_gcm_finish(uint8_t *tag, aes_gcm *gcm);

#endif
