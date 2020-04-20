----------------------------------------------------------------------
-- This script implements a test procedure, to report accuracy
-- on the test data. Nothing fancy here...
--
-- Clement Farabet
----------------------------------------------------------------------

require 'torch'   -- torch
require 'xlua'    -- xlua provides useful tools, like progress bars
require 'optim'   -- an optimization package, for online and batch methods

----------------------------------------------------------------------
print '==> defining test procedure'


-- test function
function test()
   -- local vars
   local time = sys.clock()

   -- averaged param use?
   if average then
      cachedparams = parameters:clone()
      parameters:copy(average)
   end

   -- test over test data
   print('==> testing on test set:')
	test_predict = {}
	test_prob1 = {}
	test_prob2 = {}
	test_features = {}
	test_output = {}
	--mx=-9999
   --mn=9999
	th=opt.th
	npos_pred=0
   for t = 1,testData:size() do
      -- disp progress
      xlua.progress(t, testData:size())

      -- get new sample
      local input = testData.data[t]
      if opt.type == 'double' then input = input:double()
      elseif opt.type == 'cuda' then input = input:cuda() end
      local target = testData.labels[t]

      -- test sample
      local pred = model:forward(input)
      --confusion:add(pred, target)
		
		local prediction_1d = prediction_1d or torch.FloatTensor(2)
      prediction_1d:copy(pred)
      local predval,predtmp = prediction_1d:max(1)
		--print(prediction_1d)
		--print(predval)
		test_output[t]=prediction_1d
      test_prob1[t]=math.exp(test_output[t][1]) -- TODO: validate if is not nn.LogSoftMax
		test_prob2[t]=math.exp(test_output[t][2]) 
		
		if test_prob1[t]>=th then
         predtmp=1
         npos_pred=npos_pred+1
      else
         predtmp=2
      end
      test_predict[t]=(predtmp*(-2)+3)
      confusion:add(predtmp, testData.labels[t])
      --test_predict[t]=(predtmp[1]*(-2)+3)	
		--if test_prob[t]<mn then mn=test_prob[t] end
      --if test_prob[t]>mx then mx=test_prob[t] end
		
		-- get learned features
		local nmodules = #model.modules
		local learnedfeatures = learnedfeatures or torch.DoubleTensor(nstates2[#nstates2])
		learnedfeatures:copy(model.modules[nmodules-2].output)
		test_features[t] = learnedfeatures
   end

   --th=opt.th
	--npos_pred=0
   --for t = 1,testData:size() do
   --   test_prob[t]=(test_prob[t]-mn)/(mx-mn)
   --   if test_prob[t]>=th then
   --      predtmp=1
	--		npos_pred=npos_pred+1
   --   else
   --      predtmp=2
   --   end
   --   test_predict[t]=(predtmp*(-2)+3)
   --   confusion:add(predtmp, testData.labels[t])
   --end

   -- timing
   time = sys.clock() - time
   timefull = time
   time = time / testData:size()
	timesample = time
	times = {
      full = timefull,
      sample = timesample
   }
   print("\n==> time to test 1 sample = " .. (time*1000) .. 'ms')

	-- calculate performance of candidates against ground truth samples
	print("\n==> calculating performance of prediction vs ground truth " )
	D = torch.Tensor(#test_gt_files,#test_predict)
	D:zero()
	for i=1,#test_gt_files do
		xT=test_gt_centroids[i][1]
      yT=test_gt_centroids[i][2]
		for j=1,#test_predict do
			if test_predict[j]==1 then
				xC=testData.centroids[j][1]
 		      yC=testData.centroids[j][2]
				dist = math.sqrt((xT-xC)*(xT-xC) + (yT-yC)*(yT-yC))		
				if dist <= pixDist then
					D[i][j]=1
				end
			end
		end
	end
	fp_gt=0
		for j=1,#test_predict do
         if test_predict[j]==1 and testData.labels[j]~=1 then
				fp_gt=fp_gt+1
         end
      end

   groundTruthMeasures(D,fp_gt)
	

    -- print performance measures
   local tp = confusion.mat[1][1]
   local fn = confusion.mat[1][2]
   local fp = confusion.mat[2][1]
   local tn = confusion.mat[2][2]
   showmeasures(tp,fp,tn,fn)
   print("(test) F-measure: " .. measures.Fmeasure)



   -- print confusion matrix
   print(confusion)
	print(times)
   testLogger:add{['Fmeasure (test set)'] = measures.Fmeasure}
	
	testOutFile:write(epoch-1 .. "\t" .. times.sample .. "\t" .. times.full .. "\t".. measures.TP .. "\t" .. measures.FN .. "\t" ..measures.FP .. "\t" ..  measures.TN .. "\t" .. measures.Precision .. "\t" .. measures.Recall .. "\t" .. measures.Fmeasure ..  "\t" .. tp_gt .. "\t" .. fn_gt  .. "\t" .. fp_gt  .. "\t" .. Pr_gt .. "\t" .. Rc_gt .. "\t" .. F1_gt .. "\n")

   testOutFile:flush()
	
	 -- save learned features
	if trsize~=nil then
   	filenameout="learnedfeatures_test_" .. opt.dataset .."_CNN_per" .. opt.dsperc .. "_imtr" .. trsize .. "_imte" .. tesize .. "_epoch" .. (epoch-1)  .. ".txt"
	else
		filenameout="learnedfeatures_test_" .. opt.dataset .."_CNN_per" .. opt.dsperc .. "_imte" .. tesize .. "_epoch" .. (epoch)  .. ".txt"
	end

   featuresOutFile = io.open(paths.concat(opt.save,filenameout), "w")
   for f = 1,#test_features do
		strrow = testData.files[f] .. "\t" .. testData.centroids[f][1] .. "\t" .. testData.centroids[f][2]
		--print(strrow)
		for i=1,nstates2[#nstates2] do
			strrow = strrow .. "\t" .. test_features[f][i] 
		end
		--print(strrow)
      featuresOutFile:write(strrow .. "\n")
   end
   featuresOutFile:flush()
	
	-- save prediction
	if trsize~=nil then
   	filenameout="predictions_test_" .. opt.dataset .."_CNN_per" .. opt.dsperc .. "_imtr" .. trsize .. "_imte" .. tesize .. "_epoch" .. (epoch-1)  .. ".txt"
	else
		filenameout="predictions_test_" .. opt.dataset .."_CNN_per" .. opt.dsperc .. "_imte" .. tesize .. "_epoch" .. (epoch)  .. ".txt"
	end

   predictOutFile = io.open(paths.concat(opt.save,filenameout), "w")
   for f = 1,#test_labels do
	--print(test_prob[f])
      predictOutFile:write(testData.files[f] .. "\t" .. testData.centroids[f][1] .. "\t" .. testData.centroids[f][2] .. "\t" .. testData.labels[f] .. "\t" .. string.format('%d',test_predict[f]) ..  "\t" .. test_output[f][1] .. "\t" .. test_output[f][2] .."\t" .. test_prob1[f] .. "\t" .. test_prob2[f] .. "\n")
   end
   predictOutFile:flush()


   if opt.plot then
      testLogger:style{['Fmeasure (test set)'] = '-'}
      testLogger:plot()
   end

   -- averaged param use?
   if average then
      -- restore parameters
      parameters:copy(cachedparams)
   end
   
   -- next iteration:
   confusion:zero()
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


function groundTruthMeasures(D,fp_gt)
SumC=D:sum(1)
SumT=D:sum(2)

--fps=torch.Tensor(SumC:size(2)):zero()
--for id=1,SumC:size(2) do
--if SumC[1][id]==0 then
--fps[id]=1
--end
--end

fns=torch.Tensor(SumT:size(1)):zero()
for id=1,SumT:size(1) do
if SumT[id][1]==0 then
fns[id]=1
end
end

tps=torch.Tensor(SumT:size(1)):zero()
for id=1,SumT:size(1) do
if SumT[id][1]>0 then
tps[id]=1
end
end

tp_gt=tps:sum()
fn_gt=fns:sum()
--fp_gt=fps:sum()

	if tp_gt == 0 and fp_gt ==0 then
          Pr_gt = 0
   else
          Pr_gt = tp_gt / (tp_gt + fp_gt)
   end
   if tp_gt == 0 and fn_gt ==0 then
      Rc_gt = 0
   else
      Rc_gt = tp_gt / (tp_gt + fn_gt)
   end
   if Pr_gt == 0 and Rc_gt ==0 then
      F1_gt = 0
   else
      F1_gt = (2*Pr_gt*Rc_gt)/(Pr_gt+Rc_gt)
   end

end

