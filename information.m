clear;clc;
load ./data/T2Information
Information = cell(size(data, 1), 7);
for i=1:size(data, 1)
    case_file = data(i, :);
    S = regexp(case_file, '+', 'split');
    casename = regexp(S{1}, '_', 'split');
    casename = casename{1};
    if ~isnan(str2double(casename));casename = num2str(str2double(casename));end
    spacing = str2double(S{2});
    row = str2double(S{3});
    column = str2double(S{4});
    manufacturer = S{5};
    manufacturer(logical(isspace(manufacturer)))=[];
    slicethickness = str2double(S{6});
    slicespacing = str2double(S{7});
    
    Information{i, 1} = casename;
    Information{i, 2} = spacing;
    Information{i, 3} = row;
    Information{i, 4} = column;
    Information{i, 5} = manufacturer;
    Information{i, 6} = slicethickness;
    Information{i, 7} = slicespacing;
end
x_siemens =ind2sub(size(Information(:, 5)),find(cellfun(@(x)strcmp(x,'SIEMENS'),Information(:, 5))));
x_ge =ind2sub(size(Information(:, 5)),find(cellfun(@(x)strcmp(x,'GEMEDICALSYSTEMS'),Information(:, 5))));
Siemens_Information = Information(x_siemens, :);
Ge_Information = Information(x_ge, :);
save('./MRI/T2InformationAll.mat', 'Information');
%%
% spacing = cell2mat(Information(:, 2));
% row = cell2mat(Information(:, 3));
% column = cell2mat(Information(:, 4));
% slicethickness = cell2mat(Information(:, 6));
% slicespacing = cell2mat(Information(:, 7));
% manufacturer = Information(:, 5);