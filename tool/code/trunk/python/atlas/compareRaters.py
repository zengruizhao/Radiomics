# -*- coding: utf-8 -*-
"""
Spyder Editor

This is a temporary script file.
"""

import SimpleITK as sitk
import os
import path
import numpy as np
import sklearn.metrics as me

path_in = "C:\Mirabela\Projects\AtlasComparison\Data"
#prfx_old = "MirabelaRevisedSegmentations"
prfx_old = "OriginalData"
prfx_new = "AndreiSegmentations"
path_out = os.path.join('C:\Mirabela\Projects\AtlasComparison','Results','VolumeComparison.txt')

results = open(path_out,'w')
results.write('{},{},{},{},{},{},{},{}\n'.format('SubjectID', 'DiffVol', 'volOld', 
                          'volNew', 'DicePy', 'diceItk', 'haussdorffDist','Kappa'))

exp_path_in = os.path.join(path_in, prfx_new)

for d in os.listdir(exp_path_in):
    exp_d = os.path.join(exp_path_in, d)
    
    if os.path.isdir(exp_d):
        print ("Parsing", d)
        for f in os.listdir(exp_d):
            exp_old_path_in = os.path.join(path_in, prfx_old,d,f)
            exp_new_path_in = os.path.join(path_in, prfx_new,d,f)
            
            img_old = sitk.ReadImage(exp_old_path_in,sitk.sitkFloat32);
            img_new = sitk.ReadImage(exp_new_path_in,sitk.sitkFloat32);
            img_new.SetOrigin(img_old.GetOrigin())
            
            img_oa = sitk.GetArrayFromImage(img_old)
            img_na = sitk.GetArrayFromImage(img_new)

            nEl = img_oa.shape[0]*img_oa.shape[1]*img_oa.shape[2]
            kappa = me.cohen_kappa_score(np.reshape(img_oa,nEl), np.reshape(img_na,nEl))
            
            img_old_int = sitk.ReadImage(exp_old_path_in,sitk.sitkInt16);
            img_new_int = sitk.ReadImage(exp_new_path_in,sitk.sitkInt16);            
            img_new_int.SetOrigin(img_old_int.GetOrigin())
            img_new_int.SetDirection(img_old_int.GetDirection())
            
            overlapFilter = sitk.LabelOverlapMeasuresImageFilter()                        
            overlapFilter.Execute(img_old_int, img_new_int)
            diceItk = overlapFilter.GetDiceCoefficient()
            
            haussdorff = sitk.HausdorffDistanceImageFilter()
            haussdorff.Execute(img_old_int, img_new_int)
            haussdorffDist = haussdorff.GetHausdorffDistance()
            
            
            vol_oa = np.sum(img_oa)
            vol_na = np.sum(img_na)
            
            
                        
            dice = 2*np.sum(np.logical_and(img_oa,img_na))/(vol_oa+vol_na)
            
            sp = img_old.GetSpacing()
            vol_oa *= sp[0]*sp[1]*sp[2]
            vol_na *= sp[0]*sp[1]*sp[2]
            
            diff = np.sum(abs(img_oa - img_na))*sp[0]*sp[1]*sp[2]
            
            print(f, diff, vol_oa, vol_na, dice, diceItk, haussdorffDist, kappa)
            results.write('{},{},{},{},{},{},{},{}\n'.format(f, diff, vol_oa, 
                          vol_na, dice, diceItk, haussdorffDist,kappa))
            
            
results.close()