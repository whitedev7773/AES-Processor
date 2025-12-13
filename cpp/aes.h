#ifndef AES_H
#define AES_H

#include <cstdint>
#include <vector>

#define AES_BLOCK_SIZE 16
#define AES_KEY_SIZE 16

class AES {
public:
    AES(const std::vector<uint8_t>& key);
    std::vector<uint8_t> encrypt(const std::vector<uint8_t>& input);
    std::vector<uint8_t> decrypt(const std::vector<uint8_t>& input);

private:
    void key_expansion(const std::vector<uint8_t>& key);
    void add_round_key(uint8_t state[4][4], int round);
    void sub_bytes(uint8_t state[4][4]);
    void shift_rows(uint8_t state[4][4]);
    void mix_columns(uint8_t state[4][4]);
    void inv_sub_bytes(uint8_t state[4][4]);
    void inv_shift_rows(uint8_t state[4][4]);
    void inv_mix_columns(uint8_t state[4][4]);

    std::vector<uint8_t> round_key;
};

#endif // AES_H
