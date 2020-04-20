function [shufd,it,tries,endt]=shuffle(inpt,sequential,sequential_parity,quiet)
% SHUFFLE Shuffle an input array with constraints.
%   SHUFFLED=SHUFFLE(X) shuffles array X once and outputs the result.
%
%   SHUFFLED=SHUFFLE(X,NSEQUENTIAL) shuffles array X until no
%   value in X occurs consecutively more than NSEQUENTIAL times.
%
%   SHUFFLED=SHUFFLE(X,NSEQUENTIAL,NSEQUENTIAL_PARITY) shuffles
%   array X until no value in X occurs consecutively more than
%   NSEQUENTIAL times.  Also, no elements of the same parity
%   (i.e. evens/odds) may occur consecutively more than NSEQUENTIAL_PARITY
%   times.
%
%   [SHUFFLED,TRIES,DURATION]=SHUFFLE(...) returns the shuffled array,
%   SHUFFLED, the number of times shuffles, TRIES, and the time to complete
%   the shuffle, DURATION.
%
%   shuffle(repmat(1:4,1,4),Inf,1)+shuffle(repmat(1:4,1,4),Inf,1)*i
%
% chappjc@eden.rutgers.edu

% Make sure input  is 1-by-N
if size(inpt,1)~=1,inpt=inpt(:)';end

% Initialize
% len=length(inpt);
shufd=inpt;
done=0;
tries=0;

% Parse inputs
if nargin<4,
    quiet=1;
    if nargin<3,
        sequential_parity=Inf;
        if nargin<2,
            sequential=Inf;
        end
    end
end
last=[];

% Counnt the number of occurences of each unique element in input array
uniq=unique(inpt); occurrences=zeros(1,length(uniq));
for elem=1:length(uniq), occurrences(elem)=length(find(inpt==uniq(elem)));end

% Which unique element occurs most?
whichmax=uniq(occurrences==max(occurrences));

% Convert input array to parity boolean array
parity_inpt=mod(shufd,2);

% Count even and odd elements
parity_count(1)=length(find(parity_inpt)); %odd count
parity_count(2)=length(find(parity_inpt==0)); %even count

% Which occurs more often?
whichmax_parity=find(parity_count==max(parity_count)); % 1=odd 2=even
if whichmax_parity==1,whichmax_parity='odd';else whichmax_parity='even';end

% Check if there are enough remaining element to properly shuffle the most
% prevalent element.
if length(inpt)-max(occurrences)<max(occurrences)/sequential-1,
    % Remainder should not be less than most occuring element./allowed
    % sequential minus one on the end.
    fprintf('There is no way to achieve the requested shuffle.\n');
    fprintf('Too many %d''s', whichmax);
    error('sorry!')
end
if sequential*(length(inpt)-max(occurrences))<max(occurrences)-sequential,
    % Remainder * allowed sequential should not be less than occurrences - 
    % extra on end.
    fprintf('There is no way to achieve the requested shuffle.2\n');
    fprintf('Too many %d''s', whichmax);
    error('sorry!')
end
if sequential*(length(inpt)-max(occurrences))+sequential<max(occurrences),
    % Remainder * allowed sequential + extra sequence on end should not be 
    % less than the most occuring number.
    fprintf('There is no way to achieve the requested shuffle.3\n');
    fprintf('Too many %d''s', whichmax);
    error('sorry!')
end

% Check if parity constraints are feasible
if length(inpt)-max(parity_count)<max(parity_count)/sequential_parity-1,
    % Remainder should not be less than most occuring element./allowed
    % sequential minus one on the end.
    fprintf('There is no way to achieve the requested shuffle.\n');
    fprintf('Too many %ss.', whichmax_parity);
    error('sorry!')
end

if ~quiet,
    fprintf('Trying combinations:\n')
    tic;
end

whatever=round(clock);  % seed for random number generator
rand(whatever(6),1);    % throw it out

% Beging trying combinations
while ~done,

    % We'll randomize the array order without sort:
    [it,shufd]=randomizer(inpt);

    % ...or the easy way:
    % it=randperm(len);
    % shufd=inpt(it);

    % Print progress in console
    tries=tries+1;
    if (mod(tries,2000)==0),
        fprintf(repmat('\b',1,length(last)));
        fprintf(num2str(tries));
        drawnow;
        last=num2str(tries);
    end

    % Convert array to parity boolean
    parity=mod(shufd,2);

    % See if we meet the shuffling criteria
    if max(diff([0 find(diff(shufd)) length(shufd)]))<=sequential && ...
            max(diff([0 find(diff(parity)) length(parity)]))<=sequential_parity,
        break
    end
end

% Inform the user of how long the shuffling took
fprintf(repmat('\b',1,length(last)));
if ~quiet,
    endt=toc;warning('off','MATLAB:divideByZero')
    fprintf(['\b\nSuccessful in ' num2str(tries) ' tries\n']);
    if isfinite(tries/endt),
        fprintf([num2str(tries/endt) ' tries/second\n']);
    end
    warning('on','MATLAB:divideByZero')
end

function [it,shufd]=randomizer(sequ)
% Randomize array order
len=length(sequ);
j=floor(rand(1,len).*((len-(1:len))+1)+(1:len));    %pick a num j from 1:length
shufd=sequ;
it=1:len;                            % ind tracker
for i=1:len-1,
    temp=shufd(i);                  % swap i with j
    shufd(i)=shufd(j(i));
    shufd(j(i))=temp;
    ittemp=it(i);                   % remember what went where
    it(i)=it(j(i));
    it(j(i))=ittemp;
end
