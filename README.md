# Usage
Deriving `BaseState` in [`general_solver.h`](general_solver.h) with custom `nextStates`, `isFinal`, `hash`, and `operator==`, then calling `bfs` should suffice. Remember to `FORWARD_HASH(<your_class>)` to partial-specialize `std::hash` for your class.
See [`beetle_lock.cpp`](beetle_lock.cpp) for an example.

See also [CRTP](https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern) for the inheritance idiom.

# Solvers for my games
* [`beetle_lock.cpp`](beetle_lock.cpp): solver for the final "beetle lock" puzzle in [Morphopolis](https://store.steampowered.com/app/314020/Morphopolis/).
![beetle lock](beetle_lock.jpg)

