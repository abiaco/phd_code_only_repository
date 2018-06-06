%Adding Channel Locations to Dataset

%Make sure that you change the path to indicate the location of eeglab within your file system

EEG=pop_chanedit(EEG,'lookup', '/Users/etfoo/Documents/MATLAB/eeglab9_0_2_3b/plugins/dipfit2.2/standard_BESA/standard-10-5-cap385.elp');

% Create an EventList, saving a text version in a file named 'elist.txt' in Matlab's current folder
EEG  = pop_creabasiceventlist( EEG , 'Eventlist', 'elist.txt', 'BoundaryNumeric', { -99 } ...
                             , 'BoundaryString', { 'boundary' }, 'Warning', 'on' );
                         
% The first step is to create a file named elist_equations.txt with the following contents:
%   22 "Frequent_Digit"     1     "Frequent Category (Digit)"
%   12 "Rare_Letter"        2     "Rare Category (Letter)"  
%    9 "Corr_Resp"          []    ""      
%    8 "Err_Resp"           []    ""
%
% The following command creates the event list using these equations, using a fully specified path
% A text file with the event list saved in elist.txt in the same path
EEG = pop_editeventlist(EEG,'/Users/etfoo/Desktop/tutorial_script/elist_equations.txt', 'elist.txt', {'boundary'}, {-99});
% The following command uses Matlab's Current Folder
EEG = pop_editeventlist(EEG,'elist_equations.txt', 'elist.txt', {'boundary'}, {-99});
% Copy event labels into the EEG structure
EEG = pop_overwritevent( EEG, 'codelabel');

% Create Bin-Based EEG Epochs
% Extracts bin-based epoch from -200 ms to +800 ms
% 'pre' means use the prestimulus period for baseline correction
% Use 'post' to use the poststimulus period for baseline correction
% Use 'all' to use the poststimulus period for baseline correction
% Use two numbers in quotes (e.g., '-150 +50') to specify a custom interval for baseline correction
EEG = pop_epochbin( EEG , [-200.0  800.0], 'pre');

% Saving after Epoching
% Setname is S1_EEG_elist_be
% Filename is S1_EEG_elist_be.set
% Path is /Users/etfoo/Desktop/tutorial_script/
% Note that you will need to replace the path with the actual location in your file system
EEG.setname = 'S1_EEG_elist_be';
EEG=pop_saveset(EEG, 'filename', 'S1_EEG_elist_be.set', 'filepath', '/Users/etfoo/Desktop/tutorial_script/');

% The following command uses Matlab's Current Folder
EEG.setname = 'S1_EEG_elist_be';

EEG = pop_saveset(EEG, 'filename', 'S1_EEG_elist_be.set');

% Artifact Detection with Moving window peak to peak threshold
% Test period [-200 798]. Voltage threshold 100uV
% Moving window width: 200 ms. Window step: 50. Channels 1:16
% Mark flags 1 and 4 (you must always mark flag 1
 EEG = pop_artmwppth( EEG, 'Channel' , 1:16, 'Flag' , [1 2], 'Threshold', 100 , 'Twindow', [-200798], 'Windowsize' , 200, ' Windowstep' , 50);
% The script-based routine does not show you the data. To see the data, do this:
pop_eegplot(EEG);

% Now we will save the dataset
% Setname is S1_EEG_elist_be_ar
% Filename is S1_EEG_elist_be_ar.set
% Path is /Users/etfoo/Desktop/tutorial_script/
% Note that you will need to replace the path with the actual location in your file system
EEG.setname='S1_EEG_elist_be_ar';
EEG=pop_saveset(EEG, 'filename', 'S1_EEG_elist_be_ar.set', 'filepath', '/Users/etfoo/Desktop/tutorial_script/');

%The following command uses Matlab's Current Folder
EEG.setname='S1_EEG_elist_be_ar';
EEG=pop_saveset(EEG, 'filename', 'S1_EEG_elist_be_ar.set');



%____________________________________________________________________________________________________________________



%filters
EEG  = pop_basicfilter( EEG,  1:20 , 'Boundary', 'boundary', 'Cutoff', [ 1 120], 'Design', 'butter', 'Filter', 'bandpass', 'Order',  6,...
 'RemoveDC', 'on' ); % GUI: 19-Jun-2017 08:23:29

EEG  = pop_basicfilter( EEG,  1:20 , 'Boundary', 'boundary', 'Cutoff',  50, 'Design', 'notch', 'Filter', 'PMnotch', 'Order',  180, 'RemoveDC',...
 'on' ); % GUI: 19-Jun-2017 08:25:00

%eventlist
EEG  = pop_editeventlist( EEG , 'AlphanumericCleaning', 'on', 'BoundaryNumeric', { -99}, 'BoundaryString', { 'boundary' }, 'List',...
 'D:\Projects\Data\EEGLab Datasets\eventlist.txt', 'SendEL2', 'EEG', 'UpdateEEG', 'on', 'Warning', 'on' ); % GUI: 19-Jun-2017 08:28:19

%bin based epochs
EEG = pop_epochbin( EEG , [-200.0  2000.0],  'pre'); % GUI: 19-Jun-2017 08:29:50
