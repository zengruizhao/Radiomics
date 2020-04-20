function [Hist] = Hog_feature( img )
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here
[m n,d]=size(img);

img=sqrt(double(img));      %٤��У��

%���������Ե
fy=[-1 0 1];        %������ֱģ��,���ڼ���y�����ϵ��ݶ�ֵ������Ϊ������
fx=fy';             %����ˮƽģ��,���ڼ���x�����ϵ��ݶ�ֵ
Iy=imfilter(img,fy,'replicate');    %��ֱ��Ե/�ݶ�
Ix=imfilter(img,fx,'replicate');    %ˮƽ��Ե/�ݶ�
Ied=sqrt(Ix.^2+Iy.^2);              %��Եǿ��/�ݶ�
Iphase=Iy./Ix;              %��Եб�ʣ���ЩΪinf,-inf,nan������nan��Ҫ�ٴ���һ��

%��������cell %only one cell
% step=512;                %step*step��������Ϊһ��cell
orient=20;               %����ֱ��ͼ�ķ������
jiao=360/orient;        %ÿ����������ĽǶ���
% Cell=cell(1,1);              %���еĽǶ�ֱ��ͼ,cell�ǿ��Զ�̬���ӵģ�����������һ��

tmpx = Ix(1:m,1:n);
tmped = Ied(1:m,1:n);
tmped = tmped/sum(sum(tmped));
tmpphase = Iphase(1:m,1:n);
Hist = zeros(1,orient);
for p=1:m
    for q=1:n
        if isnan(tmpphase(p,q))==1  %0/0��õ�nan�����������nan������Ϊ0
            tmpphase(p,q)=0;
        end
        ang=atan(tmpphase(p,q));    %atan�����[-90 90]��֮��
        ang=mod(ang*180/pi,360);    %ȫ��������[0,360), -90��270
        if tmpx(p,q)<0              %����x����ȷ�������ĽǶ�
            if ang<90               %����ǵ�һ����
                ang=ang+180;        %�Ƶ���������
            end
            if ang>270              %����ǵ�������
                ang=ang-180;        %�Ƶ��ڶ�����
            end
        end
        ang=ang+0.0000001;          %��ֹangΪ0
        Hist(ceil(ang/jiao))=Hist(ceil(ang/jiao))+tmped(p,q);   %ceil����ȡ����ʹ�ñ�Եǿ�ȼ�Ȩ
    end
end
Hist=Hist/sum(Hist);    %����ֱ��ͼ��һ��
end
