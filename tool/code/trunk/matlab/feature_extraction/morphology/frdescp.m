function z = frdescp(s)

%FRDESCP Computes fourier descriptors.
% Z=FRDESCP(S) computes the Fourier descriptors of S, which is an np by 2
% sequence o image coordinates describing a boundary
%
%Due to symmetry considerations when working with inverse Fourier
%descriptors based on fewer than np terms, the number of points in S when
%computing the descriptos must be even.  If the number of points is odd,
%FRDESCP duplictates the end point and adds it at the end of the sequence.
%If a different treatment is desired, the sequence must be processed
%externally so that it has even number of points.
% 
% see IFREDESCP for computing the inverse descriptors

%preliminaries
[np,nc]=size(s);
if nc ~= 2
    error('S must be of size np-by-2');
end
if np/2 ~= round (np/2);
    s(end+1,:)=s(end,:);
    np=np+1;
end

%create an alternating sequence of 1s and -1s for use in centering the
%transform

%I changed this by just translating the lesions
%Rachel Sparks
x=0:(np-1);
x_c=mean(s(:,1));
y_c=mean(s(:,2));
%Multiply the input sequence by alternating 1s and -1s to center the
%transform.

s(:,1)=s(:,1)-x_c;
s(:,2)=s(:,2)-y_c;
%convert coordinates to complex numbers.
s=s(:,1)+i*s(:,2);
[dummy,ind]=sort(angle(s));
s=s(ind);
%compute the descriptors
%take the abs of the fft to remove rotational effects
%-RS
z=abs(fft(s));

%normalizing wrt first Fourier Descriptor component
%-RS
z=z/z(2);