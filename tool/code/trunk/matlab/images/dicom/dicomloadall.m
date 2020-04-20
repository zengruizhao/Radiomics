function [ims,imtags,slicezcoords]=dicomloadall(sourcedir,sortfield)
% DICOMLOADALL Load all dicom files in a folder into a single 3D array.
%   DICOMLOADALL(SOURCEDIR) specified the source directory in which to look
%   for DICOM files.  SOURCEDIR may also contain wildcards for the files.
%
%   DICOMLOADALL(DICOMDIRDAT) when a struct is input, interpret it as a
%   the output from DICOMDIR and load the files described in the struct.
%
%JC

if nargin<2, sortfield='InstanceNumber'; end

if nargin>0,
    if ischar(sourcedir),
        [ims,imtags,slicezcoords]=dcmdirloaddir(sourcedir,sortfield);
    elseif isstruct(sourcedir),
        [ims,imtags]=dcmdirloadfiles(sourcedir,sortfield);
        slicezcoords=[];
    else
        error('Unrecognized input');
    end
else
    [ims,imtags,slicezcoords]=dcmdirloaddir(pwd,sortfield);
end

end

function [ims,dicomdirdata]=dcmdirloadfiles(dicomdirdata,sortfield)

[instances,neworder]=sort([dicomdirdata.(sortfield)]);

fprintf('Loading image data.');
im1=dicomread(['.' filesep dicomdirdata(1).OriginalFileLocation filesep dicomdirdata(1).OriginalFileName]);
siuid=dicomdirdata(1).SeriesInstanceUID;
ims=zeros([size(im1) numel(instances)],class(im1)); % numel(dcmlocs) == dcmfilecount
for i=1:numel(instances),
    fname=['.' filesep dicomdirdata(neworder(i)).OriginalFileLocation filesep dicomdirdata(neworder(i)).OriginalFileName];
    if ~strcmp(siuid,dicomdirdata(i).SeriesInstanceUID),
        warning('dicomloadall:dcmdirloadfiles:SeriesInstanceUIDMismatch','Series instance UID mismatch.');
    end
    ims(:,:,i)=dicomread(fname);
    fprintf('.');
end
fprintf('\n')

end

function [ims,imtags,slicezcoords]=dcmdirloaddir(sourcedir,sortfield)

if nargin<1, sourcedir=pwd; end
d=dir(sourcedir);
d([d.isdir])=[];

instancenum=zeros(numel(d),1);
slicecoords=zeros(3,numel(d));
slicezcoords=zeros(1,numel(d));
imtags=cell(numel(d),1);
fprintf('Checking file order:\n');
dcmfilecount=0;
isdcmfile=false(numel(d),1);
for i=1:numel(d),
    try
        imtags{dcmfilecount+1}=dicominfo(d(i).name);
        instancenum(dcmfilecount+1)=imtags{dcmfilecount+1}.(sortfield);
        dcmfilecount=dcmfilecount+1;
        isdcmfile(i)=true;
        
        % Calculate slice normal vector from I.O.P. (cross product)
        [Rv,Rvnormal]=dcmiop2rot(imtags{dcmfilecount}.ImageOrientationPatient);
        
        % Calculate z position (dot product)
        slicecoords(:,i)=imtags{dcmfilecount}.ImagePositionPatient'*Rv;  % x,y,z position
        slicezcoords(i)=imtags{dcmfilecount}.ImagePositionPatient'*Rvnormal; % just z pos.
    catch %#ok<CTCH> % not a dicom file
        warning('dicomloadall:dcmdirloaddir:nonDICOMFileDetected','non dicom file detected.');
    end
    fprintf('.');
    if mod(i+1,80)==0, fprintf('\n'); end
end

imtags=imtags(1:dcmfilecount);
instancenum=instancenum(1:dcmfilecount);
slicecoords=slicecoords(:,1:dcmfilecount);
slicezcoords=slicezcoords(1:dcmfilecount);

dcmlocs=find(isdcmfile);
ddcm=d(dcmlocs);

[~,reorderinds]=sort(instancenum);
ddcm=ddcm(reorderinds);
slicecoords=slicecoords(reorderinds);
slicezcoords=slicezcoords(reorderinds);
imtags=imtags(reorderinds);
fprintf('\n');

fprintf('Loading image data:\n');
im1=dicomread(ddcm(1).name);
ims=zeros([size(im1) numel(dcmlocs)],class(im1)); % numel(dcmlocs) == dcmfilecount
for i=1:numel(dcmlocs),
    ims(:,:,i)=dicomread(ddcm(i).name);
    fprintf('.');
    if mod(i+1,80)==0, fprintf('\n'); end
end
fprintf('\n');

end
