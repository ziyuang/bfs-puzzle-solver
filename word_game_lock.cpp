#include <array>
#include <numeric>
#include <string>

#include "general_solver.h"

class WordLockState : public BaseState<WordLockState, std::string> {
private:
  constexpr static int CHAR_COUNT = 7;
  using Chars = std::array<std::string, CHAR_COUNT>;

public:
  WordLockState(const Chars &characters) : c_(characters) {}
  size_t hash() const {
    // https://stackoverflow.com/a/72073933/688080
    return std::accumulate(c_.begin(), c_.end(), c_.size(), [](std::size_t h, const std::string &mbchar) {
      size_t x = std::hash<std::string>{}(mbchar);
      x = ((x >> 16) ^ x) * 0x45d9f3b;
      x = ((x >> 16) ^ x) * 0x45d9f3b;
      x = (x >> 16) ^ x;
      return h ^ (x + 0x9e3779b9 + (h << 6) + (h >> 2));
    });
  }
  bool operator==(const WordLockState &other) const { return equalsTo<CHAR_COUNT - 1>(other.c_); }
  bool isFinal() const { return (*this) == answer_; }
  std::vector<State_Move> nextStates() {
    return std::vector<State_Move>{{WordLockState(rotateMiddle()), "middle"}, {WordLockState(rotateRight()), "right"}};
  }

private:
  Chars c_;
  static const WordLockState answer_;

  template <int tail> bool equalsTo(const Chars &other) const {
    if constexpr (tail == 0)
      return c_[0] == other[0];
    else
      return c_[tail] == other[tail] ? equalsTo<tail - 1>(other) : false;
  }

  Chars rotateMiddle() { return Chars{c_[2], c_[1], c_[5], c_[0], c_[4], c_[3], c_[6]}; }
  Chars rotateRight() { return Chars{c_[0], c_[3], c_[2], c_[6], c_[1], c_[5], c_[4]}; }
};

const WordLockState WordLockState::answer_({"的", "蠢", "封", "连", "的", "可", "维"});

int main() {
  WordLockState initState({"可", "封", "的", "蠢", "维", "的", "连"});
  // right -> right -> right -> middle -> middle -> right -> right
  printMoves(bfs<WordLockState>(initState));
  return 0;
}