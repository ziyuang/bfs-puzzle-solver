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
  using StateWithMove = std::pair<State, Move>;

public:
  std::vector<StateWithMove> nextStates() { return static_cast<State *>(this)->nextStates(); }
  bool isFinal() const { return static_cast<State *>(this)->isFinal(); }
};

namespace {

template <typename State, typename Move, typename = void> struct QueueImpl {
  using type = std::queue<std::pair<State, Move>>;
};

template <typename State, typename Move>
struct QueueImpl<State, Move, decltype(std::less<State>(std::declval<State>(), std::declval<State>()))> {
  using type = std::priority_queue<std::pair<State, Move>, std::vector<std::pair<State, Move>>, std::less<State>>;
};

template <typename State, typename Move> using Queue = typename QueueImpl<State, Move>::type;

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
  using StateWithMove = std::pair<State, Move>;
  std::unordered_map<StateWithMove, StateWithMove> parents;
  std::unordered_set<State> visited;
  Queue<State, Move> q;
  q.push(StateWithMove({initState, Move{}}));
  while (q.size() > 0) {
    StateWithMove stateWithMove = q.front();
    q.pop();
    auto &[state, move] = stateWithMove;
    if (state.isFinal())
      return extractPath(stateWithMove, parents);
    if (visited.find(state) == visited.end()) {
      visited.insert(state);
      for (StateWithMove &nextStateWithMove : state.nextStates()) {
        if (visited.find(nextStateWithMove.first) == visited.end()) {
          parents.insert({nextStateWithMove, stateWithMove});
          q.push(nextStateWithMove);
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