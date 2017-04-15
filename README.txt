This is my implementation of a soln to the eight queens problem using a genetic algorithm. 

Chromosome encoding
Each board was represented as objects of a custom Board class. The only data members of this class were an array and an integer to hold the fitness of the board. The array itself held the chromosome encoding of the entire board and had columns and rows represented as index and array values respectively. Because of this approach, there could be only one queen per column.

Mutation
Mutations were handled using a global constant value of 10%. So, every time there was a crossover between two parent boards, there was a 10% chance that an index and value (column and row) would be randomly changed. Here, I used a Bernoulli function from the standard library for the mutation probability to decide to perform a mutation or not.

Crossover
I originally wrote the crossover function to be separate for the sake of functional decomposition, but I ended up making the actual crossover functionality one of the Board class’ constructors. There is still a separate crossover function but its only responsible for facilitating the breeding of the parent boards and managing the population. I use a vector to hold the population and it's sorted in descending order with smallest fitness at the back. During a crossover, the fittest half of boards are bred and then held in separate populations (alone with their children) until the entire crossover process is complete. This way, I’m not breeding the same set over parent boards more than once. Before returning to the calling function, both parents and children are added back to the original population. Boards with highest fitness are removed and destroyed.

Fitness function
My fitness function is very straight forward. It uses another function to help it determine the number of attacking queens on a board and that number is used to represent the fitness. To determine if a queen is attacking or not, I count the number of collisions in the same row as a queen and the upper right and lower left diagonal. Then, it only need to check to the right of each queen because everything to the left was already checked when I checked the previous queen. I also don’t check the same column because of the design decision that I made to put only a single queen in a column. A fitness of 8 means there are 8 queens on a board that can attack. Obviously, lower fitness values are desired as they are closer to the problem’s solution.

Results
In the small sample of experiments I ran, it took an average of 61 iterations to converge to a result. However, some of these solutions were often to a fitness of 1 or 2. Very infrequently did my program converge to a solution where there were no attacking queens. The average value of the fitness function of chromosomes in the population was mostly in line with whatever the best fitness in the population
was at each iteration. In the beginning, the average fitness would be one or two numbers higher than the current best fitness, but it would quickly converge to the same value. The best solution was
[ 5, 1, 6, 0, 2, 4, 7, 3 ] which took 56 iterations to solve.

Built With
C++14

Author
Frank Fuentes
francizco.com

License
This project is licensed under the MIT License.