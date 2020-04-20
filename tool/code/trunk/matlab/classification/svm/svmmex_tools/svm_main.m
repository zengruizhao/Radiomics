function [outputs,params] = svm_main(training_data,training_labels,testing_data,testing_labels,varargin)
% USAGE: [outputs,params] = svm_main(training_data, training_labels, testing_data, testing_labels, varargin);
% 
% This function performs basic SVM classification suitable for general lab 
% use.  It uses the mex versions of the latest LIBSVM-MAT code.  Please make 
% sure that the ENTIRE dataset has been scaled (zero mean, unit std deviation) 
% via the 'svm_scale' function before passing training and testing subsets 
% into 'svm_main'.
% 
% MANDATORY INPUTS:
%   training_data       [N x P double] Training data containing N observations and P features.
%   training_labels     [N x 1 double] Labels for N observations.
%   testing_data        [M x P double] Testing data containing M observations and P features.
%   testing_labels      [M x 1 double] Labels for M observations.
%
% SEMI-OPTIONAL INPUTS: Defaults will be used if this argument is left blank or empty. Otherwise,you pass a single structure containing the following:
%   params.debug        Whether or not you wish to spit out debugging messages (1 or 0, DEFAULT = 0)
%   params.kernel       The type of kernel to use for the SVM training. Possible values are:
%                               0: Linear kernel (DEFAULT)
%                               1: Polynomial kernel
%                               2: Radial Basis Function kernel
%                               3: Sigmoid kernel
%   params.c_range            Vector of potential values for the 'c' parameter of the SVM.  
%                             See params.g below for more details.
%   params.g_range            Vector of potential values for the 'g' parameter of the SVM.
%                             The actual values evaluated are 2.^(params.g). 
%                              Basically, if you're doing a bunch of cross-validation experiments 
%                              on large data and you don't want to re-compute the 
%                              optimal c and g each time, you can pass them in as 
%                              parameters here. This can save quite a bit of time if
%                              the SVM is taking super-long. If one or both are
%                              not given, they are calculated.
%							   p.s. Linear SVMs do not take a 'g' parameter, so maybe set 
%							   'g_range' to 1 (though the code is set to do this as well)
%   params.cvfolds      The number of folds to do cross-validation when
%                       training the SVM (DEFAULT = 3). 
%   params.svm_model	If you would like to use an existing SVM model, please include it here.
%   params.cachesize    LIBSVM uses an efficient memory optimization called
%                       SMO. You can set the cachesize here as a trade-off
%                       between holding those many elements in memory
%                       (using more memory is faster) and memory
%                       constraints (in the end, there are only so many
%                       elements you can store in memory). (DEFAULT = 1000)
%   params.epsilon      Can be used to relax the tolerance of optimization.
%                       Is useful if you want a quicker, albeit possible
%                       less accurate, solution for large-memory problems
%                       (DEFAULT = 0.001)
%
% NOTE: 
% (1) Shrinking heuristics in svmtrain are turned on by default in this
% function for speed/memory improvements. If you are not sure what this is,
% ignore it. If you want to know more, please read the LIBSVM website for
% detailed info on how shrinking is done and what its effects might be.
% TL; DR: Shrinking heuristics allow for quicker SVM running with (supposedly) no major
% effects on classification accuracy.
% (2) 'cachesize' and 'epsilon' appear to be exact parallels to
% 'kernelcachelimit' and 'tolkkt' in MATLAB's own 'svmtrain'.
% You might want to check that out for additional explanation as well.
%
%   
% OUTPUTS:The outputs consist of two structures, 'outputs' and 'params'
%   outputs.c                   The 'c' parameter used to generate the SVM model.
%   outputs.g                   The 'g' parameter used to generate the SVM model
%   outputs.predicted_labels    The classification labels of the testing data
%   outputs.prob_estimates      The probability of classification (surrogate for distance to support vectors)
%   outputs.tp                  True Positives
%   outputs.tn                  True Negatives
%   outputs.fp                  False Positives
%   outputs.fn                  False Negativesparams.epsilon = 0.001; %default in svmtrain
%   outputs.acc                 Accuracy
%   outputs.svm_model           Model of the SVM, Structure
%       outputs.svm_model.Parameters              ??
%       outputs.svm_model.nr_class                Number of classes
%       outputs.svm_model.totalSV                 Number of Support Vectors, i.e. size(SVs,1)
%       outputs.svm_model.rho                     ??
%       outputs.svm_model.Label                   Positive and Negative labels
%       outputs.svm_model.ProbA                   ??
%       outputs.svm_model.ProbB                   ??
%       outputs.svm_model.nSV                     Number of support vectors per class (in case one class has more SVs than another)
%       outputs.svm_model.sv_coef                 Coefficients of the support vectors
%       outputs.svm_model.SVs                     Actual coordinates of the SVs 
%   params                      The 'params' structure is identical to the
%                               optional input variable; any parameters left out 
%                               are included with appropriate default values.
%
% Original: Ajay Basavanhally, May 2010
% Updated: Satish Viswanath, Jul 2012
%
% See also: SVMTRAIN, SVMCLASSIFY, PREDICTSVM


%% preprocessing
% check existing parameters
switch nargin
    case 4
        warning('trainsvm:UsingDefaultParams','SVM parameters not specified.  Defaults will be used.  Type ''help svm_main'' for more info.');
        [params,flags] = set_defaults();
    case 5
        if isempty(varargin{1})
            warning('trainsvm:UsingDefaultParams','SVM parameters not specified.  Defaults will be used.  Type ''help svm_main'' for more info.');
            [params,flags] = set_defaults();
        elseif ~isstruct(varargin{1})
            error('Argument containing parameters must be formatted as a struct.  Type ''help svm_main'' for more info.');
        else
            [params,flags] = parse_inputs(varargin{1});
        end
    otherwise
        error('Too many inputs.  Argument containing parameters must be formatted as a struct.  Type ''help svm_main'' for more info.');
end

if ~logical(exist('trainsvm','file'))
    error('TRAINSVM not in your path. Please add ''svm'' folder to path and retry.');
end

% select kernel
switch lower(params.kernel)
    case {0,'linear'}
        params.kernel = 0;
		% todo: switch to using liblinear for linear SVMs?
    case {1,'poly','polynomial'}
        params.kernel = 1;
    case {2,'rbf'}
        params.kernel = 2;
    case {3,'sigmoid'}
        params.kernel = 3;
    otherwise
        params.kernel = 0;
        warning('trainsvm:UsingDefaultKernel','Invalid kernel specified. Defaulting to ''linear''.');
end

% set debug switch
if params.debug
    quietswitch = '';
else
    quietswitch = ' -q';
end

% fix labels to 1 and -1; restructure data so that "1" comes first
[reorg,orig_idx_train,orig_idx_test] = fixlabels(training_labels,testing_labels);
training_data = [training_data(reorg.idx_train_high,:); training_data(reorg.idx_train_low,:)];
testing_data = [testing_data(reorg.idx_test_high,:); testing_data(reorg.idx_test_low,:)];
training_labels = [ones(nnz(reorg.idx_train_high),1); -ones(nnz(reorg.idx_train_low),1)];
testing_labels = [ones(nnz(reorg.idx_test_high),1); -ones(nnz(reorg.idx_test_low),1)];


%% find optimal SVM parameters c and gamma
if flags.do_training && flags.do_svmparam
    best_accuracy = -1;
    outputs.c = -1;
    outputs.g = -1;
	
    if isempty(params.c_range) % defaults
        warning('trainsvm:UsingDefaultCRange','No range for ''C'' specified. Using default: [-5:2:10].');
        params.c_range = -5:2:10;
    end
    if isempty(params.g_range) && ~(params.kernel == 0)
        warning('trainsvm:UsingDefaultGRange','No range for g specified. Using default: [-10:2:3].');
        params.g_range = -10:2:3;
    elseif isempty(params.g_range) && (params.kernel == 0)
		warning('trainsvm:UsingNoGRange','Linear SVM, no range for g set.');
		params.g_range = 0;
    end
    
    %if length(training_labels) > 5000
    %    warning('trainsvm:TooManyTrainSamples','Too many training samples, downsampling to estimate C,g parameters');        
    %end

    % cross-validation for parameter search
	paramcomb = 0;
    for c = params.c_range
        for g = params.g_range            
			paramcomb = paramcomb+1;
            svm_model = trainsvm(training_labels, training_data,['-t ',num2str(params.kernel),' -g ',num2str(2^g),' -c ',num2str(2^c),' -m ',num2str(params.cachesize),' -e ',num2str(params.epsilon),' -h 1 -b 0',' -v ',num2str(params.cvfolds),quietswitch]);            

            if( svm_model > best_accuracy )
                outputs.c = c;
                outputs.g = g;
                best_accuracy = svm_model;
            end
			fprintf('\nDone with %d of %d parameter combinations.',paramcomb,length(params.c_range)*length(params.g_range));
        end
    end
    if outputs.c == min(params.c_range), fprintf('Warning: Smallest ''c'' parameter used.  You may need to adjust ''params.c_range''\n'); end
    if outputs.c == max(params.c_range), fprintf('Warning: Largest ''c'' parameter used.  You may need to adjust ''params.c_range''\n'); end
    if outputs.g == min(params.g_range), fprintf('Warning: Smallest ''g'' parameter used.  You may need to adjust ''params.g_range''\n'); end
    if outputs.g == max(params.g_range), fprintf('Warning: Largest ''g'' parameter used.  You may need to adjust ''params.g_range''\n'); end
	
elseif flags.do_training && ~flags.do_svmparam
    outputs.c = params.c_range(1);
    outputs.g = params.g_range(1);
else
    outputs.c = [];
    outputs.g = [];
end

%% train SVM
if flags.do_training
    svm_model = trainsvm(training_labels,training_data,['-t ',num2str(params.kernel),' -g ',num2str(2^outputs.g),' -c ',num2str(2^outputs.c),' -m ',num2str(params.cachesize),' -e ',num2str(params.epsilon),' -h 1 -b 1',quietswitch]);
else
    svm_model = params.svm_model;
end

%% evaluate SVM
[predicted_labels,~,prob_estimates] = predictsvm(testing_labels,testing_data,svm_model,'-b 1');

%%% everything needs to be converted back into the original index
% below is the original incorrect code
% training_data = training_data(orig_idx_train,:);
% training_labels = training_labels(orig_idx_train);
% testing_data = testing_data(orig_idx_test,:);
% testing_labels = testing_labels(orig_idx_test);
% predicted_labels = predicted_labels(orig_idx_test);
% prob_estimates = prob_estimates(orig_idx_test,:);

% this code will actually convert to original index
testing_labels(orig_idx_test) = testing_labels;
predicted_labels(orig_idx_test) = predicted_labels;
prob_estimates(orig_idx_test,:) = prob_estimates;

% calculate results
trues = testing_labels == predicted_labels;
positives = predicted_labels == 1;

%% outputs
outputs.predicted_labels = predicted_labels;
outputs.prob_estimates = prob_estimates;
outputs.tp = nnz(trues & positives);
outputs.tn = nnz(trues & ~positives);
outputs.fp = nnz(~trues & positives);
outputs.fn = nnz(~trues & ~positives);
outputs.acc = (outputs.tp + outputs.tn) / (outputs.tp + outputs.tn + outputs.fp + outputs.fn);
outputs.svm_model = svm_model;





function [params flags] = parse_inputs(a)
params = a;
if ~isfield(params,'debug')
    params.debug = false;
end
if ~isfield(params,'kernel')
    params.kernel = 0;
    fprintf('Warning: Kernel not specified. Defaulting to ''linear''.\n');
end
if ~isfield(params,'c_range')
    flags.do_svmparam = true;
    params.c_range=[];
end
if ~isfield(params,'g_range')
    flags.do_svmparam = true;
    params.g_range=[];
end
if numel(params.c_range)==1 && numel(params.g_range)==1
    flags.do_svmparam = false;
else
    flags.do_svmparam = true;
end
if ~isfield(params,'cvfolds')
    params.cvfolds = 3;
end
if ~isfield(params,'cachesize')
    params.cachesize = 1000; %slightly higher than default in svmtrain
end
if ~isfield(params,'epsilon')
    params.epsilon = 0.001; %default in svmtrain
end
if ~isfield(params,'svm_model')
    flags.do_training = true;
else
    flags.do_training = false;
end


function [params flags] = set_defaults()
params.kernel = 0;
params.cvfolds = 3;
params.debug = false;
params.c_range = [];
params.g_range = [];
params.cachesize = 1000; %slightly higher than default in svmtrain
params.epsilon = 0.001;  %default in svmtrain
flags.do_svmparam = true;
flags.do_training = true;


function [a orig_idx_train orig_idx_test] = fixlabels(training_labels,testing_labels)
% check label data type and # of unique classes
if any(~islogical(training_labels) & ~isnumeric(training_labels)) 
    error('Training labels must be logical or numeric.');
else
    training_labels=double(training_labels(:));
end
if any(~islogical(testing_labels) & ~isnumeric(testing_labels))
    error('Testing labels must be logical or numeric.');
else
    testing_labels=double(testing_labels(:));
end
unique_labels = unique([training_labels; testing_labels]);
if numel(unique_labels) ~= 2
    error('Labels must contain 2 unique classes.');
end
if numel(unique(training_labels)) ~= 2 % must train on both classes; doesn't matter for testing
    error('Training labels must contain samples from both classes.');
end

labels = [training_labels; testing_labels];
unique_labels = unique(labels);

a.idx_train_high = training_labels == unique_labels(2);
a.idx_train_low = training_labels == unique_labels(1);
orig_idx_train = [find(a.idx_train_high); find(a.idx_train_low)];

a.idx_test_high = testing_labels == unique_labels(2);
a.idx_test_low = testing_labels == unique_labels(1);
orig_idx_test = [find(a.idx_test_high); find(a.idx_test_low)];
