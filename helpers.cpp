#include <iostream>
#include <cmath>
#include <string>
#include <cstring>
#include <iomanip>
#include <cstdio>


using namespace std;

typedef unsigned int uint32;
typedef unsigned char byte;

const uint32 constants[64] = {
  0x428a2f98,
  0x71374491,
  0xb5c0fbcf,
  0xe9b5dba5,
  0x3956c25b,
  0x59f111f1,
  0x923f82a4,
  0xab1c5ed5,
  0xd807aa98,
  0x12835b01,
  0x243185be,
  0x550c7dc3,
  0x72be5d74,
  0x80deb1fe,
  0x9bdc06a7,
  0xc19bf174,
  0xe49b69c1,
  0xefbe4786,
  0x0fc19dc6,
  0x240ca1cc,
  0x2de92c6f,
  0x4a7484aa,
  0x5cb0a9dc,
  0x76f988da,
  0x983e5152,
  0xa831c66d,
  0xb00327c8,
  0xbf597fc7,
  0xc6e00bf3,
  0xd5a79147,
  0x06ca6351,
  0x14292967,
  0x27b70a85,
  0x2e1b2138,
  0x4d2c6dfc,
  0x53380d13,
  0x650a7354,
  0x766a0abb,
  0x81c2c92e,
  0x92722c85,
  0xa2bfe8a1,
  0xa81a664b,
  0xc24b8b70,
  0xc76c51a3,
  0xd192e819,
  0xd6990624,
  0xf40e3585,
  0x106aa070,
  0x19a4c116,
  0x1e376c08,
  0x2748774c,
  0x34b0bcb5,
  0x391c0cb3,
  0x4ed8aa4a,
  0x5b9cca4f,
  0x682e6ff3,
  0x748f82ee,
  0x78a5636f,
  0x84c87814,
  0x8cc70208,
  0x90befffa,
  0xa4506ceb,
  0xbef9a3f7,
  0xc67178f2
};

#define PRIME_SEARCH_MAX 500
#define INT_BITS 32
#define BLOCK_SIZE 64

string binary(uint32 n) {
  string result = "";
  for (int i = 0; i < INT_BITS; i++) {
    if ((1 << i) & n) {
      result = "1" + result;
    } else {
      result = "0" + result;
    }
  }

  return result;
}

string binary(byte n) {
  string result = "";
  for (int i = 0; i < 8; i++) {
    if ((1 << i) & n) {
      result = "1" + result;
    } else {
      result = "0" + result;
    }
  }

  return result;
}

string hexa(uint32 n) {
  char characters[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
  string result = "";
  for (int i = 7; i >= 0; i--) {
    uint32 val = (n >> (i * 4)) & 15;

    result += characters[val];
  }

  return result;
}

bool isPrime(uint32 n) {
  if (n == 0 || n == 1) return false;

  for (uint32 i = 2; i <= n / 2; i++) {
    if (i != n) {
      if (n % i == 0) return false;
    }
  }

  return true;
}

uint32 getPrimeStartingAt(uint32 previousPrime) {
  for(uint32 i = previousPrime + 1; i <= PRIME_SEARCH_MAX; i++) {
    if (isPrime(i)) return i;
  }

  return 0;
}

uint32 rrot1(uint32 n) {
  if (n & 1) {
    return (n >> 1) | (1 << 31);
  }

  return (n >> 1);
}

uint32 lrot1(uint32 n) {
  uint32 x = (1 << 31);
  if (n & x) {
    return (n << 1) | 1;
  }

  return (n << 1);
}

uint32 lrot(uint32 n, uint32 s) {
  for(int i = 0; i < s; i++) {
    n = lrot1(n);
  }
  return n;
}

uint32 rrot(uint32 n, uint32 s) {
  for(int i = 0; i < s; i++) {
    n = rrot1(n);
  }
  return n;
}

uint32 decimal(string bin) {
  uint32 result = 0;
  for(int i = 0; i < INT_BITS; i++) {
    if (bin[i] == '1') {
      result |= (1 << (31 - i));
    }
  }

  return result;
}

void printBlock(byte * block) {
  for (uint32 i = 0; i < BLOCK_SIZE; i++) {
    cout << binary(block[i]) << endl;
  }
}

void moveByte(byte & b, uint32 n, short index) {
  b = n >> min(index * 8, 31);
}

void moveArrayDown(uint32 * array, short size) {
  for(int i = size - 1; i >= 1; i--) {
    array[i] = array[i - 1];
  }
}