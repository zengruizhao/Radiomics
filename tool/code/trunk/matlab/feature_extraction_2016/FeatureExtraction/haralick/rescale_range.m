function Iout = rescale_range(I,N1,N2)
%Rescaling to a range of values
%function Iout = rescale_range(I,N1,N2)
%N2 > N1

if ~isa(I,'double'),
    I=double(I);
end

if (max(I(:)) - min(I(:))) > eps,
    datarange=max(I(:))-min(I(:));
    wantedrange=N2-N1;
    Iout = N1+(I-min(I(:)))/(datarange/wantedrange);
else
    Iout=I;
end