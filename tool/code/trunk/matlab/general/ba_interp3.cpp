// Fast nearest, bi-linear and bi-cubic interpolation for 3d image data on a regular grid.
//
// Usage:
// ------
//     R = ba_interp3(F, X, Y, Z, [method])
//     R = ba_interp3(Fx, Fy, Fz, F, X, Y, Z, [method])
//
// where method is one off nearest, linear, or cubic.
//
// Fx, Fy, Fz 
//         are the coordinate system in which F is given. Only the first and
//         last entry in Fx, Fy, Fz are used, and it is assumed that the
//         inbetween values are linearly interpolated.
// F       is a WxHxDxC Image with an arbitray number of channels C.
// X, Y, Z are I_1 x ... x I_n matrices with the x and y coordinates to
//         interpolate.
// R       is a I_1 x ... x I_n x C matrix, which contains the interpolated image channels.
//
// Notes:
// ------
// This method handles the border by repeating the closest values to the point accessed. 
// This is different from matlabs border handling.
//
// Example
// ------
//
//    %% Interpolation of 3D volumes (e.g. distance transforms)
//    clear
//    sz=5;
//
//    % Dist 
//    dist1.D = randn(sz,sz,sz);
//    [dist1.x dist1.y dist.z] = meshgrid(linspace(-1,1,sz), linspace(-1,1,sz), linspace(-1,1,sz));
//    
//    R = [cos(pi/4) sin(pi/4); -sin(pi/4) cos(pi/4)];
//    RD = R * [Dx(:)'; Dy(:)'] + 250;
//    RDx = reshape(RD(1,:), size(Dx));
//    RDy = reshape(RD(2,:), size(Dy));
//    
//    methods = {'nearest', 'linear', 'cubic'};
//    la=nan(1,3);
//    for i=1:3
//      la(i) = subplot(2,2,i);
//      tic;
//      IMG_R = ba_interp2(IMG, RDx, RDy, methods{i});
//      elapsed=toc;
//      imshow(IMG_R);
//      title(sprintf('Rotation and zoom using %s interpolation took %gs', methods{i}, elapsed));
//    end
//    linkaxes(la);
//
// Licence:
// --------
// GPL
// (c) 2008 Brian Amberg
// http://www.brian-amberg.de/
  
#include <mex.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <iostream>

inline
static
bool outside_range(const mwSize & M, const mwSize & N, const mwSize & O, const double x, const double y, const double z) {
	return (x<1 || y<1 || z<1 || x>N || y>M || z>O);
}

inline 
static
int access(int M, int N, int O, int x, int y, int z) {
  if (x<0) x=0; else if (x>=N) x=N-1;
  if (y<0) y=0; else if (y>=M) y=M-1;
  if (z<0) z=0; else if (z>=O) z=O-1;
  return y + M*(x + N*z);
}

inline 
static
int access_unchecked(int M, int N, int O, int x, int y, int z) {
  return y + M*(x + N*z);
}

inline
static
void indices_linear(
    int &f000_i,
    int &f100_i,
    int &f010_i,
    int &f110_i,
    int &f001_i,
    int &f101_i,
    int &f011_i,
    int &f111_i,
    const int x, const int y, const int z,
    const mwSize &M, const mwSize &N, const mwSize &O) {
  if (x<=1 || y<=1 || z<=1 || x>=N-2 || y>=M-2 || z>=O-2) {
    f000_i = access(M,N,O, x,   y  , z);
    f100_i = access(M,N,O, x+1, y  , z);

    f010_i = access(M,N,O, x,   y+1, z);
    f110_i = access(M,N,O, x+1, y+1, z);

    f001_i = access(M,N,O, x,   y  , z+1);
    f101_i = access(M,N,O, x+1, y  , z+1);

    f011_i = access(M,N,O, x,   y+1, z+1);
    f111_i = access(M,N,O, x+1, y+1, z+1);
  } else {
    f000_i = access_unchecked(M,N,O, x,   y  , z);
    f100_i = access_unchecked(M,N,O, x+1, y  , z);

    f010_i = access_unchecked(M,N,O, x,   y+1, z);
    f110_i = access_unchecked(M,N,O, x+1, y+1, z);

    f001_i = access_unchecked(M,N,O, x,   y  , z+1);
    f101_i = access_unchecked(M,N,O, x+1, y  , z+1);

    f011_i = access_unchecked(M,N,O, x,   y+1, z+1);
    f111_i = access_unchecked(M,N,O, x+1, y+1, z+1);
  }
}


inline
static
void indices_cubic(
    int f_i[64],
    const int x, const int y, const int z,
    const mwSize &M, const mwSize &N, const mwSize &O) {
  if (x<=2 || y<=2 || z<=2 || x>=N-3 || y>=M-3 || z>=O-3) {
    for (int i=0; i<4; ++i)
      for (int j=0; j<4; ++j)
        for (int k=0; k<4; ++k)
          f_i[i+4*(j+4*k)] = access(M,N,O, x+i-1, y+j-1, z+k-1);
  } else {
    for (int i=0; i<4; ++i)
      for (int j=0; j<4; ++j)
        for (int k=0; k<4; ++k)
          f_i[i+4*(j+4*k)] = access_unchecked(M,N,O, x+i-1, y+j-1, z+k-1);
  }
}


template <class T>
static
void interpolate_nearest(T *pO, const T *pF, 
  const T *pX, const T *pY, const T *pZ,
  const mwSize ND, const mwSize M, const mwSize N, const mwSize O, const mwSize P,
  const T s_x, const T o_x,
  const T s_y, const T o_y,
  const T s_z, const T o_z,
  const bool extrapolate) {

  const mwSize LO = M*N*O;
  const T nan = mxGetNaN();

  for (mwSize i=0; i<ND; ++i) {
    const T &x = pX[i];
    const T &y = pY[i];
    const T &z = pZ[i];

	const bool use_nan = (!extrapolate && outside_range(M, N, O, x, y, z)); // outside range! use nan's
	if (use_nan)
	{
		for (mwSize j=0; j<P; ++j)
		  pO[i + j*ND] = nan;
	}
	else
	{
		const int x_round = int(floor(s_x*x+o_x+0.5))-1;
		const int y_round = int(floor(s_y*y+o_y+0.5))-1;
		const int z_round = int(floor(s_z*z+o_z+0.5))-1;

		const int f00_i = access(M,N,O, x_round,y_round,z_round);

		for (mwSize j=0; j<P; ++j)
		  pO[i + j*ND] = pF[f00_i + j*LO];
    } // use_nan
  } // for i
}

template <mwSize P, class T>
static
void interpolate_nearest_unrolled(T *pO, const T *pF, 
  const T *pX, const T *pY, const T *pZ,
  const mwSize ND, const mwSize M, const mwSize N, const mwSize O,
  const T s_x, const T o_x,
  const T s_y, const T o_y,
  const T s_z, const T o_z,
  const bool extrapolate) {
  
  const mwSize LO = M*N*O;
  const T nan = mxGetNaN();

  for (mwSize i=0; i<ND; ++i) {
    const T &x = pX[i];
    const T &y = pY[i];
    const T &z = pZ[i];

	const bool use_nan = (!extrapolate && outside_range(M, N, O, x, y, z)); // outside range! use nan's

	if (use_nan)
	{
		for (mwSize j=0; j<P; ++j)
			pO[i + j*ND] = nan;
	}
	else
	{
		const int x_round = int(floor(s_x*x+o_x+0.5))-1;
		const int y_round = int(floor(s_y*y+o_y+0.5))-1;
		const int z_round = int(floor(s_z*z+o_z+0.5))-1;

		const int f00_i = access(M,N,O, x_round,y_round,z_round);

	    for (mwSize j=0; j<P; ++j)
		  pO[i + j*ND] = (use_nan ? nan : pF[f00_i + j*LO]);
    } // use_nan
  } // for i
}


template <class T>
static
void interpolate_linear(T *pO, const T *pF, 
  const T *pX, const T *pY, const T *pZ,
  const mwSize ND, const mwSize M, const mwSize N, const mwSize O, const mwSize P,
  const T s_x, const T o_x,
  const T s_y, const T o_y,
  const T s_z, const T o_z,
  const bool extrapolate) {
  const mwSize LO = M*N*O;
  const double nan = mxGetNaN();

  for (mwSize i=0; i<ND; ++i) {
    const T &x_ = pX[i];
    const T &y_ = pY[i];
    const T &z_ = pZ[i];
    
    const T x = s_x*x_+o_x;
    const T y = s_y*y_+o_y;
    const T z = s_z*z_+o_z;

	const bool use_nan = (!extrapolate && outside_range(M, N, O, x, y, z)); // outside range! use nan's
	if (use_nan)
	{
		for (mwSize j=0; j<P; ++j)
		  pO[i + j*ND] = nan;
	}
	else
	{
		const T x_floor = floor(x);
		const T y_floor = floor(y);
		const T z_floor = floor(z);

		const T dx = x-x_floor;
		const T dy = y-y_floor;
		const T dz = z-z_floor;

		const T wx0 = 1.0-dx;
		const T wx1 = dx;

		const T wy0 = 1.0-dy;
		const T wy1 = dy;

		const T wz0 = 1.0-dz;
		const T wz1 = dz;

		int f000_i, f100_i, f010_i, f110_i,
		    f001_i, f101_i, f011_i, f111_i;

		indices_linear(
			f000_i, f100_i, f010_i, f110_i, 
			f001_i, f101_i, f011_i, f111_i, 
			int(x_floor-1), int(y_floor-1), int(z_floor-1), M, N, O);

		for (mwSize j=0; j<P; ++j) {
		  pO[i + j*ND] = 
			wz0*(
				wy0*(wx0 * pF[f000_i + j*LO] + wx1 * pF[f100_i + j*LO]) +
				wy1*(wx0 * pF[f010_i + j*LO] + wx1 * pF[f110_i + j*LO])
				)+
			wz1*(
				wy0*(wx0 * pF[f001_i + j*LO] + wx1 * pF[f101_i + j*LO]) +
				wy1*(wx0 * pF[f011_i + j*LO] + wx1 * pF[f111_i + j*LO])
				);
		}
	} // use_nan

  }
}

template <mwSize P, class T>
static
void interpolate_linear_unrolled(T *pO, const T *pF, 
  const T *pX, const T *pY, const T *pZ,
  const mwSize ND, const mwSize M, const mwSize N, const mwSize O,
  const T s_x, const T o_x,
  const T s_y, const T o_y,
  const T s_z, const T o_z,
  const bool extrapolate) {
  const mwSize LO = M*N*O;
  const T nan = mxGetNaN();

  for (mwSize i=0; i<ND; ++i) {
    const T &x_ = pX[i];
    const T &y_ = pY[i];
    const T &z_ = pZ[i];
    
    const T x = s_x*x_+o_x;
    const T y = s_y*y_+o_y;
    const T z = s_z*z_+o_z;

	const bool use_nan = (!extrapolate && outside_range(M, N, O, x, y, z)); // outside range! use nan's

	if (use_nan)
	{
		for (mwSize j=0; j<P; ++j)
			pO[i + j*ND] = nan;
	}
	else
	{

		const T x_floor = floor(x);
		const T y_floor = floor(y);
		const T z_floor = floor(z);

		const T dx = x-x_floor;
		const T dy = y-y_floor;
		const T dz = z-z_floor;

		const T wx0 = 1.0-dx;
		const T wx1 = dx;

		const T wy0 = 1.0-dy;
		const T wy1 = dy;

		const T wz0 = 1.0-dz;
		const T wz1 = dz;

		int f000_i, f100_i, f010_i, f110_i,
			f001_i, f101_i, f011_i, f111_i;

		indices_linear(
			f000_i, f100_i, f010_i, f110_i, 
			f001_i, f101_i, f011_i, f111_i, 
			int(x_floor-1), int(y_floor-1), int(z_floor-1), M, N, O);

		for (mwSize j=0; j<P; ++j) {

		  pO[i + j*ND] = 
			wz0*(
				wy0*(wx0 * pF[f000_i + j*LO] + wx1 * pF[f100_i + j*LO]) +
				wy1*(wx0 * pF[f010_i + j*LO] + wx1 * pF[f110_i + j*LO])
				)+
			wz1*(
				wy0*(wx0 * pF[f001_i + j*LO] + wx1 * pF[f101_i + j*LO]) +
				wy1*(wx0 * pF[f011_i + j*LO] + wx1 * pF[f111_i + j*LO])
				);
		} // for j
	} // use_nan
  } // for i
}

template <class T>
static
void interpolate_bicubic(T *pO, const T *pF, 
  const T *pX, const T *pY, const T *pZ, 
  const mwSize ND, const mwSize M, const mwSize N, const mwSize O, const mwSize P,
  const T s_x, const T o_x,
  const T s_y, const T o_y,
  const T s_z, const T o_z,
  const bool extrapolate) {

  const mwSize LO = M*N*O;
  const T nan = mxGetNaN();

  for (mwSize i=0; i<ND; ++i) {
    const T &x_ = pX[i];
    const T &y_ = pY[i];
    const T &z_ = pZ[i];
    
    const T x = s_x*x_+o_x;
    const T y = s_y*y_+o_y;
    const T z = s_z*z_+o_z;
    
	const bool use_nan = (!extrapolate && outside_range(M, N, O, x, y, z)); // outside range! use nan's
	if (use_nan)
	{
		for (mwSize j=0; j<P; ++j)
			pO[i + j*ND] = nan;
	}
	else
	{
		const T x_floor = floor(x);
		const T y_floor = floor(y);
		const T z_floor = floor(z);

		const T dx = x-x_floor;
		const T dy = y-y_floor;
		const T dz = z-z_floor;

		const T dxx = dx*dx;
		const T dxxx = dxx*dx;

		const T dyy = dy*dy;
		const T dyyy = dyy*dy;

		const T dzz = dz*dz;
		const T dzzz = dzz*dz;

		const T wx0 = 0.5 * (    - dx + 2.0*dxx -       dxxx);
		const T wx1 = 0.5 * (2.0      - 5.0*dxx + 3.0 * dxxx);
		const T wx2 = 0.5 * (      dx + 4.0*dxx - 3.0 * dxxx);
		const T wx3 = 0.5 * (         -     dxx +       dxxx);

		const T wy0 = 0.5 * (    - dy + 2.0*dyy -       dyyy);
		const T wy1 = 0.5 * (2.0      - 5.0*dyy + 3.0 * dyyy);
		const T wy2 = 0.5 * (      dy + 4.0*dyy - 3.0 * dyyy);
		const T wy3 = 0.5 * (         -     dyy +       dyyy);

		const T wz0 = 0.5 * (    - dz + 2.0*dzz -       dzzz);
		const T wz1 = 0.5 * (2.0      - 5.0*dzz + 3.0 * dzzz);
		const T wz2 = 0.5 * (      dz + 4.0*dzz - 3.0 * dzzz);
		const T wz3 = 0.5 * (         -     dzz +       dzzz);

		int f_i[64];

		indices_cubic(
			f_i,
			int(x_floor-1), int(y_floor-1), int(z_floor-1), M, N, O);

		for (mwSize j=0; j<P; ++j) {

		  pO[i + j*ND] = 
			wz0*(
				wy0*(wx0 * pF[f_i[0+4*(0+4*0)] + j*LO] + wx1 * pF[f_i[1+4*(0+4*0)] + j*LO] +  wx2 * pF[f_i[2+4*(0+4*0)] + j*LO] + wx3 * pF[f_i[3+4*(0+4*0)] + j*LO]) +
				wy1*(wx0 * pF[f_i[0+4*(1+4*0)] + j*LO] + wx1 * pF[f_i[1+4*(1+4*0)] + j*LO] +  wx2 * pF[f_i[2+4*(1+4*0)] + j*LO] + wx3 * pF[f_i[3+4*(1+4*0)] + j*LO]) +
				wy2*(wx0 * pF[f_i[0+4*(2+4*0)] + j*LO] + wx1 * pF[f_i[1+4*(2+4*0)] + j*LO] +  wx2 * pF[f_i[2+4*(2+4*0)] + j*LO] + wx3 * pF[f_i[3+4*(2+4*0)] + j*LO]) +
				wy3*(wx0 * pF[f_i[0+4*(3+4*0)] + j*LO] + wx1 * pF[f_i[1+4*(3+4*0)] + j*LO] +  wx2 * pF[f_i[2+4*(3+4*0)] + j*LO] + wx3 * pF[f_i[3+4*(3+4*0)] + j*LO])
				) +
			wz1*(
				wy0*(wx0 * pF[f_i[0+4*(0+4*1)] + j*LO] + wx1 * pF[f_i[1+4*(0+4*1)] + j*LO] +  wx2 * pF[f_i[2+4*(0+4*1)] + j*LO] + wx3 * pF[f_i[3+4*(0+4*1)] + j*LO]) +
				wy1*(wx0 * pF[f_i[0+4*(1+4*1)] + j*LO] + wx1 * pF[f_i[1+4*(1+4*1)] + j*LO] +  wx2 * pF[f_i[2+4*(1+4*1)] + j*LO] + wx3 * pF[f_i[3+4*(1+4*1)] + j*LO]) +
				wy2*(wx0 * pF[f_i[0+4*(2+4*1)] + j*LO] + wx1 * pF[f_i[1+4*(2+4*1)] + j*LO] +  wx2 * pF[f_i[2+4*(2+4*1)] + j*LO] + wx3 * pF[f_i[3+4*(2+4*1)] + j*LO]) +
				wy3*(wx0 * pF[f_i[0+4*(3+4*1)] + j*LO] + wx1 * pF[f_i[1+4*(3+4*1)] + j*LO] +  wx2 * pF[f_i[2+4*(3+4*1)] + j*LO] + wx3 * pF[f_i[3+4*(3+4*1)] + j*LO])
				) +
			wz2*(
				wy0*(wx0 * pF[f_i[0+4*(0+4*2)] + j*LO] + wx1 * pF[f_i[1+4*(0+4*2)] + j*LO] +  wx2 * pF[f_i[2+4*(0+4*2)] + j*LO] + wx3 * pF[f_i[3+4*(0+4*2)] + j*LO]) +
				wy1*(wx0 * pF[f_i[0+4*(1+4*2)] + j*LO] + wx1 * pF[f_i[1+4*(1+4*2)] + j*LO] +  wx2 * pF[f_i[2+4*(1+4*2)] + j*LO] + wx3 * pF[f_i[3+4*(1+4*2)] + j*LO]) +
				wy2*(wx0 * pF[f_i[0+4*(2+4*2)] + j*LO] + wx1 * pF[f_i[1+4*(2+4*2)] + j*LO] +  wx2 * pF[f_i[2+4*(2+4*2)] + j*LO] + wx3 * pF[f_i[3+4*(2+4*2)] + j*LO]) +
				wy3*(wx0 * pF[f_i[0+4*(3+4*2)] + j*LO] + wx1 * pF[f_i[1+4*(3+4*2)] + j*LO] +  wx2 * pF[f_i[2+4*(3+4*2)] + j*LO] + wx3 * pF[f_i[3+4*(3+4*2)] + j*LO])
				) +
			wz3*(
				wy0*(wx0 * pF[f_i[0+4*(0+4*3)] + j*LO] + wx1 * pF[f_i[1+4*(0+4*3)] + j*LO] +  wx2 * pF[f_i[2+4*(0+4*3)] + j*LO] + wx3 * pF[f_i[3+4*(0+4*3)] + j*LO]) +
				wy1*(wx0 * pF[f_i[0+4*(1+4*3)] + j*LO] + wx1 * pF[f_i[1+4*(1+4*3)] + j*LO] +  wx2 * pF[f_i[2+4*(1+4*3)] + j*LO] + wx3 * pF[f_i[3+4*(1+4*3)] + j*LO]) +
				wy2*(wx0 * pF[f_i[0+4*(2+4*3)] + j*LO] + wx1 * pF[f_i[1+4*(2+4*3)] + j*LO] +  wx2 * pF[f_i[2+4*(2+4*3)] + j*LO] + wx3 * pF[f_i[3+4*(2+4*3)] + j*LO]) +
				wy3*(wx0 * pF[f_i[0+4*(3+4*3)] + j*LO] + wx1 * pF[f_i[1+4*(3+4*3)] + j*LO] +  wx2 * pF[f_i[2+4*(3+4*3)] + j*LO] + wx3 * pF[f_i[3+4*(3+4*3)] + j*LO])
				);
		} // for j
	} // use_nan
  } // for i
}

template <size_t P, class T>
static
void interpolate_bicubic_unrolled(T *pO, const T *pF, 
  const T *pX, const T *pY, const T *pZ, 
  const mwSize ND, const mwSize M, const mwSize N, const mwSize O,
  const T s_x, const T o_x,
  const T s_y, const T o_y,
  const T s_z, const T o_z,
  const bool extrapolate) {

  const mwSize LO = M*N*O;
  const T nan = mxGetNaN();

  for (mwSize i=0; i<ND; ++i) {
    const T &x_ = pX[i];
    const T &y_ = pY[i];
    const T &z_ = pZ[i];
    
    const T x = s_x*x_+o_x;
    const T y = s_y*y_+o_y;
    const T z = s_z*z_+o_z;
    
	const bool use_nan = (!extrapolate && outside_range(M, N, O, x, y, z)); // outside range! use nan's
	if (use_nan)
	{
		for (mwSize j=0; j<P; ++j)
			pO[i + j*ND] = nan;
	}
	else
	{

		const T x_floor = floor(x);
		const T y_floor = floor(y);
		const T z_floor = floor(z);

		const T dx = x-x_floor;
		const T dy = y-y_floor;
		const T dz = z-z_floor;

		const T dxx = dx*dx;
		const T dxxx = dxx*dx;

		const T dyy = dy*dy;
		const T dyyy = dyy*dy;

		const T dzz = dz*dz;
		const T dzzz = dzz*dz;

		const T wx0 = 0.5 * (    - dx + 2.0*dxx -       dxxx);
		const T wx1 = 0.5 * (2.0      - 5.0*dxx + 3.0 * dxxx);
		const T wx2 = 0.5 * (      dx + 4.0*dxx - 3.0 * dxxx);
		const T wx3 = 0.5 * (         -     dxx +       dxxx);

		const T wy0 = 0.5 * (    - dy + 2.0*dyy -       dyyy);
		const T wy1 = 0.5 * (2.0      - 5.0*dyy + 3.0 * dyyy);
		const T wy2 = 0.5 * (      dy + 4.0*dyy - 3.0 * dyyy);
		const T wy3 = 0.5 * (         -     dyy +       dyyy);

		const T wz0 = 0.5 * (    - dz + 2.0*dzz -       dzzz);
		const T wz1 = 0.5 * (2.0      - 5.0*dzz + 3.0 * dzzz);
		const T wz2 = 0.5 * (      dz + 4.0*dzz - 3.0 * dzzz);
		const T wz3 = 0.5 * (         -     dzz +       dzzz);

		int f_i[64];

		indices_cubic(
			f_i,
			int(x_floor-1), int(y_floor-1), int(z_floor-1), M, N, O);

		for (mwSize j=0; j<P; ++j) {

		  pO[i + j*ND] = 
			wz0*(
				wy0*(wx0 * pF[f_i[0+4*(0+4*0)] + j*LO] + wx1 * pF[f_i[1+4*(0+4*0)] + j*LO] +  wx2 * pF[f_i[2+4*(0+4*0)] + j*LO] + wx3 * pF[f_i[3+4*(0+4*0)] + j*LO]) +
				wy1*(wx0 * pF[f_i[0+4*(1+4*0)] + j*LO] + wx1 * pF[f_i[1+4*(1+4*0)] + j*LO] +  wx2 * pF[f_i[2+4*(1+4*0)] + j*LO] + wx3 * pF[f_i[3+4*(1+4*0)] + j*LO]) +
				wy2*(wx0 * pF[f_i[0+4*(2+4*0)] + j*LO] + wx1 * pF[f_i[1+4*(2+4*0)] + j*LO] +  wx2 * pF[f_i[2+4*(2+4*0)] + j*LO] + wx3 * pF[f_i[3+4*(2+4*0)] + j*LO]) +
				wy3*(wx0 * pF[f_i[0+4*(3+4*0)] + j*LO] + wx1 * pF[f_i[1+4*(3+4*0)] + j*LO] +  wx2 * pF[f_i[2+4*(3+4*0)] + j*LO] + wx3 * pF[f_i[3+4*(3+4*0)] + j*LO])
				) +
			wz1*(
				wy0*(wx0 * pF[f_i[0+4*(0+4*1)] + j*LO] + wx1 * pF[f_i[1+4*(0+4*1)] + j*LO] +  wx2 * pF[f_i[2+4*(0+4*1)] + j*LO] + wx3 * pF[f_i[3+4*(0+4*1)] + j*LO]) +
				wy1*(wx0 * pF[f_i[0+4*(1+4*1)] + j*LO] + wx1 * pF[f_i[1+4*(1+4*1)] + j*LO] +  wx2 * pF[f_i[2+4*(1+4*1)] + j*LO] + wx3 * pF[f_i[3+4*(1+4*1)] + j*LO]) +
				wy2*(wx0 * pF[f_i[0+4*(2+4*1)] + j*LO] + wx1 * pF[f_i[1+4*(2+4*1)] + j*LO] +  wx2 * pF[f_i[2+4*(2+4*1)] + j*LO] + wx3 * pF[f_i[3+4*(2+4*1)] + j*LO]) +
				wy3*(wx0 * pF[f_i[0+4*(3+4*1)] + j*LO] + wx1 * pF[f_i[1+4*(3+4*1)] + j*LO] +  wx2 * pF[f_i[2+4*(3+4*1)] + j*LO] + wx3 * pF[f_i[3+4*(3+4*1)] + j*LO])
				) +
			wz2*(
				wy0*(wx0 * pF[f_i[0+4*(0+4*2)] + j*LO] + wx1 * pF[f_i[1+4*(0+4*2)] + j*LO] +  wx2 * pF[f_i[2+4*(0+4*2)] + j*LO] + wx3 * pF[f_i[3+4*(0+4*2)] + j*LO]) +
				wy1*(wx0 * pF[f_i[0+4*(1+4*2)] + j*LO] + wx1 * pF[f_i[1+4*(1+4*2)] + j*LO] +  wx2 * pF[f_i[2+4*(1+4*2)] + j*LO] + wx3 * pF[f_i[3+4*(1+4*2)] + j*LO]) +
				wy2*(wx0 * pF[f_i[0+4*(2+4*2)] + j*LO] + wx1 * pF[f_i[1+4*(2+4*2)] + j*LO] +  wx2 * pF[f_i[2+4*(2+4*2)] + j*LO] + wx3 * pF[f_i[3+4*(2+4*2)] + j*LO]) +
				wy3*(wx0 * pF[f_i[0+4*(3+4*2)] + j*LO] + wx1 * pF[f_i[1+4*(3+4*2)] + j*LO] +  wx2 * pF[f_i[2+4*(3+4*2)] + j*LO] + wx3 * pF[f_i[3+4*(3+4*2)] + j*LO])
				) +
			wz3*(
				wy0*(wx0 * pF[f_i[0+4*(0+4*3)] + j*LO] + wx1 * pF[f_i[1+4*(0+4*3)] + j*LO] +  wx2 * pF[f_i[2+4*(0+4*3)] + j*LO] + wx3 * pF[f_i[3+4*(0+4*3)] + j*LO]) +
				wy1*(wx0 * pF[f_i[0+4*(1+4*3)] + j*LO] + wx1 * pF[f_i[1+4*(1+4*3)] + j*LO] +  wx2 * pF[f_i[2+4*(1+4*3)] + j*LO] + wx3 * pF[f_i[3+4*(1+4*3)] + j*LO]) +
				wy2*(wx0 * pF[f_i[0+4*(2+4*3)] + j*LO] + wx1 * pF[f_i[1+4*(2+4*3)] + j*LO] +  wx2 * pF[f_i[2+4*(2+4*3)] + j*LO] + wx3 * pF[f_i[3+4*(2+4*3)] + j*LO]) +
				wy3*(wx0 * pF[f_i[0+4*(3+4*3)] + j*LO] + wx1 * pF[f_i[1+4*(3+4*3)] + j*LO] +  wx2 * pF[f_i[2+4*(3+4*3)] + j*LO] + wx3 * pF[f_i[3+4*(3+4*3)] + j*LO])
				);
		} // for j
	} //use_nan
  } // for i
}

enum InterpolationMethod { Nearest, Linear, Cubic };

static
InterpolationMethod parseInterpolationMethod(const mxArray *method_string) {
  if (method_string == NULL)
    return Cubic;

  char method[10] = "cubic    ";

  mxGetString(method_string, method, 9);

  if (std::string(method).compare(0, 7, "nearest") == 0)
    return Nearest;
  else if (std::string(method).compare(0, 6, "linear") == 0)
    return Linear;
  else if (std::string(method).compare(0, 5, "cubic") == 0)
    return Cubic;
  else
    mexErrMsgTxt("Specify one of nearest, linear, cubic as the interpolation method argument.");

  return(Cubic);
}

/*
 * Just a wrapper so we can use either double or single precision.
 */
template <class T>
void template_wrapper(
        const mxArray * Fx, const mxArray * Fy, const mxArray * Fz,
        const mxArray * F, const mxArray * X, const mxArray * Y, const mxArray * Z,
        const mxArray * method,
        mxArray * output, 
        const mwSize M, const mwSize N, const mwSize O, const mwSize P, const bool extrapolate)
{

  const mwSize ND = mxGetNumberOfElements(X);

  const T *pF = (T *)mxGetData(F);
  const T *pX = (T *)mxGetData(X);
  const T *pY = (T *)mxGetData(Y);
  const T *pZ = (T *)mxGetData(Z);
        T *pO = (T *)mxGetData(output);

  
  if (Fx) { 
    
    const T x_low  = ((T *)(mxGetData(Fx)))[0];
    const T y_low  = ((T *)(mxGetData(Fy)))[0];
    const T z_low  = ((T *)(mxGetData(Fz)))[0];
    const T x_high = ((T *)(mxGetData(Fx)))[mxGetNumberOfElements(Fx)-1];
    const T y_high = ((T *)(mxGetData(Fy)))[mxGetNumberOfElements(Fy)-1];
    const T z_high = ((T *)(mxGetData(Fz)))[mxGetNumberOfElements(Fz)-1];
    
    const T s_x = T(1-N)/(x_low - x_high);
    const T s_y = T(1-M)/(y_low - y_high);
    const T s_z = T(1-O)/(z_low - z_high);

    const T o_x = T(1)-x_low*s_x;
    const T o_y = T(1)-y_low*s_y;
    const T o_z = T(1)-z_low*s_z;
    
    // Scale X, Y, Z before accessing
    // I've deliberatly copied these two paths, such that the compiler can optimize out the multiplication with 1 and addition of 0
  switch(parseInterpolationMethod(method)) { 
    case Nearest:
      switch (P) {
        case 1: interpolate_nearest_unrolled<1,T>(pO, pF, pX, pY, pZ, ND, M, N, O, s_x, o_x, s_y, o_y, s_z, o_z, extrapolate); break;
        case 2: interpolate_nearest_unrolled<2,T>(pO, pF, pX, pY, pZ, ND, M, N, O, s_x, o_x, s_y, o_y, s_z, o_z, extrapolate); break;
        case 3: interpolate_nearest_unrolled<3,T>(pO, pF, pX, pY, pZ, ND, M, N, O, s_x, o_x, s_y, o_y, s_z, o_z, extrapolate); break;
        case 4: interpolate_nearest_unrolled<4,T>(pO, pF, pX, pY, pZ, ND, M, N, O, s_x, o_x, s_y, o_y, s_z, o_z, extrapolate); break;
        case 5: interpolate_nearest_unrolled<5,T>(pO, pF, pX, pY, pZ, ND, M, N, O, s_x, o_x, s_y, o_y, s_z, o_z, extrapolate); break;
        case 6: interpolate_nearest_unrolled<6,T>(pO, pF, pX, pY, pZ, ND, M, N, O, s_x, o_x, s_y, o_y, s_z, o_z, extrapolate); break;
        case 7: interpolate_nearest_unrolled<7,T>(pO, pF, pX, pY, pZ, ND, M, N, O, s_x, o_x, s_y, o_y, s_z, o_z, extrapolate); break;
        case 8: interpolate_nearest_unrolled<8,T>(pO, pF, pX, pY, pZ, ND, M, N, O, s_x, o_x, s_y, o_y, s_z, o_z, extrapolate); break;
        case 9: interpolate_nearest_unrolled<9,T>(pO, pF, pX, pY, pZ, ND, M, N, O, s_x, o_x, s_y, o_y, s_z, o_z, extrapolate); break;
        default: 
                interpolate_nearest<T>(pO, pF, pX, pY, pZ, ND, M, N, O, P, s_x, o_x, s_y, o_y, s_z, o_z, extrapolate);
      }
      break;
    case Linear:
      switch (P) {
        case 1: interpolate_linear_unrolled<1,T>(pO, pF, pX, pY, pZ, ND, M, N, O, s_x, o_x, s_y, o_y, s_z, o_z, extrapolate); break;
        case 2: interpolate_linear_unrolled<2,T>(pO, pF, pX, pY, pZ, ND, M, N, O, s_x, o_x, s_y, o_y, s_z, o_z, extrapolate); break;
        case 3: interpolate_linear_unrolled<3,T>(pO, pF, pX, pY, pZ, ND, M, N, O, s_x, o_x, s_y, o_y, s_z, o_z, extrapolate); break;
        case 4: interpolate_linear_unrolled<4,T>(pO, pF, pX, pY, pZ, ND, M, N, O, s_x, o_x, s_y, o_y, s_z, o_z, extrapolate); break;
        case 5: interpolate_linear_unrolled<5,T>(pO, pF, pX, pY, pZ, ND, M, N, O, s_x, o_x, s_y, o_y, s_z, o_z, extrapolate); break;
        case 6: interpolate_linear_unrolled<6,T>(pO, pF, pX, pY, pZ, ND, M, N, O, s_x, o_x, s_y, o_y, s_z, o_z, extrapolate); break;
        case 7: interpolate_linear_unrolled<7,T>(pO, pF, pX, pY, pZ, ND, M, N, O, s_x, o_x, s_y, o_y, s_z, o_z, extrapolate); break;
        case 8: interpolate_linear_unrolled<8,T>(pO, pF, pX, pY, pZ, ND, M, N, O, s_x, o_x, s_y, o_y, s_z, o_z, extrapolate); break;
        case 9: interpolate_linear_unrolled<9,T>(pO, pF, pX, pY, pZ, ND, M, N, O, s_x, o_x, s_y, o_y, s_z, o_z, extrapolate); break;
        default: 
                interpolate_linear<T>(pO, pF, pX, pY, pZ, ND, M, N, O, P, s_x, o_x, s_y, o_y, s_z, o_z, extrapolate);
      }
      break;
    case Cubic:
      switch (P) {
        case 1: interpolate_bicubic_unrolled<1,T>(pO, pF, pX, pY, pZ, ND, M, N, O, s_x, o_x, s_y, o_y, s_z, o_z, extrapolate); break;
        case 2: interpolate_bicubic_unrolled<2,T>(pO, pF, pX, pY, pZ, ND, M, N, O, s_x, o_x, s_y, o_y, s_z, o_z, extrapolate); break;
        case 3: interpolate_bicubic_unrolled<3,T>(pO, pF, pX, pY, pZ, ND, M, N, O, s_x, o_x, s_y, o_y, s_z, o_z, extrapolate); break;
        case 4: interpolate_bicubic_unrolled<4,T>(pO, pF, pX, pY, pZ, ND, M, N, O, s_x, o_x, s_y, o_y, s_z, o_z, extrapolate); break;
        case 5: interpolate_bicubic_unrolled<5,T>(pO, pF, pX, pY, pZ, ND, M, N, O, s_x, o_x, s_y, o_y, s_z, o_z, extrapolate); break;
        case 6: interpolate_bicubic_unrolled<6,T>(pO, pF, pX, pY, pZ, ND, M, N, O, s_x, o_x, s_y, o_y, s_z, o_z, extrapolate); break;
        case 7: interpolate_bicubic_unrolled<7,T>(pO, pF, pX, pY, pZ, ND, M, N, O, s_x, o_x, s_y, o_y, s_z, o_z, extrapolate); break;
        case 8: interpolate_bicubic_unrolled<8,T>(pO, pF, pX, pY, pZ, ND, M, N, O, s_x, o_x, s_y, o_y, s_z, o_z, extrapolate); break;
        case 9: interpolate_bicubic_unrolled<9,T>(pO, pF, pX, pY, pZ, ND, M, N, O, s_x, o_x, s_y, o_y, s_z, o_z, extrapolate); break;
        default: 
                interpolate_bicubic<T>(pO, pF, pX, pY, pZ, ND, M, N, O, P, s_x, o_x, s_y, o_y, s_z, o_z, extrapolate);
      }
      break;
    default:
      mexErrMsgTxt("Unimplemented interpolation method.");
  }
  } else {
  switch(parseInterpolationMethod(method)) {
    case Nearest:
      switch (P) {
        case 1: interpolate_nearest_unrolled<1,T>(pO, pF, pX, pY, pZ, ND, M, N, O, T(1), T(0), T(1), T(0), T(1), T(0), extrapolate); break;
        case 2: interpolate_nearest_unrolled<2,T>(pO, pF, pX, pY, pZ, ND, M, N, O, T(1), T(0), T(1), T(0), T(1), T(0), extrapolate); break;
        case 3: interpolate_nearest_unrolled<3,T>(pO, pF, pX, pY, pZ, ND, M, N, O, T(1), T(0), T(1), T(0), T(1), T(0), extrapolate); break;
        case 4: interpolate_nearest_unrolled<4,T>(pO, pF, pX, pY, pZ, ND, M, N, O, T(1), T(0), T(1), T(0), T(1), T(0), extrapolate); break;
        case 5: interpolate_nearest_unrolled<5,T>(pO, pF, pX, pY, pZ, ND, M, N, O, T(1), T(0), T(1), T(0), T(1), T(0), extrapolate); break;
        case 6: interpolate_nearest_unrolled<6,T>(pO, pF, pX, pY, pZ, ND, M, N, O, T(1), T(0), T(1), T(0), T(1), T(0), extrapolate); break;
        case 7: interpolate_nearest_unrolled<7,T>(pO, pF, pX, pY, pZ, ND, M, N, O, T(1), T(0), T(1), T(0), T(1), T(0), extrapolate); break;
        case 8: interpolate_nearest_unrolled<8,T>(pO, pF, pX, pY, pZ, ND, M, N, O, T(1), T(0), T(1), T(0), T(1), T(0), extrapolate); break;
        case 9: interpolate_nearest_unrolled<9,T>(pO, pF, pX, pY, pZ, ND, M, N, O, T(1), T(0), T(1), T(0), T(1), T(0), extrapolate); break;
        default: 
                interpolate_nearest<T>(pO, pF, pX, pY, pZ, ND, M, N, O, P, T(1), T(0), T(1), T(0), T(1), T(0), extrapolate);
      }
      break;
    case Linear:
      switch (P) {
        case 0: interpolate_linear_unrolled<1,T>(pO, pF, pX, pY, pZ, ND, M, N, O, T(1), T(0), T(1), T(0), T(1), T(0), extrapolate); break;
        case 2: interpolate_linear_unrolled<2,T>(pO, pF, pX, pY, pZ, ND, M, N, O, T(1), T(0), T(1), T(0), T(1), T(0), extrapolate); break;
        case 3: interpolate_linear_unrolled<3,T>(pO, pF, pX, pY, pZ, ND, M, N, O, T(1), T(0), T(1), T(0), T(1), T(0), extrapolate); break;
        case 4: interpolate_linear_unrolled<4,T>(pO, pF, pX, pY, pZ, ND, M, N, O, T(1), T(0), T(1), T(0), T(1), T(0), extrapolate); break;
        case 5: interpolate_linear_unrolled<5,T>(pO, pF, pX, pY, pZ, ND, M, N, O, T(1), T(0), T(1), T(0), T(1), T(0), extrapolate); break;
        case 6: interpolate_linear_unrolled<6,T>(pO, pF, pX, pY, pZ, ND, M, N, O, T(1), T(0), T(1), T(0), T(1), T(0), extrapolate); break;
        case 7: interpolate_linear_unrolled<7,T>(pO, pF, pX, pY, pZ, ND, M, N, O, T(1), T(0), T(1), T(0), T(1), T(0), extrapolate); break;
        case 8: interpolate_linear_unrolled<8,T>(pO, pF, pX, pY, pZ, ND, M, N, O, T(1), T(0), T(1), T(0), T(1), T(0), extrapolate); break;
        case 9: interpolate_linear_unrolled<9,T>(pO, pF, pX, pY, pZ, ND, M, N, O, T(1), T(0), T(1), T(0), T(1), T(0), extrapolate); break;
        default:
            interpolate_linear<T>(pO, pF, pX, pY, pZ, ND, M, N, O, P, 1, 0, 1, 0, 1, 0, extrapolate);
            
      }
      break;
    case Cubic:
      switch (P) {
        case 1: interpolate_bicubic_unrolled<1,T>(pO, pF, pX, pY, pZ, ND, M, N, O, T(1), T(0), T(1), T(0), T(1), T(0), extrapolate); break;
        case 2: interpolate_bicubic_unrolled<2,T>(pO, pF, pX, pY, pZ, ND, M, N, O, T(1), T(0), T(1), T(0), T(1), T(0), extrapolate); break;
        case 3: interpolate_bicubic_unrolled<3,T>(pO, pF, pX, pY, pZ, ND, M, N, O, T(1), T(0), T(1), T(0), T(1), T(0), extrapolate); break;
        case 4: interpolate_bicubic_unrolled<4,T>(pO, pF, pX, pY, pZ, ND, M, N, O, T(1), T(0), T(1), T(0), T(1), T(0), extrapolate); break;
        case 5: interpolate_bicubic_unrolled<5,T>(pO, pF, pX, pY, pZ, ND, M, N, O, T(1), T(0), T(1), T(0), T(1), T(0), extrapolate); break;
        case 6: interpolate_bicubic_unrolled<6,T>(pO, pF, pX, pY, pZ, ND, M, N, O, T(1), T(0), T(1), T(0), T(1), T(0), extrapolate); break;
        case 7: interpolate_bicubic_unrolled<7,T>(pO, pF, pX, pY, pZ, ND, M, N, O, T(1), T(0), T(1), T(0), T(1), T(0), extrapolate); break;
        case 8: interpolate_bicubic_unrolled<8,T>(pO, pF, pX, pY, pZ, ND, M, N, O, T(1), T(0), T(1), T(0), T(1), T(0), extrapolate); break;
        case 9: interpolate_bicubic_unrolled<9,T>(pO, pF, pX, pY, pZ, ND, M, N, O, T(1), T(0), T(1), T(0), T(1), T(0), extrapolate); break;
        default: 
                interpolate_bicubic<T>(pO, pF, pX, pY, pZ, ND, M, N, O, P, T(1), T(0), T(1), T(0), T(1), T(0), extrapolate);
      }
      break;
    default:
      mexErrMsgTxt("Unimplemented interpolation method.");
  }
  }
}

void mexFunction(int nlhs, mxArray *plhs[],
    int nrhs, const mxArray *prhs[]) {

  if (nlhs>1)
    mexErrMsgTxt("Wrong number of output arguments for Z = ba_interp3(Fx, Fy, Fz, F, X, Y, Z, method, extrapolate)");

  const mxArray *Fx = NULL;
  const mxArray *Fy = NULL;
  const mxArray *Fz = NULL;
  const mxArray *F = NULL;
  const mxArray *X = NULL;
  const mxArray *Y = NULL;
  const mxArray *Z = NULL;
  const mxArray *method = NULL;
  bool extrapolate = false;

  if (nrhs==4) {
    // ba_interp(F, X, Y, Z);
    F = prhs[0];
    X = prhs[1];
    Y = prhs[2];
    Z = prhs[3];
  } else if (nrhs==5) {
    // ba_interp(F, X, Y, Z, 'method');
    F = prhs[0];
    X = prhs[1];
    Y = prhs[2];
    Z = prhs[3];
    method = prhs[4];
  } else if (nrhs==6) {
    // ba_interp(F, X, Y, Z, 'method', extrapolate);
    F = prhs[0];
    X = prhs[1];
    Y = prhs[2];
    Z = prhs[3];
    method = prhs[4];
	extrapolate = mxIsLogicalScalarTrue(prhs[5]);
  } else if (nrhs==7) {
    // ba_interp(Fx, Fy, Fz, F, X, Y, Z);
    Fx= prhs[0];
    Fy= prhs[1];
    Fz= prhs[2];
    F = prhs[3];
    X = prhs[4];
    Y = prhs[5];
    Z = prhs[6];
    method = prhs[4];
  } else if (nrhs==8) {
    // ba_interp(Fx, Fy, Fz, F, X, Y, Z, 'method');
    Fx= prhs[0];
    Fy= prhs[1];
    Fz= prhs[2];
    F = prhs[3];
    X = prhs[4];
    Y = prhs[5];
    Z = prhs[6];
    method = prhs[7];
  } else if (nrhs==9) {
    // ba_interp(Fx, Fy, Fz, F, X, Y, Z, 'method', extrapolate);
    Fx= prhs[0];
    Fy= prhs[1];
    Fz= prhs[2];
    F = prhs[3];
    X = prhs[4];
    Y = prhs[5];
    Z = prhs[6];
    method = prhs[7];
	extrapolate = mxIsLogicalScalarTrue(prhs[8]);
  } else {
    mexErrMsgTxt("Wrong number of input arguments for Z = ba_interp3(Fx, Fy, Fz, F, X, Y, Z, method, extrapolate)");
  }
  
  // determine the class of the input data
  
  const bool is_double = (
      (!Fx || mxIsDouble(Fx)) && (!Fy || mxIsDouble(Fy)) && (!Fz || mxIsDouble(Fz)) &&
      (!F || mxIsDouble(F)) &&
      (!X || mxIsDouble(X)) && (!Y || mxIsDouble(Y)) && (!Z || mxIsDouble(Z)));
  const bool is_single (
      (!Fx || mxIsClass(Fx, "single")) && (!Fy || mxIsClass(Fy, "single")) && (!Fz || mxIsClass(Fz, "single")) &&
      (!F || mxIsClass(F, "single")) &&
      (!X || mxIsClass(X, "single")) && (!Y || mxIsClass(Y, "single")) && (!Z || mxIsClass(Z, "single")));

  if (!is_double && !is_single)
    mexErrMsgTxt("ba_interp3 takes only all single or all double arguments for Fx,Fy,Fz,F,X,Y,Z");

  const mwSize *F_dims = mxGetDimensions(F);
  const mwSize *X_dims = mxGetDimensions(X);
  const mwSize *Y_dims = mxGetDimensions(Y);
  const mwSize *Z_dims = mxGetDimensions(Z);

  const mwSize ndims = mxGetNumberOfDimensions(F);
  const mwSize M=(ndims == 0 ? 1 : F_dims[0]);
  const mwSize N=(ndims == 1 ? 1 : F_dims[1]);
  const mwSize O=(ndims == 2 ? 1 : F_dims[2]);

  if (Fx && mxGetNumberOfElements(Fx)<2) mexErrMsgTxt("Fx needs at least two elements.");
  if (Fy && mxGetNumberOfElements(Fy)<2) mexErrMsgTxt("Fy needs at least two elements.");
  if (Fz && mxGetNumberOfElements(Fz)<2) mexErrMsgTxt("Fz needs at least two elements.");

  if ((mxGetNumberOfDimensions(X) != mxGetNumberOfDimensions(Y)) ||
      (mxGetNumberOfDimensions(X) != mxGetNumberOfDimensions(Z)) ||
      (mxGetNumberOfElements(X) != mxGetNumberOfElements(Y)) ||
      (mxGetNumberOfElements(X) != mxGetNumberOfElements(Z)))
    mexErrMsgTxt("X, Y, Z should have the same size");

  mwSize P=1;

  mwSize outDims[50];
  if (mxGetNumberOfDimensions(X) + mxGetNumberOfDimensions(F) - 3 > 50)
    mexErrMsgTxt("Can't have that many dimensions in interpolated data.");

  for (mwSize i=0; i<mxGetNumberOfDimensions(X); ++i) outDims[i] = X_dims[i];

  for (mwSize i=3; i<mxGetNumberOfDimensions(F); ++i) {
    outDims[mxGetNumberOfDimensions(X)+i-3] = F_dims[i];
    P *= F_dims[i];
  }

  // create the output (either double or single)
  plhs[0] = mxCreateNumericArray(mxGetNumberOfDimensions(X) + mxGetNumberOfDimensions(F) - 3, 
          outDims, (is_double ? mxDOUBLE_CLASS : mxSINGLE_CLASS), mxREAL);
          
  // use the correct template, and away we go!
  if (is_single)
    template_wrapper<float> (Fx, Fy, Fz, F, X, Y, Z, method, plhs[0], M, N, O, P, extrapolate);
  else if (is_double)
	template_wrapper<double>(Fx, Fy, Fz, F, X, Y, Z, method, plhs[0], M, N, O, P, extrapolate);

  
} // mex function
