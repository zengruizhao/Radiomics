function phi=compute_phi(e)
%DIP equations (11.3-17) through (11.3-23)

phi(1)=e.eta20+e.eta02;
phi(2)=(e.eta20-e.eta02)^2+4*e.eta11^2;
phi(3)=(e.eta30-3*e.eta12)^2+(3*e.eta21-e.eta03)^2;
phi(4)=(e.eta30+e.eta12)^2+(e.eta21+e.eta03)^2;
phi(5)=(e.eta30-3*e.eta12)*(e.eta30+e.eta12)*((e.eta30+e.eta12)^2-3*(e.eta21+e.eta03)^2)+(3*e.eta21-e.eta03)*(e.eta21+e.eta03)*(3*(e.eta30+e.eta12)^2-(e.eta21+e.eta03)^2);
phi(6)=(e.eta20-e.eta02)*((e.eta30+e.eta12)^2-(e.eta21+e.eta03)^2)+4*e.eta11*(e.eta30+e.eta12)*(e.eta21+e.eta03);
phi(7)=(3.*e.eta21-e.eta03)*(e.eta30+e.eta12)*((e.eta30+e.eta12)^2-3*(e.eta21+e.eta03)^2)+(3*e.eta12-e.eta30)*(e.eta21+e.eta03)*(3*(e.eta30+e.eta12)^2-(e.eta21+e.eta03)^2);
