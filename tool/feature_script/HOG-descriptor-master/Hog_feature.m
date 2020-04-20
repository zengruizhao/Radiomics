function [Hist] = Hog_feature( img )
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here
[m n,d]=size(img);

img=sqrt(double(img));      %伽马校正

%下面是求边缘
fy=[-1 0 1];        %定义竖直模板,用于计算y方向上的梯度值，向右为正方向
fx=fy';             %定义水平模板,用于计算x方向上的梯度值
Iy=imfilter(img,fy,'replicate');    %竖直边缘/梯度
Ix=imfilter(img,fx,'replicate');    %水平边缘/梯度
Ied=sqrt(Ix.^2+Iy.^2);              %边缘强度/梯度
Iphase=Iy./Ix;              %边缘斜率，有些为inf,-inf,nan，其中nan需要再处理一下

%下面是求cell %only one cell
% step=512;                %step*step个像素作为一个cell
orient=20;               %方向直方图的方向个数
jiao=360/orient;        %每个方向包含的角度数
% Cell=cell(1,1);              %所有的角度直方图,cell是可以动态增加的，所以先设了一个

tmpx = Ix(1:m,1:n);
tmped = Ied(1:m,1:n);
tmped = tmped/sum(sum(tmped));
tmpphase = Iphase(1:m,1:n);
Hist = zeros(1,orient);
for p=1:m
    for q=1:n
        if isnan(tmpphase(p,q))==1  %0/0会得到nan，如果像素是nan，重设为0
            tmpphase(p,q)=0;
        end
        ang=atan(tmpphase(p,q));    %atan求的是[-90 90]度之间
        ang=mod(ang*180/pi,360);    %全部变正，[0,360), -90变270
        if tmpx(p,q)<0              %根据x方向确定真正的角度
            if ang<90               %如果是第一象限
                ang=ang+180;        %移到第三象限
            end
            if ang>270              %如果是第四象限
                ang=ang-180;        %移到第二象限
            end
        end
        ang=ang+0.0000001;          %防止ang为0
        Hist(ceil(ang/jiao))=Hist(ceil(ang/jiao))+tmped(p,q);   %ceil向上取整，使用边缘强度加权
    end
end
Hist=Hist/sum(Hist);    %方向直方图归一化
end
