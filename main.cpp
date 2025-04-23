#include "Solver.hpp"
#include <iostream>
#include <string>
#include <vector>

using namespace GameSolver::Connect4;
using namespace std;

/**
 * Main function.
 * Reads Connect 4 positions, line by line, from standard input
 * and writes one line per position to standard output containing:
 *  - score of the position
 *  - number of nodes explored
 *  - time spent in microsecond to solve the position.
 *
 *  Any invalid position (invalid sequence of move, or already won game)
 *  will generate an error message to standard error and an empty line to standard output.
 * 
 *  Weak solver
    Another benefit of alpha-beta is that you can easily implement a weak solver that only tells 
    you the win/draw/loss outcome of a position by calling evaluating a node with the [-1;1] score 
    window.
 */
int main(int argc, char** argv) {
  Solver solver;
  bool weak = false;
  bool analyze = true;

  std::string opening_book = "7x6.book";
  for(int i = 1; i < argc; i++) {
    if(argv[i][0] == '-') {
      if(argv[i][1] == 'w') weak = true; // parameter -w: use weak solver
      else if(argv[i][1] == 'b') { // paramater -b: define an alternative opening book
        if(++i < argc) opening_book = std::string(argv[i]);
      }
      else if(argv[i][1] == 'a') { // paramater -a: make an analysis of all possible moves
        analyze = true;
      }
    }
  }
  solver.loadBook(opening_book);

  std::string line;

  // freopen("input.INP", "r", stdin);
  // freopen("cpp.txt", "w", stdout);

  // int d=0;
  // string s;
  // while (cin>>s)
  // {
  //   if(s=="") break;
  //   Position P;
  //   if(P.play(s) != s.size()) {
  //     std::cerr << "Line " << 1 << ": Invalid move " << (P.nbMoves() + 1) << " \"" << s << "\"" << std::endl;
  //     d++;
  //     if(d==10) return 0;
  //   } else {
  //     std::vector<int> scores = solver.analyze(P, weak);
  //     for(int i = 0; i < Position::WIDTH; i++) cout << scores[i] << " ";
  //     cout<<'\n';
  //   }
  // }

  for(int l = 1; std::getline(std::cin, line); l++) {
    Position P;
    if(P.play(line) != line.size()) {
      std::cerr << "Line " << l << ": Invalid move " << (P.nbMoves() + 1) << " \"" << line << "\"" << std::endl;
    } else {
      std::cout << line;
      if(analyze) {
        std::vector<int> scores = solver.analyze(P, weak);
        for(int i = 0; i < Position::WIDTH; i++) std::cout << " " << scores[i];
      }
      else {
        int score = solver.solve(P, weak);
        std::cout << " " << score;
      }
      std::cout << std::endl;
    }
  }
}
