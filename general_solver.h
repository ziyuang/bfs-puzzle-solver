#pragma once

#include <functional>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

template <typename State, typename Move> class BaseState {
protected:
  using State_Move = std::pair<State, Move>;

public:
  using MoveType = Move; // for both convenience and SFINAE
  std::vector<State_Move> nextStates() { return static_cast<State *>(this)->nextStates(); }
  size_t hash() const { return static_cast<State *>(this)->hash(); }
  bool isFinal() const { return static_cast<State *>(this)->isFinal(); }
};

namespace {

template <typename State, typename Move>
std::vector<Move> extractPath(const std::pair<State, Move> &finalState,
                              const std::unordered_map<std::pair<State, Move>, std::pair<State, Move>> &parents) {
  std::vector<Move> path = {finalState.second};
  std::pair<State, Move> state(finalState);
  while (parents.find(state) != parents.end()) {
    state = parents.at(state);
    path.push_back(state.second);
  }
  return std::vector<Move>(path.rbegin() + 1, path.rend());
}

template <typename F, typename S, typename = void> struct QueueImpl { using type = std::queue<std::pair<F, S>>; };

template <typename F, typename S> struct QueueImpl<F, S, std::void_t<decltype(std::declval<F>() < std::declval<F>())>> {
  using type = std::priority_queue<std::pair<F, S>>;
};

template <typename F, typename S> using Queue = typename QueueImpl<F, S>::type;

// https://stackoverflow.com/a/61144425/688080
template <typename F, typename... Others> using First = F;

template <int N = sizeof(size_t)> size_t makeMask(size_t currentMask = 0x55) {
  return makeMask<N - 1>((currentMask << 8) | (currentMask & 0xff));
}
template <> size_t makeMask<0>(size_t currentMask) { return currentMask; }

} // namespace

template <typename F, typename S> struct std::hash<std::pair<F, S>> {
  size_t operator()(const std::pair<F, S> &pair) const {
    size_t mask = makeMask<>();
    return (std::hash<F>{}(pair.first) & mask) ^ (std::hash<S>{}(pair.second) & ~mask);
  }
};

template <typename State> struct std::hash<First<State, typename State::MoveType>> {
  size_t operator()(const State &s) const { return s.hash(); }
};

template <typename State, typename = typename State::MoveType>
std::vector<typename State::MoveType> bfs(const State &initState) {
  using Move = typename State::MoveType;
  using State_Move = std::pair<State, Move>;
  std::unordered_map<State_Move, State_Move> parents;
  std::unordered_set<State> visited;
  Queue<State, Move> q;
  q.push(State_Move({initState, Move{}}));
  while (q.size() > 0) {
    State_Move state_move = q.front();
    q.pop();
    auto &[state, move] = state_move;
    if (state.isFinal())
      return extractPath(state_move, parents);
    if (visited.find(state) == visited.end()) {
      visited.insert(state);
      for (State_Move &nextState_nextMove : state.nextStates()) {
        if (visited.find(nextState_nextMove.first) == visited.end()) {
          parents.insert({nextState_nextMove, state_move});
          q.push(nextState_nextMove);
        }
      }
    }
  }
  return std::vector<Move>{};
}

template <typename Move> void printMoves(const std::vector<Move> &moves, std::string sep = " -> ") {
  bool printSep = false;
  for (auto &move : moves)
    std::cout << (printSep ? sep : "") << (printSep = true, move);
  std::cout << std::endl;
}