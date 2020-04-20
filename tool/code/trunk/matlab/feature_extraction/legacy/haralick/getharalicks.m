%function getharalicks(vols)
%Saves 13 Haralick features x 3 window sizes x 3 gray level ranges to disk
%Uses haralick.m
%Satish Viswanath
clear; close all; clc;
path = '/home/satish/';
%path = 'Z:/';
addpath([path,'Code/matlabcode/general/']);
xdim = 256;ydim=256;nfeat=13;

for i=[67,68,71,72,73,113,117,122,123,124,125,126,128,129,130,131,132,133,134]
%for i=[133,134]
	fprintf('\n-----------------------------Set %d----------------------------\n',i);
    load([path,'InvivoMATFiles/ACRIN/',num2str(i),'.mat'],'data');
    %v = volopen(['FinalImageSets/',vols{i}],[xdim,ydim],'single');
    v = data.images;
    zdim = size(v,3);
	load([path,'Masks/',num2str(i),'.mat']);
    if size(mask,3) ~= zdim, fprintf('Problem with Set %d',i); continue; end
    
    n=0;
    dist = 1;
    Hf = zeros(xdim,ydim,zdim,nfeat);
    for grays = [64,128,256]
        for win = [1,2,3]
            n = n+1;
            fprintf('\nWindow-size: %d & Graylevels: %d \n',win,grays);
            if (win==1)||(win==2)
                for sl=1:zdim
                    warning('off')
                    img = uint16(rescale_range(v(:,:,sl),0,grays-1));                    
                    fprintf('Slice %d: ',sl);
                    f = haralick(img,mask(:,:,sl),grays,win,dist,1); %#ok<COLND>
                    Hf(:,:,sl,:) = f.img3;                  
                    %fprintf('\n');
                    warning('on')
                end
            else
                v1=zeros(xdim,ydim,zdim+2);mask1=logical(zeros(xdim,ydim,zdim+2));
                v1(:,:,2:zdim+1) = v;
				mask1(:,:,2:zdim+1) = mask;
                for sl=2:zdim+1
					warning('off')
                    img = uint16(rescale_range(v1(:,:,sl-1:sl+1),0,grays-1));
					warning('on')
                    fprintf('Slice %d: ',sl-1);
                    f = haralick(img,mask1(:,:,sl-1:sl+1),grays,win,dist,1);
                    Hf(:,:,sl-1,:) = f.img3;
                    %fprintf('\n');                
                end
            end
            HaralickFeat{n} = single(Hf);
        end
    end
    save([path,'Features/',num2str(i),'.mat'],'HaralickFeat','-append');    
end
    
