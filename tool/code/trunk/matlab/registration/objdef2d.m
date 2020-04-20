function err = objdef2d(rigidparams,I,J,fillwith,N,nodisp,paramweights,measure,interpmethod,paramoffsets,preM,coordshift,onescale)
% objective function for MI based registration - I is target, J is template

persistent err_record regiter

if nargin==0,
    err=err_record(1:regiter);
    clear err_record regiter
    return;
end

if isempty(regiter),
    regiter=0;
    err_record=zeros(1,2e3); % allocate for 2000 iterations
end
regiter=regiter+1;

if nargin<13,
    onescale=false; % says how to interpret rigidparams and paramweights when numel(rigidparams)==5
    if nargin<12 || isempty(coordshift),
        coordshift=zeros(2,1);
        if nargin<11,
            preM=[];
        end
    end
end

% Weight parameters so optimization routine works - REMEMBER THESE VALUES
rot=rigidparams(1)*paramweights(1)+paramoffsets(1);
trans=rigidparams(2:3)'.*paramweights(2:3)'+paramoffsets(2:3)';

% Fix paramoffsets to length 6 if calling routine did not already
if numel(paramoffsets)==4,
    paramoffsets(5:6)=[paramoffsets(4); 0];
% elseif numel(paramoffsets)==5 && nscales==1, % ambiguous case, spring error instead
%     paramoffsets(5:6)=paramoffsets(4:5);
elseif numel(paramoffsets)<4,
    paramoffsets(4:6)=0; % paramoffsets=[paramoffsets; zeros(2,1)];
elseif numel(paramoffsets)~=6,
    error('paramoffsets must specify all scales!');
end

if numel(rigidparams)==6, rot2=rigidparams(6)*paramweights(6)+paramoffsets(6);
else rot2=0; end

% Scaling is optional
if numel(rigidparams)<4, % ==3 (no scale)
    scale=[1 1];
elseif numel(rigidparams)<5, % ==4 (isoscale)
    scale=(rigidparams([4 4])'-1).*paramweights([4 4])' + 1;
elseif numel(rigidparams)==5 && onescale,
    scale=(rigidparams([4 4])'-1).*paramweights([4 4])' + 1;
    rot2=rigidparams(5)*paramweights(5)+paramoffsets(5);
else % ==5 && scales==2 || >5 (anisoscale)
    scale=(rigidparams(4:5)'-1).*paramweights(4:5)' + 1;
end
scale=scale+paramoffsets(4:5)';

[Inrows,Incols,DIslices,DIattribs]=size(I);
[nrows,ncols,DJslices,DJattribs]=size(J);

if DIslices~=DJslices,
    error('size(I,3) ~= size(J,3): Ensure same volume size and/or put attributes on dimension 4.');
end

% Don't always convert to double, leave in integer to speed things up
Inew=imdef([rot trans scale rot2],I,interpmethod,fillwith,preM,coordshift);
Inew=round(Inew); % this should be a noop for 'nearest'

if strfind(interpmethod,'cubic'), % match *cubic and cubic
    Inew(Inew<0)=0;
    Inew(Inew>(N-1))=N-1;
end

% Same size images - Target (I) may be bigger than template (J)
if any([nrows ncols]~=[Inrows,Incols]); Inew=imshave(Inew,[nrows ncols]); end

% Similarity measure
multiattribute=DIattribs>1 || DJattribs>1;
if multiattribute,
    % Calculate MI from features
    Ivect=reshape(Inew,prod([nrows ncols DIslices]),DIattribs);
    Jvect=reshape(J,prod([nrows ncols DJslices]),DJattribs);
    switch measure
        case 'MI'
            % [err,nuniqueSA1,nuniqueSA2,nuniqueSA3,SA1,SA2,SA3]=CMIgen(Ivect,Jvect,N);
            err=CMIgen(Ivect,Jvect,N);
        case 'alphaMI'
            alpha=0.5; k=max([DIattribs DJattribs])*30;
            err=alphami(Ivect,Jvect,k,alpha);
        case 'NMI'
            % [~,nuniqueSA1,nuniqueSA2,nuniqueSA3,SA1,SA2,SA3,err]=CMIgen(Ivect,Jvect,N);
            [~,~,~,~,~,~,~,err]=CMIgen(Ivect,Jvect,N);
        case 'ECC'
            % [~,nuniqueSA1,nuniqueSA2,nuniqueSA3,SA1,SA2,SA3,junk,err]=CMIgen(Ivect,Jvect,N);
            [~,~,~,~,~,~,~,~,err]=CMIgen(Ivect,Jvect,N);
        case {'JE','EOH'}
            error('Measure not implemented for multi-attribute images.');
        otherwise
            error('Measure name not recognized.');
    end
else
    switch lower(measure)
        case 'mi'
            err=mimex(J,Inew,N); %MI(J,Iout,N);
        case 'nmi'
            % [junk,junk,junk,junk,err]=MI(J,Iout,N);
            [~,err]=mimex(J,Inew,N);
        case 'ecc'
            % [junk,junk,junk,junk,junk,err]=MI(J,Iout,N);
            [~,~,err]=mimex(J,Inew,N);
        case 'je'
            % [junk,err]=MI(J,Iout,N);
            [~,~,~,err]=mimex(J,Inew,N);
            err=-err;
        case 'eoh'
            % [junk,junk,junk,junk,junk,junk,junk,junk,junk,err]=MI(J,Iout,N);
            [~,~,~,~,~,~,err]=mimex(J,Inew,N);
        otherwise
            error('Measure name not recognized for 1D intensity images.');
    end
end

err=-err;
err_record(regiter)=err;

if ~nodisp,
%     titlestring=get(get(gca,'title'),'String');
    fprintf('Deforming: ');
    fprintf([repmat('%1.4f ',1,9) ''],[rot trans scale rot2]);
    fprintf('\n');
%     if multiattribute,
%         fprintf('# unique bins in MI term 3: %g.\n',nuniqueSA3);
%         fprintf('Joint entropy (term 3): %g.\n',SA3);
%     end
    % preserve title
    titlestring=get(get(gca,'title'),'String');
    imdisp(Inew(:,:,1))
    title(titlestring)
    drawnow
end
