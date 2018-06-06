data = load('hil2_video.easy');
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

video_sound_distractor_trials = zeros(length(video_sound_distractors), 2500, 19);
video_image_distractor_trials = zeros(length(video_image_distractors), 2500, 19);

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

video_sound_distractor_trial_no =  length(video_sound_distractors);
video_image_distractor_trial_no  = length(video_image_distractors);

video_sound_distractor_bandpower = extract_bandpower(video_sound_distractor_trials, length(video_sound_distractors));
video_image_distractor_bandpower = extract_bandpower(video_image_distractor_trials, length(video_image_distractors));
video_bandpower = extract_bandpower(video_trials, video_trial_no);

meta_file = fopen('Metadata_video.txt', 'w');

fprintf(meta_file, 'video_trial_no %d\n', video_trial_no);
fprintf(meta_file, 'video_sound_distractor_trial_no %d\n', video_sound_distractor_trial_no);
fprintf(meta_file, 'video_image_distractor_trial_no %d\n', video_image_distractor_trial_no);

fclose(meta_file);

vid_sd_file = fopen('video_sound_distractor_bandpower.txt', 'w');
jsencode(vid_sd_file, video_sound_distractor_bandpower);

vid_id_file = fopen('video_image_distractor_bandpower.txt', 'w');
jsencode(vid_id_file, video_image_distractor_bandpower);

video_file = fopen('video_bandpower.txt', 'w');
jsencode(video_file, video_bandpower);
