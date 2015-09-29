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
//  $Id: Genome.cs,v 1.1 2003/08/19 20:59:05 barry Exp $


using System;
using System.Collections;
using btl.generic;

namespace btl.generic
{
	/// <summary>
	/// Summary description for Genome.
	/// </summary>
	public class Genome
	{
		public Genome()
		{
			//
			// TODO: Add constructor logic here
			//
		}
		public Genome(int length)
		{
			m_length = length;
			m_genes = new double[ length ];
			CreateGenes();
		}
		public Genome(int length, bool createGenes)
		{
			m_length = length;
			m_genes = new double[ length ];
			if (createGenes)
				CreateGenes();
		}

		public Genome(ref double[] genes)
		{
			m_length = genes.GetLength(0);
			m_genes = new double[ m_length ];
			for (int i = 0 ; i < m_length ; i++)
				m_genes[i] = genes[i];
		}
		 

		private void CreateGenes()
		{
			// DateTime d = DateTime.UtcNow;
			for (int i = 0 ; i < m_length ; i++)
				m_genes[i] = m_random.NextDouble();
		}

		public void Crossover(ref Genome genome2, out Genome child1, out Genome child2)
		{
			int pos = (int)(m_random.NextDouble() * (double)m_length);
			child1 = new Genome(m_length, false);
			child2 = new Genome(m_length, false);
			for(int i = 0 ; i < m_length ; i++)
			{
				if (i < pos)
				{
					child1.m_genes[i] = m_genes[i];
					child2.m_genes[i] = genome2.m_genes[i];
				}
				else
				{
					child1.m_genes[i] = genome2.m_genes[i];
					child2.m_genes[i] = m_genes[i];
				}
			}
		}


		public void Mutate()
		{
			for (int pos = 0 ; pos < m_length; pos++)
			{
				if (m_random.NextDouble() < m_mutationRate)
					m_genes[pos] = (m_genes[pos] + m_random.NextDouble())/2.0;
			}
		}

		public double[] Genes()
		{
			return m_genes;
		}

		public void Output()
		{
			for (int i = 0 ; i < m_length ; i++)
			{
				System.Console.WriteLine("{0:F4}", m_genes[i]);
			}
			System.Console.Write("\n");
		}

		public void GetValues(ref double[] values)
		{
			for (int i = 0 ; i < m_length ; i++)
				values[i] = m_genes[i];
		}


		public double[] m_genes;
		private int m_length;
		private double m_fitness;
		static Random m_random = new Random();

		private static double m_mutationRate;

		public double Fitness
		{
			get
			{
				return m_fitness;
			}
			set
			{
				m_fitness = value;
			}
		}




		public static double MutationRate
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

		public int Length
		{
			get
			{
				return m_length;
			}
		}
	}
}
