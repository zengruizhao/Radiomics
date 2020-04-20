function [dicomdirdata,nondicomfiles,runningfilecount,nnondicoms,drootname,filerate,Hwb]=dicomdir(directoryname,forcedpatientname,filemask,recursiveparams)
% DICOMDIR
%   Read all files in a directory, searching and recording DICOM files.
%
%   [DICOMDIRDATA,NONDICOMFILES]=DICOMDIR will recursively scan a directory
%   tree and return information about DICOM and non-DICOM files in to the
%   structures DICOMDIRDATA and NONDICOMFILES, respectively.  The root
%   folder in which the scan will begin is selected using UIGETDIR.
%
%   [DICOMDIRDATA,NONDICOMFILES]=DICOMDIR(DIRECTORYNAME) specifies the root
%   folder in which to scan.
%
%   [DICOMDIRDATA,NONDICOMFILES]=DICOMDIR(DIRECTORYNAME,FORCEDPATIENTNAME)
%   forces the PatientName.FamilyName field to the string FORCEDPATIENTNAME.
%   If only PatientName exists (no FamilyName field) it is set instead.
%
%   Other input and output arguments are intended to facilitate recursive
%   calls to DICOMDIR.
%
%   TIP: Use dcmdictshort.txt with DICOMDICT, instead of the default.
%
%   See also: PARSEDICOMDIRDATA, STRUNIQUE, DICOMINFO, DICOMREAD, DICOMDICT.
%
%JC

% currdicomdict=dicomdict('get');
% dicomdict('set','k:\code\images\dcmdictshort.txt');

if nargin==0,
    drootname = uigetdir('.','Choose a DICOM root directory');
    directoryname = drootname; atroot=true;
    forcedpatientname=''; filemask=[];
elseif nargin==1;
    forcedpatientname=''; filemask=[];
    drootname=directoryname; atroot=true;
elseif nargin<4;
    drootname=directoryname; atroot=true;
    if nargin<3, filemask=[]; end
elseif nargin==4, % recuvsive call, parse auxiliary input struct
    [drootname, Hwb, rootfilecount, runningfilecount, nnondicoms, lastrate] = deal ...
        (recursiveparams.drootname, recursiveparams.Hwb, recursiveparams.rootfilecount,...
        recursiveparams.runningfilecount, recursiveparams.nnondicoms, recursiveparams.lastrate);
    atroot=false;
end
if isempty(drootname) || isequal(drootname,0), return; end

if ~ischar(forcedpatientname), forcedpatientname=char(forcedpatientname); end

% If at root directory, initialize stuff
if atroot, %strcmp(directoryname,drootname),
    % Count all files
    % consider doing "dir /A:-D-L /S /B | wc -l" in Windows, or in LINUX
    % doing "find . -type f | wc -l" instead of the MATLAB-only approach:
    fprintf('Counting all files under %s...      ',directoryname);
    rootfilecount=recursivefilecount(directoryname);
    fprintf(repmat('\b',1,5));
    fprintf('found %g files.\n',rootfilecount);
    % Initialize file counters to zero
    runningfilecount=0;
    nnondicoms=0;
    % Init timing
    lastrate=0;
    % Create waitbar
    Hwb = waitbar(0,sprintf('File 0 of %g',rootfilecount),'Name','Reading DICOM tags');
    set(findall(Hwb,'Interpreter','tex'),'Interpreter','none');
    % Fix figure
    wbpos=get(Hwb,'position');
    wbpos=wbpos+[0 0 225 5]; set(Hwb,'position',wbpos);
    % Fix bar
    hwbchild=get(Hwb,'children');
    wbchildpos=get(hwbchild,'position');
    wbchildpos(3)=wbpos(3)-30;
    set(get(Hwb,'children'),'position',wbchildpos);
    % Fix text
    htext=findall(Hwb,'String','waitbarstring');
    set(htext,'units',get(hwbchild,'units'));
    tpos=get(htext,'Position'); tpos(1)=wbchildpos(1)-10;
    set(htext,'HorizontalAlignment','left','Position',tpos,'Interpreter','none');
    drawnow
end

% Timing
tic;
initrunningfilecount=runningfilecount;
% currentweight=.1;
% lastweight=.9;

dicomdirdata=struct('OriginalFileName',{},'OriginalFileLocation',{},...
    'PatientName',{},'PatientID',{},'InstanceNumber',{},...
    'Width',{},'Height',{},'StudyDescription',{},...
    'SeriesDescription',{},'StudyDate',{},'StudyID',{},'StudyInstanceUID',{},...
    'SeriesNumber',{},'SeriesDate',{},'SeriesInstanceUID',{},...
    'SliceThickness',{},'SpacingBetweenSlices',{},'RepetitionTime',{},...
    'EchoTime',{},'NumberOfAverages',{},'ImagePositionPatient',{},...
    'ImageOrientationPatient',{},'FrameOfReferenceUID',{},...
    'AcquisitionMatrix',{},'SliceLocation',{},'Rows',{},'Columns',{},...
    'PixelSpacing',{},'ReferencedPatientSequence',{},'ReferencedStudySequence',{},...
    'PatientClassUID_hash',{},'StudyClassUID_hash',{},...
    'StudyInstanceUID_hash',{},'SeriesInstanceUID_hash',{});
dcmfields=fieldnames(dicomdirdata);
% filedata=[];
nondicomfiles=struct('OriginalFileName',{},'OriginalFileLocation',{});
% runningfilecount=0;

% fprintf('Checking folder %s for files and subfolders.\n',directoryname);

dcurrent=dir(directoryname);

[files,subdirs]=checkdir(dcurrent);

% Go into subdirectories
subdirinds=find(subdirs);
nsubdirs=numel(subdirinds);

for i=1:nsubdirs,
    nextsubfolder=fullfile(directoryname,dcurrent(subdirinds(i)).name);
    % fprintf('Enteing folder %s.\n',nextsubfolder);
    %[subdicomdirdata,subnondicomfiles]=dicomdir(nextsubfolder,drootname);
    [recursiveparams.drootname, recursiveparams.Hwb, recursiveparams.rootfilecount,...
        recursiveparams.runningfilecount, recursiveparams.nnondicoms, recursiveparams.lastrate] = deal ...
        (drootname, Hwb, rootfilecount, runningfilecount, nnondicoms, lastrate);
    [subdicomdirdata,subnondicomfiles,runningfilecount,nnondicoms]=dicomdir(nextsubfolder,forcedpatientname,filemask,recursiveparams);
    dicomdirdata=[dicomdirdata subdicomdirdata];
    nondicomfiles=[nondicomfiles subnondicomfiles];
end

% Then do files

currentfileinds=find(files);
nfiles=numel(currentfileinds);

% if nfiles>0, fprintf(' Loading %g files.',nfiles); end
for i=1:nfiles,
    % fprintf('.');
    try
        df=dir(fullfile(directoryname,dcurrent(currentfileinds(i)).name));
        if df.bytes<1,
            ME = MException('DICOMFileCheck:ZeroByteFile','Empty file found.');
            throw(ME);
        elseif ~isempty(filemask) && isempty(regexp(df.name, ['^' regexptranslate('wildcard',filemask) '$'], 'once')),
            ME = MException('DICOMFileCheck:NotMatchToMask','File does not match mask.');
            throw(ME);
        else
            dicomdirdata(end+1)=getdicomdata(directoryname,dcurrent(currentfileinds(i)).name,drootname,dcmfields);
            if ~isempty(forcedpatientname),
                if isfield(dicomdirdata(end).PatientName,'FamilyName'),
                    dicomdirdata(end).PatientName.FamilyName=forcedpatientname;
                    dicomdirdata(end).PatientName.GivenName='';
                    dicomdirdata(end).PatientName.MiddleName='';
                else
                    dicomdirdata(end).PatientName=forcedpatientname;
                end
            end
            runningfilecount=runningfilecount+1;
        end
    catch ME
        nnondicoms=nnondicoms+1;
        idSegLast = regexp(ME.identifier, '(?<=:)\w+$','match');
        switch idSegLast{1},
            case 'heterogenousStrucAssignment'
                error('DICOM file \"%s\" lacks correct/correctly-ordered tags.',...
                    fullfile(directoryname,dcurrent(currentfileinds(i)).name));
            case {'notDICOM', 'ZeroByteFile'}
                fprintf('Non-DICOM file \"%s\" encountered, recorded and skipped.\n',...
                    fullfile(directoryname,dcurrent(currentfileinds(i)).name));
                nondicomfiles(end+1)=struct('OriginalFileName'...
                    ,dcurrent(currentfileinds(i)).name,'OriginalFileLocation',directoryname);
            case 'NotMatchToMask'
                fprintf('File \"%s\" does not match specified file mask.\n',...
                    fullfile(directoryname,dcurrent(currentfileinds(i)).name));
                nondicomfiles(end+1)=struct('OriginalFileName'...
                    ,dcurrent(currentfileinds(i)).name,'OriginalFileLocation',directoryname);
            case 'DICOMDIRFileEncountered'
                fprintf('DICOMDIR (non-image) file \"%s\" encountered, recorded and skipped.\n',...
                    fullfile(directoryname,dcurrent(currentfileinds(i)).name));
                nondicomfiles(end+1)=struct('OriginalFileName'...
                    ,dcurrent(currentfileinds(i)).name,'OriginalFileLocation',directoryname);
            case 'NonImageDICOMFile'
                fprintf('Non-Image DICOM file \"%s\" encountered, recorded and skipped.\n',...
                    fullfile(directoryname,dcurrent(currentfileinds(i)).name));
                nondicomfiles(end+1)=struct('OriginalFileName'...
                    ,dcurrent(currentfileinds(i)).name,'OriginalFileLocation',directoryname);
            otherwise
                fprintf('Unknown error on inspection of file %s:',...
                    fullfile(directoryname,dcurrent(currentfileinds(i)).name));
                rethrow(ME);
        end
    end
    filestime=toc;
    currentrate=(runningfilecount-initrunningfilecount)/filestime;
    if lastrate==0 || isinf(lastrate) || isnan(lastrate) , lastrate=currentrate; end
    pctdone=runningfilecount/(rootfilecount-nnondicoms);
    currentweight=1-pctdone; lastweight=pctdone;
    filerate=sum([currentweight lastweight] .* [currentrate lastrate]);  % IIR
    remaintime=(rootfilecount-nnondicoms-runningfilecount)./filerate;
    remaintime=5*round(remaintime/5);
    rtmin=floor(remaintime/60); rtsec=mod(remaintime,60);
%     waitbar(runningfilecount/(rootfilecount-nnondicoms),Hwb,...
%         sprintf('File %g of %g (%g err), Rate: %g files/sec\n%s',runningfilecount,rootfilecount-nnondicoms,nnondicoms,filerate,directoryname));
    waitbar(pctdone,Hwb,...
        sprintf('File %g of %g (%g exceptions), Remain: %gmin %.0fsec\n%s',...
        runningfilecount,rootfilecount-nnondicoms,nnondicoms,rtmin,rtsec,directoryname));
end

if atroot, set(Hwb,'Name','Done!'); if nargin>0, close(Hwb); end; end

% if nfiles>0, fprintf('\n'); end

% dicomdict('set',currdicomdict);

% if atroot,
%     close(Hwb);
% end

function singlefiledata = getdicomdata(dirname,filename,drootname,dcmfields)

% Read DICOM header/tags
% dicomdict('get')
dcmhdr=dicominfo(fullfile(dirname,filename));

if isfield(dcmhdr,'DirectoryRecordSequence'),
    error('getdicomdata:DICOMDIRFileEncountered','Found DICOMDIR file instead of image.');
end

if isempty(dcmhdr.Width),
    error('getdicomdata:NonImageDICOMFile','Found non-image DICOM file.');
end

% Record original file name and relative pathname location
singlefiledata.OriginalFileName=filename;
singlefiledata.OriginalFileLocation=dirname(length(drootname)+1:end);

for i=3:numel(dcmfields),
    if isfield(dcmhdr,dcmfields{i}),
        % singlefiledata=setfield(singlefiledata,dcmfields{i},getfield(dcmhdr,dcmfields{i}));
        singlefiledata.(dcmfields{i})=dcmhdr.(dcmfields{i});
    else
        % singlefiledata=setfield(singlefiledata,dcmfields{i},[]);
        singlefiledata.(dcmfields{i})=[];
    end
end

if isfield(dcmhdr,'ReferencedPatientSequence'),
    singlefiledata.PatientClassUID_hash=strcrc(dcmhdr.ReferencedPatientSequence.Item_1.ReferencedSOPClassUID); end
if isfield(dcmhdr,'ReferencedStudySequence'),
    singlefiledata.StudyClassUID_hash=strcrc(dcmhdr.ReferencedStudySequence.Item_1.ReferencedSOPClassUID); end
if isfield(dcmhdr,'StudyInstanceUID'),
    singlefiledata.StudyInstanceUID_hash=strcrc(dcmhdr.StudyInstanceUID); end
if isfield(dcmhdr,'SeriesInstanceUID'),
    singlefiledata.SeriesInstanceUID_hash=strcrc(dcmhdr.SeriesInstanceUID); end


function [files,subdirs]=checkdir(dcurrent)

% Identify directories and files
dirs=[dcurrent.isdir];
files=~dirs;

% Identify '.' and '..' directories
upperdirs=cellfun(@(x) strcmpi(x,'.')|| strcmpi(x,'..'),{dcurrent.name});

% Identify subdirectories
subdirs=dirs & ~upperdirs;
files = files & ~upperdirs;


function filecount=recursivefilecount(directoryname,filecount)

if nargin<2, filecount=0; end

dcurrent=dir(directoryname);

[files,subdirs]=checkdir(dcurrent);

% Go into subdirectories
subdirinds=find(subdirs);
nsubdirs=numel(subdirinds);

for i=1:nsubdirs,
    nextsubfolder=fullfile(directoryname,dcurrent(subdirinds(i)).name);
    % subfilecount=recursivefilecount(nextsubfolder);
    % filecount=filecount+subfilecount;
    filecount=recursivefilecount(nextsubfolder,filecount);
end

% Then do files at directoryname root
currentfileinds=find(files);
ncurrentfiles=numel(currentfileinds);

filecount=filecount+ncurrentfiles;

fprintf([repmat('\b',1,6) '%6.0f'],filecount);
% fprintf('%6.0f',filecount);
    