/*
obs-websocket
Copyright (C) 2016-2021 Stephane Lepin <stephane.lepin@gmail.com>
Copyright (C) 2020-2021 Kyle Manning <tt2468@gmail.com>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program. If not, see <https://www.gnu.org/licenses/>
*/

#include <cstdint>
#include <cstring>
#include <random>
#include <string>
#include <vector>

#include "Crypto.h"
#include "plugin-macros.generated.h"

// ---- Minimal SHA-256 implementation ----

static inline uint32_t rotr32(uint32_t x, unsigned n)
{
	return (x >> n) | (x << (32 - n));
}

static const uint32_t sha256_k[64] = {
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

static void sha256_compress(uint32_t state[8], const uint8_t block[64])
{
	uint32_t w[64];
	for (int i = 0; i < 16; i++) {
		w[i] = ((uint32_t)block[i * 4] << 24) | ((uint32_t)block[i * 4 + 1] << 16) |
		       ((uint32_t)block[i * 4 + 2] << 8) | (uint32_t)block[i * 4 + 3];
	}
	for (int i = 16; i < 64; i++) {
		uint32_t s0 = rotr32(w[i - 15], 7) ^ rotr32(w[i - 15], 18) ^ (w[i - 15] >> 3);
		uint32_t s1 = rotr32(w[i - 2], 17) ^ rotr32(w[i - 2], 19) ^ (w[i - 2] >> 10);
		w[i] = w[i - 16] + s0 + w[i - 7] + s1;
	}
	uint32_t a = state[0], b = state[1], c = state[2], d = state[3];
	uint32_t e = state[4], f = state[5], g = state[6], h = state[7];
	for (int i = 0; i < 64; i++) {
		uint32_t S1 = rotr32(e, 6) ^ rotr32(e, 11) ^ rotr32(e, 25);
		uint32_t ch = (e & f) ^ ((~e) & g);
		uint32_t temp1 = h + S1 + ch + sha256_k[i] + w[i];
		uint32_t S0 = rotr32(a, 2) ^ rotr32(a, 13) ^ rotr32(a, 22);
		uint32_t maj = (a & b) ^ (a & c) ^ (b & c);
		uint32_t temp2 = S0 + maj;
		h = g;
		g = f;
		f = e;
		e = d + temp1;
		d = c;
		c = b;
		b = a;
		a = temp1 + temp2;
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

static void sha256_hash(const uint8_t *data, size_t len, uint8_t out[32])
{
	uint32_t state[8] = {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
			     0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};
	uint64_t bitlen = (uint64_t)len * 8;
	uint8_t block[64];

	while (len >= 64) {
		sha256_compress(state, data);
		data += 64;
		len -= 64;
	}

	memset(block, 0, 64);
	memcpy(block, data, len);
	block[len] = 0x80;
	if (len >= 56) {
		sha256_compress(state, block);
		memset(block, 0, 64);
	}
	for (int i = 0; i < 8; i++)
		block[56 + i] = (uint8_t)(bitlen >> (56 - 8 * i));
	sha256_compress(state, block);

	for (int i = 0; i < 8; i++) {
		out[i * 4 + 0] = (uint8_t)(state[i] >> 24);
		out[i * 4 + 1] = (uint8_t)(state[i] >> 16);
		out[i * 4 + 2] = (uint8_t)(state[i] >> 8);
		out[i * 4 + 3] = (uint8_t)(state[i]);
	}
}

// ---- Base64 encoding ----

static const char base64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static std::string base64_encode(const uint8_t *data, size_t len)
{
	std::string out;
	out.reserve(((len + 2) / 3) * 4);
	for (size_t i = 0; i < len; i += 3) {
		uint32_t b = (uint32_t)data[i] << 16;
		if (i + 1 < len)
			b |= (uint32_t)data[i + 1] << 8;
		if (i + 2 < len)
			b |= (uint32_t)data[i + 2];
		out += base64_chars[(b >> 18) & 0x3f];
		out += base64_chars[(b >> 12) & 0x3f];
		out += (i + 1 < len) ? base64_chars[(b >> 6) & 0x3f] : '=';
		out += (i + 2 < len) ? base64_chars[b & 0x3f] : '=';
	}
	return out;
}

// ---- Public API ----

static const char allowedChars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
static const int allowedCharsCount = static_cast<int>(sizeof(allowedChars) - 1);

std::string Utils::Crypto::GenerateSalt()
{
	// Use std::random_device directly for better randomness
	std::random_device rd;
	std::vector<uint8_t> randomBytes(32);
	for (size_t i = 0; i + 3 < randomBytes.size(); i += 4) {
		uint32_t r = rd();
		randomBytes[i + 0] = (uint8_t)(r & 0xFF);
		randomBytes[i + 1] = (uint8_t)((r >> 8) & 0xFF);
		randomBytes[i + 2] = (uint8_t)((r >> 16) & 0xFF);
		randomBytes[i + 3] = (uint8_t)((r >> 24) & 0xFF);
	}
	return base64_encode(randomBytes.data(), randomBytes.size());
}

std::string Utils::Crypto::GenerateSecret(std::string password, std::string salt)
{
	std::string combined = password + salt;
	uint8_t digest[32];
	sha256_hash(reinterpret_cast<const uint8_t *>(combined.data()), combined.size(), digest);
	return base64_encode(digest, 32);
}

bool Utils::Crypto::CheckAuthenticationString(std::string secret, std::string challenge, std::string authenticationString)
{
	std::string combined = secret + challenge;
	uint8_t digest[32];
	sha256_hash(reinterpret_cast<const uint8_t *>(combined.data()), combined.size(), digest);
	std::string expected = base64_encode(digest, 32);
	return (authenticationString == expected);
}

std::string Utils::Crypto::GeneratePassword(size_t length)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dist(0, allowedCharsCount - 1);

	std::string ret;
	ret.reserve(length);
	for (size_t i = 0; i < length; i++)
		ret += allowedChars[dist(gen)];
	return ret;
}
