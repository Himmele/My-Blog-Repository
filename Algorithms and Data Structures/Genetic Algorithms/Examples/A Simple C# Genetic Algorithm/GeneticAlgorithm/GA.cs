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
//  $Id: GA.cs,v 1.1 2003/08/19 20:59:05 barry Exp $

using System;
using System.Collections;
using System.IO;

namespace btl.generic
{

	public delegate double GAFunction(double[] values);

	/// <summary>
	/// Genetic Algorithm class
	/// </summary>
	public class GA
	{
		/// <summary>
		/// Default constructor sets mutation rate to 5%, crossover to 80%, population to 100,
		/// and generations to 2000.
		/// </summary>
		public GA()
		{
			InitialValues();
			m_mutationRate = 0.05;
			m_crossoverRate = 0.80;
			m_populationSize = 100;
			m_generationSize = 2000;
			m_strFitness = "";
		}

		public GA(double crossoverRate, double mutationRate, int populationSize, int generationSize, int genomeSize)
		{
			InitialValues();
			m_mutationRate = mutationRate;
			m_crossoverRate = crossoverRate;
			m_populationSize = populationSize;
			m_generationSize = generationSize;
			m_genomeSize = genomeSize;
			m_strFitness = "";
		}

		public GA(int genomeSize)
		{
			InitialValues();
			m_genomeSize = genomeSize;
		}


		public void InitialValues()
		{
			m_elitism = false;
		}


		/// <summary>
		/// Method which starts the GA executing.
		/// </summary>
		public void Go()
		{
			if (getFitness == null)
				throw new ArgumentNullException("Need to supply fitness function");
			if (m_genomeSize == 0)
				throw new IndexOutOfRangeException("Genome size not set");

			//  Create the fitness table.
			m_fitnessTable = new ArrayList();
			m_thisGeneration = new ArrayList(m_generationSize);
			m_nextGeneration = new ArrayList(m_generationSize);
			Genome.MutationRate = m_mutationRate;


			CreateGenomes();
			RankPopulation();

			StreamWriter outputFitness = null;
			bool write = false;
			if (m_strFitness != "")
			{
				write = true;
				outputFitness = new StreamWriter(m_strFitness);
			}

			for (int i = 0; i < m_generationSize; i++)
			{
				CreateNextGeneration();
				RankPopulation();
				if (write)
				{
					if (outputFitness != null)
					{
						double d = (double)((Genome)m_thisGeneration[m_populationSize-1]).Fitness;
						outputFitness.WriteLine("{0},{1}",i,d);
					}
				}
			}
			if (outputFitness != null)
				outputFitness.Close();
		}

		/// <summary>
		/// After ranking all the genomes by fitness, use a 'roulette wheel' selection
		/// method.  This allocates a large probability of selection to those with the 
		/// highest fitness.
		/// </summary>
		/// <returns>Random individual biased towards highest fitness</returns>
		private int RouletteSelection()
		{
			double randomFitness = m_random.NextDouble() * m_totalFitness;
			int idx = -1;
			int mid;
			int first = 0;
			int last = m_populationSize -1;
			mid = (last - first)/2;

			//  ArrayList's BinarySearch is for exact values only
			//  so do this by hand.
			while (idx == -1 && first <= last)
			{
				if (randomFitness < (double)m_fitnessTable[mid])
				{
					last = mid;
				}
				else if (randomFitness > (double)m_fitnessTable[mid])
				{
					first = mid;
				}
				mid = (first + last)/2;
				//  lies between i and i+1
				if ((last - first) == 1)
					idx = last;
			}
			return idx;
		}

		/// <summary>
		/// Rank population and sort in order of fitness.
		/// </summary>
		private void RankPopulation()
		{
			m_totalFitness = 0;
			for (int i = 0; i < m_populationSize; i++)
			{
				Genome g = ((Genome) m_thisGeneration[i]);
				g.Fitness = FitnessFunction(g.Genes());
				m_totalFitness += g.Fitness;
			}
			m_thisGeneration.Sort(new GenomeComparer());

			//  now sorted in order of fitness.
			double fitness = 0.0;
			m_fitnessTable.Clear();
			for (int i = 0; i < m_populationSize; i++)
			{
				fitness += ((Genome)m_thisGeneration[i]).Fitness;
				m_fitnessTable.Add((double)fitness);
			}
		}

		/// <summary>
		/// Create the *initial* genomes by repeated calling the supplied fitness function
		/// </summary>
		private void CreateGenomes()
		{
			for (int i = 0; i < m_populationSize ; i++)
			{
				Genome g = new Genome(m_genomeSize);
				m_thisGeneration.Add(g);
			}
		}

		private void CreateNextGeneration()
		{
			m_nextGeneration.Clear();
			Genome g = null;
			if (m_elitism)
				g = (Genome)m_thisGeneration[m_populationSize - 1];

			for (int i = 0 ; i < m_populationSize ; i+=2)
			{
				int pidx1 = RouletteSelection();
				int pidx2 = RouletteSelection();
				Genome parent1, parent2, child1, child2;
				parent1 = ((Genome) m_thisGeneration[pidx1]);
				parent2 = ((Genome) m_thisGeneration[pidx2]);

				if (m_random.NextDouble() < m_crossoverRate)
				{
					parent1.Crossover(ref parent2, out child1, out child2);
				}
				else
				{
					child1 = parent1;
					child2 = parent2;
				}
				child1.Mutate();
				child2.Mutate();

				m_nextGeneration.Add(child1);
				m_nextGeneration.Add(child2);
			}
			if (m_elitism && g != null)
				m_nextGeneration[0] = g;

			m_thisGeneration.Clear();
			for (int i = 0 ; i < m_populationSize; i++)
				m_thisGeneration.Add(m_nextGeneration[i]);
		}

	
		private double m_mutationRate;
		private double m_crossoverRate;
		private int m_populationSize;
		private int m_generationSize;
		private int m_genomeSize;
		private double m_totalFitness;
		private string m_strFitness;
		private bool m_elitism;
		
		private ArrayList m_thisGeneration;
		private ArrayList m_nextGeneration;
		private ArrayList m_fitnessTable;
		
		static Random m_random = new Random();



		static private GAFunction getFitness;
		public GAFunction FitnessFunction
		{
			get	
			{
				return getFitness;
			}
			set
			{
				getFitness = value;
			}
		}


		//  Properties
		public int PopulationSize
		{
			get
			{
				return m_populationSize;
			}
			set
			{
				m_populationSize = value;
			}
		}

		public int Generations
		{
			get
			{
				return m_generationSize;
			}
			set
			{
				m_generationSize = value;
			}
		}

		public int GenomeSize
		{
			get
			{
				return m_genomeSize;
			}
			set
			{
				m_genomeSize = value;
			}
		}

		public double CrossoverRate
		{
			get
			{
				return m_crossoverRate;
			}
			set
			{
				m_crossoverRate = value;
			}
		}
		public double MutationRate
		{
			get
			{
				return m_mutationRate;
			}
			set
			{
				m_mutationRate = value;
			}
		}

		public string FitnessFile
		{
			get
			{
				return m_strFitness;
			}
			set
			{
				m_strFitness = value;
			}
		}

		/// <summary>
		/// Keep previous generation's fittest individual in place of worst in current
		/// </summary>
		public bool Elitism
		{
			get
			{
				return m_elitism;
			}
			set
			{
				m_elitism = value;
			}
		}

		public void GetBest(out double[] values, out double fitness)
		{
			Genome g = ((Genome)m_thisGeneration[m_populationSize-1]);
			values = new double[g.Length];
			g.GetValues(ref values);
			fitness = (double)g.Fitness;
		}

		public void GetWorst(out double[] values, out double fitness)
		{
			GetNthGenome(0, out values, out fitness);
		}

		public void GetNthGenome(int n, out double[] values, out double fitness)
		{
			if (n < 0 || n > m_populationSize-1)
				throw new ArgumentOutOfRangeException("n too large, or too small");
			Genome g = ((Genome)m_thisGeneration[n]);
			values = new double[g.Length];
			g.GetValues(ref values);
			fitness = (double)g.Fitness;
		}
	}
}
