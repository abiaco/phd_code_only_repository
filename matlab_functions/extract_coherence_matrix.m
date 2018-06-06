function [ coherence_features ] = extract_coherence_matrix( trial_set, trial_no )
%EXTRACT_BANDPOWER Summary of this function goes here
%   Detailed explanation goes here

freq_vector = [];
fs = 500;
for i=1:120
    tmp = [freq_vector, i];
    freq_vector = tmp;
end

coherence_features = zeros(trial_no, 171*120);
% N(N+1)/2 === 19(19+1)/2 = 190 - excluding duplicate pairs

for i=1:trial_no
    coherence_mat = zeros(1, 171*120);
    index = 1;
    disp('trial: ');
    disp(i);
    disp('');
    for ch1=1:19
        coherence_ch1_ch2 = zeros(1, (19-ch1)*120);
        ind = 1;
        for ch2=(ch1+1):19
            [c12] = mscohere(trial_set(i, :, ch1), trial_set(i, :, ch2), hamming(500), 250, freq_vector, fs);
            coherence_ch1_ch2(ind : ind+119) = c12;
            ind = ind + 120;
        end
        
        coherence_mat(index : index+((19-ch1)*120 -1)) = coherence_ch1_ch2(:);
        index = index + (19-ch1)*120;
    end
    coherence_features(i,:) = coherence_mat;
end

