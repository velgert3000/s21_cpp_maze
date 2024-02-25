#include "s21_maze.h"

#include <algorithm>
#include <map>
#include <tuple>

void s21::Maze::Initialize(const std::string filename) {
  ClearData();
  std::tie(rows_, cols_) = ParseFile(filename, "maze", r_walls_, b_walls_);
  is_empty_ = false;
}

bool s21::Maze::Empty() const { return is_empty_; }

void s21::Maze::ClearData() {
  r_walls_.clear();
  b_walls_.clear();
  is_empty_ = true;
}

int s21::Maze::GetRows() const { return rows_; }
int s21::Maze::GetCols() const { return cols_; }
const std::vector<bool>& s21::Maze::GetRWalls() const { return r_walls_; }
const std::vector<bool>& s21::Maze::GetBWalls() const { return b_walls_; }

void s21::Maze::Save(const std::string filename) {
  SaveToFile(filename, rows_, cols_, r_walls_, b_walls_);
}

void s21::Maze::GenerateMaze(int cols, int rows, int seed) {
  cols = std::clamp(cols, 1, 50);
  rows = std::clamp(rows, 1, 50);
  srand(seed);
  this->cols_ = cols;
  this->rows_ = rows;
  int next_set_id = 1;
  r_walls_ = std::vector<bool>(rows * cols, false);
  b_walls_ = std::vector<bool>(rows * cols, false);
  auto row_sets = std::vector<int>(cols, 0);
  for (int current_row_index = 0; current_row_index < rows;
       current_row_index++) {
    MarkupCells(row_sets, next_set_id, current_row_index);
    PlaceRightWalls(row_sets, current_row_index);
    PlaceBottomWalls(row_sets, current_row_index);
  }

  this->is_empty_ = false;
}

void s21::Maze::PlaceRightWalls(std::vector<int>& row_sets,
                                int current_row_index) {
  for (int i = 0; i < cols_; i++) {
    if (i == cols_ - 1 || row_sets[i] == row_sets[i + 1] ||
        rand() > RAND_MAX / 2) {
      r_walls_[current_row_index * cols_ + i] = true;
    } else {
      int old_set_id = row_sets[i];
      for (int j = 0; j < cols_; j++) {
        if (row_sets[j] == old_set_id) {
          row_sets[j] = row_sets[i + 1];
        }
      }
    }
  }
  if (current_row_index == rows_ - 1) {
    for (int i = 0; i < cols_ - 1; i++) {
      if (row_sets[i] != row_sets[i + 1]) {
        r_walls_[current_row_index * cols_ + i] = false;
        int old_set_id = row_sets[i];
        for (int j = 0; j < cols_; j++) {
          if (row_sets[j] == old_set_id) {
            row_sets[j] = row_sets[i + 1];
          }
        }
      }
    }
  }
}

void s21::Maze::PlaceBottomWalls(std::vector<int>& row_sets,
                                 int current_row_index) {
  std::map<int, int> set_size;
  for (int i = 0; i < cols_; i++) {
    set_size[row_sets[i]] += 1;
  }
  for (int i = 0; i < cols_; i++) {
    if (rand() > (RAND_MAX / 2) && set_size[row_sets[i]] > 1) {
      b_walls_[current_row_index * cols_ + i] = true;
      set_size[row_sets[i]] -= 1;
    } else if (current_row_index == rows_ - 1) {
      b_walls_[current_row_index * cols_ + i] = true;
    }
  }
}

void s21::Maze::MarkupCells(std::vector<int>& row_sets_, int& next_set_id,
                            int current_row_index) {
  for (int i = 0; i < cols_; i++) {
    if (current_row_index > 0 && IsBWall(current_row_index - 1, i)) {
      row_sets_[i] = 0;
    }
    if (row_sets_[i] == 0) {
      row_sets_[i] = next_set_id++;
    }
  }
}

bool s21::Maze::IsCanMoveLeft(std::vector<std::vector<int>>& visited_,
                              std::stack<std::pair<int, int>>& path_,
                              std::pair<int, int>& current_) {
  bool result = true;
  if ((current_.second - 1) < 0 ||
      visited_[current_.first][current_.second - 1] ||
      IsRWall(current_.first, current_.second - 1)) {
    result = false;
  } else {
    path_.push({current_.first, current_.second - 1});
    visited_[current_.first][current_.second - 1] = 1;
  }
  return result;
}

bool s21::Maze::IsCanMoveRight(std::vector<std::vector<int>>& visited_,
                               std::stack<std::pair<int, int>>& path_,
                               std::pair<int, int>& current_) {
  bool result = true;
  if ((current_.second + 1) >= cols_ ||
      visited_[current_.first][current_.second + 1] ||
      IsRWall(current_.first, current_.second)) {
    result = false;
  } else {
    path_.push({current_.first, current_.second + 1});
    visited_[current_.first][current_.second + 1] = 1;
  }
  return result;
}

bool s21::Maze::IsCanMoveUp(std::vector<std::vector<int>>& visited_,
                            std::stack<std::pair<int, int>>& path_,
                            std::pair<int, int>& current_) {
  bool result = true;
  if ((current_.first - 1) < 0 ||
      visited_[current_.first - 1][current_.second] ||
      IsBWall(current_.first - 1, current_.second)) {
    result = false;
  } else {
    path_.push({current_.first - 1, current_.second});
    visited_[current_.first - 1][current_.second] = 1;
  }
  return result;
}

bool s21::Maze::IsCanMoveDown(std::vector<std::vector<int>>& visited_,
                              std::stack<std::pair<int, int>>& path_,
                              std::pair<int, int>& current_) {
  bool result = true;
  if ((current_.first + 1) >= rows_ ||
      visited_[current_.first + 1][current_.second] ||
      IsBWall(current_.first, current_.second)) {
    result = false;
  } else {
    path_.push({current_.first + 1, current_.second});
    visited_[current_.first + 1][current_.second] = 1;
  }
  return result;
}

bool s21::Maze::IsBWall(int r, int c) { return b_walls_[r * cols_ + c]; }

bool s21::Maze::IsRWall(int r, int c) { return r_walls_[r * cols_ + c]; }

void s21::Maze::FindPath(std::stack<std::pair<int, int>>& path,
                         std::pair<int, int> start, std::pair<int, int> end) {
  start.first = std::clamp(start.first, 0, rows_ - 1);
  start.second = std::clamp(start.second, 0, cols_ - 1);
  end.first = std::clamp(end.first, 0, rows_ - 1);
  end.second = std::clamp(end.second, 0, cols_ - 1);

  auto visited = std::vector<std::vector<int>>(
      std::clamp(rows_, 1, 50), std::vector<int>(std::clamp(cols_, 1, 50), 0));
  std::pair<int, int> current;

  visited[start.first][start.second] = 1;
  path.push(start);
  while (!path.empty()) {
    current = path.top();
    if (current == end) {
      break;
    }
    if (!(IsCanMoveDown(visited, path, current) ||
          IsCanMoveLeft(visited, path, current) ||
          IsCanMoveRight(visited, path, current) ||
          IsCanMoveUp(visited, path, current))) {
      path.pop();
    }
  }
}