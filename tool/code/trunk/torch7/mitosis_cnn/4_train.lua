----------------------------------------------------------------------
-- This script demonstrates how to define a training procedure,
-- irrespective of the model/loss functions chosen.
--
-- It shows how to:
--   + construct mini-batches on the fly
--   + define a closure to estimate (a noisy) loss
--     function, as well as its derivatives wrt the parameters of the
--     model to be trained
--   + optimize the function, according to several optmization
--     methods: SGD, L-BFGS.
--
-- Clement Farabet
----------------------------------------------------------------------

require 'torch'   -- torch
require 'xlua'    -- xlua provides useful tools, like progress bars
require 'optim'   -- an optimization package, for online and batch methods

----------------------------------------------------------------------
-- parse command line arguments
if not opt then
   print '==> processing options'
   cmd = torch.CmdLine()
   cmd:text()
   cmd:text('MITOS Training/Optimization')
   cmd:text()
   cmd:text('Options:')
   cmd:option('-save', 'results', 'subdirectory to save/log experiments in')
   cmd:option('-visualize', false, 'visualize input data and weights during training')
   cmd:option('-plot', false, 'live plot')
   cmd:option('-optimization', 'SGD', 'optimization method: SGD | ASGD | CG | LBFGS')
   cmd:option('-learningRate', 1e-3, 'learning rate at t=0')
	cmd:option('-learningRateDecay',1e-7,'learning rate decay')
   cmd:option('-batchSize', 1, 'mini-batch size (1 = pure stochastic)')
   cmd:option('-weightDecay', 0, 'weight decay (SGD only)')
   cmd:option('-momentum', 0, 'momentum (SGD only)')
   cmd:option('-t0', 1, 'start averaging at t0 (ASGD only), in nb of epochs')
   cmd:option('-maxIter', 2, 'maximum nb of iterations for CG and LBFGS')
	cmd:option('-th',0.5,'threshould for classification between 0 and 1 (default 0.5)')
   cmd:text()
   opt = cmd:parse(arg or {})
end

----------------------------------------------------------------------
-- CUDA?
if opt.type == 'cuda' then
   --mlp =  model
   --mlp:cuda()
   --model = nn.Sequential()
   --model:add(nn.Copy('torch.FloatTensor','torch.CudaTensor'))
  --model:add(mlp)
  --model:add(nn.Copy('torch.CudaTensor','torch.FloatTensor'))  

   model:cuda()
   criterion:cuda()
end

----------------------------------------------------------------------
print '==> defining some tools'

-- classes
--classes = {'1','2','3','4','5','6','7','8','9','0'}
classes ={'mitosis','no-mitosis'}

-- This matrix records the current confusion across classes
confusion = optim.ConfusionMatrix(classes)

-- Log results to files
trainLogger = optim.Logger(paths.concat(opt.save, 'f1_train.log'))
testLogger = optim.Logger(paths.concat(opt.save, 'f1_test.log'))

-- full record
trainOutFile = io.open(paths.concat(opt.save,"train_measures.txt"), "a")
testOutFile = io.open(paths.concat(opt.save,"test_measures.txt"), "a")

trainOutFile:write("--------------------" .. os.date("%Y_%m_%d_%X") .. "--------------------\n")
testOutFile:write("--------------------" .. os.date("%Y_%m_%d_%X") .. "--------------------\n")


trainOutFile:write("epoch\t" ..  "Tsample\t" .. "Tfull\t".. "TP\t" .. "FN\t" .. "FP\t" ..  "TN\t" .. "Precision\t" .. "Recall\t" .. "Fmeasure\n")

testOutFile:write("epoch\t" ..  "Tsample\t" .. "Tfull\t".. "TP\t" .. "FN\t" .. "FP\t" ..  "TN\t" .. "Precision\t" .. "Recall\t" .. "Fmeasure\n")

--trainTPLogger = optim.Logger(paths.concat(opt.save, 'train_TP.log'))
--testTPLogger = optim.Logger(paths.concat(opt.save, 'test_fmeasure.log'))

--trainF1Logger = optim.Logger(paths.concat(opt.save, 'train_fmeasure.log'))
--testF1Logger = optim.Logger(paths.concat(opt.save, 'test_fmeasure.log'))


-- Retrieve parameters and gradients:
-- this extracts and flattens all the trainable parameters of the mode
-- into a 1-dim vector
if model then
   parameters,gradParameters = model:getParameters()
end


--print(model)

----------------------------------------------------------------------
print '==> configuring optimizer'

if opt.optimization == 'CG' then
   optimState = {
      maxIter = opt.maxIter
   }
   optimMethod = optim.cg

elseif opt.optimization == 'LBFGS' then
   optimState = {
      learningRate = opt.learningRate,
      maxIter = opt.maxIter,
      nCorrection = 10
   }
   optimMethod = optim.lbfgs

elseif opt.optimization == 'SGD' then
   optimState = {
      learningRate = opt.learningRate,
      weightDecay = opt.weightDecay,
      momentum = opt.momentum,
      learningRateDecay = opt.learningRateDecay --1e-7
   }
   optimMethod = optim.sgd

elseif opt.optimization == 'ASGD' then
   optimState = {
      eta0 = opt.learningRate,
      t0 = trsize * opt.t0
   }
   optimMethod = optim.asgd

else
   error('unknown optimization method')
end

----------------------------------------------------------------------
print '==> defining training procedure'

function train()

   -- epoch tracker
   epoch = epoch or 1

   -- local vars
   local time = sys.clock()
   --print(trsize)
   --print(trainData:size())
   -- shuffle at each epoch
    
   shuffle = torch.randperm(trainData:size())

   -- do one epoch
   print('==> doing epoch on training data:')
   print("==> online epoch # " .. epoch .. ' [batchSize = ' .. opt.batchSize .. ']')

   for t = 1,trainData:size(),opt.batchSize do
      -- disp progress
      xlua.progress(t, trainData:size())

      -- create mini batch
      local inputs = {}
      local targets = {}
      for i = t,math.min(t+opt.batchSize-1,trainData:size()) do
         -- load new sample
         local input = trainData.data[shuffle[i]]
         local target = trainData.labels[shuffle[i]]
         if opt.type == 'double' then input = input:double()
         elseif opt.type == 'cuda' then input = input:cuda() end
         --if opt.type == 'double' then target = target:double()
         --elseif opt.type == 'cuda' then target = target:cuda()
         --else target = target:float() end
         table.insert(inputs, input)
         table.insert(targets, target)
      end
      -- create closure to evaluate f(X) and df/dX
      local feval = function(x)
      	-- get new parameters
	      if x ~= parameters then
   	   	parameters:copy(x)
      	end

	      -- reset gradients
      	gradParameters:zero()

      	-- f is the average of all criterions
      	local f = 0
      	-- evaluate function for complete mini batch
      	for i = 1,#inputs do
      		-- estimate f
         	local output = model:forward(inputs[i])
				--print(output)
				local err = nil
				if opt.type == 'cuda' then
         		err = criterion:forward(output, targets[i])
				else
			    	err = criterion:forward(output, targets[i])
				end
         	f = f + err

         	-- estimate df/dW
				local df_do = nil
				if opt.type == 'cuda' then
         		df_do = criterion:backward(output, targets[i])
				else
			    	df_do = criterion:backward(output, targets[i])
				end
         	model:backward(inputs[i], df_do)

 				-- update confusion
         	--confusion:add(output, targets[i])
      	end

      	-- normalize gradients and f(X)
      	gradParameters:div(#inputs)
      	f = f/#inputs

      	-- return f and df/dX
      	return f,gradParameters
   	end

    	-- optimize on current mini-batch
   	if optimMethod == optim.asgd then
   		_,_,average = optimMethod(feval, parameters, optimState)
   	else
   		optimMethod(feval, parameters, optimState)
   	end	
	end

	train_predict = {}
	train_prob1 = {}
	train_prob2 = {}
	train_features = {}
	train_output = {}
	--mx=-9999
	--mn=9999
	th=opt.th
	for t = 1,trainData:size() do
		-- disp progress
      xlua.progress(t, trainData:size())
 		-- get new sample
      local input = trainData.data[t]
      if opt.type == 'double' then input = input:double()
      elseif opt.type == 'cuda' then input = input:cuda() end
      local target = trainData.labels[t]

      -- test sample
      local pred = model:forward(input)
      --confusion:add(pred, target)

		local prediction_1d = prediction_1d or torch.FloatTensor(2)
      prediction_1d:copy(pred)
      local predval,predtmp = prediction_1d:max(1)
      --print(prediction_1d)
      --print(predval)
      --print(t)
		train_output[t]=prediction_1d
      train_prob1[t]=math.exp(train_output[t][1]) --/(train_output[t][1] + train_output[t][2])
		train_prob2[t]=math.exp(train_output[t][2]) --/(train_output[t][1] + train_output[t][2])
		
	   if train_prob1[t]>=th then
         predtmp=1
      else
         predtmp=2
      end
      train_predict[t]=(predtmp*(-2)+3)
      confusion:add(predtmp, trainData.labels[t])

		
      --train_predict[t]=(predtmp[1]*(-2)+3)
		--if train_prob[t]<mn then mn=train_prob[t] end
		--if train_prob[t]>mx then mx=train_prob[t] end

      -- get learned features
      local nmodules = #model.modules
      local learnedfeatures = learnedfeatures or torch.DoubleTensor(nstates2[#nstates2])
      learnedfeatures:copy(model.modules[nmodules-2].output)
      train_features[t] = learnedfeatures
	end	
	--th=opt.th
	--for t = 1,trainData:size() do
	--	train_prob[t]=(train_prob[t]-mn)/(mx-mn)
	--	if train_prob[t]>=th then
	--		predtmp=1
--		else
--			predtmp=2
--		end
--		train_predict[t]=(predtmp*(-2)+3)
--		confusion:add(predtmp, trainData.labels[t])
--	end	

   -- time taken
   time = sys.clock() - time
	timefull = time
   time = time / trainData:size()
	timesample = time
	times = {
		full = timefull,
		sample = timesample
	}
   print("\n==> time to learn 1 sample = " .. (time*1000) .. 'ms')

   local tp = confusion.mat[1][1]
   local fn = confusion.mat[1][2]
   local fp = confusion.mat[2][1]
   local tn = confusion.mat[2][2]
   showmeasures(tp,fp,tn,fn)
   print("(train) F-measure: " .. measures.Fmeasure)
   --trainF1Logger:add{['% F-measure (train set)'] = measures.Fmeasure * 100}

   -- print confusion matrix
   print(confusion)

	print(times)
	trainLogger:add{['Fmeasure (train set)'] = measures.Fmeasure}
	
	trainOutFile:write(epoch .. "\t" .. times.sample .. "\t" .. times.full .. "\t".. measures.TP .. "\t" .. measures.FN .. "\t" ..measures.FP .. "\t" ..  measures.TN .. "\t" .. measures.Precision .. "\t" .. measures.Recall .. "\t" .. measures.Fmeasure  .. "\n")

	trainOutFile:flush()


	 -- save learned features
   filenameout="learnedfeatures_train_" .. opt.dataset .."_CNN_per" .. opt.dsperc .. "_imtr" .. trsize .. "_imte" .. tesize .. "_epoch" .. (epoch)  .. ".txt"
   featuresOutFile = io.open(paths.concat(opt.save,filenameout), "w")
   for f = 1,#train_features do
      strrow = trainData.files[f] .. "\t" .. trainData.centroids[f][1] .. "\t" .. trainData.centroids[f][2]
      --print(strrow)
      for i=1,nstates2[#nstates2] do
         strrow = strrow .. "\t" .. train_features[f][i]
      end
      --print(strrow)
      featuresOutFile:write(strrow .. "\n")
   end
   featuresOutFile:flush()

	-- save prediction
   filenameout="predictions_train_" .. opt.dataset .."_CNN_per" .. opt.dsperc .. "_imtr" .. trsize .. "_imte" .. tesize .. "_epoch" .. (epoch)  .. ".txt"
   predictOutFile = io.open(paths.concat(opt.save,filenameout), "w")
   for f = 1,#train_labels do
   --print(test_prob[f])
	--print(train_predict[f])
      predictOutFile:write(trainData.files[f] .. "\t" .. trainData.centroids[f][1] .. "\t" .. trainData.centroids[f][2] .. "\t" .. trainData.labels[f] .. "\t" .. string.format('%d',train_predict[f]) .. "\t" .. train_output[f][1] .. "\t" .. train_output[f][2] .. "\t" .. train_prob1[f] .. "\t" .. train_prob2[f].. "\n")
   end
   predictOutFile:flush()



   if opt.plot then
      trainLogger:style{['Fmeasure (train set)'] = '-'}
      trainLogger:plot()
   end

   -- save/log current net
   local filename = paths.concat(opt.save, 'model_epoch' .. epoch .. '.net')
   os.execute('mkdir -p ' .. sys.dirname(filename))
   print('==> saving model to '..filename)
   torch.save(filename, model)

   -- next epoch
   confusion:zero()
   epoch = epoch + 1
end


function showmeasures(tp,fp,tn,fn)
   if tp == 0 and fp ==0 then
          Pr = 0
   else
          Pr = tp / (tp + fp)
   end
   if tp == 0 and fn ==0 then
      Rc = 0
   else
      Rc = tp / (tp + fn)
   end
   if Pr == 0 and Rc ==0 then
      F1 = 0
   else
      F1 = (2*Pr*Rc)/(Pr+Rc)
   end
   measures = {
      TP = tp,
      FP = fp,
      FN = fn,
      TN = tn,
      Precision = Pr,
      Recall = Rc,
      Fmeasure = F1
   }
   -- print('TP:' .. tp .. ' fn:' .. fn .. ' fp:' .. fp .. ' tn:' .. tn .. ' Pr:' .. Pr .. ' Rc:' .. Rc .. ' F1:' .. F1)
   print(measures)
end


