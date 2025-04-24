#include "Solver.h"
#include <iostream>
#include <string>
#include <vector>

using namespace GameSolver::Connect4;
using namespace std;

int main(int argc, char** argv) {
  Solver solver;
  bool weak = false;
  bool analyze = true;

  string opening_book = "7x6.book";
  solver.loadBook(opening_book);

  string line;

  // freopen("input.INP", "r", stdin);
  // freopen("cpp.txt", "w", stdout);

  // int d=0;
  // string s;
  // while (cin>>s)
  // {
  //   if(s=="") break;
  //   Position P;
  //   if(P.play(s) != s.size()) {
  //     cerr << "Line " << 1 << ": Invalid move " << (P.nbMoves() + 1) << " \"" << s << "\"" << endl;
  //     d++;
  //     if(d==10) return 0;
  //   } else {
  //     vector<int> scores = solver.analyze(P, weak);
  //     for(int i = 0; i < Position::WIDTH; i++) cout << scores[i] << " ";
  //     cout<<'\n';
  //   }
  // }

  for(int l = 1; getline(cin, line); l++) {
    Position P;
    if(P.play(line) != line.size()) {
      cerr << "Line " << l << ": Invalid move " << (P.nbMoves() + 1) << " \"" << line << "\"" << endl;
    } else {
      cout << line;
      if(analyze) {
        vector<int> scores = solver.analyze(P, weak);
        for(int i = 0; i < Position::WIDTH; i++) cout << " " << scores[i];
      }
      else {
        int score = solver.solve(P, weak);
        cout << " " << score;
      }
      cout << endl;
    }
  }
}
