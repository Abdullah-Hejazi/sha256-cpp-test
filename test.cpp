#include "helpers.cpp"

// abc hash:
// BA7816BF8F01CFEA414140DE5DAE2223B00361A396177A9CB410FF61F20015AD

class sha256 {
  string message;
  byte ** blocks;
  uint32 size;
  uint32 initialCompressionValues[8];

  uint32 compression[8] = {
    0x6a09e667,
    0xbb67ae85,
    0x3c6ef372,
    0xa54ff53a,
    0x510e527f,
    0x9b05688c,
    0x1f83d9ab,
    0x5be0cd19
  };

  public:
    sha256(string msg) {
      message = msg;

      fill();
      padding();

      for(uint32 i = 0; i < size; i++) {
        messageSchedule(blocks[i]);
      }
    }

    void fill() {
      size = ceil((float)message.size() / (float)BLOCK_SIZE);
      if (message.size() % BLOCK_SIZE == 0) size++;

      cout << "Message size: " << message.size() << endl;
      cout << "Number of blocks: " << size << endl;

      blocks = new byte*[size];

      for(uint32 i = 0; i < size; i++) {
        blocks[i] = new byte[BLOCK_SIZE];
      }

      for(uint32 i = 0; i < size; i++) {
        for(uint32 j = 0; j < message.size(); j++) {
          blocks[i][j] = (byte)message[(i * BLOCK_SIZE) + j];
        }
      }
    }

    void padding() {
      blocks[size - 1][(message.size() % 64)] = (1 << 7);

      cout << "Size in last block: " << (message.size() % 64) << endl;

      for(uint32 i = (message.size() % 64) + 1; i < BLOCK_SIZE; i++) {
        blocks[size - 1][i] = 0;
      }

      uint32 bitsSize = message.size() * 8;
      for(uint32 i = 64 - 2; i < 64; i++) {
        moveByte(blocks[size - 1][i], bitsSize, 64 - i - 1);
      }
    }

    void print() {
      for(int i = 0; i < size; i++) {
        printBlock(blocks[i]);
      }
    }

    void copyInitialValues() {
      for(uint32 i = 0; i < 8; i++) {
        initialCompressionValues[i] = compression[i];
      }
    }

    void messageSchedule(byte * block) {
      uint32 words[64];
      for(uint32 i = 0; i < 64; i += 4) {
        uint32 j = (uint32)(i / 4);
        words[j] = (block[i] << 24) | (block[i + 1] << 16) | (block[i + 2] << 8) | (block[i + 3]);
      }

      for(uint32 i = 16; i < 64; i++) {
        words[i] = segma1(words[i-2]) + words[i-7] + segma0(words[i-15]) + words[i-16];
      }

      performCompression(words);
    }

    void performCompression(uint32 schedule[64]) {
      copyInitialValues();

      for (uint32 i = 0; i < 64; i++) {
        uint32 T1 = csegma1(compression[4]) + 
                    choice(compression[4], compression[5], compression[6]) +
                    compression[7] + constants[i] + schedule[i];

        uint32 T2 = csegma0(compression[0]) + 
                    majority(compression[0], compression[1], compression[2]);

        moveArrayDown(compression, 8);
        compression[0] = T1+T2;
        compression[4] += T1;

      }

      for(uint32 j = 0; j < 8; j++) {
        compression[j] += initialCompressionValues[j];
      }
    }

    ~sha256() {
      for(uint32 i = 0; i < size; i++) {
        delete[] blocks[i];
      }

      delete[] blocks;
    }

    uint32 segma0(uint32 n) {
      return rrot(n, 7) ^ rrot(n, 18) ^ (n >> 3);
    }

    uint32 segma1(uint32 n) {
      return rrot(n, 17) ^ rrot(n, 19) ^ (n >> 10);
    }

    // capital segma 0
    uint32 csegma0(uint32 n) {
      return rrot(n, 2) ^ rrot(n, 13) ^ rrot(n, 22);
    }

    // capital segma 1
    uint32 csegma1(uint32 n) {
      return rrot(n, 6) ^ rrot(n, 11) ^ rrot(n, 25);
    }

    uint32 choice(uint32 x, uint32 y, uint32 z) {
      uint32 result = 0;
      for(int i = 0; i < INT_BITS; i++) {
        if ((1 << i) & x) {
          result |= ((1<<i) & y);
        } else {
          result |= ((1<<i) & z);
        }
      }

      return result;
    }

    uint32 majority(uint32 x, uint32 y, uint32 z) {
      uint32 result = 0;
      for(int i = 0; i < INT_BITS; i++) {
        uint32 xx = ((1 << i) & x) ? 1 : 0;
        uint32 yy = ((1 << i) & y) ? 1 : 0;
        uint32 zz = ((1 << i) & z) ? 1 : 0;

        if (xx + yy + zz == 3 || xx + yy + zz == 2) {
          result |= (1<<i);
        }
      }

      return result;
    }

    string getResult() {
      string result = "";

      for(uint32 i = 0; i < 8; i++) {
        result += hexa(compression[i]);
      }

      return result;
    }
};

int main() {
  //sha256 sha("abc");
  sha256 sha("abcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabca");
  cout << sha.getResult() << endl;
}