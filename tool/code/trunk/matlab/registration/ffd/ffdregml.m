function varargout=ffdregml(targetset,templateset,N,fillwith,measure,nodisp,npx,npy)
%function varargout=recoverTPSdefbyLMS(slicenum,N,field_grade,noise_level,usedfeatnums,nodisp,useintensity)

%% Register with COFEMI-FFD/COLLINARUS
% Initialize registration results
% targetsetFFDrecovered=zeros(size(targetsetTPSwarpedN),class(Itemplate));

% Save parameters
regresults.N=N;

% Initial CMI - with bias field & noise & deformations
regresults.initialCMI=CMIgen(reshape(templateset,size(templateset,1)*size(templateset,2)*size(templateset,3),size(templateset,4)),...
    reshape(targetset,size(targetset,1)*size(targetset,2)*size(targetset,3),size(targetset,4)),N);

% Perform COFEMI-FDD registration
% npx=[5 6 7];
% npy=[5 6 7];
% hierarchical mesh points that move
% xcpoptlim1=[1 5]; ycpoptlim1=[1 5];
% xcpoptlim2=[1 6]; ycpoptlim2=[1 6];
% xcpoptlim3=[1 7]; ycpoptlim3=[1 7];
% npx=[5 6 8];
% npy=[5 6 8];
% npx=[4 5 6 8];
% npy=[4 5 6 8];
% npx=[4 ];
% npy=[4 ];
nffdlevels=numel(npx);
% hierarchical mesh points that move
xcpoptlims=zeros(nffdlevels,2); ycpoptlims=zeros(nffdlevels,2);
xcpoptlims(:,1)=1; xcpoptlims(:,2)=npx(:);
ycpoptlims(:,1)=1; ycpoptlims(:,2)=npy(:);
% xcpoptlims(1,:)=[1 npx(1)]; ycpoptlims(1,:)=[1 npy(1)];
% xcpoptlims(2,:)=[1 npx(2)]; ycpoptlims(2,:)=[1 npy(2)];
% xcpoptlims(3,:)=[1 npx(3)]; ycpoptlims(3,:)=[1 npy(3)];
% xcpoptlims(4,:)=[1 npx(4)]; ycpoptlims(4,:)=[1 npy(4)];

tlocalxilv=[]; tlocalyilv=[];
for lv=1:nffdlevels, % levels
    [targetsetFFDrecovered,optgriddispl,Pxdispl,Pydispl,Px,Py,MIiters,meshdata,tlocalx,tlocaly] ...
        = ffdreg2(targetset,templateset,npx(lv),npy(lv),N,...
        'quiet',nodisp,'xcpoptlim',xcpoptlims(lv,:),'ycpoptlim',ycpoptlims(lv,:),...
        'fillwith',fillwith,'measure',measure,...
        'pretlocalx',tlocalxilv,'pretlocaly',tlocalyilv);
    tlocalxilv=tlocalx; tlocalyilv=tlocaly;
end

close all

% targetsetFFDrecovered=forwarddisp_testing(targetset,tlocalx,tlocaly);
Itarget_recovered=targetsetFFDrecovered(:,:,:,1);

regresults.optgriddispl=optgriddispl;
regresults.Pxdispl=Pxdispl;
regresults.Pydispl=Pydispl;
regresults.Px=Px;
regresults.Py=Py;
regresults.MIiters=MIiters;
regresults.meshdata=meshdata;
regresults.tlocalx=tlocalx;
regresults.tlocaly=tlocaly;
regresults.Iout=targetsetFFDrecovered;

% PDrecovered=forwarddisp_testing(Itarget_in_tps,tlocalx,tlocaly);
% PDrecoveredN=targetsetFFDrecovered(:,:,1);
% 
% regresults.PDrecovered=PDrecovered;
% regresults.PDrecoveredN=PDrecoveredN;

% figure;
% imdisp(PDrecovered)

% Itarget_in_clipped=rescale(uint8(Itarget_in)); % uint8 to clip
% regresults.Itarget_in_clipped=Itarget_in_clipped;
% figure;
% imdisp(regresults.Itarget_in_clipped)
% 
% figure;
% imdisp(regresults.Itarget_in_tps)

if nargout>0,
    varargout{1}=regresults;
    if nargout>1,
        varargout{2}=Itarget_recovered;
    end
end
