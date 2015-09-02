#
# Makefile for the Boggle solver.
#
CC=g++
FLAGS=-std=c++11 -Wall -Wextra -pedantic -O3 

boggle: boggle_solver.cxx trie.cxx
	     $(CC) -o boggle boggle_solver.cxx trie.cxx -I $(FLAGS)

