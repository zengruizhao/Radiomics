function [N W pos] = callaghan(binmap,Tr,Tt,res,idx)
% clear all; clc; close all;
% multiplier for distance constraint
% Tr = 10;
% Tt = 30;

if nargin < 5
    idx = [];
end

% input
% binmap = imread('SGOT-001-b_ANB_BCbin.tif');
[X Y] = find(binmap(:,:,1)); pos=[X Y];
N = cell(1,length(X));


% calculating d(Pi,Pj)
W = squareform(pdist([X Y]));
isValid = true(size(W)); isValid(W==0)=false;

% for stupid reasons
W_alt = W;
W_alt(W_alt == 0) = NaN;
W_alt_2 = W_alt.^2;
 
%% distance constraints
% calculating d(Pi,Pi1) * Tr
distBound = min(W_alt) * Tr / res;
distBound = repmat(distBound,size(W,1),1);

% keep only d(Pi,Pj) <= d(Pi,Pi1) * Tr
isValid(W > distBound) = false;

%% direction constraints
direcTest = @(Pi,Pj,Pk) (W_alt_2(Pj,Pi)+W_alt_2(Pk,Pj)-W_alt_2(Pk,Pi))/(W_alt(Pj,Pi)*W_alt(Pk,Pj));
Ta = 2*cosd(Tt);
% Da=[];  
% D= [];

if isempty(idx)
    idx = 1:length(X); % if user passes idx, we use it to limit the pixels being examined
end

for i=idx % i is index of pixel we are examining
    curValid = isValid(:,i);
    [unused ord] = sort(W_alt(:,i));
    for j=1:length(ord) % ord(j) is index of all other pixels (in ascending order) (for reference)
        if curValid(ord(j))
            for k=1:length(ord) % ord(k) is index of all other pixels (for testing)
                %compare X values for i and j and then if i is smaller then
%          D=direcTest(i,ord(j),ord(k));
%          Da(end+1)=D;
                if curValid(ord(k)) && ord(j)~=ord(k) && direcTest(i,ord(j),ord(k)) < Ta
                    curValid(ord(k)) = false;             
                    isValid(ord(k),i) = false;
                end
               
            end
        end
    end
    N{i} = find(isValid(:,i));
%     A=acosd(Da/2);
end
% A = cell(1,length(N));
% A1 = cell(1,length(N));
% for x=1:length(N)
%     N1=N{x};
%     D=A1{x};
%     for y=1:length(N1)
%         for z=1:length(N1)
%             if y~=z
%                D(end+1)=direcTest(x,N1(y),N1(z));
%             end
%         end
%         
%         
%     end
%      A{x}=acosd(D/2);
% end
     