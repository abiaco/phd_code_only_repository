data = load('20171103101050_17LP_alpha_eyes_closed.easy');
fs = 500;

% data(:,1) - first channel data.
% data(:,21) - flag
% data(:, 22) - timestamp
% data(:, 20) - EXT

% filter signal

% bandpass - 0.4 - 50hz


[d,c] = butter(6, [2 120]/(fs/2), 'bandpass');
[b,a] = butter(6, [49 51]/(fs / 2), 'stop');


% for each channel, apply 4th order butterworth bandpass filter;
for i=1:19
    data(:,i) = filtfilt(d, c, data(:,i));
    data(:,i) = filtfilt(b, a, data(:,i)); 
end

% extract video data
video_start = 1;
while data(video_start, 21) ~=1
    video_start = video_start + 1;
end

video_stop = video_start + 1;
while data(video_stop, 21) ~= 2
    video_stop = video_stop + 1;
end
disp('extracted video data locations');
% extract game data
game_start = video_stop + 1;
while data(game_start, 21) ~= 1
    game_start = game_start + 1;
end

game_end = game_start + 1;
while data(game_end, 21) ~= 2
    game_end = game_end + 1;
end
disp('extracted game data locations');
% extract video and game distractors
video_sound_distractors = [];
video_image_distractors = [];
for i=video_start:video_stop
    if data(i, 21) == 3
        video_sound_distractors = [video_sound_distractors i];
    end
    if data(i, 21) == 4
        video_image_distractors = [video_image_distractors i];
    end
end
disp('extracted video distractor locations');
game_sound_distractors = [];
game_image_distractors = [];
game_summoner_spells = [];
game_deaths = [];
game_sync_tokens = [];
deaths = 0;
summs = 0;
for i=game_start: game_end
    if data(i, 21) == 3
        game_sound_distractors = [game_sound_distractors i];
    end
    if data(i, 21) == 4
        game_image_distractors = [game_image_distractors i];
    end
    if data(i, 21) == 6
        game_summoner_spells = [game_summoner_spells i];
        summs = summs + 1;
    end
    if data(i, 21) == 5
        game_deaths = [game_deaths i];
        deaths = deaths + 1;
    end
    if data(i, 21) == 10
        game_sync_tokens = [game_sync_tokens i];
    end
end
disp('extracted game distractor locations');

pre_death_data = zeros(deaths,30001,22);

for i=1:deaths
    startseq = game_deaths(i) - 30000;
    endseq = game_deaths(i);
    pre_death_data(i,:, :) = data(startseq:endseq, :);
end

summ_data = zeros(summs, 10001, 22);
for i=1:summs
    startseq = game_summoner_spells(i) - 5000;
    endseq = game_summoner_spells(i) + 5000;
    summ_data(i,:, :) = data(startseq:endseq, :);
end

% extract trials

% extract pre death trials
death_trial_no = 0;
i = 1;
while (i + 2499) <= 30001
    death_trial_no = death_trial_no + 1;
    i = i + 250;
end

death_trial_no = death_trial_no * deaths;
death_trials = zeros(death_trial_no, 2500 , 19);
index = 1;
for i=1:deaths
    j = 1;
    while (j + 2499) <= 30001
        trial = pre_death_data(i, j:j+2499, 1:19);
        death_trials(index, :) = trial(:);
        j = j + 250;
        index = index + 1;
    end
end

% getting summoner spell trials
summ_trial_no = 0;
i = 1;
while (i + 2499) <= 10001
    summ_trial_no = summ_trial_no + 1;
    i = i + 250;
end
summ_trial_no = summ_trial_no * summs;

summ_trials = zeros(summ_trial_no, 2500, 19);
index = 1;
for i=1:summs
    j = 1;
    while (j + 2499) < 10001
        trial = summ_data(i, j:j+2499, 1:19);
        summ_trials(index, :) = trial(:);
        j = j + 250;
        index = index + 1;
    end
end


% calculate number of trials in video with sliding window
i = video_start;
video_trial_no = 0;
while (i + 2499) <= video_stop
    video_trial_no = video_trial_no + 1;
    i = i + 250;
end

% extract video trials with window
i = video_start;
video_trials = zeros(video_trial_no, 2500, 19);
index = 1;
while (i + 2499) <= video_stop
    trial = data(i:i+2499, 1:19);
    video_trials(index,:) = trial(:);
    i = i + 250; % half a second sliding time window
    index = index + 1;
    if index == video_trial_no
        break;
    end
end
disp('extracted video trials');


% calculate number of trials in game with sliding window
i = game_start;
game_trial_no = 0;
while (i + 2499) <= game_end
    game_trial_no = game_trial_no + 1;
    i = i + 250;
end

% extract game trials with window
i = game_start;
game_trials = zeros(game_trial_no, 2500, 19);
index = 1;
while (i + 2499) <= game_end
    trial = data(i:i+2499, 1:19);
    game_trials(index,:) = trial(:);
    i = i + 250; % half a second sliding time window
    index = index + 1;
    if index == game_trial_no
        break;
    end
end
disp('extracted game trials');

% extracting distractor trials

video_sound_distractor_trials = zeros(length(video_sound_distractors), 2500, 19);
video_image_distractor_trials = zeros(length(video_image_distractors), 2500, 19);

game_sound_distractor_trials = zeros(length(game_sound_distractors), 2500, 19);
game_image_distractor_trials = zeros(length(game_image_distractors), 2500, 19);

for i=1:length(video_sound_distractors)
    trial_start = video_sound_distractors(i) - 1500;
    trial_end = video_sound_distractors(i) + 999;
    trial = data(trial_start:trial_end, 1:19);
    video_sound_distractor_trials(i,:) = trial(:); 
end

for i=1:length(video_image_distractors)
    trial_start = video_image_distractors(i) - 1500;
    trial_end = video_image_distractors(i) + 999;
    trial = data(trial_start:trial_end, 1:19);
    video_image_distractor_trials(i,:) = trial(:); 
end

for i=1:length(game_sound_distractors)
    trial_start = game_sound_distractors(i) - 1500;
    trial_end = game_sound_distractors(i) + 999;
    trial = data(trial_start:trial_end, 1:19);
    game_sound_distractor_trials(i,:) = trial(:); 
end

for i=1:length(game_image_distractors)
    trial_start = game_image_distractors(i) - 1500;
    trial_end = game_image_distractors(i) + 999;
    trial = data(trial_start:trial_end, 1:19);
    game_image_distractor_trials(i,:) = trial(:); 
end

disp('extracted distractor trials')
% extract bandpower


%bpfeats = [video_bandpower, summs_bandpower];
labels = zeros(summ_trial_no + video_trial_no, 1);
labels(1:video_trial_no, 1) = 1;
labels(video_trial_no + 1: video_trial_no + summ_trial_no, 1) = 2;

video_sound_distractor_trial_no =  length(video_sound_distractors);
video_image_distractor_trial_no  = length(video_image_distractors);
game_sound_distractor_trial_no = length(game_sound_distractors);
game_image_distractor_trial_no = length(game_image_distractors);

video_sound_distractor_bandpower = extract_bandpower(video_sound_distractor_trials, length(video_sound_distractors));
video_image_distractor_bandpower = extract_bandpower(video_image_distractor_trials, length(video_image_distractors));

game_sound_distractor_bandpower = extract_bandpower(game_sound_distractor_trials, length(game_sound_distractors));
game_image_distractor_bandpower = extract_bandpower(game_image_distractor_trials, length(game_image_distractors));

summs_bandpower = extract_bandpower(summ_trials, summ_trial_no);
death_bandpower = extract_bandpower(death_trials, death_trial_no);

video_bandpower = extract_bandpower(video_trials, video_trial_no);

game_overall_bandpower = extract_bandpower(game_trials, game_trial_no);

meta_file = fopen('Metadata.txt', 'w');

fprintf(meta_file, 'video_trial_no %d\n', video_trial_no);
fprintf(meta_file, 'game_trial_no %d\n', game_trial_no);
fprintf(meta_file, 'summ_trial_no %d\n', summ_trial_no);
fprintf(meta_file, 'death_trial_no %d\n', death_trial_no);
fprintf(meta_file, 'video_sound_distractor_trial_no %d\n', video_sound_distractor_trial_no);
fprintf(meta_file, 'video_image_distractor_trial_no %d\n', video_image_distractor_trial_no);
fprintf(meta_file, 'game_sound_distractor_trial_no %d\n', game_sound_distractor_trial_no);
fprintf(meta_file, 'game_image_distractor_trial_no %d\n', game_image_distractor_trial_no);

fclose(meta_file);

vid_sd_file = fopen('video_sound_distractor_bandpower.txt', 'w');
jsencode(vid_sd_file, video_sound_distractor_bandpower);

vid_id_file = fopen('video_image_distractor_bandpower.txt', 'w');
jsencode(vid_id_file, video_image_distractor_bandpower);

game_sd_file = fopen('game_sound_distractor_bandpower.txt', 'w');
jsencode(game_sd_file, game_sound_distractor_bandpower);

game_id_file = fopen('game_image_distractor_bandpower.txt', 'w');
jsencode(game_id_file, game_image_distractor_bandpower);

deaths_file = fopen('deaths_bandpower.txt', 'w');
jsencode(deaths_file, death_bandpower);

summs_file = fopen('summs_bandpower.txt', 'w');
jsencode(summs_file, summs_bandpower);

video_file = fopen('video_bandpower.txt', 'w');
jsencode(video_file, video_bandpower);

game_file = fopen('game_bandpower.txt', 'w');
jsencode(game_file, game_overall_bandpower);