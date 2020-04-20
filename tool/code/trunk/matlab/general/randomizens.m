function [shufd,it]=randomizens(sequ)
% Randomize array order

len=length(sequ);
it=1:len;                            % ind tracker
j=floor(rand(1,len).*(len-it+1)) + it;    %pick a num j from 1:length
shufd=sequ;
getits=nargout==2;
for i=1:len-1,
    % Swap elements i and j in the sequence
    temp=shufd(i);
    shufd(i)=shufd(j(i));
    shufd(j(i))=temp;
%     shufd([i j(i)])=shufd([j(i) i]);
    if getits,
        % Remember where the elements go
        ittemp=it(i);
        it(i)=it(j(i));
        it(j(i))=ittemp;
    end
end
