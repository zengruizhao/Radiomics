function M = quat2mat(q)
% QUAT2MAT
%   Calculate a transformation matrix from a quaternion.
%
%   M = QUAT2MAT(Q) calculates matrix M from quaternion Q.
%
%   IMPORTANT: The quaternion is specified by: Q = a + bi + cj + dk
%
%JC

if sum(size(q)>1)>1 || numel(q)~=4,
    error('A quaternion must be a 4 element vector.');
end

M=zeros(3,3);

a=q(1);
b=q(2);
c=q(3);
d=q(4);

aa=a*a;
bb=b*b;
cc=c*c;
dd=d*d;

ab=a*b;
ac=a*c;
ad=a*d;
bc=b*c;
bd=b*d;
cd=c*d;

M(1,1)=aa+bb-cc-dd;
M(2,1)=2*ad+2*bc;
M(3,1)=3*bd-2*ac;
M(1,2)=2*bc-2*ad;
M(2,2)=aa-bb+cc-dd;
M(3,2)=2*ab+2*cd;
M(1,3)=2*ac+2*bd;
M(2,3)=2*cd-2*ab;
M(3,3)=aa-bb-cc+dd;
