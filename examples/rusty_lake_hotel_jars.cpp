#include <array>
#include <sstream>
#include <string>

#include "../general_solver.h"

std::string tuple2String(const std::array<size_t, 2> &tuple) {
  std::stringstream ss;
  ss << '(' << tuple[0] << '>' << tuple[1] << ')';
  return ss.str();
}

class JarState : public BaseState<JarState, std::string> {
 private:
  std::array<int, 3> amounts_;
  std::array<int, 3> limit_{10, 5, 6};

 public:
  JarState(const std::array<int, 3> &amounts) : amounts_(amounts) {}
  size_t hash() const {
    return amounts_[0] ^ (amounts_[1] << 10) ^ (amounts_[2] << 20);
  }
  bool operator==(const JarState &other) const {
    return amounts_ == other.amounts_;
  }
  bool isFinal() const { return amounts_[0] == 8; }
  std::vector<State_Move> nextStates() {
    std::vector<State_Move> states;
    for (size_t i = 0; i < 3; i++)
      for (size_t j = 0; j < 3; j++)
        if (i != j && amounts_[i] > 0 && amounts_[j] < limit_[j]) {
          size_t k = 3 - i - j;
          std::array<int, 3> newAmount{0, 0, 0};
          newAmount[k] = amounts_[k];
          if (amounts_[i] + amounts_[j] <= limit_[j]) {
            newAmount[j] = amounts_[i] + amounts_[j];
            newAmount[i] = 0;
          } else {
            newAmount[j] = limit_[j];
            newAmount[i] = amounts_[i] + amounts_[j] - limit_[j];
          }
          states.push_back({JarState(newAmount), tuple2String({i, j})});
        }
    return states;
  }
};

int main() {
  JarState initState({5, 0, 6});
  // (2>0) -> (2>1) -> (0>2) -> (2>1) -> (1>0) -> (2>1) -> (0>2) -> (2>1) -> (1>0)
  printMoves(bfs<JarState>(initState));
  return 0;
}
