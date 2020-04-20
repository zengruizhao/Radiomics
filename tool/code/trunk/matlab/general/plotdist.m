function plotdist(data,linecolor)
% plotdist(DATA,LINECOLOR)
% Plot distribution of input variable DATA 
% Note: 
% 1. Will round data to ensure that an actual distribution can be built
% 2. Will smooth out distribution by convolving with a Gaussian
% (fspecial,sigma=25)
% 3. Needs function 'count' to be your path
% 4. Assumes you want to "delete" counts for graylevel = 0 (look at line 26).
%
% Satish Viswanath, Jan 2013
%
% SEE ALSO: int_stdn.m, int_stdn_landmarks.m

if logical(~exist('count','file'))
	error('Get COUNT into your path!');
end 

checkifInt = find(data); checkifInt = data(checkifInt(1));
if ceil(checkifInt) ~= floor(checkifInt)    
    disp('Rounding non-integer values in DATA');
    data = round(data);
end

[cnts,whtcnted] = count(data(:));
if min(whtcnted) == 0
    %dropping graylevel = 0, and corresponding count
    cnts = cnts(2:end);whtcnted = whtcnted(2:end);      
end

k = fspecial('average',[1 25]);
smoothhist = conv(cnts/sum(cnts),k,'same'); 
plot(whtcnted,smoothhist,linecolor, 'LineWidth', 2);

end