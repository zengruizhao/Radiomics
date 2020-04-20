function [imdat,iminf]=idfread(idffile)
% IDFREAD Read in an IDF header file and its associated INT2 data file.
%   [IMDAT,IMINF]=IDFREAD(IDFFILE) parses the idf file named in the string
%   IDFFILE and outputs the image data IMDAT and certain values from the
%   .idf file in IMINF.
%
%JC

if nargin<1,
    [fn,pn] = uigetfile({'*.idf','IDF image header file (*.idf)'}, 'Pick an IDF file.');
    if isequal(fn,0) || isequal(pn,0),
        imdat=[]; iminf=[];
        return;
    end
    idffile=[pn fn];
end

% fid=fopen(idffile,'r');
% idf=char(fread(fid))';
% fseek(fid,0,-1) % rewind
% fclose(fid)

[pn,namebase,ext]=fileparts(idffile); %#ok<NASGU>
if ~exist(fullfile(pn,[namebase '.int2']),'file'), warning('idfread:noint2','No corresponding .int2 file.'); end

fid=fopen(idffile,'r');

i=1; ndims=0; dcoslines=0;
while 1,
    tline=fgetl(fid);
    if ~ischar(tline), break; end
    % strs=textscan(tline,'%s%f');
    
    sidcells=regexp(tline,'^studyid:\s*(\w*)(.*)$','tokens');
    if ~isempty(sidcells),
        iminf.studyid=sidcells{1}{1};
        continue;
    end
%     sidcells=regexp(tline,'^studyid:\s*(\w*)(.*)$','tokens');
%     if ~isempty(sidcells),
%         iminf.studyid=sidcells{1}{1};
%         continue;
%     end
    rnval=regexp(tline,'^rootname: (.*)$','tokens');
    if ~isempty(rnval),
        iminf.rootname=rnval{1}{1};
        continue;
    end
    comval=regexp(tline,'^comment: (.*)$','tokens');
    if ~isempty(comval),
        iminf.comment=comval{1}{1};
        continue;
    end
    dimcells=regexp(tline,'^dimension:\s*(\d*)(.*)$','tokens');
    if ~isempty(dimcells) && ~isempty(dimcells{1}(1)),
        ndims=ndims+1;
        iminf.dimensions(ndims)=str2num(dimcells{1}{1});
        continue;
    end
    pixcells=regexp(tline,'^npix:\s*(\d*)\s*fov\(mm\):\s*([-]*\d*\.\d*)\s*center\(mm\):\s*([-]*\d*\.\d*)\s*pixelsize\D*(\d*\.\d*)(.*)$','tokens');
    if ~isempty(pixcells),
        iminf.npix(ndims)=str2num(pixcells{1}{1});
        iminf.fovmm(ndims)=str2num(pixcells{1}{2});
        iminf.center(ndims)=str2num(pixcells{1}{3});
        iminf.pixelsize(ndims)=str2num(pixcells{1}{4});
        continue;
    end
    slthcells=regexp(tline,'^slice thickness \(mm\):\s*(\d*\.\d*)(.*)$','tokens');
    if ~isempty(slthcells),
        iminf.slice_thickness=str2num(slthcells{1}{1});
        continue;
    end
    rangecells=regexp(tline,'^minimum:\s*([-]*\d*\.\d*e\+\d*)\s*maximum:\s*(\d*\.\d*e\+\d*)(.*)$','tokens');
    if ~isempty(rangecells),
        iminf.minimum=str2num(rangecells{1}{1});
        iminf.maximum=str2num(rangecells{1}{2});
        continue;
    end
    scalecells=regexp(tline,'^scale:\s*(\d*\.\d*)(.*)$','tokens');
    if ~isempty(scalecells),
        iminf.scale=str2num(scalecells{1}{1});
        continue;
    end
    slicesreadcells=regexp(tline,'^first slice read:\s*(\d*)\s*last slice read:\s*(\d*)\s*sliceskip:\s*([-]*\d*)(.*)$','tokens');
    if ~isempty(slicesreadcells),
        iminf.firstreadslice=str2num(slicesreadcells{1}{1}); %#ok<*ST2NM>
        iminf.lastreadslice=str2num(slicesreadcells{1}{2});
        iminf.sliceskip=str2num(slicesreadcells{1}{3});
        continue;
    end
    centercells=regexp(tline,'^center:\s*([-]*\d*\.\d*)\s*([-]*\d*\.\d*)\s*([-]*\d*\.\d*)(.*)$','tokens');
    if ~isempty(centercells),
        iminf.center_firstread(1)=str2num(centercells{1}{1});
        iminf.center_firstread(2)=str2num(centercells{1}{2});
        iminf.center_firstread(3)=str2num(centercells{1}{3});
        continue;
    end
    toplccells=regexp(tline,'^toplc:\s*([-]*\d*\.\d*)\s*([-]*\d*\.\d*)\s*([-]*\d*\.\d*)(.*)$','tokens');
    if ~isempty(toplccells),
        iminf.toplc_firstread(1)=str2num(toplccells{1}{1});
        iminf.toplc_firstread(2)=str2num(toplccells{1}{2});
        iminf.toplc_firstread(3)=str2num(toplccells{1}{3});
        continue;
    end
    dcoscells=regexp(tline,'^dcos[1-3]:\s*([-]*\d*\.\d*)\s*([-]*\d*\.\d*)\s*([-]*\d*\.\d*)(.*)$','tokens');
    if ~isempty(dcoscells),
        dcoslines=dcoslines+1;
        iminf.dcos(dcoslines,1)=str2num(dcoscells{1}{1});
        iminf.dcos(dcoslines,2)=str2num(dcoscells{1}{2});
        iminf.dcos(dcoslines,3)=str2num(dcoscells{1}{3});
        continue;
    end
    i=i+1;
end
fclose(fid);

int2file=fullfile(pn,[iminf.rootname '.int2']);

if ~exist(int2file,'file'),
    imdat=[];
    return;
end

fid=fopen(int2file,'r','ieee-be');
imdat=fread(fid,'uint16');
fclose(fid);
if numel(imdat)==prod(iminf.npix),
    imdat=reshape(imdat,iminf.npix);
else
    warning('idfread:BadPixelCount','Number of observations does not match pixels specified in .idf file.');
end

% imdat=permute(imdat,[2 1 3]);
% ADCpre=flipdim(ADCpre,2);
% imdat=flipdim(imdat,3);