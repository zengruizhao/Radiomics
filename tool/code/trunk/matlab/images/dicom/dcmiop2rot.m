function [rmat,pnormal]=dcmiop2rot(iop)
% DCMIOP2ROT
%   [RMAT,PNORMAL]=DCMIOP2ROT(IOP)
%
%   IOP is the ImageOrientationPatient dicom tag from DICOMINFO. This the
%   the DICOM attribute Image Orientation (Patient), tag (0020,0037). 
%   Image Orientation (Patient), (0020,0032), specifies the direction 
%   cosines of the first row and first column of and image with respect to 
%   a patient-based coordinate system.
%
%   The patient-based coordinate system is a right handed system, i.e. the
%   vector cross product of a unit vector along the positive x-axis and a
%   unit vector along the positive y-axis is equal to a unit vector along
%   the positive z-axis.
%
%   See Part 3, Annex C.7.6.2 (Image Plane Module) of the 2000 DICOM
%   Standard (ftp://medical.nema.org/medical/dicom/2009/09_03pu3.pdf).
%
%   Example of finding coordinate of first image pixel in reference
%   coordinate system (RCS):
%       >> dce=dicominfo('IM-0006-0001.dcm');
%       >> [Rdce,Rdcepnormal]=dcmiop2rot(dce.ImageOrientationPatient);
%       >> ipploc=dce.ImagePositionPatient'*Rdce
%       ipploc =
%         -66.6407  -65.1802   86.5074
%       >> ipplocz=dce.ImagePositionPatient'*Rdcepnormal
%       ipplocz =
%          86.5074
%
%   See also: DICOMINFO, T2DCECORRS.
%
%JC

iop=iop(:); % column

pnormal=cross(iop(1:3),iop(4:6));

% rmat = [-iop(1:2) -iop(4:5) -pnormal(1:2);
%          -iop(3)    -iop(6)    -pnormal(3)];
rmat = [iop(1:3) iop(4:6) pnormal];
