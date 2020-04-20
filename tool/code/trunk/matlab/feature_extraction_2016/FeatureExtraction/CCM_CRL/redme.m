%% ON 21 Nov. 2015, get the annotation from Dr. Lewis
%%% we have in total 118 cases/cores

% Regarding Oral Cavity, here are the annotations below.?
% Each case has between 2 and 4 cores on the array.? 
% You just need to analyze the single cores that have numbers on them and nothing else.? 
% I am attaching a list of ?ground truth? progressors vs not? for your modeling.? Only 40 cases, as Anant had advised.

%  below are the Code for 40 cases out of the 120 overall.
% Data below is 20 progressors and 20 non-progressors for you to use to begin modeling the classifier. 
% Then we can test it on the additional 80 patients.

% Non-Progressors
% 9-
% 42-
% 47-
% 65-
% 69-
% 98-
% 101-
% 110-
% 117-
% 118-
% 7-
% 36-
% 40-
% 48-
% 76-
% 83-
% 82-
% 81-
% 92-
% 106-
% 
% progressors
% 4
% 35-
% 43-
% 45-
% 56-
% 58-
% 63-
% 75-
% 94-
% 115-
% 19-
% 22-
% 26-
% 38-
% 49-
% 61-
% 70-
% 90-
% 109-
% 114-

% Dr. Lewis add 10 more cases for the training set.
% for the 50 training
% set_trainNon=[9 42 47 65 69 98 101 110 117 118 7 36 40 48 76 83 82 81 92 106 50 24 30 79 67];
% set_trainPro=[4 35 43 45 56 58 63 75 94 115 19 22 26 38 49 61 70 90 109 114 32 107 80 15 72];
% 
% set_train=[set_trainNon set_trainPro];

%% to get feature data use nucleiseg_nucleiclusterfinding_extractfullfeature_OroCavity_v4.m
%% to organize and analysis the data use LFeatureAnalysis_get_usefull_feature_build_classifier.m
%% to visulize features use Lvisual_features.m