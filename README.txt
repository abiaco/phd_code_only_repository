This repository contains most of the code files used in the my phd project currently.

The project is an attempt at indentifying and perhaps quantifying the state of immersion / presence / perhaps even cognitive flow during gameplay, using Brain-Computer Interfaces (EEG) and gameplay metrics.

Each folder has its own readme file explaining the purpose of the files within. An overview of the contents of each folder would be:
"Data Analysis" - contains C# code for statistics and machine learning.
"Loggers" - contains C++ code for a key and mouse event logger, as well as a TCP eeg recorder using the Neuroelectrics Enobio 20 device and NIC EEG recording software.
"Logs" - is a sample directory structure for the various loggers used in the project.
"Python Distractors" - contains Python scripts that generate audio and visual distractors and send TCP triggers to the aforementioned NIC software.
"python_utils" - contains a few Python utility scripts for log analysis.
"RecordGUI" - contains QT C++ code for a simple GUI that uses TCP sockets and buttons in order to connect and synchronize the various parts of the project together.
"Utils" - contains C++ utility classes and files for the different data loggers.