#include <iostream>
#include <vector>
#include <cassert>
#include "aes.h"

// Helper function to convert a hex string to a byte vector
std::vector<uint8_t> hex_string_to_bytes(const std::string& hex_str) {
    std::vector<uint8_t> bytes;
    for (size_t i = 0; i < hex_str.length(); i += 2) {
        std::string byte_string = hex_str.substr(i, 2);
        uint8_t byte = static_cast<uint8_t>(std::stoul(byte_string, nullptr, 16));
        bytes.push_back(byte);
    }
    return bytes;
}

int main() {
    std::vector<uint8_t> key = hex_string_to_bytes("2b7e151628aed2a6abf7158809cf4f3c");
    std::vector<uint8_t> plaintext = hex_string_to_bytes("3243f6a8885a308d313198a2e0370734");
    std::vector<uint8_t> expected_ciphertext = hex_string_to_bytes("3925841d02dc09fbdc118597196a0b32");

    AES aes(key);

    // Test encryption
    std::vector<uint8_t> ciphertext = aes.encrypt(plaintext);
    assert(ciphertext == expected_ciphertext);
    std::cout << "Encryption test passed." << std::endl;

    // Test decryption
    std::vector<uint8_t> decrypted_text = aes.decrypt(ciphertext);
    assert(decrypted_text == plaintext);
    std::cout << "Decryption test passed." << std::endl;

    return 0;
}
