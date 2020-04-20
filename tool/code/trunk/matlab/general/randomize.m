function [randomized, uniquerands] = randomize(numbers)
% RANDOMIZE Shuffle the elements of a 1D array.
%   RANDOMIZE(orderedarray) returns the the elements of the input
%   orderedarray in random order.
%
%   See also randperm.
%
%JC

% Sort random numbers
[uniquerands,I]=sort(ceil(rand(1,length(numbers))*1e8)); % multiply times 1e8 to reduce chance of repeats

% Reorder using sorting indexes found above
randomized=numbers(I);
