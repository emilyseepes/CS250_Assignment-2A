# Simulation Game #
### Prompt: ###
> This simulation involves 5 foxes and 15 rabbits that roam around a grid. If a fox is next to a rabbit, the rabbit is captured and removed from the grid. In the visualization of the grid, the foxes are marked with an 'x' while rabbits are marked with an 'o'.
> + **_The Grid_** file deals with code related to printing, updating, and querying characteristics related to grid position.
> + **_The Foxes_** file mainly deals with the foxes' movements
> + **_The Rabbits_** file mainly deals with the rabbits' movements
> + **_The Utils_** file contains functions utilized by both the rabbit and foxes 

### Game Modes ###
+ **Positions from File**:
  + Coordinates will be loaded from the files rabbits.txt and foxes.txt
+ **Random Positions**:
  + Coordinates of the foxes and rabbits will be randomly generated.
+ **Stationary Rabbits**:
  + Rabbits will not roam around the grid, but will instead remain in their initial position.
+ **Moving Rabbits**:
  + The rabbits will move randomly.
+ **Random Fox Movement**:
  + The foxes will move randomly.
+ **Chase Fox Movement**:
  + The foxes move based on a simple chase algorithm, based on which the fox moves towards the direction of the closest rabbit.
