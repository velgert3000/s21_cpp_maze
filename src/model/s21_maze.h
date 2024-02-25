#ifndef S21_MAZE_MAZE_H
#define S21_MAZE_MAZE_H

#include <stack>
#include <string>
#include <vector>

#include "s21_labyrinth.h"

namespace s21 {

class Maze : public Labyrinth {
 public:
  void Initialize(const std::string filename);
  bool Empty() const;
  int GetRows() const;
  int GetCols() const;
  const std::vector<bool>& GetRWalls() const;
  const std::vector<bool>& GetBWalls() const;
  void Save(const std::string filename);
  void GenerateMaze(int cols, int rows, int seed = time(0));
  void FindPath(std::stack<std::pair<int, int>>& path,
                std::pair<int, int> start, std::pair<int, int> end);

 private:
  std::vector<bool> r_walls_, b_walls_;
  void ClearData();
  void MarkupCells(std::vector<int>& row_sets, int& next_set_id_,
                   int current_row_index);
  void PlaceRightWalls(std::vector<int>& row_sets, int current_row_index);
  void PlaceBottomWalls(std::vector<int>& row_sets, int current_row_index);
  bool IsCanMoveLeft(std::vector<std::vector<int>>& visited,
                     std::stack<std::pair<int, int>>& path,
                     std::pair<int, int>& current);
  bool IsCanMoveRight(std::vector<std::vector<int>>& visited,
                      std::stack<std::pair<int, int>>& path,
                      std::pair<int, int>& current);
  bool IsCanMoveUp(std::vector<std::vector<int>>& visited,
                   std::stack<std::pair<int, int>>& path,
                   std::pair<int, int>& current_);
  bool IsCanMoveDown(std::vector<std::vector<int>>& visited,
                     std::stack<std::pair<int, int>>& path,
                     std::pair<int, int>& current);
  bool IsBWall(int r, int c);
  bool IsRWall(int r, int c);
};
}  // namespace s21
#endif  // S21_MAZE_MAZE_H
