# -*- coding: utf-8 -*-
"""
Spyder Editor

This is a temporary script file.
"""

import SimpleITK as sitk
import os
import path
import numpy as np

r1_path_in = "C:\Mirabela\Projects\AtlasComparison\Results" #Andrei
r2_path_in = "C:\Mirabela\Projects\AtlasComparison\Results\old" #Mirabela

prfx_l = ["Disease","Normal"]
atls = "UH_UC_SV"
regn_l = ["CG","Prostate"]

regn_idx = 0
prfx_idx = 0

path_out = 'C:\Mirabela\Projects\AtlasComparison\Results\VolumeComparisonAtlases.txt'

results = open(path_out,'w')
results.write('{},{},{},{},{},{},{}\n'.format('SubjectID', 'DiffVol', 'volOld', 
                          'volNew', 'DicePy', 'diceItk', 'haussdorffDist'))
                              
 
for prfx_idx, prfx in enumerate(prfx_l):
    for regn_idx, regn in enumerate(regn_l):
                             
        fn = os.path.join(prfx, "3_ffd",prfx+'_'+atls+'_atlas_label_'+regn+".mha")
        
        r1_exp_path_in = os.path.join(r1_path_in, fn)
        r2_exp_path_in = os.path.join(r2_path_in, fn)
        
        
        r1_img = sitk.ReadImage(r1_exp_path_in);
        r2_img = sitk.ReadImage(r2_exp_path_in);

        r1_grad = sitk.GradientMagnitude(r1_img)
        r2_grad = sitk.GradientMagnitude(r2_img)
        
        r1_grad_arr = sitk.GetArrayFromImage(r1_grad)
        r2_grad_arr = sitk.GetArrayFromImage(r2_grad)
        
        #print "Edge Voxels", r1_exp_path_in, np.sum(r1_grad_arr>0)
        #print "Edge Voxels", r2_exp_path_in, np.sum(r2_grad_arr>0)
        print "Edge Voxels", fn, max(np.sum(r1_grad_arr>0),np.sum(r2_grad_arr>0))
 
        fn_out = os.path.join(prfx, "3_ffd",prfx+'_'+atls+'_atlas_label_'+regn+"_grad.mha")
        
        sitk.WriteImage(r1_grad,os.path.join(r1_path_in, fn_out))
        sitk.WriteImage(r2_grad,os.path.join(r2_path_in, fn_out))
        
        overlapFilter = sitk.LabelOverlapMeasuresImageFilter()                        
        overlapFilter.Execute(r1_img, r2_img)
        diceItk = overlapFilter.GetDiceCoefficient()
        
        haussdorff = sitk.HausdorffDistanceImageFilter()
        haussdorff.Execute(r1_img, r2_img)
        haussdorffDist = haussdorff.GetHausdorffDistance()
        
        r1_img_a = sitk.GetArrayFromImage(r1_img)
        r2_img_a = sitk.GetArrayFromImage(r2_img)
        
        vol_oa = np.sum(r1_img_a)
        vol_na = np.sum(r2_img_a)
        
        r1_sp = r1_img.GetSpacing()
        vol_oa *= r1_sp[0]*r1_sp[1]*r1_sp[2]
        vol_na *= r1_sp[0]*r1_sp[1]*r1_sp[2]
        
        diff = np.sum(abs(r1_img_a.astype('float') - r2_img_a.astype('float')))*r1_sp[0]*r1_sp[1]*r1_sp[2]
        
        print(prfx, regn, diff, vol_oa, vol_na, diceItk, haussdorffDist)
        results.write('{},{},{},{},{},{},{}\n'.format(prfx, regn, diff, vol_oa, 
                      vol_na, diceItk, haussdorffDist))

results.close()