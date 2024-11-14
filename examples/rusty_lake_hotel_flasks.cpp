#include <array>
#include <sstream>
#include <string>

#include "../general_solver.h"

std::string pair2String(const std::array<size_t, 2> &pair) {
  std::stringstream ss;
  ss << '(' << pair[0] << '>' << pair[1] << ')';
  return ss.str();
}

class FlaskState : public BaseState<FlaskState, std::string> {
 private:
  std::array<int, 3> amounts_;
  std::array<int, 3> limit_{10, 5, 6};

 public:
  FlaskState(const std::array<int, 3> &amounts) : amounts_(amounts) {}
  size_t hash() const {
    return amounts_[0] ^ (amounts_[1] << 10) ^ (amounts_[2] << 20);
  }
  bool operator==(const FlaskState &other) const {
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
          states.push_back({FlaskState(newAmount), pair2String({i, j})});
        }
    return states;
  }
};

int main() {
  FlaskState initState({5, 0, 6});
  // (2>0) -> (2>1) -> (0>2) -> (2>1) -> (1>0) -> (2>1) -> (0>2) -> (2>1) ->
  // (1>0)
  printMoves(bfs<FlaskState>(initState));
  return 0;
}
