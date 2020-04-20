function [whichv1matches_calcd,whichv2matches_calcd,whichv1matches,whichv2matches,v1locz,v2locz,v1info,v2info,v1loc,v2loc]=dicomslicecorrs(varargin)
% DICOMSLICECORRS
%   For 2 different image sets, calculates the nearest slice in other set,
%   based on z-axis position data in DICOM tags.
%
%   [WHICHV1,WHICHV2]=DICOMSLICECORRS first prompts for the unique directory 
%   locations of the data for volumes V1 and V2.  Output arguments WHICHV1 
%   and WHICHV2 contain the slice numbers of the images in the V1 and V2 
%   image sets which correspond to the image in the V2 and V1 sets, 
%   respectively.
%
%   NOTE: If there are N images in V2 and M images in V2, WHICHV1 will be
%   of size N-by-1 and containing values between 1 and M.  On the other 
%   hand, WHICHV2 will be of size M-by-1 and contain values between 1 and
%   ND.
%
%   [WHICHV1,WHICHV2,WHICHV1_SL,WHICHV2_SL]=DICOMSLICECORRS returns two
%   additional arrays with the same sizes and meanings of WHICHV1 and 
%   WHICHV2, except they are not calculated from ImageOrientationPatient
%   and ImagePositionPatient, but rather taken directly from SliceLocation.
%   This is not advized as SliceLocation is not a required or reliable
%   DICOM tag.
%
%   See code comments for Implementation Details.
%
%   [...,V1LOCZ,V2LOCZ]=DICOMSLICECORRS returns into the 5th and 6th arguments
%   the mm locations of each slice in V1 and V2 volumes as calculated  from
%   the DICOM tags.
%
%   [...,V1INFO,V2INFO]=DICOMSLICECORRS returns into the 7th and 8th arguments
%   a verbose list of all DICOM tags for each image set.
%
%   Run with this syntax to get all outputs:
%   [whichv1matches_calcd,whichv2matches_calcd,...
%    whichv1matches,whichv2matches,...
%    v1locz,v2locz,v1info,v2info] = dicomslicecorrs;
%
%   See also DCMIOP2ROT, DICOMINFO.
%
%JC

%   Implementation details:
%       A number of coordinate tags are defined in terms of a patient-based
%       coordinate system. We desire to map these coorinates onto a
%       reference coorindate system, or RCS (Part 3, Section 3.17.1).
%
%       The DICOM tag (0020,0037), Image Orientation (Patient), specifies
%       the direction cosines of the first row and first column.
%
%       The DICOM tag (0020,0033), Image Position (Patient), specifies the
%       x, y, and z coordinates of the upper left hand corner of the image.
%
%       See Part 3, Annex C.7.6.2 (Image Plane Module) of the 2008 DICOM
%       Standard (ftp://medical.nema.org/medical/dicom/2008/, 08_03pu.pdf).
%
%       To calculate the z-axis position in along an axis along the normal
%       of the images, we first calculate the normal vector for each slice
%       by the cross product of the direction cosines in Image Orientation
%       (Patient) (see DCMIOP2ROT).  Then, Image Position (Patient) is 
%       projected on this normal by the dot product of the two vectors,
%       yielding an axial position location in mm.

if nargin==0,
    patientfolder = uigetdir('','Select the parent directory for the patient');
    v1folder = uigetdir(patientfolder,'Select the directory containing Volume 1');
    v2folder = uigetdir(patientfolder,'Select the directory containing Volume 2');
elseif nargin==1,
    patientfolder=varargin{1};
    v1folder = uigetdir(patientfolder,'Select the directory containing Volume 1');
    v2folder = uigetdir(patientfolder,'Select the directory containing Volume 2');
elseif nargin==2,
    v1folder=varargin{1};
    v2folder=varargin{2};
    % fileseplocs=strfind(v1folder,filesep); patientfolder=v1folder(1:fileseplocs(end)-1);
elseif nargin>2
    error('Wrong number of input arguments.');
end

% Identify V1 DICOM images by the .dcm extension
dv1=dir([v1folder '/*.dcm']);
nv1=numel(dv1); % count images

% Inialize array for V1 z-axis locations of each slice
v1locz=zeros(1,nv1);
v1loc=zeros(3,nv1);

% Calculate z-axis location for each slice
fprintf('Reading %g DICOM files comprising V1:\n',nv1);
for i=1:nv1,
    % Read DICOM header/tags into v1info struct
    v1info(i)=dicominfo([v1folder '/' dv1(i).name]); %#ok<AGROW>
    
    % Calculate slice normal vector from I.O.P. (cross product)
    [Rv1,Rv1pnormal]=dcmiop2rot(v1info(i).ImageOrientationPatient);
    
    % Calculate z position (dot product)
    v1loc(:,i)=v1info(i).ImagePositionPatient'*Rv1;  % x,y,z position
    v1locz(i)=v1info(i).ImagePositionPatient'*Rv1pnormal; % just z pos.
    fprintf('.');  % progress
    
    if ~mod(i,80) && i~=nv1, fprintf('\n'); end
end
fprintf('\n');

% Identify V2 data
dv2=dir([v2folder '/*.dcm']);
nv2=numel(dv2);
v2locz=zeros(1,nv2);
v2loc=zeros(3,nv2);
fprintf('Reading %g DICOM files comprising V2:\n',nv2);
for i=1:nv2,
    v2info(i)=dicominfo([v2folder '/' dv2(i).name]); %#ok<AGROW>
    [Rv2,Rv2pnormal]=dcmiop2rot(v2info(i).ImageOrientationPatient);
    v2loc(:,i)=v2info(i).ImagePositionPatient'*Rv2;
    v2locz(i)=v2info(i).ImagePositionPatient'*Rv2pnormal;
    fprintf('.');
    
    if ~mod(i,80) && i~=nv2, fprintf('\n'); end
end
fprintf('\n');

% Grab the V2 z position from the unreliable SliceLocation variable
% v2z=[v2info.SliceLocation];

% Make sure slice locations are monotonic
v1subvolstring=''; v2subvolstring='';
if ~(all(diff(v1locz)>0) || all(diff(v1locz)<0)),
    % warning('dicomslicecorrs:V1NotMonotonic','V1 slice locations not monotonic.');
    fprintf('V1 slice locations not monotonic. Checking for possible sub-volumes...\n');
    v1subvolumelocz=checkslicelocz(v1locz);
    v1locz(v1subvolumelocz(2):end)=[];
    nv1=numel(v1locz);
    v1subvolstring=' (1st subvolume)';
end
if ~(all(diff(v2locz)>0) || all(diff(v2locz)<0)),
    % warning('dicomslicecorrs:V2NotMonotonic','V2 slice locations not monotonic.');
    fprintf('V2 slice locations not monotonic. Checking for possible sub-volumes...\n');
    v2subvolumelocz=checkslicelocz(v2locz);
    v2locz(v2subvolumelocz(2):end)=[];
    nv2=numel(v2locz);
    v2subvolstring=' (1st subvolume)';
end
% max(abs(diff(diff(v2locz))))
fprintf('Comparing %g slices%s of V1 to %g slices%s of V2.\n',nv1,v1subvolstring,nv2,v2subvolstring);

% Check the unique frame of reference UID, which should indicate if the
% scan sessions are significantly different (i.e. the coordinate systems)
if ~strcmp(v1info(1).FrameOfReferenceUID,v2info(1).FrameOfReferenceUID),
    fprintf('Frame of reference differnent between V1 and V2, bailing out!.\n');
    return
end

% Now identify the corresponding V1 slices for each slice in the V2 volume
whichv1matches=zeros(nv2,1);
whichv1matches_calcd=zeros(nv2,1);
for i=1:nv2,
    [clostestdist,whichv1matches(i)]=min(abs([v1info.SliceLocation]-v2info(i).SliceLocation));
    [clostestdist,whichv1matches_calcd(i)]=min(abs(v1locz-v2locz(i)));
end

% Now identify the corresponding V2 slices for each slice in the V1 volume
whichv2matches=zeros(nv1,1);
whichv2matches_calcd=zeros(nv1,1);
for i=1:nv1,
    [clostestdist,whichv2matches(i)]=min(abs([v2info(1:nv2).SliceLocation]-v1info(i).SliceLocation));
    [clostestdist,whichv2matches_calcd(i)]=min(abs(v2locz-v1locz(i)));
end

return

function subvolumelocz=checkslicelocz(slicelocz)

negativesdifflocz=find(diff(slicelocz)<0);
positivedifflocz=find(diff(slicelocz)>0);
if numel(positivedifflocz)<numel(negativesdifflocz),
    subvolumelocz=[1 positivedifflocz+1];
elseif numel(positivedifflocz)>numel(negativesdifflocz),
    subvolumelocz=[1 negativesdifflocz+1];
else
    error('Cannot figure out the slices/sub-volumes.');
end

if diff(diff(subvolumelocz))>0,
    error('Sub-volumes not the same size!');
end

return 
