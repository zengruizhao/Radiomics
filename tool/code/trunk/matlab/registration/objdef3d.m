function err = objdef3d(rigidparams,I,Itemp,fillwith,N,nodisp,paramweights,measure,interpmethod,paramoffsets,coordshift,iszpadded,preM,usebox)
% objective function for MI based registration

persistent err_record regiter

if nargin==0,
    err=err_record(1:regiter);
    clear err_record regiter
    return;
end

if isempty(regiter),
    regiter=0;
    err_record=zeros(1,2e3);
end
regiter=regiter+1;

% Weight parameters so optimization routine works - REMEMBER THESE VALUES
rots=rigidparams(1:3)'.*paramweights(1:3)'+paramoffsets(1:3)';
trans=rigidparams(4:6)'.*paramweights(4:6)'+paramoffsets(4:6)';
% Scaling is optional
nscales=numel(rigidparams)-6;
if nscales,
    if nscales==1,
        scale=(rigidparams([7 7 7])'-1).*paramweights([7 7 7])' + 1;
    elseif nscales==2,
        scale=(rigidparams([7 7 8])'-1).*paramweights([7 7 8])' + 1;
    else % nscales==3
        scale=(rigidparams(7:9)'-1).*paramweights(7:9)' + 1;
    end
else % nscales==0
    scale=[1 1 1];
end
scale=scale+paramoffsets(7:9)'; % input paramoffsets must be 9-by-1

% Don't necessarily convert to double, leave in single, etc. to speed things up
Inew=imdef3([rots trans scale],I,interpmethod,fillwith,'preM',preM,'coordshift',coordshift);
Inew=round(Inew); % this should be a noop for 'nearest'

if strcmpi(interpmethod,'cubic'),
    Inew(Inew<0)=0;
    Inew(Inew>(N-1))=N-1;
end

% Same size images
Itempsize=size(Itemp);
if any(Itempsize~=size(Inew)) || iszpadded,
    % crop to same size (evenly in-plane, high slices in z, and any pad slice)
    Inew=cropvolume(Inew,Itempsize,double(iszpadded)); % must be done the same way in affinereg3, after optimization!
%     Inew=imshave(Inew,Itempsize); % 2D frame
%     % 3D slices, crop target (instead of pad template)
%     if iszpadded, padslice=1; else padslice=0; end
%     if Itempsize(3)~=size(Inew,3),
%         extraslices=size(Inew,3)-Itempsize(3);
%         Inew=Inew(:,:,(1:end-extraslices)+padslice); %1:Itempsize(3)
%     end
end

% Similarity measure - no multiattribute yet.
QQMI=nargin==14 && ~isempty(usebox);
if QQMI,
    % lessyxz=(Itempsize-usebox)/2; lessyxz(lessyxz<0)=0;
    if numel(usebox)<=2,
        if numel(usebox)==1, usebox=usebox([1 1]); end
        usebox=[Itempsize/2-usebox/2+1 usebox];
        toobig=usebox(3:4)>size(Inew); usebox(toobig)=Itempsize([toobig false]);
    end
    usebox=round(usebox); usebox(usebox<1)=1;
    switch lower(measure)
        case 'mi'
%             err=mimex(Itemp(lessyxz(1)+1:end-lessyxz(1),lessyxz(2)+1:end-lessyxz(2),lessyxz(3)+1:end-lessyxz(3)),...
%                 Inew(lessyxz(1)+1:end-lessyxz(1),lessyxz(2)+1:end-lessyxz(2),lessyxz(3)+1:end-lessyxz(3)),N);
            err=mimex(Itemp(usebox(2)+(1:usebox(4)), usebox(1)+(1:usebox(3)), :),...
                Inew(usebox(2)+(1:usebox(4)), usebox(1)+(1:usebox(3)), :),N);
        case 'nmi'
            % [~,~,~,~,err]=MI(Itemp,Inew,N);
            [~,err]=mimex(Itemp(usebox(2)+(1:usebox(4))-1, usebox(1)+(1:usebox(3))-1, :),...
                Inew(usebox(2)+(1:usebox(4))-1, usebox(1)+(1:usebox(3))-1, :),N);
        case 'ecc'
            [~,~,err]=mimex(Itemp(usebox(2)+(1:usebox(4))-1, usebox(1)+(1:usebox(3))-1, :),...
                Inew(usebox(2)+(1:usebox(4))-1, usebox(1)+(1:usebox(3))-1, :),N);
        case 'je'
            [~,~,~,err]=mimex(Itemp(usebox(2)+(1:usebox(4))-1, usebox(1)+(1:usebox(3))-1, :),...
                Inew(usebox(2)+(1:usebox(4))-1, usebox(1)+(1:usebox(3))-1, :),N);
            err=-err;
        case 'eoh'
            [~,~,~,~,~,~,err]=mimex(Itemp(usebox(2)+(1:usebox(4))-1, usebox(1)+(1:usebox(3))-1, :),...
                Inew(usebox(2)+(1:usebox(4))-1, usebox(1)+(1:usebox(3))-1, :),N);
        case 'euclidean'
            err = -sqrt(sum(sum((Itemp(usebox(2)+(1:usebox(4))-1, usebox(1)+(1:usebox(3))-1, :) - ...
                Inew(usebox(2)+(1:usebox(4))-1, usebox(1)+(1:usebox(3))-1, :)).^2)));
        otherwise
            error('Measure name not recognized.');
    end
else
    switch lower(measure)
        case 'mi'
            % err=MI(Itemp,Inew,N);
            err=mimex(Itemp,Inew,N);
        case 'nmi'
            % [~,~,~,~,err]=MI(Itemp,Inew,N);
            [~,err]=mimex(Itemp,Inew,N);
        case 'ecc'
            [~,~,err]=MI(Itemp,Inew,N);
        case 'je'
            [~,~,~,err]=MI(Itemp,Inew,N);
            err=-err;
        case 'eoh'
            [~,~,~,~,~,~,err]=MI(Itemp,Inew,N);
        case 'euclidean'
            err = -sqrt(sum(sum((Itemp-Inew).^2))); % -norm(Itemp(:) - Inew(:));
        otherwise
            error('Measure name not recognized.');
    end
end

err=-err;
% err_record(end+1)=err;
err_record(regiter)=err;

if ~nodisp,
%     titlestring=get(get(gca,'title'),'String');
    fprintf('Deforming: ');
    fprintf([repmat('%6.4f ',1,3) repmat('%8.4f ',1,3) repmat('%6.4f ',1,3) ''],[rots trans scale]);
    fprintf('\n');
    % preserve title
    titlestring=get(get(gca,'title'),'String');
    imdisp(Inew(:,:,round(size(Inew,3)/2)))
    title(titlestring)
    if QQMI,
%         hold on;plot([lessyxz(2)+1 size(Inew,2)-lessyxz(2) size(Inew,2)-lessyxz(2) lessyxz(2)+1  lessyxz(2)+1],...
%             [lessyxz(1)+1 lessyxz(1)+1 size(Inew,1)-lessyxz(1) size(Inew,1)-lessyxz(1) lessyxz(1)+1],'LineWidth',2)
%         subplot(121)
%         hold on;plot([lessyxz(2)+1 size(Inew,2)-lessyxz(2) size(Inew,2)-lessyxz(2) lessyxz(2)+1  lessyxz(2)+1],...
%             [lessyxz(1)+1 lessyxz(1)+1 size(Inew,1)-lessyxz(1) size(Inew,1)-lessyxz(1) lessyxz(1)+1],'LineWidth',2)
        hold on;plot([usebox(1)+1 usebox(1) usebox(1)+usebox(3) usebox(1)+usebox(3) usebox(1)+1],...
            [usebox(2)+1 usebox(2)+usebox(4) usebox(2)+usebox(4) usebox(2)+1 usebox(2)+1],'LineWidth',2)
        subplot(121)
        hold on;plot([usebox(1)+1 usebox(1) usebox(1)+usebox(3) usebox(1)+usebox(3) usebox(1)+1],...
            [usebox(2)+1 usebox(2)+usebox(4) usebox(2)+usebox(4) usebox(2)+1 usebox(2)+1],'LineWidth',2)
        subplot(122)
    end
    drawnow
end
