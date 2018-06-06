fs = 500;
files_eyes_closed = dir('D:\Projects\Alpha_Immersion\analysis\matlab\alpha_eyes_closed\*.easy');
files_eyes_open = dir('D:\Projects\Alpha_Immersion\analysis\matlab\alpha_eyes_open\*.easy');
disp('files loaded:');
for k=1:(length(files_eyes_closed))
    disp(files_eyes_closed(k).name);
end

for k=1:(length(files_eyes_closed))
    
    disp(files_eyes_closed(k).name);
    data = load(strcat('D:\Projects\Alpha_Immersion\analysis\matlab\alpha_eyes_closed\', files_eyes_closed(k).name));
    uname = regexp(files_eyes_closed(k).name, '_[1-9]+([a-z][A-Z])+_', 'match');
    uname = files_eyes_closed(k).name(15:19);

    disp(uname);
    % data(:,1) - first channel data.
    % data(:, 20) - EXT
    % data(:,21) - flag
    % data(:, 22) - timestamp
    

    % filter signal

    % bandpass - 0.4 - 50hz

    [d,c] = butter(6, [2 120]/(fs/2), 'bandpass');
    [b,a] = butter(6, [49 51]/(fs / 2), 'stop');

    % for each channel, apply 4th order butterworth bandpass filter;
    for i=1:19
        data(:,i) = filtfilt(d, c, data(:,i));
        data(:,i) = filtfilt(b, a, data(:,i)); 
    end


    trial_no =int32 (length(data) / (5 * fs)); % 5 second trials @ 500 samples / second

    i = 1;
    trials = zeros(trial_no, 2500, 19);
    index = 1;
    while (i + 2499) <= (trial_no * 2500)
        trial = data(i:i+2499, 1:19);
        trials(index,:) = trial(:);
        i = i + 250; % half a second sliding time window
        index = index + 1;
        if index == trial_no
            break;
        end
    end
    disp('extracted video trials');
    
    
    
    
    %[full_trial_bandpower, full_trial_dominant_frequency] = alpha_bandpower(full_trial, 1);
    %[trial_split_bandpowers, trial_split_dominant_frequencies] = extract_bandpower(trials, trial_no);
    [trial_split_coherences] = extract_coherence_matrix(trials, trial_no);
    
   % fname = strcat(uname,'_eyes_closed_bandpower.txt');
   % bp_file = fopen(fname, 'w');
   % jsencode(bp_file, trial_split_bandpowers);
    
   % fr_fname = strcat(uname,'_eyes_closed_freq.txt');
   % fr_file = fopen(fr_fname, 'w');
   % jsencode(fr_file, trial_split_dominant_frequencies);
    
    ch_fname = strcat(uname,'_eyes_closed_coherences.txt');
    ch_file = fopen(ch_fname, 'w');
    jsencode(ch_file, trial_split_coherences);
end


for k=1:(length(files_eyes_open))
    
    disp(files_eyes_open(k).name);
    data = load(strcat('D:\Projects\Alpha_Immersion\analysis\matlab\alpha_eyes_open\', files_eyes_open(k).name));
    uname = regexp(files_eyes_open(k).name, '_[1-9]+([a-z][A-Z])+_', 'match');
    uname = files_eyes_open(k).name(15:19);

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


    trial_no =int32 (length(data) / (5 * fs)); % 5 second trials @ 500 samples / second

    i = 1;
    trials = zeros(trial_no, 2500, 19);
    index = 1;
    while (i + 2499) <= (trial_no * 2500)
        trial = data(i:i+2499, 1:19);
        trials(index,:) = trial(:);
        i = i + 250; % half a second sliding time window
        index = index + 1;
        if index == trial_no
            break;
        end
    end
    disp('extracted video trials');

   % [trial_split_bandpowers, trial_split_dominant_frequencies] = extract_bandpower(trials, trial_no);
    [trial_split_coherences] = extract_coherence_matrix(trials, trial_no);
    
  %  fname = strcat(uname,'_eyes_open_bandpower.txt');
  %  bp_file = fopen(fname, 'w');
  %  jsencode(bp_file, trial_split_bandpowers);
    
  %  fr_fname = strcat(uname,'_eyes_open_freq.txt');
  %  fr_file = fopen(fr_fname, 'w');
  %  jsencode(fr_file, trial_split_dominant_frequencies);
    
    ch_fname = strcat(uname,'_eyes_open_coherences.txt');
    ch_file = fopen(ch_fname, 'w');
    jsencode(ch_file, trial_split_coherences);
end
