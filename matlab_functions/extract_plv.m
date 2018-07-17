function [plv] = extract_plv(trialset, channel_no, num_trials, trial_size)

% first re-format the data from channel x trial 
% to channel x trialsize(ms) x num_trials

reform_data = zeros(channel_no, trial_size, num_trials);
for i=0: num_trials
    reform_data(:, :, 1i) = trialset(channel_no, 1i*trial_size : 1i*trial_size + trial_size);
end

disp(reform_data);

plv = [];

plv1 = pn_eegPLV(reform_data, 500, [250, [2, 4]]);

plv2 = pn_eegPLV(reform_data, 500, [250, [5, 7]]);

plv3 = pn_eegPLV(reform_data, 500, [250, [8, 15]]);

plv4 = pn_eegPLV(reform_data, 500, [250, [16, 31]]);

plv5 = pn_eegPLV(reform_data, 500, [250, [32, 49]]);

plv6 = pn_eegPLV(reform_data, 500, [250, [50, 120]]);

append (plv, plv1, plv2, plv3, plv4, plv5, plv6);


end