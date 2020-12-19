using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LibraryUtility
{
    class GetInstance
    {
        public static void ReadFile()
        {
            string nomeFile = "";

            // Read the file as one string.
            //string text = System.IO.File.ReadAllText(@"C:\Users\luisc\Documents\GitHub\Doutorado\Instances\LA\la01");

            //System.Console.WriteLine("Contents of WriteText.txt = {0}", text);

            Stopwatch watch = Stopwatch.StartNew();

            string[] lines = System.IO.File.ReadAllLines(@"C:\Users\luisc\Documents\GitHub\Doutorado\Instances\LA\la01");

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
            List<int> ls = new List<int>();

            for (int i = idx; i < JOB + idx; i++)
            {
                l.Add(lines[i]);
            }

            for (int i = 0; i < JOB; i++)
            {
                linhas = l[i].Split(' ');
                for (int j = 0; j < MACHINE * 2; j += 2)
                {
                    ls.Add(Convert.ToInt32(linhas[j]));
                }
            }

            watch.Stop();
            Console.WriteLine("Time: {0}ms", watch.ElapsedMilliseconds);

            foreach (var item in l)
            {

            }

            for (int i = 0; i < JOB * MACHINE; ++i)
            {

            }

            Console.WriteLine("Press any key to exit.");
            System.Console.ReadKey();

        }
    }
}
