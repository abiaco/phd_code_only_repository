using System;
using System.Data;
using System.IO;
using Accord.MachineLearning;
using Accord.Statistics.Testing;
using Accord.Math.Distances;
using MathNet.Numerics.Statistics;
using System.Collections.Generic;
using Accord.Statistics.Visualizations;
using Accord.Controls;
using Accord.Statistics.Analysis;
using Newtonsoft.Json;
using Accord.Statistics.Kernels;
using Accord.Statistics.Distributions.Univariate;
using Accord.MachineLearning.Bayes;
using Accord.MachineLearning.VectorMachines.Learning;
using System.Linq;
using Accord.Neuro;
using Accord.Neuro.Learning;
using Accord.Math;
using Newtonsoft.Json.Linq;
using Accord.Math.Optimization.Losses;
using System.Diagnostics;

namespace AnalysisCSharp
{
    static class Program
    {

        static double[][] classifier_inputs;
        static int[] classifier_tags;

        static void shuffle_inputs(this Random rng)
        {
            int n = classifier_inputs.Length;
            while (n > 1)
            {
                int k = rng.Next(n--);
                var temp1 = classifier_inputs[n];
                classifier_inputs[n] = classifier_inputs[k];
                classifier_inputs[k] = temp1;
                var temp2 = classifier_tags[n];
                classifier_tags[n] = classifier_tags[k];
                classifier_tags[k] = temp2;
            }
        }

        static int get_posx(int x)
        {
            int posx = 0;

            if (x == 0) return 0;
            else return get_posx(x - 1) + (19 - x + 1) * 120;
        }

        static int get_coherence_location(Tuple<int, int> t)
        {

            int location = get_posx(t.Item1) + (t.Item2 - (t.Item1 + 1)) * 120;

            return location;
        }

        static SortedDictionary<Tuple<int, int>, int> coherence_electrode_select(int[] electrode_list)
        {
            SortedDictionary<Tuple<int, int>, int> locations = new SortedDictionary<Tuple<int, int>, int> ();

            // form tuples:
            List<Tuple<int, int>> combs = new List<Tuple<int, int>>();
            foreach(int i in electrode_list)
            {
                foreach (int j in electrode_list)
                {
                    if (i < j) combs.Add(new Tuple<int, int>(i, j));
                }
            }
            
            foreach(Tuple<int, int> t in combs){
                locations[t] = get_coherence_location(t);
            }

            return locations;
        }

        static void load_inputs(int[] electrode_list, int[] power_bands)
        {

            List<double[]> inputs = new List<double[]>();
            List<int> tags = new List<int>();
            StreamReader reader;
            String line;

            // Adding in deaths bandpower data
            string folder = Path.GetDirectoryName(Process.GetCurrentProcess().MainModule.FileName) + @"\data\";
            string[] files = Directory.GetFiles(folder, "deaths_bandpower_*.txt");

            foreach (var f in files)
            {
                double[][] data;
                reader = new StreamReader(f);
                line = reader.ReadLine();
                data = JsonConvert.DeserializeObject<double[][]>(line);
                reader.Close();
                if (data.Length > 0)
                {
                    for (var i = 0; i < data.Length; i++)
                    {
                        var formatted_data = new List<double>();
                        for (int k = 0; k < 19; k++)
                        {
                            if (electrode_list.Contains(k)){
                                for (int j = 0; j < 6; j++)
                                {
                                   if (power_bands.Contains(j)) formatted_data.Add(data[i][k * 6 + j]);
                                }
                            }
                        }
                        inputs.Add(formatted_data.ToArray());
                        tags.Add(1);
                    }
                }
            }

            // Adding deaths coherence data


            files = Directory.GetFiles(folder, "deaths_coherences_*.txt");

            foreach (var f in files)
            {
                double[][] data;
                reader = new StreamReader(f);
                line = reader.ReadLine();
                data = JsonConvert.DeserializeObject<double[][]>(line);
                reader.Close();
                if (data.Length > 0)
                {
                    for (var i = 0; i < data.Length; i++)
                    {
                        var formatted_data = new List<double>();
                        if (electrode_list.Length == 19)
                            formatted_data.AddRange(data[i]);
                        else
                        {
                            // selecting electrodes
                            SortedDictionary<Tuple<int, int>, int> locations = coherence_electrode_select(electrode_list);
                            foreach (Tuple<int, int> t in locations.Keys)
                            {

                                if (power_bands.Length == 6) // if all bands included
                                    for (int j = 0; j < 120; j++)
                                        formatted_data.Add(data[i][locations[t] + j]);
                                else // select frequency band
                                {
                                    if (power_bands.Contains(0))
                                    {
                                        for (int j = 1; j < 4; j++)   // 0 = 1hz - skip; 1 = 2hz, 2 = 3hz, 3 = 4hz (delta band)
                                            formatted_data.Add(data[i][locations[t] + j]);
                                    }
                                    if (power_bands.Contains(1))
                                    {
                                        for (int j = 4; j < 7; j++)   // 4 = 5hz; 5 = 6hz, 6 = 7hz (theta band)
                                            formatted_data.Add(data[i][locations[t] + j]);
                                    }
                                    if (power_bands.Contains(2))
                                    {
                                        for (int j = 7; j < 15; j++)   // 7 = 8hz;  14 = 15hz (alpha band)
                                            formatted_data.Add(data[i][locations[t] + j]);
                                    }
                                    if (power_bands.Contains(3))
                                    {
                                        for (int j = 15; j < 31; j++)   // 15 = 16hz;  30 = 31hz (beta band)
                                            formatted_data.Add(data[i][locations[t] + j]);
                                    }
                                    if (power_bands.Contains(4))
                                    {
                                        for (int j = 31; j < 49; j++)   // 31 = 32hz;  48 = 49hz (lower gamma band - up to 50hz cutoff)
                                            formatted_data.Add(data[i][locations[t] + j]);
                                    }
                                    if (power_bands.Contains(5))
                                    {
                                        for (int j = 50; j < 120; j++)   // 50 = 51hz;  119 = 120hz (higher gamma band - past 50hz cutoff)
                                            formatted_data.Add(data[i][locations[t] + j]);
                                    }
                                    // 50hz excluded due to high interference from surrounding electrical equipment - standard frequency in the UK.
                                }
                            }
                        }
                        inputs.Add(formatted_data.ToArray());
                        tags.Add(1);
                    }
                }
            }

            // Adding in eyes open data

            files = Directory.GetFiles(folder, "*_eyes_*_bandpower.txt");
            foreach (var f in files)
            {
                double[][] data;
                reader = new StreamReader(f);
                line = reader.ReadLine();
                data = JsonConvert.DeserializeObject<double[][]>(line);
                reader.Close();
                for (var i = 0; i < data.Length; i++)
                {
                    var formatted_data = new List<double>();
                    for (int k = 0; k < 19; k++)
                    {
                        if (electrode_list.Contains(k))
                        {
                            for (int j = 0; j < 6; j++)
                            {
                                if (power_bands.Contains(j)) formatted_data.Add(data[i][k * 6 + j]);
                            }
                        }
                    }
                    inputs.Add(formatted_data.ToArray());
                    tags.Add(0);
                }

            }

            // Add baseline coherence data

            files = Directory.GetFiles(folder, "*_eyes_*_coherences.txt");
            foreach (var f in files)
            {
                double[][] data;
                reader = new StreamReader(f);
                line = reader.ReadLine();
                data = JsonConvert.DeserializeObject<double[][]>(line);
                reader.Close();
                for (var i = 0; i < data.Length; i++)
                {
                    
                    var formatted_data = new List<double>();
                    if (electrode_list.Length == 19)
                        formatted_data.AddRange(data[i]);
                    else
                    {
                        // selecting electrodes
                        SortedDictionary<Tuple<int, int>, int> locations = coherence_electrode_select(electrode_list);
                        foreach (Tuple<int, int> t in locations.Keys)
                        {
                            if (power_bands.Length == 6) // if all bands included
                                for (int j = 0; j < 120; j++)
                                    formatted_data.Add(data[i][locations[t] + j]);
                            else // select frequency band
                            {
                                if (power_bands.Contains(0))
                                {
                                    for (int j = 1; j < 4; j++)   // 0 = 1hz - skip; 1 = 2hz, 2 = 3hz, 3 = 4hz (delta band)
                                        formatted_data.Add(data[i][locations[t] + j]);
                                }
                                if (power_bands.Contains(1))
                                {
                                    for (int j = 4; j < 7; j++)   // 4 = 5hz; 5 = 6hz, 6 = 7hz (theta band)
                                        formatted_data.Add(data[i][locations[t] + j]);
                                }
                                if (power_bands.Contains(2))
                                {
                                    for (int j = 7; j < 15; j++)   // 7 = 8hz;  14 = 15hz (alpha band)
                                        formatted_data.Add(data[i][locations[t] + j]);
                                }
                                if (power_bands.Contains(3))
                                {
                                    for (int j = 15; j < 31; j++)   // 15 = 16hz;  30 = 31hz (beta band)
                                        formatted_data.Add(data[i][locations[t] + j]);
                                }
                                if (power_bands.Contains(4))
                                {
                                    for (int j = 31; j < 49; j++)   // 31 = 32hz;  48 = 49hz (lower gamma band - up to 50hz cutoff)
                                        formatted_data.Add(data[i][locations[t] + j]);
                                }
                                if (power_bands.Contains(5))
                                {
                                    for (int j = 50; j < 120; j++)   // 50 = 51hz;  119 = 120hz (higher gamma band - past 50hz cutoff)
                                        formatted_data.Add(data[i][locations[t] + j]);
                                }
                                // 50hz excluded due to high interference from surrounding electrical equipment - standard frequency in the UK.
                            }
                        }
                    }
                    inputs.Add(formatted_data.ToArray());
                    tags.Add(0);
                }

            }


            classifier_inputs = inputs.ToArray();
            classifier_tags = tags.ToArray();

        }

        static void cross_validation()
        {
            int folds = 10;

        }
        static void main(String[] args)
        {
            int[] electrode_list;
            int[] power_bands;
            if (args == null || args.Length == 0)
            {
                electrode_list = new int[19];
                for (int i = 0; i < 19; i++)
                {
                    electrode_list[i] = i;
                }
                power_bands = new int[] { 0, 1, 2, 3, 4, 5 };
            }
            else if (args.Length == 1)
            {
                String ls = args[0];
                electrode_list = ls.Split(',').Select(e => int.Parse(e)).ToArray();
                power_bands = new int[] { 0, 1, 2, 3, 4, 5 };
            }
            else
            {
                String ls = args[0];
                electrode_list = ls.Split(',').Select(e => int.Parse(e)).ToArray();
                ls = args[1];
                power_bands = ls.Split(',').Select(e => int.Parse(e)).ToArray();
            }

            /* power bands:
             * 0 = delta (2 - 4)
             * 1 = theta (5 - 7)
             * 2 = alpha (8 - 15)
             * 3 = beta (16 - 31)
             * 4 = gamma (32 - 49)
             * 5 = gamma (50 - 120)
            */
            
            load_inputs(electrode_list, power_bands);


            //Console.WriteLine("starting training;");
            // var nb = new NaiveBayesLearning<NormalDistribution>(); // naive bayes init
            // var classifier = nb.Learn(classifier_inputs, classifier_tags); // naive bayes

            shuffle_inputs(new Random());

            double[][] outputs = new double[classifier_tags.Length][];
            for (int i = 0; i < classifier_tags.Length; i++)
            {
                outputs[i] = new double[] { (double)classifier_tags[i] };
            }

            // LinearDiscriminantAnalysis lda = new LinearDiscriminantAnalysis(); // lda init
            //  var classifier = lda.Learn(classifier_inputs, classifier_tags); // lda

            // neural net
            /*
            var network = new ActivationNetwork(new BipolarSigmoidFunction(), inputsCount: classifier_inputs[0].Length, neuronsCount: new[] { 30, 30, 1 });
            var teacher = new Accord.Neuro.Learning.ParallelResilientBackpropagationLearning(network);
            //var teacher = new Accord.Neuro.Learning.LevenbergMarquardtLearning(network);
            GaussianWeights initializer = new GaussianWeights(network);
            initializer.Randomize();


            

            var y = outputs;
            */

            // Iterate until stop criteria is met
            //double error = double.PositiveInfinity;
            /*

            for (int i = 0; i <25 ; i++)
            {
                error = teacher.RunEpoch(classifier_inputs, y);
                
            }
            */
            //Console.WriteLine("done training;");

            //int[] answers = classifier_inputs.Apply(network.Compute).GetColumn(0).Apply(System.Math.Sign);

            // get input set score
            /*
            int correct = 0;
            int wrong = 0;
            int total = classifier_tags.Length;
            */
            /*
            for (var i = 0; i < classifier_inputs.Length; i++)
            {
                int cls = classifier.Decide(classifier_inputs[i]);
                Console.Write("expected: ");
                Console.Write(classifier_tags[i]);
                Console.Write(" : received: ");
                Console.Write(Convert.ToInt32(cls));
                Console.WriteLine("");

                if (cls == classifier_tags[i]) correct++;
                else wrong++;
            }
            */
            /*
            for (int i = 0; i < classifier_tags.Length; i++)
            {

                var ans = (int)Math.Round(network.Compute(classifier_inputs[i])[0]);

                if (ans == classifier_tags[i])
                {
                    correct++;
                }
                else wrong++;
            }
            double acc = (double)correct / (double)total;  // training set accuracy
            */

            // cross validation code starts here

            var cross_validation_accuracies = new List<double>();
            List<double[]> arr = classifier_inputs.ToList();
            List<double[]> outarr = outputs.ToList();


            int folds = 10;
            int fold_size = classifier_inputs.Length / folds;

            int cross_correct_total = 0;
            int cross_wrong_total = 0;
            int cross_total_total = 0;

            for (int i = 0; i < folds; i++)
            {

                var training_set = new List<double[]>();
                var training_tags = new List<double[]>();
                var validation_set = new List<double[]>();
                var validation_tags = new List<double[]>();

                training_set.AddRange(arr.Take(i * fold_size));
                training_tags.AddRange(outarr.Take(i * fold_size));

                validation_set.AddRange(arr.Skip(i * fold_size).Take(fold_size));
                validation_tags.AddRange(outarr.Skip(i * fold_size).Take(fold_size));

                training_set.AddRange(arr.Skip(i * fold_size + fold_size));
                training_tags.AddRange(outarr.Skip(i * fold_size + fold_size));

                var training_array = training_set.ToArray();
                var training_array_tags = training_tags.ToArray();

                var validation_array = validation_set.ToArray();
                var validation_array_tags = validation_tags.ToArray();

                var cross_network = new ActivationNetwork(new BipolarSigmoidFunction(), inputsCount: training_array[0].Length, neuronsCount: new[] { 30, 1 });
                var cross_teacher = new Accord.Neuro.Learning.ParallelResilientBackpropagationLearning(cross_network);
                //var teacher = new Accord.Neuro.Learning.LevenbergMarquardtLearning(network);
                GaussianWeights cross_initializer = new GaussianWeights(cross_network);
                cross_initializer.Randomize();

                var z = training_array_tags;

                double cross_error = double.PositiveInfinity;
                // training network
                for (int t = 0; t < 25; t++)
                {
                    cross_error = cross_teacher.RunEpoch(training_array, z);

                }
                // network trained - calculate accuracy.


                int cross_correct = 0;
                int cross_wrong = 0;
                int cross_total = validation_array_tags.Length;

                for (int t = 0; t < validation_array_tags.Length; t++)
                {

                    var ans = (int)Math.Round(cross_network.Compute(validation_array[t])[0]);
                    if (ans == validation_array_tags[t][0])
                    {
                        cross_correct++;
                    }
                    else cross_wrong++;
                }

                cross_correct_total += cross_correct;
                cross_wrong_total += cross_wrong;
                cross_total_total += cross_total;
                double cross_acc = (double)cross_correct / (double)cross_total;
                
                cross_validation_accuracies.Add(cross_acc);
            }

            var acc = cross_validation_accuracies.Average();
            // write cross_validation_accuracies to a file.
            Console.WriteLine(cross_validation_accuracies.ToString());

            

            Dictionary<string, object> collection = new Dictionary<string, object>()
            {
                {"Accuracy", acc},
                {"Fold_Accuracies", cross_validation_accuracies.ToArray()}
            };

            JObject Result = new JObject(
                new JProperty("metrics",
                    JObject.FromObject(collection)
                )
            );

            Console.WriteLine("BEGIN METRICS");
            Console.WriteLine(Result.ToString());
            Console.WriteLine("END METRICS");
            Console.ReadKey();
        }

    }
}
