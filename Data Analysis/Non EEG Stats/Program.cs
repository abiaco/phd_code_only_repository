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

namespace AnalysisCSharp
{
    class Program
    {

        static String[] usernames = { "Hil_13-03-2017", "Mihail_22-03-2017", "Alin_03-05-2017", "Alex_15-06-2017", "Dany_16-06-2017", "Mira_28-03-2017"};
        // "Faye_07-09-2017", "Aidan_07-09-2017", "Qi_23-06-2017" 
        // "Hil_13-03-2017", "Mihail_22-03-2017", "Alin_03-05-2017", "Alex_15-06-2017", "Dany_16-06-2017",  "Mira_28-03-2017",


        static DataTable createMetaTable()
        {
            DataTable tb = new DataTable();

            tb.TableName = "MetaData";
            // metadata
            tb.Columns.Add("Participant ID", typeof(int));
            tb.Columns.Add("Player Skill", typeof(string));
            tb.Columns.Add("Play Style", typeof(string));
            tb.Columns.Add("Experiment Type", typeof(string));
            tb.Columns.Add("Questionnaire Score", typeof(int));

            return tb;
        }

        static DataTable createDistractorVideoTable()
        {
            DataTable tb = new DataTable();
            tb.TableName = "Distractors Data";
            tb.Columns.Add("Participant ID", typeof(int));
            // distractors - video
            // tb.Columns.Add("Video Sound Distractors Remembered", typeof(int));
            // tb.Columns.Add("Video Image Distractors Remembered", typeof(int));
            // tb.Columns.Add("Video Sound Distractors Count", typeof(int));
            // tb.Columns.Add("Video Image Distractors Count", typeof(int));
            // distractors - game
            // tb.Columns.Add("Game Sound Distractors Remembered", typeof(int));
            // tb.Columns.Add("Game Image Distractors Remembered", typeof(int));
            // tb.Columns.Add("Game Sound Distractors Count", typeof(int));
            // tb.Columns.Add("Game Image Distractors Count", typeof(int));
            // distractors - percentages
            tb.Columns.Add("Percentage Remembered Video Sound Distractors", typeof(int));
            tb.Columns.Add("Percentage Remembered Video Image Distractors", typeof(int));
            tb.Columns.Add("Percentage Remembered Video Overall Distractors", typeof(int));
            return tb;
        }
        static DataTable createDistractorGameTable()
        {
            DataTable tb = new DataTable();
            tb.TableName = "Distractors Data";
            tb.Columns.Add("Participant ID", typeof(int));
            tb.Columns.Add("Percentage Remembered Game Sound Distractors", typeof(int));
            tb.Columns.Add("Percentage Remembered Game Image Distractors", typeof(int));
            tb.Columns.Add("Percentage Remembered Game Overall Distractors", typeof(int));

            return tb;
        }

        static DataTable createDeathsTable() {
            DataTable tb = new DataTable();
            tb.TableName = "In-Game Deaths Data";
            tb.Columns.Add("Participant ID", typeof(int));
            // deaths
            tb.Columns.Add("Deaths Reported", typeof(int));
            tb.Columns.Add("Death Count", typeof(int));
            tb.Columns.Add("Percentage Deaths Reported", typeof(int));
            return tb;
        }        
        static void Main(string[] args)
        {
            DataTable meta_table = createMetaTable();
            DataTable distractors_video_table = createDistractorVideoTable();
            DataTable distractors_game_table = createDistractorGameTable();
            DataTable deaths_table = createDeathsTable();
            var QuestionnaireScores = new List<double>();
            var video_sound_distractor_percentages = new List<double>();
            var video_image_distractor_percentages = new List<double>();
            var sound_distractor_percentages = new List<double>();
            var image_distractor_percentages = new List<double>();
            var avgdistractors = new List<double>();
            var aps = new List<double>();
            var summs = new List<double>();
            var exptype = new List<double>();
            var skill_lvl = new List<double>();
            var death_remembered_percentages = new List<double>();

            var underskill_scores = new List<double>();
            var overskill_scores = new List<double>();
            var matched_scores = new List<double>();

            var focused_scores = new List<double>();
            var average_scores = new List<double>();
            var detached_scores = new List<double>();

            var bronze_scores = new List<double>();
            var goldplus_scores = new List<double>();

            var bps = new List<double[]>();

            int participantID = 0;
            foreach (String uname in usernames)
            {
                participantID++;
                Experiment exp1 = new Experiment(uname);
                Console.Out.WriteLine("User: " + uname);
                Console.Out.WriteLine("Skill Level: " + exp1.skill_level);
                Console.Out.WriteLine("Play Style: " + exp1.style);
                Console.Out.WriteLine("Experiment Type: " + exp1.exp_type);
                Console.Out.WriteLine("Questionnaire Score: " + exp1.questionnaire_score);
                Console.Out.WriteLine("number of distractors: ");
                Console.Out.WriteLine(exp1.video_image_distractor_no + " " + exp1.video_sound_distractor_no + " " + exp1.game_sound_distractor_no + " " + exp1.game_image_distractor_no);
                Console.Out.WriteLine("distractor remembering percentages:");
                Console.Out.WriteLine("video sound distractors: " + exp1.video_sound_distractor_proc);
                Console.Out.WriteLine("video image distractors: " + exp1.video_image_distractor_proc);
                Console.Out.WriteLine("game sound distractors: " + exp1.game_sound_distractor_proc);
                Console.Out.WriteLine("game image distractors: " + exp1.game_image_distractor_proc);
                Console.Out.WriteLine("");
                Console.Out.WriteLine("");
                QuestionnaireScores.Add(exp1.questionnaire_score);
                video_sound_distractor_percentages.Add(exp1.video_sound_distractor_proc);
                video_image_distractor_percentages.Add(exp1.video_image_distractor_proc);
                sound_distractor_percentages.Add(exp1.game_sound_distractor_proc);
                image_distractor_percentages.Add(exp1.game_image_distractor_proc);
                death_remembered_percentages.Add(exp1.death_remember_proc);
                double video_avg_proc = (exp1.video_sound_distractor_proc + exp1.video_image_distractor_proc) / 2;
                double avg_proc = (exp1.game_sound_distractor_proc + exp1.game_image_distractor_proc) / 2;
                avgdistractors.Add(avg_proc);
                aps.Add(exp1.moves_per_sec);
                summs.Add(exp1.summ_no);
                exptype.Add((int)exp1.exp_type);
                skill_lvl.Add((int)exp1.skill_level);
                if (exp1.exp_type == Experiment_Types.UnderSkill)
                    underskill_scores.Add(exp1.questionnaire_score);
                else if (exp1.exp_type == Experiment_Types.Matched)
                    matched_scores.Add(exp1.questionnaire_score);
                else if (exp1.exp_type == Experiment_Types.OverSkill)
                    overskill_scores.Add(exp1.questionnaire_score);

                if (exp1.style == PlayStyle.Focused)
                    focused_scores.Add(exp1.questionnaire_score);
                else if (exp1.style == PlayStyle.Average)
                    average_scores.Add(exp1.questionnaire_score);
                else if (exp1.style == PlayStyle.Detached)
                    detached_scores.Add(exp1.questionnaire_score);

                if (exp1.skill_level == Ranks.Bronze)
                    bronze_scores.Add(exp1.questionnaire_score);
                else goldplus_scores.Add(exp1.questionnaire_score);

                foreach (var item in exp1.game_bandpower){
                    bps.Add(item);
                }

                foreach(var item in exp1.video_bandpower)
                {
                    bps.Add(item);
                }
                // KMeans tr = new KMeans(4);
                // tr.Learn(exp1.game_bandpower);

                // Console.Write(tr.ToString());

                // Console.ReadKey();

                meta_table.Rows.Add(participantID, exp1.skill_level, exp1.style, exp1.exp_type, exp1.questionnaire_score);
                deaths_table.Rows.Add(participantID, exp1.death_no, exp1.actual_death_no, exp1.death_remember_proc);
                distractors_video_table.Rows.Add(participantID, exp1.video_sound_distractor_proc, exp1.video_image_distractor_proc, video_avg_proc);
                distractors_game_table.Rows.Add(participantID, exp1.game_sound_distractor_proc, exp1.game_image_distractor_proc, avg_proc);
            }

            var test = new PearsonCorrelation();
            test.Similarity(QuestionnaireScores.ToArray(), sound_distractor_percentages.ToArray());
            Console.Out.WriteLine("accord.net similarity value: ");
            Console.Out.WriteLine(test.Similarity(QuestionnaireScores.ToArray(), sound_distractor_percentages.ToArray()));

            Console.Out.WriteLine("");
            Console.Out.WriteLine("correlation questionnaire - sound distractor rememberabce");
            Console.Out.WriteLine(Correlation.Pearson(QuestionnaireScores, sound_distractor_percentages));
            Console.Out.WriteLine("");
            Console.Out.WriteLine("correlation questionnaire - image distractor rememberabce");
            Console.Out.WriteLine(Correlation.Pearson(QuestionnaireScores, image_distractor_percentages));
            Console.Out.WriteLine("");
            Console.Out.WriteLine("correlation questionnaire - overall distractor rememberabce");
            Console.Out.WriteLine(Correlation.Pearson(QuestionnaireScores, avgdistractors));
            Console.Out.WriteLine("");
            Console.Out.WriteLine("correlation questionnaire - moves per second");
            Console.Out.WriteLine(Correlation.Pearson(QuestionnaireScores, aps));
            Console.Out.WriteLine("");
            Console.Out.WriteLine("correlation questionnaire - number of summ spells");
            Console.Out.WriteLine(Correlation.Pearson(QuestionnaireScores, summs));
            Console.Out.WriteLine("");
            
            Console.Out.WriteLine("");
            Console.Out.WriteLine("correlation questionnaire - skill");
            Console.Out.WriteLine(Correlation.Pearson(QuestionnaireScores, skill_lvl));
            Console.Out.WriteLine("");
            Console.Out.WriteLine("correlation questionnaire - death rememberabce");
            Console.Out.WriteLine(Correlation.Pearson(QuestionnaireScores, death_remembered_percentages));
            Console.Out.WriteLine("");


            var scatter_values = new List<double>();
            scatter_values.AddRange(video_sound_distractor_percentages);
            scatter_values.AddRange(sound_distractor_percentages);

            double[][] samples = {underskill_scores.ToArray(),
                                  matched_scores.ToArray(),
                                  overskill_scores.ToArray()
                                 };

            OneWayAnova anova = new OneWayAnova(samples);
            // DataGridBox.Show(anova.Table);
            //Console.ReadKey();
            double[][] style_samples = {focused_scores.ToArray(),
                                        detached_scores.ToArray(),
                                        average_scores.ToArray()
                                 };

            OneWayAnova style_anova = new OneWayAnova(style_samples);
            // DataGridBox.Show(style_anova.Table);
            // Console.ReadKey();
            double[][] skill_samples = {bronze_scores.ToArray(),
                                        goldplus_scores.ToArray()
                                       };

            OneWayAnova skill_anova = new OneWayAnova(skill_samples);
            //DataGridBox.Show(skill_anova.Table);
            //Console.ReadKey();

            String meta_table_file = "D:\\Projects\\Conference Paper\\Tables\\meta_table.xml";
            String distractors_video_table_file = "D:\\Projects\\Conference Paper\\Tables\\distractors_video_table.xml";
            String distractors_game_table_file = "D:\\Projects\\Conference Paper\\Tables\\distractors_game_table.xml";
            String deaths_table_file = "D:\\Projects\\Conference Paper\\Tables\\deaths_table.xml";

            StreamWriter metaStreamWriter = new StreamWriter(meta_table_file);
            StreamWriter distractorsVideoStreamWriter = new StreamWriter(distractors_video_table_file);
            StreamWriter distractorsGameStreamWriter = new StreamWriter(distractors_game_table_file);
            StreamWriter deathsStreamWriter = new StreamWriter(deaths_table_file);

            meta_table.WriteXml(metaStreamWriter);
            distractors_video_table.WriteXml(distractorsVideoStreamWriter);
            distractors_game_table.WriteXml(distractorsGameStreamWriter);
            deaths_table.WriteXml(deathsStreamWriter);

            Console.ReadKey();
        }

        
    }
}
