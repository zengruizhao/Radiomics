clear;clc;
path = 'F:\430\Mat\CT_PNET_Mat_4\';
case_file = dir(path);
for i=3:size(case_file, 1)
    fprintf('%d case..................\n', i-2);
    phase = dir([path case_file(i).name '\*.mat']);
    for j=1:size(phase, 1)
        phase_path = [path case_file(i).name '\' phase(j).name];
        load (phase_path);
%         [start_point,end_point] = collage_get_range(label);
%         [feat1_mask,feat2_mask] = compute_CoLlAGe3D(img,img(:,:,end_point), img(:,:,start_point), label, 1);
        [Lx, Ly, Lz]=gradient(img);
        [r, c, slice] = size(img);
        winRadius = 1;
        [I_gradient_inner1, I_gradient_inner_mag, dominant_orientation1_roi, dominant_orientation2_roi]=find_orientation_CoLlAGe_3D(Lx,Ly,Lz,winRadius,r-5,c-5);
    end
end