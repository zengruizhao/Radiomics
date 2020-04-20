function varargout = affinedecomp(M)
% AFFINEDECOMP Decompose affine/linear transformation matrix.
%   [MR1,MS,MR2] = AFFINEDECOMP(M) returns the rotation matrices MR1 & MR2 
%   and the scaling matrix MS which comprise M. (i.e. M = MR2 * MS * MR1).
%
%   [MR1,MS,MR2,MR1ALT,MR2ALT] = AFFINEDECOMP(M) also returns the alternate
%   rotation matrices MR1ALT alt MR2ALT. There is are two ways to achive M
%   via relfecting the coordinate systems (180 degree out of phase).
%
%   [...,THETA1,THETA2,THETA1ALT,THETA2ALT] = AFFINEDECOMP(M) additionally
%   returns the rotation angles corresponding to the matrices MR1, MR2,
%   MR1ALT and MR2ALT.
%
%   For example:
%   MRa=[0.5 sqrt(3)/2; -sqrt(3)/2 0.5] % pi/3 (60 degrees)
%   MS=[1.1 0; 0 .9]
%   MRb=[sqrt(2)/2 -sqrt(2)/2; sqrt(2)/2 sqrt(2)/2] % -pi/4 (-45 degrees)
%   M=MRb*MS*MRa
%   [MR1,MS,MR2,MR1alt,MR2alt,theta1, theta2] = affinedecomp(M);
%   MR1, MR2, theta1=theta1*180/pi, theta2=theta2*180/pi, scales=diag(MS)'
%
%   See also ROTMAT, CPAFFINEGUI, LSQAFFINEH, LSQSIMILARITYH.
%
%JC

% [MR1,MS,MR2,MR1alt,MR2alt,theta1, theta2, theta1alt, theta2alt, MRSRx, MRSRxalt] = affinedecomp(M)

if max(size(M))>2,
    fprintf('Reducing transformation matrix to 2-by-2 (2D).\n');
    M=M(1:2,1:2);
end

% Reflection matrices
Mxreflect=[-1 0; 0 1];
Myreflect=[1 0; 0 -1];

% Decompose M
[U,S,V]=svd(M);

% Check if we got a pure rotation matrix back, or were they reflected
if det(U)<0, % prod(diag(U))<0, % different sign diagonal elements
    % Is the reflection on the x or y axis?
    if ( U(2,2)<U(1,1) ) || ... 
            ( U(2,2)>U(1,1) && prod([U(2,1) U(1,1)])<0 ), % y-reflection
        MR2=U*Myreflect;
        MR1=Myreflect*V';
        % for rotation >180 (i.e. negative rotation)
        MR2alt=U*Mxreflect;
        MR1alt=Mxreflect*V';
    else    % x-reflection
        MR2=U*Mxreflect;
        MR1=Mxreflect*V';
        MR2alt=U*Myreflect;
        MR1alt=Myreflect*V';
    end
    theta1=acos(0.5*trace(MR1))*((MR1(1,2)>=0)*2-1); theta2=acos(0.5*trace(MR2))*((MR2(1,2)>=0)*2-1);
    theta1alt=acos(0.5*trace(MR1alt))*((MR1alt(1,2)>=0)*2-1); theta2alt=acos(0.5*trace(MR2alt))*((MR2alt(1,2)>=0)*2-1);
else % no reflections
    MR1=V';
    MR2=U;
    MR1alt=Mxreflect*(Myreflect*V');
    MR2alt=(U*Myreflect)*Mxreflect;
    theta1=acos(0.5*trace(MR1))*((MR1(1,2)>=0)*2-1); theta2=acos(0.5*trace(MR2))*((MR2(1,2)>=0)*2-1);
    theta1alt=acos(0.5*trace(MR1alt))*((MR1alt(1,2)>=0)*2-1); theta2alt=acos(0.5*trace(MR2alt))*((MR2alt(1,2)>=0)*2-1);
end
MS=S;

MRSRx=MR2*MS*MR1;
MRSRxalt=MR2alt*MS*MR1alt; % should be the same

[varargout{1:11}]=deal(MR1,MS,MR2,MR1alt,MR2alt,theta1, theta2, theta1alt, theta2alt, MRSRx, MRSRxalt);

% svderror=abs(MRSRx-M);
% if max(svderror(:))>eps*6,
%     keyboard;
% end
