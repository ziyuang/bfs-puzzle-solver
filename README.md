# Usage
Deriving `BaseState` in [`general_solver.h`](general_solver.h) with custom `nextStates`, `isFinal`, `hash`, and `operator==`, then calling `bfs` should suffice. Remember to `FORWARD_HASH(<your_class>)` to partial-specialize `std::hash` for your class.
See any specific solver for an example.

[CRTP](https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern) is used to avoid hacking hashing and equality checks of pointers.

# Solvers for some games
* [`beetle_lock.cpp`](beetle_lock.cpp): solver for the final beetle lock puzzle in [Morphopolis](https://store.steampowered.com/app/314020/Morphopolis/).

  The goal is to match the colors of all three body parts of each beetle,
  by clockwisely rotating the outer/middle/inner plates of the lock, or swapping the beetle at the top inside the lock with the beetle outside the lock.
  The intial state of the puzzle is shown below.

  (solution: middle → middle → middle → swap → outer → outer → middle → swap → middle)

  ![beetle lock](beetle_lock.jpg)

* [`word_game_lock.cpp`](word_game_lock.cpp): solver for the three-lock puzzle at the treasury vault in [Word Game](https://store.steampowered.com/app/1109570/Word_Game/).

  The goal is to present an intelligible paragraph by clockwisely rotating the three locks. 
  The intial state of the puzzle is shown below. The lock at the left is trivial and has been set to the correct state.

  (solution: right → right → right → middle → middle → right → right)

  ![word game lock](word_game_lock.jpg)