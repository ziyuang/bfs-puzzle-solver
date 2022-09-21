#include <string>
#include <utility>
#include <vector>

#include "general_solver.h"

enum class Color : size_t { Red, Yellow, Green, Blue, Purple };

class Beetle {
public:
  Beetle(Color head, Color thorax, Color abdomen) : colors_{head, thorax, abdomen} {}
  Beetle(const std::vector<Color> &colors) : colors_(colors) {}
  Beetle(const Beetle &other) : colors_(other.colors_) {}

  const std::vector<Color> &colors() const { return colors_; }

  bool matched() const { return colors_[0] == colors_[1] && colors_[1] == colors_[2]; }

  size_t hash() const {
    size_t base = 5;
    size_t h = static_cast<size_t>(colors_[0]);
    for (size_t i = 1; i < colors_.size(); i++)
      h = base * h + static_cast<size_t>(colors_[i]);
    return h;
  }

  bool operator==(const Beetle &other) const {
    return colors_[0] == other.colors_[0] && colors_[1] == other.colors_[1] && colors_[2] == other.colors_[2];
  }

private:
  // head, thorax, abdomen
  std::vector<Color> colors_;
};

class BeetleLockState : public BaseState<BeetleLockState, std::string> {
public:
  BeetleLockState(const Beetle &north, const Beetle &east, const Beetle &south, const Beetle &west,
                  const Beetle &outOfWheel)
      : inWheel_{north, east, south, west}, outOfWheel_(outOfWheel) {}
  BeetleLockState(const std::vector<Beetle> &inWheel, const Beetle &outOfWheel)
      : inWheel_{inWheel}, outOfWheel_(outOfWheel) {}
  BeetleLockState(const BeetleLockState &other) : inWheel_(other.inWheel_), outOfWheel_(other.outOfWheel_) {}

  bool isFinal() const {
    bool matched = true;
    for (auto &beetle : inWheel_) {
      matched &= beetle.matched();
      if (!matched)
        return false;
    }
    return matched && outOfWheel_.matched();
  }

  size_t hash() const {
    size_t h = 0;
    const size_t numBitsOf125 = 7;
    for (size_t i = 0; i < inWheel_.size(); i++) {
      const Beetle &beetle = inWheel_[i];
      h |= beetle.hash() << ((i + 1) * numBitsOf125);
    }
    return h | outOfWheel_.hash();
  }

  bool operator==(const BeetleLockState &other) const {
    return inWheel_[0] == other.inWheel_[0] && inWheel_[1] == other.inWheel_[1] && inWheel_[2] == other.inWheel_[2] &&
           inWheel_[3] == other.inWheel_[3] && outOfWheel_ == other.outOfWheel_;
  }

  std::vector<StateWithMove> nextStates() {
    std::vector<StateWithMove> states{
        StateWithMove(BeetleLockState(outOfWheel_, inWheel_[1], inWheel_[2], inWheel_[3], inWheel_[0]), "swap")};
    size_t wheelSize = inWheel_.size();
    size_t numBodyParts = outOfWheel_.colors().size();
    std::vector<std::string> rotateOps{"outer", "mid", "inner"};
    for (size_t i = 0; i < numBodyParts; i++) {
      std::vector<Beetle> newInWheelBeetles;
      newInWheelBeetles.reserve(wheelSize);
      for (size_t j = 0; j < wheelSize; j++) {
        std::vector<Color> colors;
        colors.reserve(numBodyParts);
        for (size_t k = 0; k < numBodyParts; k++) {
          Color color;
          if (k == i) {
            color = inWheel_[(wheelSize + j - 1) % wheelSize].colors()[k];
          } else {
            color = inWheel_[j].colors()[k];
          }
          colors.push_back(color);
        }
        newInWheelBeetles.push_back(Beetle(colors));
      }
      states.push_back(StateWithMove({BeetleLockState(newInWheelBeetles, outOfWheel_), rotateOps[i]}));
    }
    return states;
  }

private:
  // north, east, south, west
  // could have been std::array but we don't have default constructor for Beetle
  std::vector<Beetle> inWheel_;
  Beetle outOfWheel_;
};

FORWARD_HASH(BeetleLockState);

int main() {
  BeetleLockState initState(
      Beetle(Color::Red, Color::Green, Color::Red), Beetle(Color::Purple, Color::Yellow, Color::Yellow),
      Beetle(Color::Green, Color::Purple, Color::Blue), Beetle(Color::Yellow, Color::Red, Color::Purple),
      Beetle(Color::Blue, Color::Blue, Color::Green));
  std::vector<std::string> moves = bfs<BeetleLockState, std::string>(initState);
  printMoves(moves); // mid -> mid -> mid -> swap -> outer -> outer -> mid -> swap -> mid
  return 0;
}
