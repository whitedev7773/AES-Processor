#ifndef AES_H
#define AES_H

#include <stdint.h>

#define AES_BLOCK_SIZE 16 // 128 bits
#define AES_KEY_SIZE 16   // 128 bits

void aes_encrypt(const uint8_t *input, const uint8_t *key, uint8_t *output);
void aes_decrypt(const uint8_t *input, const uint8_t *key, uint8_t *output);

#endif // AES_H
