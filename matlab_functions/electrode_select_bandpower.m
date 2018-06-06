function [ bandpower_features, dominant_freqs ] = electrode_select_bandpower( trial_set, trial_no, electrodes )
%EXTRACT_BANDPOWER Summary of this function goes here
%   Detailed explanation goes here

bandpower_features = zeros(trial_no, 19* 6);
dominant_freqs_per_band = zeros(trial_no, 19*6);
index = 1;
for i=1:trial_no
    trial_feature = zeros(1, 19*6);
    trial_dominant_freqs = zeros(1, 19*6);
    ti = 1;
    for j=1:19
        features = zeros(1, 6);
        dominant_freqs = zeros(1, 6);
        channel = trial_set(i, : ,j);
        [pxx1, f1] = pwelch(channel,[],[], [2 4], 500, 'power');
        [pxx2, f2] = pwelch(channel,[],[], [5 7], 500, 'power');
        [pxx3, f3] = pwelch(channel,[],[], [8 15], 500, 'power');
        [pxx4, f4] = pwelch(channel,[],[], [16 31], 500, 'power');
        [pxx5, f5] = pwelch(channel,[],[], [32, 49], 500, 'power');
        [pxx6, f6] = pwelch(channel,[],[], [50 120], 500, 'power');
        features(1) = sum(pxx1);
        features(2) = sum(pxx2);
        features(3) = sum(pxx3);
        features(4) = sum(pxx4);
        features(5) = sum(pxx5);
        features(6) = sum(pxx6);
        trial_feature(ti:ti+5) = features;
        
        [peak_amplitude1, peak_index1] = max(pxx1);
        peak_frequency1 = f1(peak_index1);
        
        [peak_amplitude2, peak_index2] = max(pxx2);
        peak_frequency2 = f2(peak_index2);
        
        [peak_amplitude3, peak_index3] = max(pxx3);
        peak_frequency3 = f3(peak_index3);
        
        [peak_amplitude4, peak_index4] = max(pxx4);
        peak_frequency4 = f4(peak_index4);
        
        [peak_amplitude5, peak_index5] = max(pxx5);
        peak_frequency5 = f5(peak_index5);
        
        [peak_amplitude6, peak_index6] = max(pxx6);
        peak_frequency6 = f6(peak_index6);
        
        dominant_freqs(1) = peak_frequency1;
        dominant_freqs(2) = peak_frequency2;
        dominant_freqs(3) = peak_frequency3;
        dominant_freqs(4) = peak_frequency4;
        dominant_freqs(5) = peak_frequency5;
        dominant_freqs(6) = peak_frequency6;
        trial_dominant_freqs(ti:ti+5) = dominant_freqs;
        
        ti = ti + 6;
    end
    bandpower_features(index, :) = trial_feature;
    dominant_freqs_per_band(index, :) = trial_dominant_freqs;
    
    index = index + 1;
end
end

