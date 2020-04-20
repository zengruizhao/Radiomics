cd('haralick');
    general_C_include = '../general_C_templates';
    mex('-I', general_C_include, 'graycomtx_3d.cpp');
    cd('..');

cd('tracebounds');
    mex('-I', general_C_include, 'trace_bound_c.cpp');
    cd('..');
    
    
    %countnodesandedges
