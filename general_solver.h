#pragma once

#include <functional>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#define FORWARD_HASH(T)                                                                                                \
  template <> struct std::hash<T> {                                                                                    \
    size_t operator()(const T &t) const { return t.hash(); }                                                           \
  }

template <typename State, typename Move> class BaseState {
protected:
  using State_Move = std::pair<State, Move>;

public:
  std::vector<State_Move> nextStates() { return static_cast<State *>(this)->nextStates(); }
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

} // namespace

template <typename F, typename S> struct std::hash<std::pair<F, S>> {
  size_t operator()(const std::pair<F, S> &pair) const {
    return std::hash<F>{}(pair.first) ^ std::hash<S>{}(pair.second);
  }
};

template <typename State, typename Move> std::vector<Move> bfs(const State &initState) {
  using State_Move = std::pair<State, Move>;
  std::unordered_map<State_Move, State_Move> parents;
  std::unordered_set<State> visited;
  std::queue<State_Move> q;
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