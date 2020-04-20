clear;clc;
addpath(genpath('./tool/feature_script'));
addpath(genpath('./tool/general'));
addpath(genpath('./tool/code/trunk/matlab/preprocessing/biascorrection'));
addpath(genpath('./tool/code/trunk/matlab/general'));
% addpath(genpath('../CT'));
path = '/media/zzr/My Passport/newMRI/MatRaw/';
% path = 'G:\430\MRI\MRI_PNET_Mat_overall\';
case_file = dir(path);
for i=4
    fprintf('%d case: %s...\n', i-2, case_file(i).name);
    phase = dir([path case_file(i).name '/*.mat']);
    for j=1:size(phase, 1)
        if contains(phase(j).name(1:end-4), 'T2')
            phase_path = [path case_file(i).name '/' phase(j).name];
            load (phase_path)
            [corrvol,lpfvol,sigma] = lpfbiascorr(img);
            img = corrvol;
%             [corrvol,lpfvol,sigma,meanlpfvals] = lpfbiascorr3(img);
            resultPath = ['/media/zzr/My Passport/newMRI/BiasFieldCorrection/' case_file(i).name];
            if ~exist(resultPath, 'dir');mkdir(resultPath);end
            save([resultPath '/T2.mat'], 'img', 'label');
        end

    end
end