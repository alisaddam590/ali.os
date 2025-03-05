#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>

#define USERS_FILE "/etc/users"
#define SHADOW_FILE "/etc/shadow_users"

// Define ROTRIGHT macro
#define ROTRIGHT(x, n) ((x >> n) | (x << (32 - n)))

// SHA-256 Constants
static const uint32_t k[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

// SHA-256 Transformation Function
void sha256_transform(uint32_t *state, const uint8_t *data) {
    uint32_t w[64], a, b, c, d, e, f, g, h, i, j, t1, t2;
    for (i = 0; i < 16; ++i)
        w[i] = (data[i * 4] << 24) | (data[i * 4 + 1] << 16) | (data[i * 4 + 2] << 8) | (data[i * 4 + 3]);
    for (; i < 64; ++i)
        w[i] = w[i - 16] + (ROTRIGHT(w[i - 15], 7) ^ ROTRIGHT(w[i - 15], 18) ^ (w[i - 15] >> 3)) +
               w[i - 7] + (ROTRIGHT(w[i - 2], 17) ^ ROTRIGHT(w[i - 2], 19) ^ (w[i - 2] >> 10));

    a = state[0];
    b = state[1];
    c = state[2];
    d = state[3];
    e = state[4];
    f = state[5];
    g = state[6];
    h = state[7];

    for (i = 0; i < 64; ++i) {
        t1 = h + (ROTRIGHT(e, 6) ^ ROTRIGHT(e, 11) ^ ROTRIGHT(e, 25)) + ((e & f) ^ (~e & g)) + k[i] + w[i];
        t2 = (ROTRIGHT(a, 2) ^ ROTRIGHT(a, 13) ^ ROTRIGHT(a, 22)) + ((a & b) ^ (a & c) ^ (b & c));
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;
    }

    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
    state[4] += e;
    state[5] += f;
    state[6] += g;
    state[7] += h;
}

// SHA-256 Hashing Function
void sha256(const char *input, char *output) {
    uint32_t state[8] = { 0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
                           0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19 };
    uint8_t data[64] = {0};
    size_t len = strlen(input);
    memcpy(data, input, len);

    data[len] = 0x80;
    *(uint64_t *)(data + 56) = len * 8;

    sha256_transform(state, data);

    for (int i = 0; i < 8; i++)
        sprintf(output + (i * 8), "%08x", state[i]);

    output[64] = '\0';
}

int authenticate_user(const char *username, const char *password) {
    FILE *shadow_file = fopen(SHADOW_FILE, "r");
    if (!shadow_file) {
        perror("Error opening shadow file");
        return -1;
    }

    char line[256];
    while (fgets(line, sizeof(line), shadow_file)) {
        char stored_user[50], stored_hash[256];
        sscanf(line, "%[^:]:%s", stored_user, stored_hash);
        if (strcmp(stored_user, username) == 0) {
            fclose(shadow_file);

            // Hash the input password using SHA-256
            char hashed_pass[65];
            sha256(password, hashed_pass);

            // Compare the hashed password
            return strcmp(stored_hash, hashed_pass) == 0;
        }
    }

    fclose(shadow_file);
    return 0;
}

void start_shell(const char *username) {
    printf("Welcome, %s!\n", username);
    execlp("/bin/lash", "/bin/lash", (char *) NULL);
}

int main() {
    char username[50], password[50];

    printf("Username: ");
    scanf("%s", username);
    printf("Password: ");
    scanf("%s", password);

    if (authenticate_user(username, password)) {
        start_shell(username);
    } else {
        printf("Login failed.\n");
    }

    return 1;
}
