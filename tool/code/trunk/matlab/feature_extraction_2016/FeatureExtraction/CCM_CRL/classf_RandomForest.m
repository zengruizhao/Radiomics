% function err = classf_LDA_cheng(xtrain,ytrain,xtest,ytest)

function Yfit = classf_RandomForest(training_set , testing_set, training_labels,testing_labels)

% format labels from (-1,1) to (0,1)
% if training_labels < 0
%     training_labels(training_labels==-1) = 0;
%     testing_labels(testing_labels==-1) = 0;
% end

unq_tra_lab = unique(training_labels);
if numel(unq_tra_lab) ~= 2
    error('Only 2 labels allowed');
else
    idx1 = ismember(training_labels,unq_tra_lab(1));
    idx2 = ismember(training_labels,unq_tra_lab(2));
    training_labels(idx1) = 0;
    training_labels(idx2) = 1;
    idx1 = ismember(testing_labels,unq_tra_lab(1));
    idx2 = ismember(testing_labels,unq_tra_lab(2));
    testing_labels(idx1) = 0;
    testing_labels(idx2) = 1;
end

methodstring = 'BaggedC45';

options = statset('UseParallel','never','UseSubstreams','never');

B = TreeBagger(50,training_set,training_labels,'FBoot',0.667,'oobpred','on','Method','classification','NVarToSample','all','NPrint',4,'Options',options);    % create bagged d-tree classifiers from training
[Yfit,Scores] = predict(B,testing_set);   % use to classify testing
