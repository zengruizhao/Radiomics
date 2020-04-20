function e=compute_eta(m)

%DIP equations (11.3-14) through (11.3-16).

xbar = m.m10/m.m00;
ybar = m.m01/m.m00;

e.eta11=(m.m11-ybar*m.m10)/m.m00^2;
e.eta20=(m.m20-xbar*m.m10)/m.m00^2;
e.eta02=(m.m02-ybar*m.m01)/m.m00^2;
e.eta30=(m.m30-3*xbar*m.m20+2*xbar^2*m.m10)/m.m00^2.5;
e.eta03=(m.m03-3*ybar*m.m02+2*ybar^2*m.m01)/m.m00^2.5;
e.eta21=(m.m21-2*xbar*m.m11-ybar*m.m20+2*xbar^2*m.m01)/m.m00^2.5;
e.eta12=(m.m12-2*ybar*m.m11-xbar*m.m02+2*ybar^2*m.m10)/m.m00^2.5;
