----------------------------------------------------------------------
-- This tutorial shows how to train different models on the street
-- view house number dataset (SVHN),
-- using multiple optimization techniques (SGD, ASGD, CG), and
-- multiple types of models.
--
-- This script demonstrates a classical example of training 
-- well-known models (convnet, MLP, logistic regression)
-- on a 10-class classification problem. 
--
-- It illustrates several points:
-- 1/ description of the model
-- 2/ choice of a loss function (criterion) to minimize
-- 3/ creation of a dataset as a simple Lua table
-- 4/ description of training and test procedures
--
-- Clement Farabet
----------------------------------------------------------------------

----------------------------------------------------------------------
print '==> processing options'

cmd = torch.CmdLine()
cmd:text()
cmd:text('MITOS Function')
cmd:text()
cmd:text('Options:')
-- visualize
cmd:option('-visualize', false, 'visualize input data and weights during training')
-- global:
cmd:option('-seed', 1, 'fixed input seed for repeatable experiments')
cmd:option('-threads', 8, 'number of threads')
cmd:option('-maxepochs',50,'max number of epochs of train and test loop')
cmd:option('-epochini',0,'epoch to start from previous model')
-- data:
cmd:option('-dataset', "miccai2013", 'dataset: icpr2012+A | icpr2012+H | miccai2013')
cmd:option('-imgsize', 81, 'how big is the input image size: e.g. 81 | 101 ')
cmd:option('-duprot', 1, 'duplicate training samples by rotation')
cmd:option('-dupflip', 1, 'duplicate training samples by flipping (mirroring)')
cmd:option('-dupdist', 1, 'duplicate training samples by surronding coordinates a given distance')
cmd:option('-dsperc',1,'percentage of dataset (stratified)')
--cmd:option('-size', 'small', 'how many samples do we load: small | full | extra')
-- model:
cmd:option('-model', 'convnet', 'type of model to construct: linear | mlp | convnet')
cmd:option('-convfun','spmm','type of spatial convolution : sp | spmm | spmap ')
cmd:option('-convnorm', 1, 'apply normalization over output of convolution and pooling layer')
cmd:option('-convnormk',13,'normalization kernel size for output of convolution and pooling layer')
cmd:option('-poolfun','L2','pool function [ max | L2 ]')
cmd:option('-arch','a','predefined architecture [ a | b ]')
cmd:option('-cnnf1',64,'number of features in convnet layer 1')
cmd:option('-cnnf2',128,'number of features in convnet layer 2')
cmd:option('-cnnf3',0,'number of features in convnet layer 3')
cmd:option('-cnnf4',0,'number of features in convnet layer 4')
cmd:option('-cnnf5',0,'number of features in convnet layer 5')
cmd:option('-fcf1',256,'number of features in full layer 1')
cmd:option('-fcf2',0,'number of features in full layer 2')
-- loss:
cmd:option('-loss', 'nll', 'type of loss function to minimize: nll | mse | margin')
-- training:
cmd:option('-save', 'debug', 'subdirectory to save/log experiments in')
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
cmd:option('-type', 'float', 'type: double | float | cuda')
cmd:text()
opt = cmd:parse(arg or {})

-- nb of threads and fixed seed (for repeatable experiments)
if opt.type == 'float' then
   print('==> switching to floats')
   torch.setdefaulttensortype('torch.FloatTensor')
elseif opt.type == 'cuda' then
   print('==> switching to CUDA')
   require 'cunn'
   torch.setdefaulttensortype('torch.FloatTensor')
end
torch.setnumthreads(opt.threads)
torch.manualSeed(opt.seed)

----------------------------------------------------------------------
print '==> executing all'

dofile '1_data_images.lua'
--dofile '1_data_dup_dist_images.lua'
dofile '2_model.lua'
dofile '3_loss.lua'
dofile '4_train.lua'
dofile '5_test.lua'

----------------------------------------------------------------------
print '==> training!'

if opt.epochini == 0 then
   epoch=1
else
   filename=paths.concat(opt.save, 'model_epoch' .. opt.epochini .. '.net')
   model=torch.load(filename)
   epoch=opt.epochini+1
end

while (epoch <= opt.maxepochs) do
   train()
   test()
end

