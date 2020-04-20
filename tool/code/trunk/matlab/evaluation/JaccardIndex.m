function JI = JaccardIndex( argins )
%JaccardIndex Returns the Jaccard index of the data 
%   JaccardIndex(A,B) = A&&B/A||B
%   data is a 4 dim matrix where the 4th dimension corresponds to each
%   entry

dims  = size(size(argins),2);

if (dims<4)
    disp(['Input must be either a 4D dataset or two 3D datasets ']);
    JI = 0;
    return;
end

data = argins;

I = data(:,:,:,1);
U = I;

for i = 2:size(argins,dims)
    I = I & data(:,:,:,i);
    U = U | data(:,:,:,i);
end

if sumall(U) ~= 0
    JI = sumall(I)/sumall(U);
else
    JI = 0;
end

end

