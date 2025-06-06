#ifndef MOVE_SORTER_H
#define MOVE_SORTER_H

#include "Position.h"

namespace GameSolver {
namespace Connect4 {

/**
 * This class helps sorting the next moves
 *
 * You have to add moves first with their score
 * then you can get them back in decreasing score
 *
 * This class implement an insertion sort that is in practice very
 * efficient for small number of move to sort (max is Position::WIDTH)
 * and also efficient if the move are pushed in approximatively increasing
 * order which can be acheived by using a simpler column ordering heuristic.
 */
class MoveSorter {
 public:

  /**
   * Add a move in the container with its score.
   * You cannot add more than Position::WIDTH moves
   */
  void add(const uint64_t move, const int score) {
    int pos = size++;
    for(; pos && entries[pos - 1].score > score; --pos) entries[pos] = entries[pos - 1];
    entries[pos].move = move;
    entries[pos].score = score;
  }

  /**
   * Get next move
   * @return next remaining move with max score and remove it from the container.
   * If no more move is available return 0
   */
  uint64_t getNext() {
    if(size)
      return entries[--size].move;
    else
      return 0;
  }

  /**
   * Build an empty container
   */
  MoveSorter(): size{0} {
  }

 private:
  // number of stored moves
  unsigned int size;

  // Contains size moves with their score ordered by score
  struct {
    uint64_t move;
    int score;
  } entries[Position::WIDTH];
};

} // namespace Connect4
} // namespace GameSolver
#endif
