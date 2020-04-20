function stats = run_length_vector_feature(tGLRLM,N_cells)

%GRAYCOPROPS Properties of gray-level run-length vector.

%  Requirements:
%  -------------------------------------------
%   GLRLM mustbe an cell array of valid gray-level run-length vector
%   .Recall that a valid glrlm must be logical or numerical.
%  -------------------------------------------
%  Current supported statistics include:
%  -------------------------------------------
%   Short Run Emphasis (SRE)
%   Long Run Emphasis (LRE)
%   Run Length Nonuniformity (RLN)
%   Run Percentage (RP)
%  --------------------------------------------
%  Reference:
%  --------------------------------------------
%   Xiaoou Tang,Texture Information in Run-Length Matrices
%   IEEE TRANSACTIONS ON IMAGE PROCESSING, VOL.7, NO.11,NOVEMBER 1998
% ---------------------------------------------
%
% ---------------------------------------------
% Author:
% ---------------------------------------------
%    (C)Cheng Lu <hacylu@gmail.com>
% ---------------------------------------------
% History:
% ---------------------------------------------

% Initialize output stats structure.

% Total number of runs
N_runs = sum(tGLRLM);
% colum indicator
c_vector =1:length(tGLRLM);
%------------------------Statistics-------------------------------
% 1. Short Run Emphasis (SRE)
SRE = sum(tGLRLM./(c_vector.^2))/N_runs;
% 2. Long Run Emphasis (LRE)
LRE = sum(tGLRLM.*(c_vector.^2))/N_runs;
% 3. Run Length Nonuniformity (RLN)
RLN = sum(tGLRLM.^2)/N_runs;
% 4. Run Percentage (RP)
RP = N_runs/N_cells;

%----------------insert statistics----------------------------
stats=[SRE LRE RLN RP];