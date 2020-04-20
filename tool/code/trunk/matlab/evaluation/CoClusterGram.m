function [cocl]=CoClusterGram(Data,Label,Option)

%---------------------------------------------------------
% Data is NxM matrix of features, N is the number of patients, M is the number of Features
% Label is Nx1 vector of labels
% Option.K is the maximum number of clusters
% Option.PerPatient=0 or 1, for normalizing the matrix of features per row
% Option.PerFeature=0 or 1, for normalizing the matrix of features per column
% 
% Default values:
% Option.K=40;
% Option.PerPatient=0
% Option.PerFeature=1
%
% Written By, Mehdi Alilou
% me.alilou@gmail.com
%----------------------------------------------------------
groupsAll=Label;
dataAll=Data;

%% dataAll
%-- Normalize per Feature

if ~exist('Option.PerFeature')
    Option.PerFeature=1;
end
if ~exist('Option.PerPatient')
    Option.PerPatient=0;
end

if ~exist('Option.K')
    Option.K=40;
end

if Option.PerFeature==1
    for nc=1:size(dataAll,2)
        dataAll(:,nc)=dataAll(:,nc)./norm(dataAll(:,nc));
    end
end

%-- Normalize per Paitient
if Option.PerPatient==1
    for nc=1:size(dataAll,1)
        dataAll(nc,:)=dataAll(nc,:)./norm(dataAll(nc,:));
    end
end
%-- Rescaling
for j=1:size(dataAll,2)
    rangej= max(dataAll(:,j))-min(dataAll(:,j));
    dataAll(:,j)=(dataAll(:,j)-min(dataAll(:,j)))/rangej;
    x=0;y=1;
    range2 = y - x;
    dataAll(:,j) = (dataAll(:,j)*range2) + x;
end


%% kmeans
K=Option.K;

cocl=zeros(length(Label),length(Label));
for p=1:3
    for k=2:K
        
        switch p
            case 1
                idx=kmeans(dataAll(:,:),k,'distance','sqeuclidean','MaxIter',1000,...
                    'Display','final','Replicates',5);
            case 2
                idx=kmeans(dataAll(:,:),k,'distance','cityblock','MaxIter',1000,...
                    'Display','final','Replicates',5);
                
            case 3
                try
                idx=kmeans(dataAll(:,:),k,'distance','correlation','MaxIter',1000,...
                    'Display','final','Replicates',5);
                catch
                end
        end
        for intk=1:k
            coinds = find(idx==intk);
            
            for ii=1:size(coinds,1)
                for jj=1:size(coinds,1)
                    cocl(coinds(ii),coinds(jj))=cocl(coinds(ii),coinds(jj))+1;
                end
            end
        end
    end
end
for nc=1:size(cocl,2)
    cocl(:,nc)=cocl(:,nc)./norm(cocl(:,nc));
end
for nc=1:size(cocl,1)
    cocl(nc,:)=cocl(nc,:)./norm(cocl(nc,:));
end
% %--
for j=1:size(cocl,2)
    rangej= max(cocl(:,j))-min(cocl(:,j));
    cocl(:,j)=(cocl(:,j)-min(cocl(:,j)))/rangej;
    x=-1;y=1;
    range2 = y - x;
    cocl(:,j) = (cocl(:,j)*range2) + x;
end
% cocl=cocl/(3*(K-1));

%-- Plotting
HeatMap(cocl,'RowLabels',groupsAll,'ColumnLabels',groupsAll,'Colormap','pink');
