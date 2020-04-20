clear;clc
path = '/media/zzr/My Passport/430/Mat/CT_PNET_Mat_5/';
case_file = dir(path);
num_img_values = 128;
for i=3:size(case_file, 1)
    fprintf('%d case..................\n', i-2);
    phase = dir([path case_file(i).name '/*.mat']);
    for j=1:size(phase, 1)
        phase_path = [path case_file(i).name '/' phase(j).name];
        load (phase_path)
%         img = img - min(img(:));
        [bounding_box,ROI_conn_3D_6,ROI_conn_3D_26,binary_dir_connectivity] = ...
            determine_ROI_3D_connectivity(label);
        % Take the ROI sub-volume within the bounding box:
        mask_vol_subvol = label(bounding_box(1,1):bounding_box(1,2) , ...
                                   bounding_box(2,1):bounding_box(2,2) , ...
                                   bounding_box(3,1):bounding_box(3,2) );      
        % Now take the image sub-volume that corresponds to this mask:
        img_vol_subvol = img(bounding_box(1,1):bounding_box(1,2) , ...
                                 bounding_box(2,1):bounding_box(2,2) , ...
                                 bounding_box(3,1):bounding_box(3,2) );
        num_ROI_voxels = length(find(mask_vol_subvol));
        img_min = min(img_vol_subvol(logical(mask_vol_subvol)));
        img_max = max(img_vol_subvol(logical(mask_vol_subvol))); 
        % Rescale to image volume to [0,N]:
        img_vol_subvol = num_img_values .* (img_vol_subvol - img_min)/(img_max - img_min) ;

        % Discretize and add 1 to get values {1,2,...,N+1}:
        img_vol_subvol = floor(img_vol_subvol) + 1;
        %%%%%
        %%%%% Histogram-based computations:
        %%%%%
        % Compute the histogram of the ROI and probability of each voxel value:
        vox_val_hist = zeros(num_img_values,1);
        for this_vox_value = 1:num_img_values
            vox_val_hist(this_vox_value) = length(find((img_vol_subvol == this_vox_value) & (mask_vol_subvol == 1) ));
        end
        % Compute the relative probabilities from the histogram:
        vox_val_probs = vox_val_hist / num_ROI_voxels;
        % Compute the histogram_based metrics:
        texture_metrics(1:6) = compute_histogram_metrics(vox_val_probs,num_img_values);
        %%%%%
        %%%%% GTDSM (Co-occurance) Matrix calculations:
        %%%%%
        % Create the Gray-Tone-Spatial-Dependence-Matrix (GTSDM):
        % The max value is currently one higher than it should be (N+1), so put 
        % those voxels at the max value:
        img_vol_subvol(img_vol_subvol==num_img_values+1) = num_img_values;
        GTSDM = compute_3D_GTSDM(mask_vol_subvol,img_vol_subvol,binary_dir_connectivity,num_img_values);
        GTSDM_metrics = zeros(13,20);
        for this_direction = 1:13
            % Compute the metrics for this combination:
            GTSDM_metrics(this_direction,:) = compute_GTSDM_metrics(GTSDM(:,:,this_direction));
        end
        % Now take the mean texture metric value over the all directions:
        texture_metrics(7:26) = mean(GTSDM_metrics,1);
        %%%%%
        %%%%% NTGDM Matrices and metrics
        %%%%%
        [NGTDM,vox_occurances_NGD26] = compute_3D_NGTDM(mask_vol_subvol,img_vol_subvol,binary_dir_connectivity,num_img_values);
        % Compute NGTDM metrics:
        texture_metrics(27:31) = compute_NGTDM_metrics(NGTDM,num_img_values,vox_occurances_NGD26);
        %%%%%
        %%%%% Zone Size matrix and Metrics:
        %%%%%
        % Create the Zone Size Matrix:
        GLZSM = compute_GLZSM(mask_vol_subvol,img_vol_subvol,num_img_values);
        % Compute the Zone Size Metrics:
        texture_metrics(32:42) = compute_zone_size_metrics(GLZSM,num_ROI_voxels);
        feature = texture_metrics;
        if strfind(phase(j).name, 'C-')
           Feature{i-2, 1} = feature;
        elseif strfind(phase(j).name, 'A1')
           Feature{i-2, 2} = feature;
        elseif strfind(phase(j).name, 'P')
           Feature{i-2, 3} = feature;
        elseif strfind(phase(j).name, 'V')
           Feature{i-2, 4} = feature;
        else
           Feature{i-2, 5} = feature;
        end
    end
end
save('Feature_5_3d', 'Feature');
