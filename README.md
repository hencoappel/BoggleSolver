# Boggle Solver

For those unaware, Boggle is a word game where a grid of lettered dice is used and players attempt to find the most words by connecting adjacent (including diagonal) letters, letters may not be used more than once for a word. So the aim of the game is to be quick, so why not try and implement a very fast Boggle solver. 

### How do I run this thing?
It's pretty straight forward, run `make` to compile and then `boggle board_file dict_file` to run and the program will print out all the available words. To get you started there are two small boards and a large board along with a typical Unix dictionary and a more normal dictionary included. Example of what to run:
```
boggle smallboard.txt dict.txt
```

## How does it work?
So the first issue to overcome is the issue of searching through a list of words, [*tries*](https://en.wikipedia.org/wiki/Trie) (prefix trees) to the rescue! The list of words is converted to a basic trie, this allows for effecient lookups of words. The program uses recursion for each location on a grid to check what words can be produced starting at that location. It also recurses on the trie at the same time so that the actual lookup is constant time.

The program reduces the size of the built trie by creating building an adjacency matrix for the characters in the grid, then words which have letters adjacent that aren't in the matrix get ignored.

Keeping track of visited location is done using a bitset. This is used as the copying of the visited data was the limiting factor as the grid grew large.
