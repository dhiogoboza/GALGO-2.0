//=================================================================================================
//                    Copyright (C) 2017 Olivier Mallet - All Rights Reserved                      
//=================================================================================================

#include "Galgo.hpp"

// objective class example
template <typename T>
class MyObjective
{
public:
   // objective function example : Rosenbrock function
   // minimizing f(x,y) = (1 - x)^2 + 100 * (y - x^2)^2
   static std::vector<T> Objective(const std::vector<T>& x)
   {
      //T obj = -(pow(1-x[0],2)+100*pow(x[1]-x[0]*x[0],2));
      T obj = -(pow(x[0], 2) - (2 * x[0] * x[1]) + (6 * x[0]) + pow(x[1], 2) - (6 * x[1]));
      return {obj};
   }
   // NB: GALGO maximize by default so we will maximize -f(x,y)
};

// constraints example:
// 1) x * y + x - y + 1.5 <= 0
// 2) 10 - x * y <= 0
template <typename T>
std::vector<T> MyConstraint(const std::vector<T>& x)
{
   //return {x[0]*x[1]+x[0]-x[1]+1.5,10-x[0]*x[1]};
   return {pow(x[0], 2) - (2 * x[0] * x[1]) + (6 * x[0]) + pow(x[1], 2) - (6 * x[1]) };
}
// NB: a penalty will be applied if one of the constraints is > 0 
// using the default adaptation to constraint(s) method

int get_argument(int index, int argc, char const *argv[], int default_value) {
   int value;
   if (argc >= index) {
      std::istringstream iss(argv[index - 1]);
      
      if  (! (iss >> value)) {
         value = default_value;
      }
   } else {
      value = default_value;
   }

   return value;
}

int main(int argc, char const *argv[])
{
   // initializing parameters lower and upper bounds
   // an initial value can be added inside the initializer list after the upper bound
   galgo::Parameter<double> par1({-1.0,1.0});
   galgo::Parameter<double> par2({-1.0,1.0});
   // here both parameter will be encoded using 16 bits the default value inside the template declaration
   // this value can be modified but has to remain between 1 and 64

   // initiliazing genetic algorithm
   //GeneticAlgorithm(Func<T> objective, int popsize, int nbgen, bool output, const Parameter<T,N>&...args);

   int population = 100; //get_argument(2, argc, argv, 100);
   int generations = 50; //get_argument(3, argc, argv, 50);

   std::cout << "population: " << population << std::endl;
   std::cout << "generations: " << generations << std::endl;

   galgo::GeneticAlgorithm<double> ga(MyObjective<double>::Objective, population, generations, true, par1, par2);

   // setting constraints
   ga.Constraint = MyConstraint;

   ga.tolerance = 0.001;
   ga.genstep = 1;

   ga.elitpop = 1;
   ga.covrate = .50;
   ga.mutrate = .05;

   // running genetic algorithm
   ga.run();
}
