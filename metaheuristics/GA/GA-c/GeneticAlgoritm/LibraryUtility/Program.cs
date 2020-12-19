using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;

namespace LibraryUtility
{
    internal class Program
    {

        public static void Main(string[] args)
        {
            ReadFile();

            Console.ReadLine();
        }

        public static void ReadFile()
        {
            string nomeFile = "";

            // Read the file as one string.
            //string text = System.IO.File.ReadAllText(@"C:\Users\luisc\Documents\GitHub\Doutorado\Instances\LA\la01");

            //System.Console.WriteLine("Contents of WriteText.txt = {0}", text);

            Stopwatch watch = Stopwatch.StartNew();

            string[] lines = System.IO.File.ReadAllLines(@"C:\Users\luisc\Documents\GitHub\Doutorado\Instances\LA\la37");

            //Get the number JOB and MACHINE
            string[] jm = lines[4].Split(' ');

            int JOB = Convert.ToInt32(jm[0].ToString());
            int MACHINE = Convert.ToInt32(jm[1].ToString());
            int T = 0;
            int idx = 5;
            List<string> vs = new List<string>();
            string[] linhas = new string[JOB];
            string[] lin = new string[JOB];
            int[,] jobs = new int[JOB, MACHINE];

            List<string> l = new List<string>();
            List<int> Solution = new List<int>();
            List<int> Times = new List<int>();

            for (int i = idx; i < JOB+idx; i++)
            {
                l.Add(lines[i]);
            }

            // Get Machine in jobs
            for (int i = 0; i < JOB; i++)
            {
                linhas = l[i].Split(' ');

                for (int b = 0; b < linhas.Length; ++b)
                {
                    if (linhas[b].Trim().ToString().Equals(""))
                        linhas[b] = "x";
                    
                }

                for (int j = 0; j < MACHINE*2; ++j)
                {
                    if (j % 2 == 0)
                    {
                        if (linhas[j].ToString().Equals("x"))
                        {
                            if (j != 0) --j;                            
                            continue;
                        }

                        Solution.Add(Convert.ToInt32(linhas[j]));
                        continue;
                    }                        
                    Times.Add(Convert.ToInt32(linhas[j]));
                }
            }
            int k = 0;
            for (int i = 0; i < Solution.Count; i++)
            {
                if (i % 5 == 0) ++k;

                Console.WriteLine("O job {0} na maquina {1} possui tempo total de {2} ", k,  Solution[i], Times[i]);
            }

            watch.Stop();
            Console.WriteLine("Time: {0}ms", watch.ElapsedMilliseconds);
            
            //Console.WriteLine(JOB[1].ToString());

            // Display the file contents by using a foreach loop.
            //foreach (string line in lines)
            //{
            //    Console.WriteLine("\t" + line);
            //}

            // Keep the console window open in debug mode.
            Console.WriteLine("Press any key to exit.");
            System.Console.ReadKey();
                
        }
    }
}