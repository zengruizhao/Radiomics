function dicomdircopy(drootname,filedata,destdir,folderfields,forcedpatientname)
% DICOMDIRCOPY
%   Reorganizes a DICOM directory.
%
%   Operates by copying DICOM files into a different folder, while creating
%   a new, user-specified subdirectory structure based on DICOM tags.
%
%   DICOMDIRCOPY(ROOTDIR,FILEDATA,DESTDIR) reads files contained in the 
%   ROOTDIR folder and specified by the FILEDATA structure, and copies them
%   into a new directory structure rooted in DESTDIR.
%
%   The structure FILEDATA contains information about the targeted files
%   that is used in creating the final directory structure under DESTDIR.
%   To create FILEDATA, use the DICOMDIR function (i.e. FILEDATA=DICOMDIR;).
%
%   DICOMDIRCOPY(...,FOLDERFIELDS) allows a fourth argument, FOLDERFIELDS,
%   to specify the subfolder organization of DESTDIR. By default, DESTDIR 
%   will be organized with the follwing subfolders:
%       [PatientName.FamilyName]/[SeriesDescription]/[SeriesNumber]
%   where the above values in [] brackets are DICOM tags read by DICOMINFO.
%   The FOLDERFIELDS argument is a cell array of strings, where the default
%   is {'PatientName.FamilyName','SeriesDescription','SeriesNumber'}.
%
%   See also: DICOMDIR, PARSEDICOMDIRDATA, DICOMINFO, DICOMREAD.
%
% JC

if isdir(destdir);
    fprintf('Destination directory exists... ');
    destexistans=questdlg('Destination directory exists. Continue?','Destination exists.','Yes','No','No');
    if strcmp(destexistans,'No');
        fprintf('BREAKING on user request.\n');
        return
    end
    fprintf('CONTINUING anyway.\n');
    % fprintf('WARNING: destination directory %s already exists!\nHit ENTER to continue or CTRL-C to quit.\n',destdir);
    % pause
end

if nargin<4 || isempty(folderfields),
    folderfields={'PatientName.FamilyName','SeriesDescription','SeriesNumber'};
    % folderfields={'PatientName.FamilyName','StudyDate','StudyDescription','SeriesDescription','SeriesNumber'};
end
nsubfolders=numel(folderfields);

if nargin==5, 
    copyfunc=@(sf,df) copyandpatchpatientname(sf,df,forcedpatientname);
    fprintf('Changing PatientName.FamilyName to %s.\n',forcedpatientname);
else
    copyfunc=@(sf,df) copyfile(sf,df,'f');
end

% Set up waitbar
nfiles=numel(filedata);
Hwb = waitbar(0,'waitbarstring','Name','Rebuilding DICOM directory');
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
drawnow;

% Overwriting
yesall=false; noall=false;

for k=1:nfiles,
    
    origfile=filedata(k).OriginalFileName;
    origfiledir=filedata(k).OriginalFileLocation;
    
    if ~ispc, % UNIX
        origfiledir=strrep(origfiledir,'\','/');
    end
    srcfilefull=fullfile(drootname, origfiledir, origfile);
    
    destfile=sprintf('image%04.0f.dcm',filedata(k).InstanceNumber);
    
    destdirfull=destdir;
    for kd=1:nsubfolders,
        subfolder=eval(['filedata(k).' folderfields{kd}]);
        if isnumeric(subfolder), subfolder=num2str(subfolder); end % for SeriesNumber
        if ~ischar(subfolder),
            display(subfolder);
            error('Chosen field %s does not result in string.',folderfields{kd})
        end
        if isempty(subfolder), subfolder=['NO_' folderfields{kd}]; end
        if exist('forcedpatientname','var')
            if kd == 1 %assuming this is Pt ID
                subfolder = forcedpatientname;
            end
        end
        destdirfull=fullfile(destdirfull,cleansubdirname(subfolder));
    end
    
    if ~exist(destdirfull,'dir'),
        pause(.25)
        [mkdirsuccess,mkdirmess,mkdirmessid]=mkdir(destdirfull); pause(.5)
        if ~mkdirsuccess, destdirfull, error(mkdirmessid,mkdirmess); end
        if ~isempty(mkdirmess), warning(mkdirmessid,mkdirmess); end
    % else
    %     fprintf('WARNING: Patient directory already exists.\n');
    end

    destfilefull=fullfile(destdirfull, destfile);
    
    %if ~mod(k-1,10),
        waitbar(k/nfiles,Hwb,sprintf('Src: %s\nDest: %s',srcfilefull,destfilefull));
    %end
    
    % fprintf('Copying %s to %s\n',srcfilefull, destfilefull);
    if exist(destfilefull,'file'),
        if noall,
            cpsuccess=true;
            fprintf('NOT overwriting %s with %s\n',srcfilefull, destfilefull);
        elseif yesall,
            fprintf('Overwriting %s with %s\n',srcfilefull, destfilefull);
            [cpsuccess,cpmes,cpmesid] = copyfunc(srcfilefull, destfilefull);
        else
            fprintf('Overwrite %s with %s?\n',destfilefull, srcfilefull);
            waitbar(k/nfiles,Hwb,sprintf('Src: %s\nDest: %s',srcfilefull,destfilefull));
            overwriteans=questdlg('File exists. Overwrite?','Overwrite?','Yes','No','Cancel','No');
            switch overwriteans,
                case 'Yes',
                    fprintf('Overwriting %s with %s\n',destfilefull, srcfilefull);
                    [cpsuccess,cpmes,cpmesid] = copyfunc(srcfilefull, destfilefull);
                case 'No',
                    cpsuccess=true;
                    fprintf('NOT overwriting %s with %s\n',srcfilefull, destfilefull);
%                 case 'Cancel',
%                     fprintf('Breaking on user request.\n');
%                     return
                otherwise
                    fprintf('Breaking on user request.\n');
                    fprintf('Src: %s\nDest: %s\n',srcfilefull,destfilefull);
                    return
            end
            doall=questdlg('Repeat for all?','All?','All','Just Once','Just Once');
            switch doall,
                case 'All',
                    if strcmp(overwriteans,'Yes'), yesall=true;
                    else noall=true; end
                otherwise
            end
        end
    else
        [cpsuccess,cpmes,cpmesid] = copyfunc(srcfilefull, destfilefull);
    end
    
    if ~cpsuccess, fprintf('\nSrc: %s\nDest: %s\n',srcfilefull,destfilefull); error(cpmesid,'%s',cpmes); end
    
end

set(Hwb,'Name','Done!');

function dclean = cleansubdirname(d)

dclean=d;
badfolderchars=' /\:*?"<>|';
replacemtchars='-------()-';
for i=1:numel(badfolderchars), dclean=strrep(dclean,badfolderchars(i),replacemtchars(i)); end
% dclean=strrep(strrep(strrep(d,'\','-'),'/','-'),' ','-');

function [cpsuccess,cpmes,cpmesid] = copyandpatchpatientname(srcfilefull,destfilefull,forcedpatientname)

X=dicomread(srcfilefull);
metadata = dicominfo(srcfilefull);

  if isfield(metadata.PatientName,'FamilyName')
    metadata.PatientName.FamilyName=forcedpatientname;
    metadata.PatientName.GivenName='';
    metadata.PatientName.MiddleName='';
  end
  if isfield( metadata, 'PatientID' )
    metadata.PatientID=([forcedpatientname,'-ID']);
  end
  if isfield (metadata,'PatientAddress' )
      metadata.PatientAddress = '';
  end
  
  if isfield (metadata,'PatientBirthDate' )
      metadata.PatientBirthDate = '';
  end
  if isfield (metadata,'PatientSize' )
      metadata.PatientSize = '';
  end  
  
  if isfield(metadata, 'ReferringPhysicianName')
    metadata.ReferringPhysicianName.FamilyName='';
    metadata.ReferringPhysicianName.GivenName='';
    metadata.ReferringPhysicianName.MiddleName='';
  end
  if isfield(metadata, 'PhysiciansOfRecord')
    metadata.PhysiciansOfRecord.FamilyName='';
    metadata.PhysiciansOfRecord.GivenName='';
    metadata.PhysiciansOfRecord.MiddleName='';
  end
  if isfield(metadata, 'PhysicianOfRecord')
    metadata.PhysicianOfRecord.FamilyName='';
    metadata.PhysicianOfRecord.GivenName='';
    metadata.PhysicianOfRecord.MiddleName='';
  end  
  if isfield(metadata, 'PerformingPhysicianName')
    metadata.PerformingPhysicianName.FamilyName='';
    metadata.PerformingPhysicianName.GivenName='';
    metadata.PerformingPhysicianName.MiddleName='';
  end 
  if isfield(metadata, 'RequestingPhysician')
    metadata.RequestingPhysician.FamilyName='';
    metadata.RequestingPhysician.GivenName='';
    metadata.RequestingPhysician.MiddleName='';
  end   
  
  if isfield(metadata, 'IssuerOfPatientID' )
    metadata.IssuerOfPatientID ='';
  end
 
  if isfield(metadata, 'InstitutionName' )
    metadata.InstitutionName ='';
  end
  if isfield(metadata, 'InstitutionAddress' )
    metadata.InstitutionAddress ='';
  end  
cpmes=dicomwrite(X, destfilefull, metadata, 'CreateMode', 'copy');
cpsuccess=isempty(cpmes);
if ~cpsuccess, cpmesid=['copyandpatchpatientname:' cpmes]; else cpmesid=[]; end
