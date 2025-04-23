#ifndef OPENING_BOOK_H
#define OPENING_BOOK_H

#include <iostream>
#include <fstream>
#include <cmath>
#include "Position.h"
#include "TranspositionTable.h"

using namespace std;

namespace GameSolver {
namespace Connect4 {

  class Book {
    private:
        int width;
        int height;
        int depth = -1;
        int log_size = 0;
        int size = 0;
        int partial_key_bytes = 0;
        int value_bytes = 1;
        
        vector<uint8_t> partial_keys;
        vector<uint8_t> values;
        
        uint64_t partial_key_mask = 0;
    
        uint64_t calculate_partial_key(uint64_t full_key) const {
            return full_key & partial_key_mask;
        }
    
        static bool has_factor(int n, int max) {
            for (int i = 3; i <= max; i += 2)
                if (n % i == 0) return true;
            return false;
        }
    
        static int next_prime(int n) {
            if (n <= 2) return 2;
            if (n % 2 == 0) n++;
            
            while (true) {
                const int max = sqrt(n) + 1;
                if (!has_factor(n, max)) return n;
                n += 2;
            }
        }
    
    public:
        Book(int w, int h) : width(w), height(h) {}
    
        bool load(const string& filename) {
            ifstream ifs(filename, ios::binary);
            if (!ifs.is_open()) {
                cerr << "Error opening file: " << filename << endl;
                return false;
            }
    
            // Read header
            char header[6];
            ifs.read(header, 6);
            
            int _width = header[0];
            int _height = header[1];
            int _depth = header[2];
            int _partial_key_bytes = header[3];
            int _value_bytes = header[4];
            int _log_size = header[5];
    
            // Validate header
            if (_width != width || _height != height || _value_bytes != 1) {
                cerr << "Invalid header" << endl;
                return false;
            }
    
            // Calculate table size
            int target_size = 1 << _log_size;
            size = next_prime(target_size);
            
            // Initialize partial key mask
            switch (_partial_key_bytes) {
                case 1: partial_key_mask = 0xFF; break;
                case 2: partial_key_mask = 0xFFFF; break;
                case 4: partial_key_mask = 0xFFFFFFFF; break;
                default:
                    cerr << "Invalid partial key size" << endl;
                    return false;
            }
    
            // Read partial keys
            partial_keys.resize(size * _partial_key_bytes);
            ifs.read(reinterpret_cast<char*>(partial_keys.data()), partial_keys.size());
    
            // Read values
            values.resize(size);
            ifs.read(reinterpret_cast<char*>(values.data()), values.size());
    
            if (!ifs) {
                cerr << "Error reading file" << endl;
                return false;
            }
    
            depth = _depth;
            log_size = _log_size;
            partial_key_bytes = _partial_key_bytes;
            return true;
        }
    
        bool save(const string& filename) const {
            ofstream ofs(filename, ios::binary);
            if (!ofs.is_open()) return false;
    
            // Write header
            char header[6] = {
                static_cast<char>(width),
                static_cast<char>(height),
                static_cast<char>(depth),
                static_cast<char>(partial_key_bytes),
                static_cast<char>(value_bytes),
                static_cast<char>(log_size)
            };
            ofs.write(header, 6);
    
            // Write partial keys and values
            ofs.write(reinterpret_cast<const char*>(partial_keys.data()), partial_keys.size());
            ofs.write(reinterpret_cast<const char*>(values.data()), values.size());
    
            return !!ofs;
        }
    
        int get(const Position& P) const {
            if (depth == -1 || P.nbMoves() > depth || size == 0)
                return 0;
    
            const uint64_t full_key = P.key3();
            const size_t index = full_key % size;
            const uint64_t expected_key = calculate_partial_key(full_key);
    
            // Read stored partial key
            uint64_t stored_key = 0;
            memcpy(&stored_key, partial_keys.data() + index * partial_key_bytes, partial_key_bytes);
    
            return (stored_key == expected_key) ? values[index] : 0;
        }
    
        bool is_loaded() const { 
            return depth != -1 && !partial_keys.empty() && !values.empty();
        }
    };
} // namespace Connect4
} // namespace GameSolver
#endif
