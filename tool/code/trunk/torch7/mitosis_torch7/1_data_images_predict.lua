
----------------------------------------------------------------------
-- This script demonstrates how to load the (SVHN) House Numbers 
-- training data, and pre-process it to facilitate learning.
--
-- The SVHN is a typicaly example of supervised training dataset. 
-- The problem to solve is a 10-class classification problem, similar
-- to the quite known MNIST challenge.
--
-- It's a good idea to run this script with the interactive mode:
-- $ torch -i 1_data.lua
-- this will give you a Torch interpreter at the end, that you
-- can use to analyze/visualize the data you've just loaded.
--
-- Clement Farabet
----------------------------------------------------------------------

require 'torch'   -- torch
require 'image'   -- to visualize the dataset
require 'nn'      -- provides a normalization operator

----------------------------------------------------------------------
-- parse command line arguments

--TODO: Esto creo que no es necesario... Quitar en el proximo analisis del codigo
if not opt then
   print '==> processing options'
   cmd = torch.CmdLine()
   cmd:text()
   cmd:text('MITOS Dataset Preprocessing')
   cmd:text()
   cmd:text('Options:')
   cmd:option('-imgsize', 81, 'how big is the input image size: e.g. 81 | 101 ')
	cmd:option('-dataset', "S2mini", 'dataset: icpr2012+A | icpr2012+H | miccai2013')
	cmd:option('-subset', "test", 'subset: train | test')
	cmd:option('-meanfile', "mean_icpr2012+A.dat", 'file path for mean array for preprocessing step')
	cmd:option('-stdfile', "std_icpr2012+A.dat", 'file path for mean array for preprocessing step')
   cmd:option('-dsperc',5,'percentage of dataset (stratified)')
   cmd:option('-visualize', true, 'visualize input data and weights during training')
   cmd:text()
   opt = cmd:parse(arg or {})
end

imgsize = opt.imgsize

--Descomentar siguiente par de lineas para dejar como antes

--mean=torch.load(opt.meanfile)
--std=torch.load(opt.stdfile)

mean = {}
std = {}


--Esto puede ser un error se esta utilizando el mean de otro dataset para realizar la normalizacion!!


--HPC

--prefixdatapath="/mnt/Data7/aac79/Experiments/datasets/mitosis/hpfs/"
--prefixdatapath="/home/der68/data7Home/datasets"
--prefixdatapath="/home/der68/data7Home/datasets/mitosis/hpfs/"
--prefixdatapath="/home/der68/data7Home/datasets/mitosis_s2_mini/"
--prefixdatapath="/home/der68/data7Home/datasets/mitosis/hpfs/"


--Mirabela's PC

--prefixdatapath="/home/der68/datasets/mitosis_s2_mini/"
prefixdatapath="/home/der68/datasets/mitosis/hpfs/"


print("==> reading dataset " .. opt.dataset)
if opt.dataset == "icpr2012+H" then
	gtcoord="XY"   -- ICPR:XY  MICCAI:YX
   pixDist = 21.99    -- ICPR: 20.4, MICCAI: 30
   --train_img_dir = "/home/aacruz/Experiments/datasets/mitosis/hpfs/icpr2012/H/train"
	if opt.subset == 'train' then
   	test_img_dir = prefixdatapath .. "icpr2012/H/train"
	elseif opt.subset == 'test' then
	test_img_dir = prefixdatapath .. "icpr2012/H/test"	
	end
   file_ext = "png"
elseif opt.dataset == "icpr2012+A" then
	gtcoord="XY" 	-- ICPR:XY  MICCAI:YX
	pixDist = 20.4 	-- ICPR: 20.4, MICCAI: 30
	--train_img_dir = "/home/aacruz/Experiments/datasets/mitosis/hpfs/icpr2012/A/train"
	if opt.subset == 'train' then
		test_img_dir = prefixdatapath .. "icpr2012/A/train"
	elseif opt.subset == 'test' then
		test_img_dir = prefixdatapath .. "icpr2012/A/test"
	end
	file_ext = "png"
elseif opt.dataset == "miccai2013" then
	gtcoord="XY"   -- ICPR:XY  MICCAI:YX
        pixDist = 30    -- ICPR: 20.4, MICCAI: 30
   --train_img_dir = "/home/aacruz/Experiments/datasets/mitosis/hpfs/miccai2013/train"
	if opt.subset == 'train' then
		test_img_dir = prefixdatapath .. "miccai2013/train"
	elseif opt.subset == 'test' then
	   test_img_dir = prefixdatapath .. "miccai2013/test"
	end
   file_ext = "png"	

elseif opt.dataset == "S2mini" then
   	gtcoord="XY"   -- ICPR:XY  MICCAI:YX
	pixDist = 30    -- ICPR: 20.4, MICCAI: 30
   --train_img_dir = "/home/aacruz/Experiments/datasets/mitosis/hpfs/miccai2013/train"
	if opt.subset == 'train' then
	   test_img_dir = prefixdatapath .. "mitosis_s2_mini/train"
	elseif opt.subset == 'test' then
	   test_img_dir = prefixdatapath .. "mitosis_s2_mini/test"
	end
	file_ext = "png"
end


test_can_files = {}
--test_can_labels = {}
test_can_centroids ={}
--ctPos=1
ctCan=1

for file in paths.files(test_img_dir) do
   if file:find(file_ext .. '$') then
	  --file_cenT = test_img_dir .. "/" .. string.gsub(file, ".png", "_cenT.txt") -- ground truth centroids
	  file_cenC = test_img_dir .. "/" .. string.gsub(file, ".png", "_cenC.txt") -- candidates centroids
	  for coordC in io.lines(file_cenC) do	-- look for negatives
		xtmp = string.gsub(coordC:sub(1,(coordC:find('\t'))),"\t","")
		ytmp = string.gsub(coordC:sub((coordC:find('\t'))),"\t","")
		if gtcoord =="XY" then
			xC=tonumber(xtmp)
			yC=tonumber(ytmp)
		else
			yC=tonumber(xtmp)
			xC=tonumber(ytmp)
		end
			centroid = torch.IntTensor(2)
			centroid[1]=xC
			centroid[2]=yC
			test_can_centroids[ctCan]=centroid
			test_can_files[ctCan]=test_img_dir .. "/" .. file
			--test_can_labels[ctCan]=-1
			ctCan=ctCan+1	
	  end
   end
end

tesize_can = #test_can_files

----------------------------------------------------------------------

-- select a stratified subset percentage of data 

samples_perc = opt.dsperc
tesize_can = math.floor(tesize_can*samples_perc/100)

tesize = tesize_can
test_files = {}
test_centroids = {}

shuffle = torch.range(1, #test_can_files)

--shuffle = torch.range(tesize_can*1 + 1, tesize_can*2)




for i=1, tesize_can do
-- print(shuffle[i])
    test_files[i]=test_can_files[shuffle[i]]
    test_centroids[i]=test_can_centroids[shuffle[i]]	
end

print '==> loading dataset'


print("<dataset> loading test dataset # " .. tesize )
test_images = {}
local hpf = nil
patch = nil
file_ant = nil
for i,file in ipairs(test_files) do
   -- load each image
	if math.fmod(i,100)==0 then
	   print("Loading... " .. i .. " / " .. #test_files)
	end

	if file_ant == nil or  file_ant ~= file then
	   hpf = image.load(file)
	end
	file_ant=file
	hgt = hpf:size(2)
	wdt = hpf:size(3)
	x = test_centroids[i][1]
	y = test_centroids[i][2]
	startx = x - math.floor(imgsize/2)
	starty = y - math.floor(imgsize/2)
	endx = startx + imgsize
	endy = starty + imgsize
	if startx < 1 then
	   startx=1
	   endx=startx+imgsize
	end
	if starty < 1 then
	   starty=1
	   endy=starty+imgsize
	end
	if endx > wdt then
	   endx=wdt
	   startx=endx-imgsize
	end
	if endy > hgt then
	   endy=hgt
	   starty=endy-imgsize
	end
	
	local patch = image.crop(hpf,startx,starty,endx,endy)
	table.insert(test_images, patch)
	patch = nil
	--hpf = nil
	collectgarbage()
end

testData = {
   data = torch.Tensor(tesize,3,imgsize,imgsize),
   --labels = torch.Tensor(tesize),
   centroids = torch.Tensor(tesize,2),
   files = test_files,
   size = function() return tesize end
}

for i=1,tesize do
   testData.data[{i}] = test_images[i]
   --testData.labels[{i}] = test_labels[i]
   testData.centroids[{i}] = test_centroids[i]
end

test_images = nil
collectgarbage()


if opt.visualize then
   print '==> visualizing data examples'
   
   samples={}
   shuffle = torch.randperm(testData:size())
   for i=1,100 do
	samples[i] = testData.data[shuffle[i]]
   end
   image.display{image=samples, nrow=10, legend='Some testing examples'}
end


----------------------------------------------------------------------
print '==> preprocessing data'

-- Preprocessing requires a floating point representation (the original
-- data is stored on bytes). Types can be easily converted in Torch, 
-- in general by doing: dst = src:type('torch.TypeTensor'), 
-- where Type=='Float','Double','Byte','Int',... Shortcuts are provided
-- for simplicity (float(),double(),cuda(),...):

--trainData.data = trainData.data:float()
testData.data = testData.data:float()

-- We now preprocess the data. Preprocessing is crucial
-- when applying pretty much any kind of machine learning algorithm.

-- For natural images, we use several intuitive tricks:
--   + images are mapped into YUV space, to separate luminance information
--     from color information
--   + the luminance channel (Y) is locally normalized, using a contrastive
--     normalization operator: for each neighborhood, defined by a Gaussian
--     kernel, the mean is suppressed, and the standard deviation is normalized
--     to one.
--   + color channels are normalized globally, across the entire dataset;
--     as a result, each color component has 0-mean and 1-norm across the dataset.

-- Convert all images to YUV
print '==> preprocessing data: colorspace RGB -> YUV'

for i = 1,testData:size() do
   testData.data[i] = image.rgb2yuv(testData.data[i])
end

-- Name channels for convenience
channels = {'y','u','v'}

-- Normalize each channel, and store mean/std
-- per channel. These values are important, as they are part of
-- the trainable parameters. At test time, test data will be normalized
-- using these values.
print '==> preprocessing data: normalize each feature (channel) globally'

-- Normalize test data, using the training means/stds
for i,channel in ipairs(channels) do
   -- normalize each channel globally:
   
   mean[i] = testData.data[{ {},i,{},{} }]:mean()
   std[i] = testData.data[{ {},i,{},{} }]:std()
   
   testData.data[{ {},i,{},{} }]:add(-mean[i])
   testData.data[{ {},i,{},{} }]:div(std[i])
end

-- Local normalization
print '==> preprocessing data: normalize all three channels locally'

-- Define the normalization neighborhood:
neighborhood = image.gaussian1D(47)

-- Define our local normalization operator (It is an actual nn module, 
-- which could be inserted into a trainable model):
normalization = nn.SpatialContrastiveNormalization(1, neighborhood, 1):float()

-- Normalize all channels locally:
for c in ipairs(channels) do
   for i = 1,testData:size() do
      testData.data[{ i,{c},{},{} }] = normalization:forward(testData.data[{ i,{c},{},{} }])
   end
end

----------------------------------------------------------------------
print '==> verify statistics'

-- It's always good practice to verify that data is properly
-- normalized.

for i,channel in ipairs(channels) do

   testMean = testData.data[{ {},i }]:mean()
   testStd = testData.data[{ {},i }]:std()


   print('test data, '..channel..'-channel, mean: ' .. testMean)
   print('test data, '..channel..'-channel, standard deviation: ' .. testStd)
end

