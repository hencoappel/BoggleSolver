#
# Makefile for the Boggle solver.
#
CC=g++

boggle: boggle_solver.cxx trie.cxx
	     $(CC) -O3 -o boggle boggle_solver.cxx trie.cxx -I.

