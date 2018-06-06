using System;
using System.IO;
using Newtonsoft.Json;

namespace AnalysisCSharp
{

    enum Experiment_Types
    {
        UnderSkill,
        Matched,
        OverSkill
        
    }
    enum Ranks
    {
        Unranked,
        Bronze,
        Silver,
        Gold,
        Platinum,
        Diamond
    }

    enum PlayStyle
    {
        Focused,
        Average,
        Detached
    }

    class Experiment
    {
        public String filepath;
        public Ranks skill_level;
        public Ranks rank;
        public PlayStyle style;
        public Experiment_Types exp_type;

        public int questionnaire_score;
        public int video_image_distractor_no, video_sound_distractor_no, game_sound_distractor_no, game_image_distractor_no;
        public int video_trial_no, game_trial_no, summ_no, death_trial_no;
        public int actual_death_no, death_no;

        public double[][] summ_bandpower, deaths_bandpower, video_bandpower, game_bandpower;
        public double[][] video_sound_distractor_bandpower, video_image_distractor_bandpower, game_sound_distractor_bandpower, game_image_distractor_bandpower;

        public double key_per_sec, moves_per_sec;
        public double video_sound_distractor_proc, video_image_distractor_proc, game_sound_distractor_proc, game_image_distractor_proc;
        public double death_remember_proc;

        public Experiment(String uname)
        {
            filepath = "D:\\Projects\\Data\\DataPacks\\" + uname + "\\";
            load_profile();
            load_metadata();
            load_features();
        }

        Ranks parseRank(string r)
        {
            Ranks rank;
            switch (r)
            {
                case "NA":
                    rank = Ranks.Unranked;
                    break;
                case "Bronze":
                    rank = Ranks.Bronze;
                    break;
                case "Silver":
                    rank = Ranks.Silver;
                    break;
                case "Gold":
                    rank = Ranks.Gold;
                    break;
                case "Platinum":
                    rank = Ranks.Platinum;
                    break;
                case "Diamond":
                    rank = Ranks.Diamond;
                    break;
                default:
                    rank = Ranks.Unranked;
                    break;
            }
            return rank;
        }

        PlayStyle parsePlayStyle(String ps)
        {
            PlayStyle st;
            switch (ps)
            {
                case "Focused":
                    st = PlayStyle.Focused;
                    break;
                case "Average":
                    st = PlayStyle.Average;
                    break;
                case "Detached":
                    st = PlayStyle.Detached;
                    break;
                default:
                    st = PlayStyle.Detached;
                    break;
            }
            return st;
        }

        Experiment_Types parseType(String tp)
        {
            Experiment_Types type;
            switch (tp)
            {
                case "Matched":
                    type = Experiment_Types.Matched;
                    break;
                case "Overskill":
                    type = Experiment_Types.OverSkill;
                    break;
                case "Underskill":
                    type = Experiment_Types.UnderSkill;
                    break;
                default:
                    type = Experiment_Types.Matched;
                    break;
            }
            return type;
        }

        void load_profile()
        {
            String line;
            StreamReader reader = new StreamReader(filepath + "Player Profile\\Profile.txt");
            while ((line = reader.ReadLine()) != null)
            {
                string[] comps = line.Split(' ');
                switch (comps[0]){
                    case "Rank:":
                        this.rank = parseRank(comps[1]);
                        break;
                    case "SkillLevel:":
                        this.skill_level = parseRank(comps[1]);
                        break;
                    case "PlayStyle:":
                        this.style = parsePlayStyle(comps[1]);
                        break;
                    case "ExperimentType:":
                        this.exp_type = parseType(comps[1]);
                        break;
                }
                
            }
        }
        void load_metadata()
        {
            string line;
            StreamReader reader = new StreamReader(filepath + "Features\\Metadata.txt");
            while ((line = reader.ReadLine()) != null)
            {
                string[] comps = line.Split(' ');
                switch (comps[0])
                {
                    case "video_trial_no":
                        this.video_trial_no = Int32.Parse(comps[1]);
                        break;
                    case "game_trial_no":
                        this.game_trial_no = Int32.Parse(comps[1]);
                        break;
                    case "summ_trial_no":
                        this.summ_no = Int32.Parse(comps[1]);
                        break;
                    case "death_trial_no":
                        this.death_trial_no = Int32.Parse(comps[1]);
                        this.death_no = death_trial_no / 111;
                        break;
                    case "video_sound_distractor_trial_no":
                        this.video_sound_distractor_no = Int32.Parse(comps[1]);
                        break;
                    case "video_image_distractor_trial_no":
                        this.video_image_distractor_no = Int32.Parse(comps[1]);
                        break;
                    case "game_sound_distractor_trial_no":
                        this.game_sound_distractor_no = Int32.Parse(comps[1]);
                        break;
                    case "game_image_distractor_trial_no":
                        this.game_image_distractor_no = Int32.Parse(comps[1]);
                        break;
                    default:
                        break;
                }
            }

            reader.Close();
            reader = new StreamReader(filepath + "Questionnaire\\Score.txt");
            line = reader.ReadLine();
            if (line != null)
            {
                string[] comps = line.Split(' ');
                this.questionnaire_score = Int32.Parse(comps[0]);
            }
            reader.Close();

            reader = new StreamReader(filepath + "Deaths\\actual_number.txt");
            line = reader.ReadLine();
            if (line != null)
            {
                string[] comps = line.Split(' ');
                this.actual_death_no = Int32.Parse(comps[0]);

                death_remember_proc = ((double)death_no / actual_death_no) * 100;
            }
            reader.Close();

            

            reader = new StreamReader(filepath + "Distractors\\Distractors.txt");
            while ((line = reader.ReadLine()) != null)
            {
                double remembered;
                string[] comps = line.Split(' ');
                switch (comps[0])
                {
                    case "Video_Sound_Distractors":
                        remembered = Int32.Parse(comps[1]);
                        
                        video_sound_distractor_proc = (remembered / video_sound_distractor_no) * 100;
                        break;
                    case "Video_Image_Distractors":
                        remembered = Int32.Parse(comps[1]);
                        video_image_distractor_proc = (remembered / video_image_distractor_no) * 100;
                        break;
                    case "Game_Sound_Distractors":
                        remembered = Int32.Parse(comps[1]);
                        game_sound_distractor_proc = (remembered / game_sound_distractor_no) * 100;
                        break;
                    case "Game_Image_Distractors":
                        remembered = Int32.Parse(comps[1]);
                        game_image_distractor_proc = (remembered / game_image_distractor_no) * 100;
                        break;
                    default:
                        break;
                }
            }
            reader.Close();


            reader = new StreamReader(filepath + "KeyLog\\keyscore.txt");
            line = reader.ReadLine();
            key_per_sec = Double.Parse(line);
            reader.Close();

            reader = new StreamReader(filepath + "MouseLog\\mousescore.txt");
            line = reader.ReadLine();
            moves_per_sec = Double.Parse(line);
            reader.Close();
        }

        void load_features()
        {
            StreamReader reader;
            String line;

            reader = new StreamReader(filepath + "Features//video_bandpower.txt");
            line = reader.ReadLine();
            video_bandpower = JsonConvert.DeserializeObject<double[][]>(line);
            reader.Close();


            reader = new StreamReader(filepath + "Features//game_bandpower.txt");
            line = reader.ReadLine();
            game_bandpower = JsonConvert.DeserializeObject<double[][]>(line);
            reader.Close();

            reader = new StreamReader(filepath + "Features//video_sound_distractor_bandpower.txt");
            line = reader.ReadLine();
            video_sound_distractor_bandpower = JsonConvert.DeserializeObject<double[][]>(line);
            reader.Close();

            reader = new StreamReader(filepath + "Features//video_image_distractor_bandpower.txt");
            line = reader.ReadLine();
            video_image_distractor_bandpower = JsonConvert.DeserializeObject<double[][]>(line);
            reader.Close();

            reader = new StreamReader(filepath + "Features//game_sound_distractor_bandpower.txt");
            line = reader.ReadLine();
            game_sound_distractor_bandpower = JsonConvert.DeserializeObject<double[][]>(line);
            reader.Close();

            reader = new StreamReader(filepath + "Features//game_image_distractor_bandpower.txt");
            line = reader.ReadLine();
            game_image_distractor_bandpower = JsonConvert.DeserializeObject<double[][]>(line);
            reader.Close();

            reader = new StreamReader(filepath + "Features//deaths_bandpower.txt");
            line = reader.ReadLine();
            deaths_bandpower = JsonConvert.DeserializeObject<double[][]>(line);
            reader.Close();

            reader = new StreamReader(filepath + "Features//summs_bandpower.txt");
            line = reader.ReadLine();
            summ_bandpower = JsonConvert.DeserializeObject<double[][]>(line);
            reader.Close();
        }

    }
}
