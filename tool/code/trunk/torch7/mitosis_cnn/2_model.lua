----------------------------------------------------------------------
-- This script demonstrates how to define a couple of different
-- models:
--   + linear
--   + 2-layer neural network (MLP)
--   + convolutional network (ConvNet)
--
-- It's a good idea to run this script with the interactive mode:
-- $ torch -i 2_model.lua
-- this will give you a Torch interpreter at the end, that you
-- can use to play with the model.
--
-- Clement Farabet
----------------------------------------------------------------------

require 'torch'   -- torch
require 'image'   -- to visualize the dataset
require 'nn'      -- provides all sorts of trainable modules/layers

----------------------------------------------------------------------
-- parse command line arguments
if not opt then
   print '==> processing options'
   cmd = torch.CmdLine()
   cmd:text()
   cmd:text('MITOS Model Definition')
   cmd:text()
   cmd:text('Options:')
   cmd:option('-model', 'convnet', 'type of model to construct: linear | mlp | convnet')
	cmd:option('-convfun','spmm','type of spatial convolution : sp | spmm | spmap ')
	cmd:option('-convnorm', 1, 'apply normalization over output of convolution and pooling layer')
	cmd:option('-convnormk',7,'normalization kernel size for output of convolution and pooling layer')
	cmd:option('-poolfun','L2','pool function [ max | L2 ]')
	cmd:option('-arch','a','predefined architecture [ a | b ]')
	cmd:option('-cnnf1',16,'number of features in convnet layer 1')
	cmd:option('-cnnf2',256,'number of features in convnet layer 2')
	cmd:option('-cnnf3',0,'number of features in convnet layer 3')
	cmd:option('-cnnf4',0,'number of features in convnet layer 4')
	cmd:option('-cnnf5',0,'number of features in convnet layer 5')
	cmd:option('-fcf1',128,'number of features in full layer 1')
	cmd:option('-fcf2',0,'number of features in full layer 2')
	cmd:option('-imgsize', 'small', 'how big is the input image size: small | orig , 81x81 and 101x101 respectively')
   cmd:option('-visualize', false, 'visualize input data and weights during training')
   cmd:text()
   opt = cmd:parse(arg or {})
end


if opt.imgsize == 'orig' then
   imgsize = 101
else
   imgsize = 81
end

----------------------------------------------------------------------
print '==> define parameters'

-- 2-class problem
noutputs = 2

-- input dimensions
nfeats = 3
width = imgsize
height = imgsize
ninputs = nfeats*width*height

-- number of hidden units (for MLP only):
nhiddens = 1000 --ninputs / 2
--nhiddens = 200

-- architectures supported for convnet

if opt.cnnf1==0 then
	opt.cnnf1=nil
end
if opt.cnnf2==0 then
   opt.cnnf2=nil
end
if opt.cnnf3==0 then
   opt.cnnf3=nil
end
if opt.cnnf4==0 then
   opt.cnnf4=nil
end
if opt.cnnf5==0 then
   opt.cnnf5=nil
end
if opt.fcf1==0 then
   opt.fcf1=nil
end
if opt.fcf2==0 then
   opt.fcf2=nil
end


nstates1 = {opt.cnnf1,opt.cnnf2,opt.cnnf3,opt.cnnf4,opt.cnnf5}
nstates2 = {opt.fcf1,opt.fcf2}

depth = #nstates1

nosupported = 0
if depth == 2 then
	if opt.arch == 'a' then
		filtsize = {8,8}
		poolsize = {2,2}
	else
      nosupported = 1
   end
elseif depth == 3 then
	if opt.arch == 'a' and opt.imgsize == 'small' then
		filtsize = {4,4,3}
   	poolsize = {2,2,2}
	elseif opt.arch == 'a' and opt.imgsize == 'orig' then
   	filtsize = {4,4,4}
   	poolsize = {2,2,2}
	elseif opt.arch == 'b' and opt.imgsize == 'orig' then
   	filtsize = {2,3,3}
   	poolsize = {2,2,2}
	else 
		nosupported = 1
	end
elseif depth == 4 then
	if opt.arch == 'a' and opt.imgsize == 'small' then
   	filtsize = {4,4,3,3}
   	poolsize = {2,2,2,2}
	elseif opt.arch == 'b' and opt.imgsize == 'small' then
   	filtsize = {2,3,2,2}
   	poolsize = {2,2,2,2}
	elseif opt.arch == 'a' and opt.imgsize == 'orig' then
   	filtsize = {4,4,4,3}
   	poolsize = {2,2,2,2}
	elseif opt.arch == 'b' and opt.imgsize == 'orig' then
   	filtsize = {2,3,3,2}
   	poolsize = {2,2,2,2}
	else 
		nosupported = 1
   end
elseif depth == 5 then
	if opt.arch == 'a' and opt.imgsize == 'orig' then
   	filtsize = {2,3,3,2,2}
   	poolsize = {2,2,2,2,2}
	else
      nosupported = 1
   end
end

if nosupported == 1 then
	error('depth=' .. depth .. ' and arch=' .. opt.arch .. ' not supported')
end


--filtsize = 5
--poolsize = 2


normkernel = image.gaussian1D(opt.convnormk)


----------------------------------------------------------------------
print '==> construct model'

if opt.model == 'linear' then

   -- Simple linear model
   model = nn.Sequential()
   model:add(nn.Reshape(ninputs))
   model:add(nn.Linear(ninputs,noutputs))

elseif opt.model == 'mlp' then

   -- Simple 2-layer neural network, with tanh hidden units
   model = nn.Sequential()
   model:add(nn.Reshape(ninputs))
   model:add(nn.Linear(ninputs,nhiddens))
   model:add(nn.Tanh())
   model:add(nn.Linear(nhiddens,noutputs))

elseif opt.model == 'convnet' then

   model = nn.Sequential()
	
	nstates_ant = nfeats
	finalDim1 = width
	finalDim2 = height
	nlayers1 = #nstates1
	for l = 1,nlayers1 do
		if opt.convfun == 'sp' then
			model:add(nn.SpatialConvolution(nstates_ant, nstates1[l], filtsize[l], filtsize[l]))
		elseif opt.convfun == 'spmm' then
			model:add(nn.SpatialConvolutionMM(nstates_ant, nstates1[l], filtsize[l], filtsize[l]))
		elseif opt.convfun == 'spmap' then
			model:add(nn.SpatialConvolutionMap(nstates_ant, nstates1[l], filtsize[l], filtsize[l])) -- need to review
		else
			error('convfun=' .. opt.convfun .. ' no supported')
		end
   	model:add(nn.Tanh())		-- check if there are other options
		if opt.poolfun == 'max' then	-- check for more options
			model:add(nn.SpatialMaxPooling(poolsize[l],poolsize[l],poolsize[l],poolsize[l]))
		elseif opt.poolfun == 'L2' then
   		model:add(nn.SpatialLPPooling(nstates1[l],2,poolsize[l],poolsize[l],poolsize[l],poolsize[l]))
		else 
			error('poolfun=' .. opt.poolfun .. ' no supported')
		end
		if opt.convnorm then
			model:add(nn.SpatialSubtractiveNormalization(nstates1[l], normkernel))		
		end
		nstates_ant = nstates1[l]
		finalDim1 = (finalDim1-filtsize[l]+1)/poolsize[l]
		finalDim2 = (finalDim2-filtsize[l]+1)/poolsize[l]
		--print(nstates1[l])
		--print(finalDim1)
		--print(finalDim2)
	end

	--print(nstates1[nlayers1])
	nstates_ant = nstates1[nlayers1]*finalDim1*finalDim2
	model:add(nn.Reshape(nstates_ant))
	nlayers2 = #nstates2
	--print(nlayers2)
	--print(nstates_ant)
   for l = 1,nlayers2 do
   	model:add(nn.Linear(nstates_ant, nstates2[l]))
   	model:add(nn.Tanh())
		nstates_ant = nstates2[l]
   end
	--print(nstates_ant)
	model:add(nn.Linear(nstates_ant, noutputs))

else

   error('unknown -model')

end

----------------------------------------------------------------------
print '==> here is the model:'
print(model)

----------------------------------------------------------------------
-- Visualization is quite easy, using image.display(). Check out:
-- help(image.display), for more info about options.

if opt.visualize then
   if opt.model == 'convnet' then
      print '==> visualizing ConvNet filters'
      image.display{image=model:get(1).weight, padding=2, zoom=4, legend='filters @ layer 1'}
      image.display{image=model:get(5).weight, padding=2, zoom=4, nrow=32, legend='filters @ layer 2'}
   end
end
