files_game = dir('D:\Projects\Alpha_Immersion\analysis\matlab\game\*.easy');
fs = 500;
f = fopen('D:\Projects\Alpha_Immersion\analysis\matlab\game.csv', 'w');
fprintf(f, '%s\n', 'UID,Beeps,Deaths,Summs');
for k=1:(length(files_game))
    
    disp(files_game(k).name);
    data = load(strcat('D:\Projects\Alpha_Immersion\analysis\matlab\game\', files_game(k).name));
    uname = regexp(files_game(k).name, '_[1-9]+([a-z][A-Z])+_', 'match');
    uname = files_game(k).name(15:19);

    disp(uname);

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

    game_start = 1;
    game_end = length(data) - 2500;


    game_sound_distractors = [];
    game_image_distractors = [];
    game_summoner_spells = [];
    game_deaths = [];
    game_sync_tokens = [];
    deaths = 0;
    summs = 0;
    for i=game_start: game_end
        if data(i, 21) == 4
            game_sound_distractors = [game_sound_distractors i];
        end
        if data(i, 21) == 3
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
    
    fprintf(f, '%s,%d,%d,%d\n',uname,length(game_sound_distractors),deaths,summs);
    
    
    pre_death_data = zeros(deaths,30001,22);

    for i=1:deaths
        startseq = game_deaths(i) - 30000;
        if startseq < 1
            continue;
        end
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

    game_sound_distractor_trials = zeros(length(game_sound_distractors), 2500, 19);
    game_image_distractor_trials = zeros(length(game_image_distractors), 2500, 19);

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

    game_sound_distractor_trial_no = length(game_sound_distractors);
    game_image_distractor_trial_no = length(game_image_distractors);
    

   % [game_sound_distractor_bandpower, game_sound_distractor_freq] = extract_bandpower(game_sound_distractor_trials, length(game_sound_distractors));
   % [game_image_distractor_bandpower, game_image_distractor_freq] = extract_bandpower(game_image_distractor_trials, length(game_image_distractors));

   % [summs_bandpower, summs_freq] = extract_bandpower(summ_trials, summ_trial_no);
   % [death_bandpower, death_freq] = extract_bandpower(death_trials, death_trial_no);

   % [game_overall_bandpower, game_overall_freq] = extract_bandpower(game_trials, game_trial_no);
    
    [game_sound_distractor_coherences] = extract_coherence_matrix(game_sound_distractor_trials, length(game_sound_distractors));
    [summs_coherences] = extract_coherence_matrix(summ_trials, summ_trial_no);
    [death_coherences] = extract_coherence_matrix(death_trials, death_trial_no);
    [game_overall_coherences] = extract_coherence_matrix(game_trials, game_trial_no);
    
    
    meta_file_name = strcat('Metadata_game_',uname,'.txt');
    meta_file = fopen(meta_file_name, 'w');

    fprintf(meta_file, 'game_trial_no %d\n', game_trial_no);
    fprintf(meta_file, 'summ_trial_no %d\n', summ_trial_no);
    fprintf(meta_file, 'death_trial_no %d\n', death_trial_no);
    fprintf(meta_file, 'game_sound_distractor_trial_no %d\n', game_sound_distractor_trial_no);
   
    fclose(meta_file);
    
    
  %  game_sd_fname = strcat('game_sound_distractor_bandpower_',uname,'.txt');
  %  game_sd_file = fopen(game_sd_fname, 'w');
  %  jsencode(game_sd_file, game_sound_distractor_bandpower);
    
    game_sdch_fname = strcat('game_sound_distractor_coherences_',uname,'.txt');
    game_sdch_file = fopen(game_sdch_fname, 'w');
    jsencode(game_sdch_file, game_sound_distractor_coherences);
    
  %  game_sdfr_fname = strcat('game_sound_distractor_freq_',uname,'.txt');
  %  game_sdfr_file = fopen(game_sdfr_fname, 'w');
  %  jsencode(game_sdfr_file, game_sound_distractor_freq);
    
  %  deaths_fname = strcat('deaths_bandpower_',uname,'.txt');
  %  deaths_file = fopen(deaths_fname, 'w');
  %  jsencode(deaths_file, death_bandpower);
    
    deathsch_fname = strcat('deaths_coherences_',uname,'.txt');
    deathsch_file = fopen(deathsch_fname, 'w');
    jsencode(deathsch_file, death_coherences);
    
  %  deathsfr_fname = strcat('deaths_freq_',uname,'.txt');
  %  deathsfr_file = fopen(deathsfr_fname, 'w');
  %  jsencode(deathsfr_file, death_freq);
    
  %  summ_fname = strcat('summs_bandpower_',uname,'.txt');
  %  summs_file = fopen(summ_fname, 'w');
  %  jsencode(summs_file, summs_bandpower);
    
    summch_fname = strcat('summs_coherences_',uname,'.txt');
    summsch_file = fopen(summch_fname, 'w');
    jsencode(summsch_file, summs_coherences);
    
  %  summfr_fname = strcat('summs_freq_',uname,'.txt');
  %  summsfr_file = fopen(summfr_fname, 'w');
  %  jsencode(summsfr_file, summs_freq);

  %  game_fname =strcat('game_bandpower_',uname,'.txt');
  %  game_file = fopen(game_fname, 'w');
  %  jsencode(game_file, game_overall_bandpower);
    
    gamech_fname =strcat('game_coherences_',uname,'.txt');
    gamech_file = fopen(gamech_fname, 'w');
    jsencode(gamech_file, game_overall_coherences);
    
  %  gamefr_fname =strcat('game_freq_',uname,'.txt');
  %  gamefr_file = fopen(gamefr_fname, 'w');
  %  jsencode(gamefr_file, game_overall_freq);
    
end


net = selforgmap([10 10]);
net = train(net, game_overall_freq);

y = net(x);
classes = vec2ind(y);
