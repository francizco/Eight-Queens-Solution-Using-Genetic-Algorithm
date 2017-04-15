#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
#include <functional>
#include <stdlib.h>
#include <time.h>
#include <random>
using namespace std;

struct Board;
int getFitness(const Board &b);
int attackingQ(const Board &b, int k);
void mutation(int* &c);
void addSort(Board b);
bool boardSortAsc(const Board &a, const Board &b);
bool boardSortDesc(const Board &a, const Board &b);
void updateGlobal();
void crossover();

vector<Board> pop;
const float mutProb = 0.1;
int GLOBAL_FITNESS = 100;
bool update = false;

// Chromosome encoding
// An array of size 8 where each index
// represents a different column on the board
// and each number at each index represents
// the row number.
struct Board
{
	int* C;
	int fitness;
	Board()
	{
		int* A = new int[8];
		for (int j = 0; j < 8; j++)
		{
			//srand(time(NULL));
			A[j] = rand() % 8;
		}
		C = A;
		fitness = 0;
	}

	Board(int* A)
	{
		C = A;
		fitness = 0;
	}

	// Crossover incorporated into constructor that creates a new child board
	// from two parent boards.
	Board(const Board &A, const Board &B)
	{

		// Perform crossover using half of first parent
		// and half of second parent
		int* M = new int[8];

		int rangeStart = rand() % 6;
		int rangeEnd = rand() % 7;
		for(int i = 0; i < rangeStart; i++)
		{
			M[i] = B.C[i];
		}

		for(int j = rangeStart; j < rangeEnd; j++)
		{ 
			M[j] = A.C[j];
		}

		for(int k = rangeEnd; k < 8; k++)
		{
			M[k] = B.C[k];
		}

		C = M;
		fitness = 0;
		mutation(C);
	}

	Board(const Board &b)
	{
		C = new int[8];
		for (int i = 0; i < 8; i++)
		{
			C[i] = b.C[i];
		}

		fitness = b.fitness;
	}

	Board& operator=(const Board& b)
	{
		if (this != &b)
		{
			delete[] C;
			C = new int[8];
			for (int i = 0; i < 8; i++)
			{
				C[i] = b.C[i];
			}

			fitness = b.fitness;
		}
		return *this;
	}

	~Board()
	{
		delete[] C;
	}

	bool operator < (const Board& b) const
	{
		return (fitness < b.fitness);
	}

	bool operator > (const Board& b) const
	{
		return (fitness > b.fitness);
	}
};

// Fitness function
// Determine number of attacking queens
// Lowest number of attacking queens is better
int getFitness(const Board &b)
{
	// Return the number of attacking queens 
	int count = 0;
	for(int i = 0; i < 8; i++)
	{
		count = count + attackingQ(b, i);
	}

	return count;
}

// Determine number of attacking queens
int attackingQ(const Board &b, int k)
{
	int count = 0;
	// Check rows
	for (int i = k + 1; i < 8; i++)
	{
		if (b.C[k] == b.C[i]) { count = count + 1; }
	}
	// Check diagonals
	int up = b.C[k];
	int down = b.C[k];
	for (int i = k + 1; i < 8; i++)
	{
		// check upper diagonal
		up = up - 1;
		if (b.C[i] == up) { count = count + 1; }
		// check lower diagonal
		down = down + 1;
		if (b.C[i] == down) { count = count + 1; }
	}
	return count;
}

// Randomly decide when and what part of a
// chromosome to mutate. Basically,
// this function will be biased towards
// no mutation and when it does mutate 
// a chromosome, it's only changing the
// row placement of a queen.
void mutation(int* &c)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::bernoulli_distribution distribution(mutProb);

	if(distribution(gen))
	{
		//srand(time(NULL));
		int index = rand() % 8;
		//srand(time(NULL));
		int row = rand() % 8;
		c[index] = row;
	}
}

// Add a Board to the population
// and then sort the population by
// fitness. Kill off boards with
// highest fitness.
void addSort(Board b)
{
	int d = getFitness(b);
	b.fitness = d;
	pop.push_back(b);
	std::sort(pop.begin(), pop.end(), boardSortDesc);
	updateGlobal();

	// Kill some off
	sort(pop.begin(), pop.end(),boardSortAsc);
	while (pop.size() > 10) 
	{ 
		pop.pop_back();
	}

	sort(pop.begin(), pop.end(), boardSortDesc);
}

// Function used to sort population in ascending order
bool boardSortAsc(const Board &a, const Board &b)
{
	return a.fitness < b.fitness;
}

// Function used to sort population in descending order
bool boardSortDesc(const Board &a, const Board &b)
{
	return a.fitness > b.fitness;
}

// Updates the GLOBAL_FITNESS
void updateGlobal()
{
	int minFitness = pop.back().fitness;
	if (minFitness < GLOBAL_FITNESS)
	{ 
		GLOBAL_FITNESS = minFitness;
		update = true;

	}
}

// Produces children from the top half of the population
// Kills off worst fit
void crossover()
{
	vector<Board> removedParents;
	vector<Board> children;
	for (int i = 0; i < 2; i++)
	{
		// Get parent boards
		Board P1 = pop.back();
		pop.pop_back();
		Board P2 = pop.back();
		pop.pop_back();
		removedParents.push_back(P1);
		removedParents.push_back(P2);

		// Produce children
		Board c1(P1, P2);
		Board c2(P2,P1);
		children.push_back(c1);
		children.push_back(c2);
	}

	// Add children and parents to population
	while(removedParents.size() > 0)
	{ 
		addSort(removedParents.back());
		removedParents.pop_back();
	}
	while(children.size() > 0)
	{
		addSort(children.back());
		children.pop_back();
	}

}

// Print the chromosome -> array of Queens
void printBoard(Board b)
{
	cout << "[ ";
	for (int i = 0; i < 8; i++)
	{
		if(i == 7) { cout << b.C[i]; }
		else { cout << b.C[i] << ", "; }
	}
	cout << " ]" << endl;
}


// Clear the population and reset GLOBAL_FITNESS for
// a new experiment
void resetProgram()
{
	while (pop.size() > 0)
	{
		pop.pop_back();
		GLOBAL_FITNESS = 100;
	}
}

// Returns the average fitness of a population
int getAvgFitness()
{
	int avg = 0;
	for(int i = 0; i < pop.size(); i++)
	{
		avg = avg + getFitness(pop[i]);
	}

	return avg / pop.size();
}

int main()
{
	srand(time(NULL));
	const int experiments = 5;
	int totalIterations = 0;
	int avgFitness = 0;
	int avgAvgFitness = 0;

	for (int e = 0; e < experiments; e++)
	{
			// Randomly generate a population of boards
		for (int i = 0; i < 10; i++)
		{
			Board b;
			addSort(b);
		}

		// run algorithm 1000 times or until there's a soln
		int count = 0;
		while (count < 1000 && GLOBAL_FITNESS != 0)
		{
			crossover();
			count = count + 1;
			avgFitness = getAvgFitness();
			avgAvgFitness = avgAvgFitness + avgFitness;
			if (update)
			{
				cout << "New global fitness: " << GLOBAL_FITNESS << endl;
				cout << "Count: " << count - 1 << endl << endl;
				cout << "Average fitness: " << avgFitness << endl;
				if(count > 0) { cout << "Average of Average fitness: " << avgAvgFitness / count << endl << endl; }
				update = false;
			}
		}

		totalIterations = totalIterations + count;
		Board Z =  pop.back();
		cout << "Experiment #" << e << endl;
		printBoard(Z);
		cout << "Iterations:" << count << endl;
		cout << "Global fitness: " << GLOBAL_FITNESS << endl;
		cout << endl;

		resetProgram();
		avgFitness = 0;
		avgAvgFitness = 0;
	}

	cout << "Average iterations: " << totalIterations / experiments << endl;

	system("PAUSE");

}