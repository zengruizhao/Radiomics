function [patientstruct,patientnames,patientlabels] = parsedicomdirdata(filedata)
% PARSEDICOMDIRDATA
%   Sort through the struct returned by DICOMDIR, FILEDATA.  Return a
%   summary of patients, and the studies and images of each patient.
%
%   [PATIENTSTRUCT,PATIENTNAMES] = PARSEDICOMDIRDATA(FILEDATA) parses the
%   structure FILEDATA and returns the structure PATIENTSTRUCT, which
%   contains information about all files in FILEDATA that belong to a given
%   patient.
%
%   [PATIENTSTRUCT,PATIENTNAMES] = PARSEDICOMDIRDATA(..) returns a cell
%   array of strings containing the patient names.  Note that
%   numel(PATIENTNAMES) == numel(PATIENTSTRUCT).
%
%   [PATIENTSTRUCT,PATIENTNAMES,PATIENTLABELS] = PARSEDICOMDIRDATA(...)
%   returns a double array of integral value indexes into PATIENTNAMES for
%   each file in FILEDATA. (i.e. numel(PATIENTLABELS) == numel(FILEDATA))
%
%   See also: DICOMDIR, STRUNIQUE, DICOMINFO, DICOMREAD.
%
%JC

fprintf('Processing data for %g images.\n',numel(filedata));
fprintf('-Identifying unique patient Names... ');

% The patient name might be in either PatientName or PatientName.FamilyName
if isfield(filedata(1).PatientName,'FamilyName'),
    celldefstring=sprintf('filedata(%g).PatientName.FamilyName ',1:numel(filedata));
    cellofstr=eval(['{' celldefstring '};']);
    [patientnames,patientfileinds,patientlabels]=strunique(cellofstr,false);
else
    [patientnames,patientfileinds,patientlabels]=strunique({filedata.PatientName},false);
end

fprintf(' done.\n');

patientstruct=struct([]);

npatients=numel(patientnames); npctchars=0;
fprintf('-Parsing files for each of %g patients...',npatients);
for i=1:npatients,
    patientstruct(i).patientname=patientnames{i}; %#ok<*AGROW>
    patientstruct(i).fileinds=patientfileinds{i};
    patientstruct(i).seriesdesc={filedata(patientfileinds{i}).SeriesDescription};
    [uniqueseriesdescs,useriesinds,userieslabels]=strunique(patientstruct(i).seriesdesc);
    patientstruct(i).serieslabel=userieslabels;
    patientstruct(i).serieslabelnames=uniqueseriesdescs;
    patientstruct(i).studydesc={filedata(patientfileinds{i}).StudyDescription};
%     patientstruct{patientcount}.imagenum={filedata(patientfileinds{i}).InstanceNumber};
    patientstruct(i).seriesnum={filedata(patientfileinds{i}).SeriesNumber};
    
    pctchecked=100*(i/npatients);
    pcttext=sprintf('%5.1f%%',pctchecked);
    backspaces=repmat('\b',1,npctchars);
    fprintf([backspaces '%s'],pcttext);
    npctchars=numel(pcttext);
end
fprintf('\n');

return

% indexes of all images
imginds=1:length(filedata);
remainingimginds=imginds;

patientcount=0;
while numel(remainingimginds)>0,
    % Grab next patient name from first of remaining images
    currentname=filedata(remainingimginds(1)).PatientName;
    patientnames{patientcount+1}=currentname;

    % Check which of all remaining images belong to this patient
    iscurrentpatient=cellfun(@(x) strcmp(x,currentname), {filedata(remainingimginds).PatientName});
    currentpatientinds=remainingimginds(iscurrentpatient);
    remainingimginds=setdiff(remainingimginds,currentpatientinds);
   
    % patientfileinds{patientcount+1}=currentpatieninds; %#ok<AGROW>
    % patientseriesdesc{patientcount+1}={filedata(currentpatieninds).SeriesDescription}; %#ok<AGROW>
    % patientinstancenumber{patientcount+1}={filedata(currentpatieninds).InstanceNumber}; %#ok<AGROW>
    % patientseriesnum{patientcount+1}={filedata(currentpatieninds).SeriesNumber}; %#ok<AGROW>
    % patientstudydesc{patientcount+1}={filedata(currentpatieninds).StudyDescription}; %#ok<AGROW>
    
    patientcount=patientcount+1;
    
    patientstruct(patientcount).patientname=currentname; %#ok<*AGROW>
    patientstruct(patientcount).fileinds=currentpatientinds;
    patientstruct(patientcount).seriesdesc={filedata(currentpatieninds).SeriesDescription};
    patientstruct(patientcount).studydesc={filedata(currentpatieninds).StudyDescription};
%     patientstruct{patientcount}.imagenum={filedata(currentpatieninds).InstanceNumber};
    patientstruct(patientcount).seriesnum={filedata(currentpatieninds).SeriesNumber};
end
