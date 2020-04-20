r_max = Geometry.DepthEnd.value* 1e3;

theta_start = (pi/2)-(Geometry.Width.value)/2;
theta_end = (pi/2)+(Geometry.Width.value)/2;

beta_start = (pi/2)-(Geometry.ElevationWidth.value)/2;
beta_end = (pi/2)+(Geometry.ElevationWidth.value)/2;

b = abs( Geometry.Origo.value(2) )*1e3;

[r, theta, beta ] = meshgrid( 0:(r_max/20):r_max, theta_start: (theta_end-theta_start)/20: theta_end, beta_start:( beta_end-beta_start )/20 :beta_end );
