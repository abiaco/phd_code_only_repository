This is the code used for the simple GUI that links the various parts of the project together and ensures the synchronisation of all data collection parts.

It is written in QT C++ and uses TCP sockets to connect to the key/mouse loggers, sound/image distractor generators and the EEG recording software. This serves to synchronize the data collection process and enables software triggers to be recorded on the EEG timeline.

TCP sockets have a latency of approx. 2ms on localhost - making them suitable for this task.