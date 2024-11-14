# Usage
Deriving `BaseState` in [`general_solver.h`](general_solver.h) with custom `nextStates`, `isFinal`, `hash`, and `operator==`, then calling `bfs` should suffice.
See any specific solver for an example.

[CRTP](https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern) is used to avoid hacking hashing and equality checks of pointers.

# Solvers for some games
* [`beetle_lock.cpp`](examples/beetle_lock.cpp): solver for the final beetle lock puzzle in [Morphopolis](https://store.steampowered.com/app/314020/Morphopolis/).

  The goal is to match the colors of all three body parts of each beetle,
  by clockwisely rotating the outer/middle/inner plates of the lock, or swapping the beetle at the top inside the lock with the beetle outside the lock.
  The intial state of the puzzle is shown below.

  (solution: middle → middle → middle → swap → outer → outer → middle → swap → middle)

  ![beetle lock](examples/beetle_lock.jpg)

* [`word_game_lock.cpp`](examples/word_game_lock.cpp): solver for the three-lock puzzle at the treasury vault in [Word Game](https://store.steampowered.com/app/1109570/Word_Game/).

  The goal is to present an intelligible paragraph by clockwisely rotating the three locks. 
  The intial state of the puzzle is shown below. The lock at the left is trivial and has been set to the correct state.

  (solution: right → right → right → middle → middle → right → right)

  ![word game lock](examples/word_game_lock.jpg)


* [`rusty_lake_hotel_flasks.cpp`](examples/rusty_lake_hotel_flasks.cpp):  solver for the three-flask puzzle at the deer's room in [Rusty Lake Hotel](https://store.steampowered.com/app/435120/Rusty_Lake_Hotel/).

  The goal is to make the first flask (flask 0) hold water of an exact amount of 8 by pouring the water from one flask to another. 
  The intial state of the puzzle is shown below.

  (solution: (2>0) → (2>1) → (0>2) → (2>1) → (1>0) → (2>1) → (0>2) → (2>1) → (1>0))

  ![rusty lake hotel flasks](examples/rusty_lake_hotel_flasks.jpg)
