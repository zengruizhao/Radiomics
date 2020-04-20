function [h_slice, h_axes, h_surf] = ShowErrorsOverlay(segmentation, gt, dim, display_image, options)

    % define the options
    if ~exist('options', 'var') || isempty(options) || ~isstruct(options), options = struct; end
    if ~isfield(options, 'max_mm'), options.max_mm = 10; end
    
    % call the surface errors
    clf;
    h_surf = SurfaceErrors(segmentation, gt, dim, options);
    
    % set up the mesh grid for the image
    image_size = size(display_image);
    [m_x, m_y, m_z] = meshgrid(dim(1) * (1:image_size(1)), dim(2) * (1:image_size(2)), dim(3) * (1:image_size(3)));
    
    % find which slices to display based on the center of mass (com)
    [com_y, com_x, com_z] = ind2sub(image_size, find(segmentation));
    com = [mean(com_x), mean(com_y), mean(com_z)] .* dim;
    if ~isfield(options, 'slice_x')
        slice_x = com(1);
    else
        slice_x = options.slice_x;
    end    
    if ~isfield(options, 'slice_y')
        slice_y = com(2);
    else
        slice_y = options.slice_y;
    end
    if ~isfield(options, 'slice_z')
        slice_z = com(3);
    else
        slice_z= options.slice_z;
    end
    
    % rescale and display the image
    hold on;
    display_image = rescale_range(display_image, -options.max_mm, -eps);
    h_slice = slice(m_x, m_y, m_z, display_image, slice_x, slice_y, slice_z);
    
    % define the visualization properties
    h_axes = gca;
    set(h_slice,'CDataMapping','scaled')
    set(h_slice,'EdgeColor','none')
    set(h_slice,'FaceAlpha',0.75)
    set(h_axes,'Clim',[-options.max_mm, options.max_mm]);
    axis on
    
    % define the colormap with gray and jet
    my_colormap = cat(1, gray(512), jet(512));
    colormap (my_colormap);

    % fix the displayed colorbar
    h_colorbar = findobj(gcf, 'tag', 'Colorbar'); % get its handle
    set(h_colorbar, 'Ylim', [0, options.max_mm]);
    
    % define the labels
    xlabel('x (mm'); ylabel('y (mm)'); zlabel('z (mm)');
    axis off; % hide them
    
end % function