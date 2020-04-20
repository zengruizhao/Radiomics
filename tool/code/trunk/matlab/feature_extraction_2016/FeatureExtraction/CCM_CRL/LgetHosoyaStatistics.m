function LgetHosoyaStatistics(HosoyaF,labels,set_alpha)
%% check out the hosoya range for each CCG density
Hosoya_cat=[]; Hosoya_range=[];
Hosoya_Non=[];Hosoya_Pro=[];
for i=1:size(HosoyaF,1)
    Hosoya_cur=[];Pro=[];Non=[];
    for j=1:size(HosoyaF,2)
        temp=cell2mat(HosoyaF{i,j});
        Hosoya_cur=[Hosoya_cur temp];
        if labels(j)
           Pro=[Pro temp];
        else
            Non=[Non temp];
        end
    end
    tmp=unique(Hosoya_cur);
    [stmp]=sort(tmp,'descend');
    
    Hosoya_cur(Hosoya_cur==Inf)=stmp(2);
    Non(Non==Inf)=stmp(2);
    Pro(Pro==Inf)=stmp(2);
    
    Hosoya_range(i,1)=min(Hosoya_cur);
    Hosoya_range(i,2)=max(Hosoya_cur);    
    Hosoya_All{i}=Hosoya_cur; 
    Hosoya_Non{i}=Non;
    Hosoya_Pro{i}=Pro;    
end
%% get the statistics for each patient
HF_stat=[];
for i=1:size(HosoyaF,1)
    for j=1:size(HosoyaF,2)       
        temp=cell2mat(HosoyaF{i,j});
        tmp=unique(temp);
        [stmp]=sort(tmp,'descend');
        if sum(temp==Inf)>0
            temp(temp==Inf)=stmp(2);
        end
        
        HF_stat{i,j}=[mean(temp),...
            median(temp),...
            std(temp),...
            range(temp),kurtosis(temp),...
            skewness(temp)];
    end
end
modifier = [{'mean'} {'median'} {'std'} {'range'} {'kurtosis'} {'skewness'} ];

%% seperate the class and compare in terms of statistics
Pro=[];Non=[];
for i=1:size(HosoyaF,1)
    for j=1:size(HosoyaF,2)       
        temp=HF_stat{i,j};
        if labels(j)
            Pro(i,j,:)=temp;
        else
            Non(i,j,:)=temp;
        end
    end
end

%% plot out the boxplot for each CCG dense level and each statistics
for i=1:3
    h=figure(i);
    set(h,'Name',['a=' num2str(set_alpha(i))]);
    for j=1:size(Pro,3)
        data=[Pro(i,labels==1,j) Non(i,labels==0,j)];
        subplot(3,2,j);boxplot(data,labels);title(modifier{j});
    end
end