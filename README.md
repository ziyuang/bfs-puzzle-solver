# Usage
Deriving `BaseState` in [`general_solver.h`](general_solver.h) with custom `nextStates`, `isFinal`, `hash`, and `operator==`, then calling `bfs` should suffice. Remember to `FORWARD_HASH(<your_class>)` to partial-specialize `std::hash` for your class.
See any specific solver for an example.

[CRTP](https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern) is used to avoid hacking hashing and equality checks of pointers.

# Solvers for some games
* [`beetle_lock.cpp`](beetle_lock.cpp): solver for the final beetle lock puzzle in [Morphopolis](https://store.steampowered.com/app/314020/Morphopolis/).
![beetle lock](beetle_lock.jpg)

* [`word_game_lock.cpp`](word_game_lock.cpp): solver for the three-lock puzzle in [Word Game](https://store.steampowered.com/app/1109570/Word_Game/).
![word game lock](word_game_lock.jpg)