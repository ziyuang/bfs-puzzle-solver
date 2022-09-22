#include <array>
#include <numeric>
#include <string>

#include "general_solver.h"

class WordLockState : public BaseState<WordLockState, std::string> {
public:
  WordLockState(const std::array<std::string, 7> &characters) : c_(characters) {}
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
  bool operator==(const WordLockState &other) const {
    for (size_t i = 0; i < c_.size(); i++) {
      if (c_[i] != other.c_[i])
        return false;
    }
    return true;
  }
  bool isFinal() const { return (*this) == answer_; }
  std::vector<State_Move> nextStates() {
    return std::vector<State_Move>{{WordLockState(rotateMiddle()), "middle"},
                                      {WordLockState(rotateRight()), "right"}};
  }

private:
  std::array<std::string, 7> c_;
  static const WordLockState answer_;

  std::array<std::string, 7> rotateMiddle() {
    return std::array<std::string, 7>{c_[2], c_[1], c_[5], c_[0], c_[4], c_[3], c_[6]};
  }
  std::array<std::string, 7> rotateRight() {
    return std::array<std::string, 7>{c_[0], c_[3], c_[2], c_[6], c_[1], c_[5], c_[4]};
  }
};

const WordLockState WordLockState::answer_({"的", "蠢", "封", "连", "的", "可", "维"});

FORWARD_HASH(WordLockState);

int main() {
  WordLockState initState({"可", "封", "的", "蠢", "维", "的", "连"});
  // right -> right -> right -> middle -> middle -> right -> right
  printMoves(bfs<WordLockState, std::string>(initState));
  return 0;
}