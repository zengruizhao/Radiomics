%---------------    VERY IMPORTNAT  ------------
%    There are two hypotheses, Hyp1 and Hyp2, such as malignant and benign
%    There are N1 and N2 samples for Hyp1 and Hyp2, respectively.
%    There are NumFtrs number of features.
%    Data corresponding to two hypotheses should in the following format:
%    Hyp1 = [N1 x NumFtrs]   % The first hypothesis, such as malignant
%    Hyp2 = [N2 x NumFtrs]   % The second hypothesis, such as benign

IndexDomFtrs=1;           % Index of the plotting feature
figure
PlotFtrHyp1=Hyp1(:,IndexDomFtrs);
PlotFtrHyp2=Hyp2(:,IndexDomFtrs);
group = [ repmat({'Hypothesis_1'}, length(PlotFtrHyp1), 1)...
         ;repmat({'Hypothesis_2'},length(PlotFtrHyp2), 1)]; 
     %change 'n' as required
h=boxplot([PlotFtrHyp1;PlotFtrHyp2], group,'notch','off','whisker',.5,'outliersize',2);
set(h,{'linew'},{2})
title (['Box Whisker of feature number ', num2str(IndexDomFtrs)])
