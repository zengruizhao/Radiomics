%% process the clinical data
load clinical_data_train50.mat

%% make them into binary labels

%%% Nstage, N0/N1 vs N2
N_label=zeros(50,1);
for i=1:50   
    if ~isempty(strfind(NStage{i},'2'))
        N_label(i)=1;
    end
end

%%% Tstage, T1/2 vs T3/4
T_label=zeros(50,1);
for i=1:50   
    if ~isempty(strfind(TStage{i},'3')) | ~isempty(strfind(TStage{i},'4'))
        T_label(i)=1;
    end
end

%%% PathHist, WD/MD vs PD
PathHist_label=zeros(50,1);
for i=1:50   
    if strfind(PathHisto{i},'PD')
        PathHist_label(i)=1;
    end
end
%% get label information
labels=[];
% for the 50 training patient ID
set_trainNon=[9 42 47 65 69 98 101 110 117 118 7 36 40 48 76 83 82 81 92 106 50 24 30 79 67];
set_trainPro=[4 35 43 45 56 58 63 75 94 115 19 22 26 38 49 61 70 90 109 114 32 107 80 15 72];

for i=1:50
    if ismember(PatientID(i),set_trainPro)
        labels(i)=1;% sum(labels);
    else
        labels(i)=-1;
    end
end

% sum(labels)
%% train a classifier with the clinical data as features
nIter=10;
data=[N_label T_label PathHist_label];
data=[N_label T_label];% PathHist_label];
stats = nFold_LDA(data,labels,1,10,nIter);
stats = nFold_QDA(data,labels,1,10,nIter);


[~,idxMaxAUC_Im]=max([stats.AUC]);
[~,idxMinAUC_Im]=min([stats.AUC]);
bestResult_Im=stats(idxMaxAUC_Im);
worstResult_Im=stats(idxMinAUC_Im);

figure(1);
plotroc(bestResult_Im.labels,bestResult_Im.prediction,'BaggedC45');
fprintf('The mean performance: mean AUC=%.4f, best AUC =%.4f, worst AUC =%.4f\n',...
    mean([stats.AUC]),bestResult_Im.AUC,worstResult_Im.AUC);
