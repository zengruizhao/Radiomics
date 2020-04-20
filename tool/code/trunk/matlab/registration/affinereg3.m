function [Iregd,rigidparams,varargout] = affinereg3(Itemplate,Itarget,N,varargin)
% AFFINEREG3 Register a pair of volumes using a 3D affine transformation.
%
%   ITARG_REGD = AFFINEREG3(ITEMPLATE,ITARGET,N) registers ITARGET (moving)
%   to ITEMPLATE (stationary) by optimizing an affine transformation, and 
%   returns the registered volume, the ITARGET subject to the determined 
%   transformation. The number of graylevels *must* be specified in N.
%
%   [ITARG_REGD,RIGIDPARAMS] = AFFINEREG3(...) returns the determined
%   transformation parameters RIGIDPARAMS, a 9-by-1 array.  See rotmat3.
%
%   [ITARG_REGD,RIGIDPARAMS,SIMVALS] = AFFINEREG3(...) returns the value of
%   the similarity measure at each iteration.
%
%   [ITARG_REGD,RIGIDPARAMS,SIMVALS,M] = AFFINEREG3(...) returns the linear
%   homogeneous coorinate transformation matrix M of size 4-by-4.
%
%   [ITARG_REGD,RIGIDPARAMS,SIMVALS,M,FINALMI] = AFFINEREG3(...) returns
%   the MI between ITARG_REGD and ITEMPLATE.
%
%   [ ... ] = AFFINEREG3(..., 'PARAM1',val1, 'PARAM2',val2, ...) specifies
%   optional parameter name/value pairs to control the registration.
%   Parameters are:
%
%   'InterpMethod' - The coordinate system interpolation method to use when
%      calculating intensities on the deformed target image. Possible 
%      values are 'nearest', 'linear' (default), and 'cubic'. Both 'linear'
%      and 'cubic' can return values not present in I, and 'cubic' can 
%      return values outside the range of values in I.
%
%   'Measure' - The similariy measure to drive transformation optimization.
%      Possible values are 'MI' (default), 'NMI', 'ECC', 'JE', and 'EOH'.
%
%   'BGValue' - The background intensity to use when pixels from outside of
%      the frame of the original image are required. An integer, default 0.
%
%   'NoDisp' - Whether or not to display information and images at each
%      transformation iteration.  Boolean value, default true.
%
%   'StartParams' - Starting transformation parameters (see imdef3). Double 
%      array of size 9-by-1, representing [rx ry rz tx ty tz sx sy sz]'.
%      Default is [0; 0; 0; 0; 0; 0; 1; 1; 1]. Optionaly, scale can be
%      omitted by using a 6-by-1 array. A 7-by-1 array provides 1 scale 
%      parameter for isometric xyz scale (i.e. [rx ry rz tx ty tz sxyz]').
%      A 8-by-1 array provides 1 scale parameter for isometric xy scale and
%      independent z scale (i.e. [[rx ry rz tx ty tz sxy sz]').
%
%   'ParamWeights' - Weight given to each parameter during optimization.
%      Double array of size 9-by-1, optionally length 6, 7 or 8, with the 
%      same meanings as the parameters defined by StartParams. Default is 
%      [30; 30; 125; 4000; 4000; 100; .25; .25; .1]. For example, if you 
%      think there is a large amount of translation that needs to be 
%      corrected, increase the fourth through sixth elements.
%
%   'ParamOffsets' - Constant values added to the parameter set at every
%      iteration. This is functionally different from 'StartParams' in that
%      these parameters never change, and the optimizer is BLINDED to them.
%
%   'scales' - Constrain scale between xyz dimensions. 
%         'scales' Value   |         Effect            
%         ---------------------------------------------
%                0         |        no scaling         
%                1         |     single xyz scale      
%                2         |  an xy scale and a z scale
%                3         |  three independent scales 
%      IMPORTANT: StartParams and ParamWeights lengths override 'scales'.
%
%   'initM' - Constantly pre-applied transformation matrix (see imdef). 
%      Must be 4-by-4. Applied before any other parameters tweaks, either
%      'StartParams' or 'ParamOffsets'.
%
%   'coordshift' - Offset from volume center, about which deformations are 
%      performed, [Y; X; Z]. Positive values impose a shift toward origin.
%      Default coordshift is [0; 0; 0]. MATLAB coordinates this time! This
%      parameter is not properly tested yet.
%
%   'dopadz' - For interpolation, temporarily/internally pad the target 
%      volume with a slice on both ends -- allows blending to work. 
%      Temporary  padding is removed from result. Should not affect 
%      semantics of coordshift, etc.
%
%   NOTE: This function enforces N as a required input for two reasons,
%   (1) the operator will be less likely to make a mistake about the range
%   of the data, and (2) determining or enforcing N automatically would add
%   ambiguity and at least some computation time. Hence, intensities of 
%   input images must must fall within 0 to N-1!! Scale your images 
%   appropriately, or registration will fail or be unpredictable.
%
%   See also: imdef3, rotmat3, objdef3d, affinereg2.
%
%JC

%%% Inputs
% Input Parser
interpmethods={'nearest','linear','cubic','*nearest','*linear','*cubic'};
measures={'MI','NMI','ECC','JE','EOH'};
nmaxparams=9; nminparams=6;

ip=inputParser;
ip.addParamValue('interpmethod', 'nearest', @(x)any(strcmpi(x,interpmethods)));
ip.addParamValue('measure', 'MI', @(x)any(strcmpi(x,measures)));
ip.addParamValue('bgvalue', 0, @(x)isnumeric(x) && numel(x)==1);
ip.addParamValue('nodisp', true, @islogical);
ip.addParamValue('startparams', [0; 0; 0; 0; 0; 0; 1; 1; 1], @(x)isfloat(x) && numel(x)>=nminparams && numel(x)<=nmaxparams);
ip.addParamValue('paramweights', [30; 30; 125; 4000; 4000; 100; .25; .25; .1], @(x)isfloat(x) && numel(x)>=nminparams && numel(x)<=nmaxparams);
ip.addParamValue('scales', 3, @(x)isnumeric(x) && numel(x)==1 && any(x==[0 1 2 3]));
ip.addParamValue('paramoffsets', zeros(9,1), @(x)isfloat(x) && numel(x)>=nminparams && numel(x)<=nmaxparams);
ip.addParamValue('initM', eye(4), @(x)isfloat(x) && numel(x)==16 && size(x,1)==4);
ip.addParamValue('coordshift', zeros(3,1), @(x)isfloat(x) && numel(x)==3);
ip.addParamValue('dopadz', false, @islogical);
ip.addParamValue('usebox', [], @(x)isnumeric(x) && any(numel(x)==[0 1 2 4]));
ip.parse(varargin{:});

% Check *CUSTOM* 'startparams' and 'paramweights'
customfields=setdiff({'startparams','paramweights'},ip.UsingDefaults);
ninputparams=numel(customfields);
if ninputparams,
    % Check length of each input
    inputparamlengths=zeros(ninputparams,1);
    for k=1:ninputparams,
        customfield=char(customfields(k));
        inputparamlengths=numel(ip.Results.(customfield));
        eval([customfield '=ip.Results.' customfield ';']);
    end
    
    % Ensure each input is the same length
    if numel(inputparamlengths)>1 && any(diff(inputparamlengths)),
        error('Input StartParams and ParamWeights should be the same length.'); end
    
    % Check input parameters
    noptimparams=inputparamlengths(1);
    % Based on the number of input parameters, decide how to scale
    scales=noptimparams-nminparams;

    % Verify that input scales agree with any input params
    if ~any(strcmp(ip.UsingDefaults,{'scales'})), % scales was input
        if scales~=ip.Results.scales, % scales was input, and does not agree with other params
            error('Length of input(s) StartParams/ParamWeights do not agree with scales. Omit scales or fix StartParams/ParamWeights.');
        end
        warning('affinereg3:IgnoringInputScales','Input scales redundant with StartParams and/or ParamWeights. Ignoring scales!\n');
    end
    
else % no input ParamWeights or StartParams, only defaults
    % Get scales, may be default or input
    scales=ip.Results.scales;
end

% Check *DEFAULT* 'startparams' and 'paramweights', adjust to input length
defaultfields=intersect(ip.UsingDefaults,{'startparams','paramweights'});
ndefaultparams=numel(defaultfields);
if ndefaultparams,
    % scales may be specified directly or determined from other inputs
    if scales==1,
        optimparams=1:nminparams+1; % plus one param for scale
    elseif scales==2,
        optimparams=[1:nminparams+1 nmaxparams]; % one param for xy scale and another for z
    elseif scales==3,
        optimparams=1:nmaxparams; % all defaults or inputs indicate scales=3
    else % no scale
        optimparams=1:nminparams; % no params for scale
    end
    noptimparams=numel(optimparams); % only necessary when startparams and paramweights both default
    
    % Fix the defaults to contain only the parameters we wish top optimize
    for k=1:ndefaultparams,
        defaultfield=char(defaultfields(k));
        defaultfieldopt=ip.Results.(defaultfield)(optimparams); %#ok<NASGU>
        eval([defaultfield '=defaultfieldopt;']);
    end
end

% Change to columns
% (these were generated by eval, hence the M-lint suppression)
startparams=startparams(:); paramweights=paramweights(:); %#ok<NODEF>

% Pad paramoffsets to nmaxparams (9), if necessary
paramoffsets=ip.Results.paramoffsets(:);
nparamoffsets=numel(paramoffsets);
if nparamoffsets==nminparams || nparamoffsets==nmaxparams,
    paramoffsets=[paramoffsets zeros(nmaxparams-nparamoffsets,1)];
else
    if nparamoffsets==nminparams+1 && scales==1,
        threescales=[nminparams+1 nminparams+1 nminparams+1];
    elseif nparamoffsets==nminparams+2 && scales==2,
        threescales=[nminparams+1 nminparams+1 nminparams+2];
    else
        error('paramoffsets length incompatible with scales or other params');
    end
    paramoffsets(nparamoffsets:nmaxparams)=paramoffsets(threescales);
end

% Display the parameters to be used for registration
paramstring=repmat('%.4g, ',1,numel(startparams));
fprintf(['StartParams=[' paramstring '\b\b].\n'],startparams);
paramstring=repmat('%.4g, ',1,numel(paramweights));
fprintf(['ParamWeights=[' paramstring '\b\b].\n'],paramweights);
paramstring=repmat('%.4g, ',1,nmaxparams);
fprintf(['ParamOffsets=[' paramstring '\b\b].\n'],paramoffsets);

% Make some short variable names
interpmethod=ip.Results.interpmethod;
bgvalue=cast(ip.Results.bgvalue,class(Itarget));
preM=ip.Results.initM;
N=cast(N,class(Itarget));

%%% Registration
% Pad z (or not) -- we unpad later
if ip.Results.dopadz,
    % pad on both sides evenly so as not to change center
    Itarget=cat(3,bgvalue*ones(size(Itarget,1),size(Itarget,2),class(Itarget)), Itarget, bgvalue*ones(size(Itarget,1),size(Itarget,2),class(Itarget)));
    % pad is like translating 1 pixel in z: we must undo this after deformation
    padslice=1;
else
    padslice=0;
end

% Optional Display
if ~ip.Results.nodisp,
    midslicenum=ceil(size(Itemplate,3)/2); % Template!
    figure;
    subplot(121)
    imdisp(Itemplate(:,:,midslicenum))
    title('Template')

    subplot(122)
    if ~isempty(setdiff({'initM','startparams','paramoffsets'},ip.UsingDefaults)), % if any of initM, startparams or paramoffsets are not defaults
        startparamstemp=fixrigidparams(startparams,paramoffsets); % don't apply paramweights here, that's for the optimizer
        Iinit=imdef3(startparamstemp,Itarget,interpmethod,bgvalue,...
            'preM',preM,'coordshift',ip.Results.coordshift);
        Iinit=cropvolume(Iinit,size(Itemplate),padslice); % removes any padslice
        Iinit=Iinit(:,:,midslicenum); % size of Itemplate slice
    else
        Iinit=Itarget(:,:,midslicenum+padslice); % size of Itarget slice
    end
    imdisp(Iinit)
    title('Target')
    drawnow
    
    optoptions = optimset('Display','iter');
else
    optoptions = optimset('Display','notify');
end

% Adjust input startparams by paramweights to give proper deformation inside objective function
startparams_objdef=startparams(:);
startparams_objdef(1:nminparams)=startparams_objdef(1:nminparams)./paramweights(1:nminparams);
if noptimparams>nminparams,
    startparams_objdef(nminparams+1:noptimparams) = ...
        (startparams_objdef(nminparams+1:noptimparams)-1)./paramweights(nminparams+1:noptimparams) + 1;
end

% Do optimization with objective function:
usebox=ip.Results.usebox(:)';
fprintf('Measure: %s.  Interpolation: %s.\n',ip.Results.measure,ip.Results.interpmethod);
rigidparams_objdef=fminsearch(@objdef3d, startparams_objdef, optoptions,...
	Itarget, Itemplate, bgvalue, N,...
    ip.Results.nodisp,paramweights,ip.Results.measure,...
    ip.Results.interpmethod,paramoffsets,ip.Results.coordshift,...
    ip.Results.dopadz,preM,usebox);

% Grab error record
if nargout>2,
    err_record=objdef3d;
    MIiters=-err_record(2:end);
    varargout{1}=MIiters;
end

% Clear persistent variable in objective function
clear objdef3d.m

% Apply paramoffsets and weights, and make rigidparams length 9 (based on scales rules)
rigidparams = fixrigidparams(rigidparams_objdef,paramoffsets,paramweights);

% Generate the registered image
[Iregd]=imdef3(rigidparams, Itarget, interpmethod, bgvalue,...
    'preM',preM,'coordshift',ip.Results.coordshift);
[IregdLIN,M]=imdef3(rigidparams, Itarget, 'linear', bgvalue,...
    'preM',preM,'coordshift',ip.Results.coordshift);

% Fix data range for cubic interpolation
Iregd(Iregd<0)=0; Iregd(Iregd>(N-1))=N-1;

% Remove any z padding slices that were added
if ip.Results.dopadz,
    Iregd=Iregd(:,:,2:end-1);
    IregdLIN=IregdLIN(:,:,2:end-1);
end

if ~isempty(ip.UsingDefaults) && ~any(strcmp(ip.UsingDefaults,'coordshift')),
    fprintf('Remember to apply your coordshift when you use rigidparams or M!!!.\n');
end

%%% Outputs
% Crop Iregd to Itemplate size - the SAME way as done in objdef3
if nargout>3 && any(size(Iregd)~=size(Itemplate)),
    Iregd_crop=cropvolume(round(Iregd),size(Itemplate));
    IregdLIN_crop=cropvolume(round(IregdLIN),size(Itemplate));
elseif nargout>3 || ~ip.Results.nodisp, % same size, but need the cropped image variables for certain outputs and display
    IregdLIN_crop=round(IregdLIN);
    Iregd_crop=round(Iregd);
end

if any(size(Iregd)~=size(Itemplate)), fprintf('You must crop the registerd volume (first output argument) to the size of the template using cropvolume, or request/use output number 4!.\n'); end

% Deal outputs
if nargout>3,
    varargout{2}=M;
    if nargout>4,
        varargout{3}=Iregd_crop;
        if nargout>5,
            finalMI=MI(double(Itemplate),double(IregdLIN_crop),N); % why LIN? dunno
            varargout{4}=finalMI;
            % paramstring=repmat('%7.4f, ',1,numel(rigidparams));
            paramstring=repmat('%.4g, ',1,numel(rigidparams));
            fprintf(['Done: params=[' paramstring '\b\b], finalMI=%g.\n'],rigidparams,finalMI);
        else
            % paramstring=repmat('%7.4f, ',1,numel(rigidparams));
            paramstring=repmat('%.4g, ',1,numel(rigidparams));
            fprintf(['Done: params=[' paramstring '\b\b].\n'],rigidparams);
        end
    end
end

% Calculate overlay and checker images
if nargout>6 || ~ip.Results.nodisp,
    overlayweight=0.5;
    overlayims=rescale(rescale(Iregd_crop)+overlayweight*rescale(Itemplate));
    if nargout>6, varargout{5}=overlayims; end
    if nargout>7 || ~ip.Results.nodisp,
        checkerims=imchecker(rescale(Iregd_crop),rescale(Itemplate),round(size(Iregd_crop,1)/35));
        if nargout>6, varargout{6}=checkerims; end
    end
end

% Show final transformed target image
if ~ip.Results.nodisp,
    figure;
    imdisp(Iregd_crop(:,:,midslicenum));
    title(['Registration Result.  Interpolation: ' interpmethod]);

    figure; imdisp(overlayims(:,:,midslicenum));
    figure; imdisp(checkerims(:,:,midslicenum));
end

end


function rigidparams = fixrigidparams(rigidparams,paramoffsets,paramweights)
% Apply the weightings that the objective function applied - 9 params
nminparams=6; nmaxparams=9;
if nargin<3, paramweights=ones(numel(rigidparams),1); end % if already weighted

% Rotation and Translation -- weights and offsets
rigidparams(1:nminparams)=rigidparams(1:nminparams).*paramweights(1:nminparams)+paramoffsets(1:nminparams);

% Scale, isoscale (both optional)
scaleparamnums=nminparams+1:nmaxparams; % 7:9
% Based on the number of input parameters, decide how to scale
noptimparams=numel(rigidparams);
scales=noptimparams-nminparams;
if scales,
    % If any isometric scaling, convert to 9-by-1 by replicating appropriate values
    if scales==1, % one scale
        threescales=[nminparams+1 nminparams+1 nminparams+1]; % 7 7 7
    elseif scales==2, % two scales
        threescales=[nminparams+1 nminparams+1 nminparams+2]; % 7 7 8
    else % three scales
        threescales=scaleparamnums; % 7 8 9
    end
    rigidparams(scaleparamnums)=(rigidparams(threescales)'-1).*paramweights(threescales)' + 1; % apply weights
else % no scale
    rigidparams(scaleparamnums)=ones(3,1);
end
% apply paramoffsets to scale
rigidparams(scaleparamnums)=rigidparams(scaleparamnums)+paramoffsets(scaleparamnums);

end
