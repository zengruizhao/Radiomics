function [ajendiff,HaXY,HaX,HaY,beta] = ajensen(X,Y,k,gamma,alpha,HaX,HaY)

[nptsX,ndimsX]=size(X);
[nptsY,ndimsY]=size(Y);

if nptsX~=nptsY, error('X and Y must have the same number of points.'); end
if ndimsX~=ndimsY, error('X and Y must have the same number of dimensions.'); end

if nargin<5, alpha=[]; end

if nargin<7,
    [HaY,mstlength] = mstentropy(Y,k,gamma,alpha); %#ok<*NASGU>
%     mstlength
    if nargin<6,
        [HaX,mstlength] = mstentropy(X,k,gamma,alpha);
%         mstlength
    end
end
k=k*2; [HaXY,mstlength] = mstentropy([X; Y],k,gamma,alpha); % vertcat
%k=round(k*.7); [HaXY,mstlength] = mstentropy([X Y],k,gamma,alpha); % horzcat
% mstlength

beta=nptsX/(nptsX+nptsY);

ajendiff=HaXY - beta*HaX - (1-beta)*HaY;
