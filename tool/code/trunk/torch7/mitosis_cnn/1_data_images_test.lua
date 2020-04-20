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
if not opt then
   print '==> processing options'
   cmd = torch.CmdLine()
   cmd:text()
   cmd:text('MITOS Dataset Preprocessing')
   cmd:text()
   cmd:text('Options:')
   cmd:option('-imgsize', 81, 'how big is the input image size: e.g. 81 | 101 ')
	cmd:option('-dataset', "icpr2012+A", 'dataset: icpr2012+A | icpr2012+H | miccai2013')
	cmd:option('-subset', "test", 'subset: train | test')
	cmd:option('-meanfile', "mean_icpr2012+A.dat", 'file path for mean array for preprocessing step')
	cmd:option('-stdfile', "std_icpr2012+A.dat", 'file path for mean array for preprocessing step')
--   cmd:option('-duprot', 1, 'duplicate training samples by rotation')
--   cmd:option('-dupflip', 1, 'duplicate training samples by flipping (mirroring)')
--	cmd:option('-dupdist', 1, 'duplicate training samples by surronding coordinates a given distance')
   cmd:option('-dsperc',5,'percentage of dataset (stratified)')
   --cmd:option('-size', 'small', 'how many samples do we load: small | medium | full')
   cmd:option('-visualize', false, 'visualize input data and weights during training')
   cmd:text()
   opt = cmd:parse(arg or {})
end
----------------------------------------------------------------------
--if not paths.filep(train_file) then
--   os.execute('wget ' .. www .. train_file)
--end
--if not paths.filep(test_file) then
--   os.execute('wget ' .. www .. test_file)
--end
--if opt.size == 'extra' and not paths.filep(extra_file) then
--   os.execute('wget ' .. www .. extra_file)   
--end
----------------------------------------------------------------------
imgsize = opt.imgsize

mean=torch.load(opt.meanfile)
std=torch.load(opt.stdfile)

print("==> reading dataset " .. opt.dataset)
if opt.dataset == "icpr2012+H" then
	gtcoord="XY"   -- ICPR:XY  MICCAI:YX
   pixDist = 21.99    -- ICPR: 20.4, MICCAI: 30
   --train_img_dir = "/home/aacruz/Experiments/datasets/mitosis/hpfs/icpr2012/H/train"
	if opt.subset == 'train' then
   	test_img_dir = "/home/aacruz/Experiments/datasets/mitosis/hpfs/icpr2012/H/train"
	elseif opt.subset == 'test' then
      test_img_dir = "/home/aacruz/Experiments/datasets/mitosis/hpfs/icpr2012/H/test"	
	end
   file_ext = "png"
elseif opt.dataset == "icpr2012+A" then
	gtcoord="XY" 	-- ICPR:XY  MICCAI:YX
	pixDist = 20.4 	-- ICPR: 20.4, MICCAI: 30
	--train_img_dir = "/home/aacruz/Experiments/datasets/mitosis/hpfs/icpr2012/A/train"
	if opt.subset == 'train' then
		test_img_dir = "/home/aacruz/Experiments/datasets/mitosis/hpfs/icpr2012/A/train"
	elseif opt.subset == 'test' then
		test_img_dir = "/home/aacruz/Experiments/datasets/mitosis/hpfs/icpr2012/A/test"
	end
	file_ext = "png"
elseif opt.dataset == "miccai2013" then
	gtcoord="YX"   -- ICPR:XY  MICCAI:YX
   pixDist = 30    -- ICPR: 20.4, MICCAI: 30
   --train_img_dir = "/home/aacruz/Experiments/datasets/mitosis/hpfs/miccai2013/train"
	if opt.subset == 'train' then
		test_img_dir = "/home/aacruz/Experiments/datasets/mitosis/hpfs/miccai2013/train"
	elseif opt.subset == 'test' then
	   test_img_dir = "/home/aacruz/Experiments/datasets/mitosis/hpfs/miccai2013/test"
	end
   file_ext = "png"	
end

-- for duplication by surronding distance 
--nangles = 8
--ncircles = 2

-- training samples
--train_pos_files = {}
--train_pos_labels = {}
--train_pos_centroids ={}
--train_pos_dupid={}
--train_neg_files = {}
--train_neg_labels = {}
--train_neg_centroids ={}
--train_neg_dupid={}
ctPos=1
ctNeg=1
--for file in paths.files(train_img_dir) do
--   if file:find(file_ext .. '$') then
--	  file_cenT = train_img_dir .. "/" .. string.gsub(file, ".png", "_cenT.txt") -- ground truth centroids
--	  file_cenC = train_img_dir .. "/" .. string.gsub(file, ".png", "_cenC.txt") -- candidates centroids
--	  for coord in io.lines(file_cenT) do	-- look for positves
--		xtmp = string.gsub(coord:sub(1,(coord:find('\t'))),"\t","")
--		ytmp = string.gsub(coord:sub((coord:find('\t'))),"\t","")
--		if gtcoord =="XY" then
--			x=tonumber(xtmp)
--			y=tonumber(ytmp)
--		else
--			y=tonumber(xtmp)
--			x=tonumber(ytmp)
--		end
--		centroid = torch.IntTensor(2)
--		centroid[1]=x
--		centroid[2]=y
--		train_pos_centroids[ctPos]=centroid
--		train_pos_files[ctPos]=train_img_dir .. "/" .. file
--		train_pos_labels[ctPos]=1
--		train_pos_dupid[ctPos]=1		
--		ctPos=ctPos+1
--		if opt.dupflip	== 1 then
--			train_pos_centroids[ctPos]=centroid
--      	train_pos_files[ctPos]=train_img_dir .. "/" .. file
--      	train_pos_labels[ctPos]=1
--      	train_pos_dupid[ctPos]=2
--			ctPos=ctPos+1

--			train_pos_centroids[ctPos]=centroid
--         train_pos_files[ctPos]=train_img_dir .. "/" .. file
--         train_pos_labels[ctPos]=1
--         train_pos_dupid[ctPos]=3
--         ctPos=ctPos+1

--			train_pos_centroids[ctPos]=centroid
--         train_pos_files[ctPos]=train_img_dir .. "/" .. file
--         train_pos_labels[ctPos]=1
--         train_pos_dupid[ctPos]=4
--         ctPos=ctPos+1
--		end
--		if opt.duprot == 1 then
--         train_pos_centroids[ctPos]=centroid
--         train_pos_files[ctPos]=train_img_dir .. "/" .. file
--         train_pos_labels[ctPos]=1
--         train_pos_dupid[ctPos]=5
--         ctPos=ctPos+1
      
--         train_pos_centroids[ctPos]=centroid
--         train_pos_files[ctPos]=train_img_dir .. "/" .. file
--         train_pos_labels[ctPos]=1
--         train_pos_dupid[ctPos]=6
--         ctPos=ctPos+1

--         train_pos_centroids[ctPos]=centroid
--         train_pos_files[ctPos]=train_img_dir .. "/" .. file
--         train_pos_labels[ctPos]=1
--         train_pos_dupid[ctPos]=7
--         ctPos=ctPos+1

--			train_pos_centroids[ctPos]=centroid
--         train_pos_files[ctPos]=train_img_dir .. "/" .. file
--         train_pos_labels[ctPos]=1
--         train_pos_dupid[ctPos]=8
--        	ctPos=ctPos+1
--     	end
	
--		if opt.dupdist == 1 then
--			local angleincrem = (2*math.pi)/nangles
--			for ci = 1, ncircles do
--				local rad = pixDist/ci
--				for ag = 1, nangles do
--					local theta=(ag-1)*angleincrem
--					local xx = rad*math.cos(theta)
--					local yy = rad*math.sin(theta)
					
--					local centroid2 = torch.IntTensor(2)
--			      centroid2[1] = centroid[1] + xx -- xcoord
--			      centroid2[2] = centroid[2] + yy -- ycoord
					
--					train_pos_centroids[ctPos]=centroid2
--			      train_pos_files[ctPos]=train_img_dir .. "/" .. file
--			      train_pos_labels[ctPos]=1
--			      train_pos_dupid[ctPos]=1
--			      ctPos=ctPos+1
--					if opt.dupflip == 1 then
--			         train_pos_centroids[ctPos]=centroid2
--			         train_pos_files[ctPos]=train_img_dir .. "/" .. file
--			         train_pos_labels[ctPos]=1
--			         train_pos_dupid[ctPos]=2
--			         ctPos=ctPos+1

--			         train_pos_centroids[ctPos]=centroid2
--			         train_pos_files[ctPos]=train_img_dir .. "/" .. file
--			         train_pos_labels[ctPos]=1
--			         train_pos_dupid[ctPos]=3
--			         ctPos=ctPos+1

--			         train_pos_centroids[ctPos]=centroid2
--			         train_pos_files[ctPos]=train_img_dir .. "/" .. file
--			         train_pos_labels[ctPos]=1
--			         train_pos_dupid[ctPos]=4
--			         ctPos=ctPos+1
--			      end
--			      if opt.duprot == 1 then
--			         train_pos_centroids[ctPos]=centroid2
--			         train_pos_files[ctPos]=train_img_dir .. "/" .. file
--			         train_pos_labels[ctPos]=1
--			         train_pos_dupid[ctPos]=5
--			         ctPos=ctPos+1

--			         train_pos_centroids[ctPos]=centroid2
--			         train_pos_files[ctPos]=train_img_dir .. "/" .. file
--			         train_pos_labels[ctPos]=1
--			         train_pos_dupid[ctPos]=6
--			         ctPos=ctPos+1

--			         train_pos_centroids[ctPos]=centroid2
--			         train_pos_files[ctPos]=train_img_dir .. "/" .. file
--			         train_pos_labels[ctPos]=1
--			         train_pos_dupid[ctPos]=7
--			         ctPos=ctPos+1
--
--			         train_pos_centroids[ctPos]=centroid2
--			         train_pos_files[ctPos]=train_img_dir .. "/" .. file
--			         train_pos_labels[ctPos]=1
--			         train_pos_dupid[ctPos]=8
--			         ctPos=ctPos+1
--			      end
--				end
--			end
--		end
--	  end

--	  for coordC in io.lines(file_cenC) do	-- look for negatives
--		xtmp = string.gsub(coordC:sub(1,(coordC:find('\t'))),"\t","")
--		ytmp = string.gsub(coordC:sub((coordC:find('\t'))),"\t","")
--		if gtcoord =="XY" then
--			xC=tonumber(xtmp)
--			yC=tonumber(ytmp)
--		else
--			yC=tonumber(xtmp)
--			xC=tonumber(ytmp)
--		end
--		bIsPos = false
--		for coordT in io.lines(file_cenT) do
--			xtmp = string.gsub(coordT:sub(1,(coordT:find('\t'))),"\t","")
--			ytmp = string.gsub(coordT:sub((coordT:find('\t'))),"\t","")
--			if gtcoord =="XY" then
--				xT=tonumber(xtmp)
--				yT=tonumber(ytmp)
--			else
--				yT=tonumber(xtmp)
--				xT=tonumber(ytmp)
--			end
--			dist = math.sqrt((xT-xC)*(xT-xC) + (yT-yC)*(yT-yC))
--			--print(dist)
--			if dist <= pixDist then
--				bIsPos = true
--				break
--			end
--		end
--		if not bIsPos then
--			centroid = torch.IntTensor(2)
--			centroid[1]=xC
--			centroid[2]=yC
--			train_neg_centroids[ctNeg]=centroid
--			train_neg_files[ctNeg]=train_img_dir .. "/" .. file
--			train_neg_labels[ctNeg]=-1
--			train_neg_dupid[ctNeg]=1
--			ctNeg=ctNeg+1	

--			if opt.dupflip == 1 then
--         	train_neg_centroids[ctNeg]=centroid
--            train_neg_files[ctNeg]=train_img_dir .. "/" .. file
--            train_neg_labels[ctNeg]=-1
--            train_neg_dupid[ctNeg]=2
--            ctNeg=ctNeg+1

--            train_neg_centroids[ctNeg]=centroid
--            train_neg_files[ctNeg]=train_img_dir .. "/" .. file
--            train_neg_labels[ctNeg]=-1
--            train_neg_dupid[ctNeg]=3
--            ctNeg=ctNeg+1

--            train_neg_centroids[ctNeg]=centroid
--            train_neg_files[ctNeg]=train_img_dir .. "/" .. file
--            train_neg_labels[ctNeg]=-1
--            train_neg_dupid[ctNeg]=4
--            ctNeg=ctNeg+1
--         end
--         if opt.duprot == 1 then
--         	train_neg_centroids[ctNeg]=centroid
--            train_neg_files[ctNeg]=train_img_dir .. "/" .. file
--            train_neg_labels[ctNeg]=-1
--            train_neg_dupid[ctNeg]=5
--            ctNeg=ctNeg+1

--            train_neg_centroids[ctNeg]=centroid
--            train_neg_files[ctNeg]=train_img_dir .. "/" .. file
--            train_neg_labels[ctNeg]=-1
--            train_neg_dupid[ctNeg]=6
--            ctNeg=ctNeg+1

--            train_neg_centroids[ctNeg]=centroid
--            train_neg_files[ctNeg]=train_img_dir .. "/" .. file
--            train_neg_labels[ctNeg]=-1
--            train_neg_dupid[ctNeg]=7
--            ctNeg=ctNeg+1

--            train_neg_centroids[ctNeg]=centroid
--            train_neg_files[ctNeg]=train_img_dir .. "/" .. file
--            train_neg_labels[ctNeg]=-1
--            train_neg_dupid[ctNeg]=8
--				ctNeg=ctNeg+1
--         end			
--			if opt.dupdist == 1 then
--	         local angleincrem = (2*math.pi)/nangles
--	         for ci = 1, ncircles do
--	            local rad=pixDist/ci
--	            for ag = 1, nangles do
--	               local theta=(ag-1)*angleincrem
--	               local xx=rad*math.cos(theta)
--	               local yy=rad*math.sin(theta)
--
--	               local centroid2 = torch.IntTensor(2)
--	               centroid2[1] = centroid[1] + xx -- xcoord
--	               centroid2[2] = centroid[2] + yy -- ycoord
--
--	               train_neg_centroids[ctNeg]=centroid2
--	               train_neg_files[ctNeg]=train_img_dir .. "/" .. file
--	               train_neg_labels[ctNeg]=-1
--	               ctNeg=ctNeg+1               
--	            end
--	         end
--		   end
--		end		
--	  end
--   end
--end

--rint(train_neg_dupid)

-- ground truth samples in testing
test_gt_files={}
test_gt_centroids={}
ctGT=1
for file in paths.files(test_img_dir) do
	if file:find(file_ext .. '$') then
   	file_cenT = test_img_dir .. "/" .. string.gsub(file, ".png", "_cenT.txt") -- ground truth centroids
		for coord in io.lines(file_cenT) do   -- look for positves
      	xtmp = string.gsub(coord:sub(1,(coord:find('\t'))),"\t","")
	      ytmp = string.gsub(coord:sub((coord:find('\t'))),"\t","")
      	if gtcoord =="XY" then
      		x=tonumber(xtmp)
      		y=tonumber(ytmp)
      	else
      		y=tonumber(xtmp)
      		x=tonumber(ytmp)
      	end
      	centroid = torch.IntTensor(2)
      	centroid[1]=x
      	centroid[2]=y
      	test_gt_centroids[ctGT]=centroid
      	test_gt_files[ctGT]=test_img_dir .. "/" .. file
      	ctGT=ctGT+1
     end
	end
end

-- test samples
test_pos_files = {}
test_pos_labels = {}
test_pos_centroids ={}
test_neg_files = {}
test_neg_labels = {}
test_neg_centroids ={}
ctPos=1
ctNeg=1
for file in paths.files(test_img_dir) do
   if file:find(file_ext .. '$') then
	  file_cenT = test_img_dir .. "/" .. string.gsub(file, ".png", "_cenT.txt") -- ground truth centroids
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
		bIsPos = false
		for coordT in io.lines(file_cenT) do
			xtmp = string.gsub(coordT:sub(1,(coordT:find('\t'))),"\t","")
			ytmp = string.gsub(coordT:sub((coordT:find('\t'))),"\t","")
			if gtcoord =="XY" then
				xT=tonumber(xtmp)
				yT=tonumber(ytmp)
			else
				yT=tonumber(xtmp)
				xT=tonumber(ytmp)
			end
			dist = math.sqrt((xT-xC)*(xT-xC) + (yT-yC)*(yT-yC))
			--print(dist)
			if dist <= pixDist then
				bIsPos = true
				break
			end
		end
		if not bIsPos then
			centroid = torch.IntTensor(2)
			centroid[1]=xC
			centroid[2]=yC
			test_neg_centroids[ctNeg]=centroid
			test_neg_files[ctNeg]=test_img_dir .. "/" .. file
			test_neg_labels[ctNeg]=-1
			ctNeg=ctNeg+1	
		else
			centroid = torch.IntTensor(2)
         centroid[1]=xC
         centroid[2]=yC
         test_pos_centroids[ctPos]=centroid
         test_pos_files[ctPos]=test_img_dir .. "/" .. file
         test_pos_labels[ctPos]=1
         ctPos=ctPos+1
		end		
	  end
   end
end


--trsize_pos = #train_pos_files
--trsize_neg = #train_neg_files
tesize_pos = #test_pos_files
tesize_neg = #test_neg_files

----------------------------------------------------------------------

-- select a stratified subset percentage of data 

samples_perc = opt.dsperc
--trsize_pos = math.floor(trsize_pos*samples_perc/100)
--trsize_neg = math.floor(trsize_neg*samples_perc/100)
tesize_pos = math.floor(tesize_pos*samples_perc/100)
tesize_neg = math.floor(tesize_neg*samples_perc/100)
--trsize = trsize_pos + trsize_neg
tesize = tesize_pos + tesize_neg

--train_files = {}
--train_labels = {}
--train_centroids = {}
--train_dupid={}
--shuffle = torch.randperm(#train_pos_files)
--shuffle = torch.range(1,#train_pos_files)
--for i=1, trsize_pos do
--    train_files[i]=train_pos_files[shuffle[i]]
--    train_labels[i]=train_pos_labels[shuffle[i]]
--	train_centroids[i]=train_pos_centroids[shuffle[i]]
--	train_dupid[i]=train_pos_dupid[shuffle[i]]
--end
--ii=0
--shuffle = torch.randperm(#train_neg_files)
--shuffle = torch.range(1,#train_neg_files)
--for i=trsize_pos, trsize_pos+trsize_neg-1 do	
--    ii=ii+1
--    train_files[i+1]=train_neg_files[shuffle[ii]]
--    train_labels[i+1]=train_neg_labels[shuffle[ii]]
--	 train_centroids[i+1]=train_neg_centroids[shuffle[ii]]
--	 train_dupid[i+1]=train_neg_dupid[shuffle[ii]]
--end

test_files = {}
test_labels = {}
test_centroids = {}
--shuffle = torch.randperm(#test_pos_files)
shuffle = torch.range(1,#test_pos_files)
for i=1, tesize_pos do
    test_files[i]=test_pos_files[shuffle[i]]
    test_labels[i]=test_pos_labels[shuffle[i]]
	test_centroids[i]=test_pos_centroids[shuffle[i]]	
end
ii=0
--shuffle = torch.randperm(#test_neg_files)
shuffle = torch.range(1,#test_neg_files)
for i=tesize_pos, tesize_pos+tesize_neg-1 do
    ii=ii+1
    test_files[i+1]=test_neg_files[shuffle[ii]]
    test_labels[i+1]=test_neg_labels[shuffle[ii]]
	test_centroids[i+1]=test_neg_centroids[shuffle[ii]]
end

----------------------------------------------------------------------
print '==> loading dataset'

--print("<dataset> loading train dataset # " .. trsize .. ' samples (' .. trsize_pos .. ',' .. trsize_neg .. ')')
--train_images = {}
--local hpf = nil
--local patch = nil
--file_ant = nil 
--for i,file in ipairs(train_files) do
--	-- load each image
--	if math.fmod(i,2500)==0 then
--		print("Loading... " .. i .. " / " .. #train_files)
--	end
--	if file_ant == nil or  file_ant ~= file then
--		hpf = image.load(file)
--	end
--	file_ant=file
--	hgt = hpf:size(2)
--	wdt = hpf:size(3)
--	x = train_centroids[i][1]
--	y = train_centroids[i][2]	
--	startx = x - math.floor(imgsize/2)
--	starty = y - math.floor(imgsize/2)
--	endx = startx + imgsize
--	endy = starty + imgsize
--	if startx < 1 then
--		startx=1
--		endx=startx+imgsize
--	end
--	if starty < 1 then
--		starty=1
--		endy=starty+imgsize
--	end
--	if endx > wdt then
--		endx=wdt
--		startx=endx-imgsize
--	end
--	if endy > hgt then
--		endy=hgt
--		starty=endy-imgsize
--	end
--	local patch1 = image.crop(hpf,startx,starty,endx,endy)

----	table.insert(train_images, patch)
	
--	if train_dupid[i]==1 then
--		table.insert(train_images, patch1)
--	elseif train_dupid[i]==2 then
--		local patch2=image.vflip(patch1)
--		table.insert(train_images, patch2)
--	elseif train_dupid[i]==3 then
--		local patch3=image.hflip(patch1)
--		table.insert(train_images, patch3)
--	elseif train_dupid[i]==4 then
--		local patch2=image.vflip(patch1)
--		local patch4=image.hflip(patch2)
--		table.insert(train_images, patch4)
--	elseif train_dupid[i]==5 then
--		local patch5=image.rotate(patch1, math.pi/2)
--		table.insert(train_images, patch5)
--	elseif train_dupid[i]==6 then
--		local patch5=image.rotate(patch1, math.pi/2)
--		local patch6=image.vflip(patch5)
--		table.insert(train_images, patch6)
--	elseif train_dupid[i]==7 then
--		local patch5=image.rotate(patch1, math.pi/2)
--		local patch7=image.hflip(patch5)
--		table.insert(train_images, patch7)
--	elseif train_dupid[i]==8 then
--		local patch5=image.rotate(patch1, math.pi/2)
--      local patch6=image.vflip(patch5)
--		local patch8=image.hflip(patch6)
--		table.insert(train_images, patch8)
--	end
--
--	patch = nil
--	--hpf = nil
--	collectgarbage()		
--end

--trainData = {
--   data = torch.Tensor(trsize,3,imgsize,imgsize),
--   labels = torch.Tensor(trsize),
--	centroids = torch.Tensor(trsize,2),
--	files = train_files,
--   size = function() return trsize end
--}

--for i=1,trsize do
--	trainData.data[{i}] = train_images[i]
--	trainData.labels[{i}] = train_labels[i]
--	trainData.centroids[{i}] = train_centroids[i]
--end

--train_images = nil
--collectgarbage()

print("<dataset> loading test dataset # " .. tesize .. ' samples (' .. tesize_pos .. ',' .. tesize_neg .. ')')
test_images = {}
local hpf = nil
patch = nil
file_ant = nil
for i,file in ipairs(test_files) do
   -- load each image
	if math.fmod(i,2500)==0 then
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
   labels = torch.Tensor(tesize),
   centroids = torch.Tensor(tesize,2),
   files = test_files,
   size = function() return tesize end
}

for i=1,tesize do
   testData.data[{i}] = test_images[i]
   testData.labels[{i}] = test_labels[i]
   testData.centroids[{i}] = test_centroids[i]
end

test_images = nil
collectgarbage()


if opt.visualize then
   print '==> visualizing data examples'
--   samples={}
--   shuffle = torch.randperm(trainData:size())
--   for i=1,100 do
--	samples[i] = trainData.data[shuffle[i]]
--   end
--   image.display{image=samples, nrow=10, legend='Some training examples'}
   
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
--for i = 1,trainData:size() do
--   trainData.data[i] = image.rgb2yuv(trainData.data[i])
--end
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
--mean = {}
--std = {}
--for i,channel in ipairs(channels) do
--   -- normalize each channel globally:
--   mean[i] = trainData.data[{ {},i,{},{} }]:mean()
--   std[i] = trainData.data[{ {},i,{},{} }]:std()
--   trainData.data[{ {},i,{},{} }]:add(-mean[i])
--   trainData.data[{ {},i,{},{} }]:div(std[i])
--end

-- Normalize test data, using the training means/stds
for i,channel in ipairs(channels) do
   -- normalize each channel globally:
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
--   for i = 1,trainData:size() do
--      trainData.data[{ i,{c},{},{} }] = normalization:forward(trainData.data[{ i,{c},{},{} }])
--   end
   for i = 1,testData:size() do
      testData.data[{ i,{c},{},{} }] = normalization:forward(testData.data[{ i,{c},{},{} }])
   end
end

----------------------------------------------------------------------
print '==> verify statistics'

-- It's always good practice to verify that data is properly
-- normalized.

for i,channel in ipairs(channels) do
--   trainMean = trainData.data[{ {},i }]:mean()
--   trainStd = trainData.data[{ {},i }]:std()

   testMean = testData.data[{ {},i }]:mean()
   testStd = testData.data[{ {},i }]:std()

--   print('training data, '..channel..'-channel, mean: ' .. trainMean)
--   print('training data, '..channel..'-channel, standard deviation: ' .. trainStd)

   print('test data, '..channel..'-channel, mean: ' .. testMean)
   print('test data, '..channel..'-channel, standard deviation: ' .. testStd)
end

