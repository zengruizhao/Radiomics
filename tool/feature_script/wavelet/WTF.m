function f=WTF(I)
% compute the 128-D wavelet transform feature of image I  
% Input:  
% I: the input image  
% Output:  
% f:128-D feature  
%%warning:normalization must be performed after all the features are extracted from the dataset    
  
%I=imread('D:/2.jpg');  
if(ndims(I)==3) I=rgb2gray(I);end  
  
wname='db8';  
[Lo_D,Hi_D,Lo_R,Hi_R] = wfilters(wname);  
  
scale=3;  
  
%% PWT feature,24-D array (4+4+4)*2  
cA=I;  
f=[];  
for i=1:scale  
    [c,s]= wavedec2(cA,1,Lo_D,Hi_D);  
    msize=s(1,1)*s(1,2);  
    cA=c(1,1:msize);  
    %imshow(uint8(cA));  
    cA=reshape(cA,s(1,1),s(1,2));  
    %cH cV cD  
    cH=c(1,msize+(1:msize));  
    cV=c(1,2*msize+(1:msize));  
    cD=c(1,3*msize+(1:msize));  
    cH=reshape(cH,s(2,1),s(2,2));  
    cV=reshape(cV,s(2,1),s(2,2));  
    cD=reshape(cD,s(2,1),s(2,2));  
      
    %figure,colormap gray  
    %subplot(221),imagesc(cA);  
    %subplot(222),imagesc(cH);  
    %subplot(223),imagesc(cV);  
    %subplot(224),imagesc(cD);  
    %X   = waverec2(c,s,'db8');  
    %f=[mean(mean(cA) std2(cA)];  
    f=[f mean2((abs(cA))) std2(abs(cA)) mean2((abs(cH))) std2(abs(cH)) mean2((abs(cV))) std2(abs(cV)) mean2((abs(cD))) std2(abs(cD))];  
    %f=[f mean2(((cA))) std2((cA)) mean2(((cH))) std2((cH)) mean2(((cV))) std2((cV)) mean2(((cD))) std2((cD))];  
end  
f1=f;  
  
f=[];  
  
%% TWT feature,104-D 4*(1+3+9)*2 array  
set=[];  
candidate={};  
cA=I;  
set{1}=cA;%%store the frequency (LL,LH,HL) to be decomposed  
candidate{1}=cA;%%  
  
for j=1:scale  
    count=0;  
    set=candidate;  
    for i=1:length(set)  
            cA=set{i};  
        %for i=1:3  
            [c,s]= wavedec2(cA,1,Lo_D,Hi_D);  
            msize=s(1,1)*s(1,2);  
            cA=c(1,1:msize);  
            %imshow(uint8(cA));  
            cA=reshape(cA,s(1,1),s(1,2));  
            %cH cV cD  
  
            cH=c(1,msize+(1:msize));  
            cV=c(1,2*msize+(1:msize));  
            cD=c(1,3*msize+(1:msize));%%HH level   
            cH=reshape(cH,s(2,1),s(2,2));  
            cV=reshape(cV,s(2,1),s(2,2));  
            cD=reshape(cD,s(2,1),s(2,2));%%HH  
            count=count+1;  
            candidate{count}=cA;  
            count=count+1;  
            candidate{count}=cH;  
            count=count+1;  
            candidate{count}=cV;  
            %figure,colormap gray  
            %subplot(221),imagesc(cA);  
            %subplot(222),imagesc(cH);  
            %subplot(223),imagesc(cV);  
            %subplot(224),imagesc(cD);  
            %%%%%X   = waverec2(c,s,'db8');  
            %f=[mean(mean(cA) std2(cA)];  
            f=[f mean2((abs(cA))) std2(abs(cA)) mean2((abs(cH))) std2(abs(cH)) mean2((abs(cV))) std2(abs(cV)) mean2((abs(cD))) std2(abs(cD))];  
        %end  
    end  
    count ; 
end  
f2=f;  
  
f=[f1 f2];  