//  All code copyright (c) 2003 Barry Lapthorn
//  Website:  http://www.lapthorn.net
//
//  Disclaimer:  
//  All code is provided on an "AS IS" basis, without warranty. The author 
//  makes no representation, or warranty, either express or implied, with 
//  respect to the code, its quality, accuracy, or fitness for a specific 
//  purpose. Therefore, the author shall not have any liability to you or any 
//  other person or entity with respect to any liability, loss, or damage 
//  caused or alleged to have been caused directly or indirectly by the code
//  provided.  This includes, but is not limited to, interruption of service, 
//  loss of data, loss of profits, or consequential damages from the use of 
//  this code.
//
//
//  $Author: barry $
//  $Revision: 1.1 $
//
//  $Id: Test.cs,v 1.1 2003/08/19 20:59:05 barry Exp $

using System;
using btl.generic;


public class Test
{
	//  optimal solution for this is (0.5,0.5)
	public static double theActualFunction(double[] values)
	{
		if (values.GetLength(0) != 2)
			throw new ArgumentOutOfRangeException("should only have 2 args");

		double x = values[0];
		double y = values[1];
		double n = 9;  //  should be an int, but I don't want to waste time casting.

		double f1 = Math.Pow(15*x*y*(1-x)*(1-y)*Math.Sin(n*Math.PI*x)*Math.Sin(n*Math.PI*y),2);
		return f1;
	}

	public static void Main()
	{
		//  Crossover		= 80%
		//  Mutation		=  5%
		//  Population size = 100
		//  Generations		= 2000
		//  Genome size		= 2
		GA ga = new GA(0.8,0.05,100,2000,2);
		
		ga.FitnessFunction = new GAFunction(theActualFunction);

		//ga.FitnessFile = @"H:\fitness.csv";
		ga.Elitism = true;
		ga.Go();

		double[] values;
		double fitness;
		ga.GetBest(out values, out fitness);
		System.Console.WriteLine("Best ({0}):", fitness);
		for (int i = 0 ; i < values.Length ; i++)
			System.Console.WriteLine("{0} ", values[i]);
		
		ga.GetWorst(out values, out fitness);
		System.Console.WriteLine("\nWorst ({0}):", fitness);
		for (int i = 0 ; i < values.Length ; i++)
			System.Console.WriteLine("{0} ", values[i]);
		
	}
}
