This folder contains python scripts used to generate image and sound distractors during experiments.

The audio_distractors.py and visual_distract.py scripts connect to the EEG recording software via TCP/IP and send triggers on distractor generation.

The audio_distractors_noconn.py operates without the TCP/IP connection.

All scripts also dump the distractor generation times in designated text files.