function [ alpha_features, dominant_freqs ] = alpha_bandpower( trial_set, trial_no )

alpha_features = zeros(trial_no, 2);
dominant_freqs = zeros(trial_no, 2);

index = 1;
for i=1:trial_no
    trial_feature = zeros(1, 2);
    trial_dom_freq = zeros(1, 2);
    ti = 1;
    for j=7:8  %only get O1 and O2
        channel = trial_set(i, : ,j);
        [pxx, f] = pwelch(channel,[],[], [8 9 10 11 12 13], 500, 'power');
        [peak_amplitude, peak_index] = max(pxx);
        peak_frequency = f(peak_index);
        features = sum(pxx);
        trial_feature(ti) = features;
        trial_dom_freq(ti) = peak_frequency;
        ti = ti + 1;
    end
    alpha_features(index, :) = trial_feature;
    dominant_freqs(index, :) = trial_dom_freq;
    index = index + 1;
end
end