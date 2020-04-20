function v = itk_corrn_params(data,binname,mask,classes,parloopno)
% ITK_CORRN_PARAMS
%   Run inhomogeneity correction via ITK's BiasCorrector application, while
%   setting good params (as recommended by paper)
%
%   V = ITK_CORRN_PARAMS(DATA) performs MRI bias correction on DATA, returning the
%   corrected image V.
%
%   ITK_CORRN_PARAMS(DATA,BINNAME) uses the binary executable specified by the
%   string BINNAME.  By default, 'BiasCorrector.exe' is called in Windows,
%   and './BiasCorrector' is called in Linux/UNIX.  BINNAME may be a full
%   path name or a path relative to the current folder according to PWD.
%
%   NOTE: Writes MHD and volume to DIRNAME specified by TEMPDIR.
%
%   Can be run on Windows and Linux, provided the binary exists.
%   
%   ITK_CORRN_PARAMS(DATA,BINNAME,MASK) has output mask
%   defined by the logical MASK (should be same size as DATA). 
%   Output MASK will mask out the background (i.e. data that is not
%   in MASK) when applying the calculated bias field. It is RECOMMENDED
%   that this variable be passed in.
%
%   If MASK is not specified, 0 is assumed to be background value, and MASK
%   is accordingly constructed and passed into 'BiasCorrector'
%
%   NOTE: 
%	(1) --use-auto-inputmask is set to YES in calling BiasCorrector, as this will
%   	compute the input mask using the 1% quantile threshold, and mask out
%   	partial volume voxels via an edge filter.
% 	(2) If --write-mask is set to YES, GIPL files for input and output masks are saved
%		in the CURRENT WORKING directory (inmask.gipl, outmask.gipl). There does
%		seem to be any way of controlling their filename, therefore this option
%		is currently NOT set.
%
%   Everything outside DATA.*MASK is considered background during
%   calculation of mean and standard deviation if CLASSES not specified
%
%   ITK_CORRN_PARAMS(DATA,BINNAME,MASK,CLASSES) uses the labels in CLASSES to estimate
%   different class means and sigmas for input to the executable. CLASSES
%   must be the same size as DATA. Has no bearing on MASK. Only really
%   useful if multiple classes are known to exist in the data and accurate
%   class labels are available (e.g. synthetic MNI data).
%
%   ITK_CORRN_PARAMS(DATA,BINNAME,MASK,CLASSES,PARLOOPNO) uses the number
%   in PARLOOPNO to generate filenames, which are then used by MHDWRITE & 
%   VOLWRITE. Only useful if calling ITK_CORRN_PARAMS via a PARFOR loop,
%   where a file name clash is possible.   
%
%	See also: MHDWRITE, VOLWRITE, VOLOPEN, TEMPDIR, MHDREAD.
%
% If not specifying BINNAME, you must cd to the location of the 
% BiasCorrector executable binary before calling this function.
%
%SV, Apr 12 2008; JC, Mar 19 2009
%SV, Jul 2010: Additional switches specified in calling BiasCorrector


% Get a temporary directory 
dirname = tempdir;

data=single(data);
if ~exist('parloopno','var') || isempty(parloopno) %not running via PARFOR
    datafile=[dirname,'data'];
    mhdfile=[dirname,'data.mhd'];
    maskfile=[dirname,'mask'];
    maskmhdfile=[dirname,'mask.mhd'];
    cormhdfile=[dirname,'data-cor.mhd'];
    %inputmaskmhdfile=[dirname,'inputmask.mhd'];
else
    datafile=[dirname,'data-',num2str(parloopno)];
    mhdfile=[dirname,'data-',num2str(parloopno),'.mhd'];
    maskfile=[dirname,'mask-',num2str(parloopno)];
    maskmhdfile=[dirname,'mask-',num2str(parloopno),'.mhd'];
    cormhdfile=[dirname,'data-cor-',num2str(parloopno),'.mhd'];
    %inputmaskmhdfile=[dirname,'inputmask-',num2str(parloopno),'.mhd'];
end    

% Mask for output data
if ~exist('mask','var') || isempty(mask)
    mask = data~=0;
end

% Clean up files left over from previous runs
if exist(datafile,'file'), fprintf('Cleaning.\n'); delete(datafile); end
if exist(mhdfile,'file'), fprintf('Cleaning.\n'); delete(mhdfile); end

% Write the data for the bias corrector binary
volwrite(datafile,data,'single');
mhdwrite([],mhdfile,data);
volwrite(maskfile,uint16(mask),'uint16');
mhdwrite([],maskmhdfile,uint16(mask));

% Default BINNAMEs
if nargin<2 || isempty(binname),
    if ispc,
        binname='BiasCorrector.exe';
    else
        binname='./BiasCorrector';
    end
end

if ~exist('classes','var') || isempty(classes)
    objectmean=mean(data(find(mask))); %#ok<FNDSB>
    objectstd=std(data(find(mask))); %#ok<FNDSB>
else
    for i=1:max(classes(:))
        objectmean(i) = mean(data(classes==i)); %#ok<AGROW>
        objectstd(i) = std(data(classes==i)); %#ok<AGROW>
    end
end

% Run the bias corrector
status = system([binname ' --input ' mhdfile ...
        ' --output ' cormhdfile ' --class-mean ' ...
        num2str(objectmean) ' --class-sigma ' num2str(objectstd)...
        ' --output-mask ' maskmhdfile... %' --input-mask ' inputmaskmhdfile... 
        ' --degree 3 --use-log yes --growth 1.05'...
        ' --volume-max-iteration 1000 --init-step-size 1.02 --slice-direction 2 '...
        ' --use-auto-outputmask no --use-auto-inputmask yes']);

% Clean up input files
delete(datafile);
delete(mhdfile);

% Make sure the bias corrector worked
if status,
    error('command failed');
end

% Open up the corrected data file from the bias corrector
[cordatafile,cordatatype]=mhdread(cormhdfile);
v = volopen([dirname,cordatafile],size(data),cordatatype);

% Clean up output files
delete([dirname,cordatafile]);
delete(cormhdfile);
