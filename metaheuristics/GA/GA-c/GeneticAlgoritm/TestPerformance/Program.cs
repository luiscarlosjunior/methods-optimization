using System;
using System.Collections.Generic;
using System.Diagnostics;

namespace TestPerformance
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine(4/2);
            Console.ReadLine();
        }

        public void TestListArray()
        {
            List<int> list = new List<int>(600000);
            Random rand = new Random(12345);

            for (int i = 0; i < 600000; i++)
            {
                list.Add(rand.Next(5000));
            }

            int[] arr = list.ToArray();

            int chk = 0;

            Stopwatch watch = Stopwatch.StartNew();
            for (int rpt = 0; rpt < 100; rpt++)
            {
                int len = list.Count;
                for (int i = 0; i < len; i++)
                {
                    chk += list[i];
                }
            }
            watch.Stop();
            Console.WriteLine("List/for: {0}ms ({1})", watch.ElapsedMilliseconds, chk);

            chk = 0;
            watch = Stopwatch.StartNew();
            for (int rpt = 0; rpt < 100; rpt++)
            {
                for (int i = 0; i < arr.Length; i++)
                {
                    chk += arr[i];
                }
            }
            watch.Stop();
            Console.WriteLine("Array/for: {0}ms ({1})", watch.ElapsedMilliseconds, chk);

            chk = 0;
            watch = Stopwatch.StartNew();
            for (int rpt = 0; rpt < 100; rpt++)
            {
                foreach (int i in list)
                {
                    chk += i;
                }
            }
            watch.Stop();
            Console.WriteLine("List/foreach: {0}ms ({1})", watch.ElapsedMilliseconds, chk);

            chk = 0;
            watch = Stopwatch.StartNew();
            for (int rpt = 0; rpt < 100; rpt++)
            {
                foreach (int i in arr)
                {
                    chk += i;
                }
            }
            watch.Stop();
            Console.WriteLine("Array/foreach: {0}ms ({1})", watch.ElapsedMilliseconds, chk);

            Console.ReadLine();


        }
    }
}
