#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include "aes.h"

void print_usage() {
    std::cout << "Usage: ./aes --encrypt|--decrypt --key <key_file> (--input <input_file> | --text <hex_string>) --output <output_file>" << std::endl;
}

// Function to read a hex string from a file and convert it to a byte vector
std::vector<uint8_t> read_hex_file(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Error opening file: " + filename);
    }
    std::string hex_str;
    file >> hex_str;

    std::vector<uint8_t> bytes;
    for (size_t i = 0; i < hex_str.length(); i += 2) {
        std::string byte_string = hex_str.substr(i, 2);
        uint8_t byte = static_cast<uint8_t>(std::stoul(byte_string, nullptr, 16));
        bytes.push_back(byte);
    }
    return bytes;
}

// Function to convert a hex string to a byte vector
std::vector<uint8_t> hex_string_to_bytes(const std::string& hex_str) {
    std::vector<uint8_t> bytes;
    for (size_t i = 0; i < hex_str.length(); i += 2) {
        std::string byte_string = hex_str.substr(i, 2);
        uint8_t byte = static_cast<uint8_t>(std::stoul(byte_string, nullptr, 16));
        bytes.push_back(byte);
    }
    return bytes;
}

// Function to write a byte vector to a file as a hex string
void write_hex_file(const std::string& filename, const std::vector<uint8_t>& bytes) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Error opening file: " + filename);
    }
    for (const auto& byte : bytes) {
        file << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
    }
}


int main(int argc, char *argv[]) {
    if (argc != 7 && argc != 8) {
        print_usage();
        return 1;
    }

    bool encrypt = false;
    std::string key_file, input_file, text_input, output_file;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--encrypt") {
            encrypt = true;
        } else if (arg == "--decrypt") {
            encrypt = false;
        } else if (arg == "--key") {
            key_file = argv[++i];
        } else if (arg == "--input") {
            input_file = argv[++i];
        } else if (arg == "--text") {
            text_input = argv[++i];
        } else if (arg == "--output") {
            output_file = argv[++i];
        } else {
            print_usage();
            return 1;
        }
    }
    
    if (key_file.empty() || (input_file.empty() && text_input.empty()) || output_file.empty()) {
        print_usage();
        return 1;
    }

    if (!input_file.empty() && !text_input.empty()) {
        std::cerr << "Please provide input using either --input or --text, not both." << std::endl;
        print_usage();
        return 1;
    }

    try {
        std::vector<uint8_t> key = read_hex_file(key_file);
        std::vector<uint8_t> input;

        if (!input_file.empty()) {
            input = read_hex_file(input_file);
        } else {
            input = hex_string_to_bytes(text_input);
        }

        AES aes(key);
        std::vector<uint8_t> output;

        if (encrypt) {
            output = aes.encrypt(input);
        } else {
            output = aes.decrypt(input);
        }

        write_hex_file(output_file, output);
        std::cout << "Operation completed successfully." << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
