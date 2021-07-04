#include "helpers.cpp"

// abc hash:
// BA7816BF8F01CFEA414140DE5DAE2223B00361A396177A9CB410FF61F20015AD

class sha256 {
  string message;
  byte ** blocks;
  uint32 size;

  public:
    sha256(string msg) {
      message = msg;
      size = ceil((float)message.size() / (float)BLOCK_SIZE);

      blocks = new byte*[size];

      cout << "Message Size: " << size << endl;

      for(uint32 i = 0; i < size; i++) {
        blocks[i] = new byte[BLOCK_SIZE];
      }

      for(uint32 i = 0; i < size; i++) {
        for(uint32 j = 0; j < message.size(); j++) {
          blocks[i][j] = (byte)message[j];
        }
      }

      blocks[size - 1][size * message.size()] = (1 << 7);

      for(uint32 i = size * message.size() + 2; i < BLOCK_SIZE; i++) {
        blocks[size - 1][i] = 0;
      }
    }

    ~sha256() {
      for(uint32 i = 0; i < size; i++) {
        delete blocks[i];
      }

      delete blocks;
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
};

int main() {
  string x = "abc";
  sha256 sha("abc");
}