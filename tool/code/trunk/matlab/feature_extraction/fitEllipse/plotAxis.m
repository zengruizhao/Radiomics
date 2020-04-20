function [handle]=  plotAxis(bounds, axis)

handle = figure();


    % translate object so the range of the location is [1, inf] for x and y
    % location
    centered_r = ((bounds.r-min(bounds.r)))+1;
    centered_c = ((bounds.c-min(bounds.c)))+1;
    
    plot(centered_r, centered_c, '.b');
    
    hold on;
    
    plot( [0  range(centered_r)*axis(1)]+mean(centered_r), [0  range(centered_c)*axis(2)]+mean(centered_c),'-g');
end
