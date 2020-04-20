function bounding_box = findBoundingBox3D(ROI_full_logical)
ROI_full_ind = find(ROI_full_logical);
[ROI_full_row,ROI_full_col,ROI_full_slc] = ind2sub(size(ROI_full_logical),ROI_full_ind);

bounding_box = [min(ROI_full_row) , max(ROI_full_row) ; ...
                min(ROI_full_col) , max(ROI_full_col) ; ...
                min(ROI_full_slc) , max(ROI_full_slc) ];