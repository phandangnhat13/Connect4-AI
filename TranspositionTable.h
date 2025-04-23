#ifndef TRANSPOSITION_TABLE_H
#define TRANSPOSITION_TABLE_H

#include <cstring>
#include <vector>
#include <algorithm>
#include <iostream>
#include <ostream>

using namespace std;

namespace GameSolver {
namespace Connect4 {

/**
 * util functions to compute next prime at compile time
 */
constexpr long long med(long long min, long long max) {
  return (min + max) / 2;
}
/**
 * tells if an long longeger n has a a divisor between min (inclusive) and max (exclusive)
 */
constexpr bool has_factor(long long n, long long min, long long max) {
  return min * min > n ? false : // do not search for factor above sqrt(n)
         min + 1 >= max ? n % min == 0 :
         has_factor(n, min, med(min, max)) || has_factor(n, med(min, max), max);
}

// return next prime number greater or equal to n.
// n must be >= 2
constexpr long long next_prime(long long n) {
  return has_factor(n, 2, n) ? next_prime(n + 1) : n;
}

// log2(1) = 0; log2(2) = 1; log2(3) = 1; log2(4) = 2; log2(8) = 3
constexpr long long log2(long long n) {
  return n <= 1 ? 0 : log2(n / 2) + 1;
}

/**
 * Transposition Table is a simple hash map with fixed storage size.
 * In case of collision we keep the last entry and overide the previous one.
 * We keep only part of the key to reduce storage, but no error is possible thanks to Chinese theorem.
 *
 * The number of stored entries is a power of two that is defined at compile time.
 * We also define size of the entries and keys to allow optimization at compile time.
 *
 * key_size:   number of bits of the key
 * value_size: number of bits of the value
 * log_size:   base 2 log of the size of the Transposition Table.
 *             The table will contain 2^log_size elements
 */
class TranspositionTable {
 private:
  static const long long log_size = 24;
  static const long long size = next_prime(1 << log_size); // size of the transition table. Have to be odd to be prime with 2^sizeof(key_t)
  vector<uint64_t> K;     // Array to store truncated version of keys;
  vector<uint64_t> V;   // Array to store values;

  vector<uint64_t> getKeys() {return K;}
  vector<uint64_t> getValues() {return V;}

  long long getSize() {return size;}

  long long getKeySize() 
  {
    return (long long)K.size();
  }
  long long getValueSize() 
  {
    return (long long)V.size();
  }

  long long index(long long key) const {
    return key % size;
  }

 public:
  TranspositionTable() {
    K = vector<uint64_t>(size); // Array to store truncated version of keys;
    V = vector<uint64_t>(size); // Array to store values;
    reset();
  }

  ~TranspositionTable()  = default;

  /**
   * Empty the Transition Table.
   */
  void reset() {
    fill(K.begin(), K.end(), 0);
    fill(V.begin(), V.end(), 0);
  }

  /**
   * Store a value for a given key
   * @param key: must be less than key_size bits.
   * @param value: must be less than value_size bits. null (0) value is used to encode missing data
   */
  void put(uint64_t key, uint64_t value) {
    long long pos = index(key);
    K[pos] = key; // key is possibly trucated as key_t is possibly less than key_size bits.
    V[pos] = value;
  }

  /**
   * Get the value of a key
   * @param key: must be less than key_size bits.
   * @return value_size bits value associated with the key if present, 0 otherwise.
   */
  uint64_t get(uint64_t key) const {
    long long pos = index(key);
    // cerr<<"key: "<<key<<", pos: "<<pos<<", K[pos]: "<<K[pos]<<'\n';
    if(K[pos] == key) return V[pos]; // need to cast to key_t because key may be truncated due to size of key_t
    else return 0;
  }
};

} // namespace Connect4
} // namespace GameSolver

#endif
