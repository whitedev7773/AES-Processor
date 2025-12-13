#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "aes.h"

void print_usage() {
    printf("Usage: ./aes --encrypt|--decrypt --key <key_file> (--input <input_file> | --text <hex_string>) --output <output_file>\n");
}

// Function to read a hex string from a file and convert it to a byte array
int read_hex_file(const char *filename, uint8_t *buffer, int buffer_size) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("Error opening file");
        return -1;
    }

    char hex_str[buffer_size * 2 + 1];
    if (fgets(hex_str, sizeof(hex_str), fp) == NULL) {
        fprintf(stderr, "Error reading from file\n");
        fclose(fp);
        return -1;
    }

    for (int i = 0; i < buffer_size; i++) {
        if (sscanf(&hex_str[i * 2], "%2hhx", &buffer[i]) != 1) {
            fprintf(stderr, "Invalid hex string in file\n");
            fclose(fp);
            return -1;
        }
    }

    fclose(fp);
    return 0;
}

int hex_string_to_bytes(const char *hex_str, uint8_t *buffer, int buffer_size) {
    if (strlen(hex_str) != buffer_size * 2) {
        fprintf(stderr, "Invalid hex string length\n");
        return -1;
    }
    for (int i = 0; i < buffer_size; i++) {
        if (sscanf(&hex_str[i * 2], "%2hhx", &buffer[i]) != 1) {
            fprintf(stderr, "Invalid hex string\n");
            return -1;
        }
    }
    return 0;
}


// Function to write a byte array to a file as a hex string
int write_hex_file(const char *filename, const uint8_t *buffer, int buffer_size) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        perror("Error opening file");
        return -1;
    }

    for (int i = 0; i < buffer_size; i++) {
        fprintf(fp, "%02x", buffer[i]);
    }

    fclose(fp);
    return 0;
}


int main(int argc, char *argv[]) {
    if (argc != 7 && argc != 8) {
        print_usage();
        return 1;
    }

    int encrypt = -1; // 1 for encrypt, 0 for decrypt
    char *key_file = NULL;
    char *input_file = NULL;
    char *text_input = NULL;
    char *output_file = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--encrypt") == 0) {
            encrypt = 1;
        } else if (strcmp(argv[i], "--decrypt") == 0) {
            encrypt = 0;
        } else if (strcmp(argv[i], "--key") == 0) {
            key_file = argv[++i];
        } else if (strcmp(argv[i], "--input") == 0) {
            input_file = argv[++i];
        } else if (strcmp(argv[i], "--text") == 0) {
            text_input = argv[++i];
        }
        else if (strcmp(argv[i], "--output") == 0) {
            output_file = argv[++i];
        } else {
            print_usage();
            return 1;
        }
    }

    if (encrypt == -1 || key_file == NULL || (input_file == NULL && text_input == NULL) || output_file == NULL) {
        print_usage();
        return 1;
    }

    if (input_file != NULL && text_input != NULL) {
        printf("Please provide input using either --input or --text, not both.\n");
        print_usage();
        return 1;
    }


    uint8_t key[AES_KEY_SIZE];
    uint8_t input[AES_BLOCK_SIZE];
    uint8_t output[AES_BLOCK_SIZE];

    if (read_hex_file(key_file, key, AES_KEY_SIZE) != 0) {
        fprintf(stderr, "Error reading key file\n");
        return 1;
    }

    if (input_file != NULL) {
        if (read_hex_file(input_file, input, AES_BLOCK_SIZE) != 0) {
            fprintf(stderr, "Error reading input file\n");
            return 1;
        }
    } else {
        if (hex_string_to_bytes(text_input, input, AES_BLOCK_SIZE) != 0) {
            fprintf(stderr, "Error parsing text input\n");
            return 1;
        }
    }


    if (encrypt) {
        aes_encrypt(input, key, output);
    } else {
        aes_decrypt(input, key, output);
    }

    if (write_hex_file(output_file, output, AES_BLOCK_SIZE) != 0) {
        fprintf(stderr, "Error writing output file\n");
        return 1;
    }

    printf("Operation completed successfully.\n");

    return 0;
}
