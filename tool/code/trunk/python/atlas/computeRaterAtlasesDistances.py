import SimpleITK as sitk
import os
import path
import numpy as np
import matplotlib.pyplot as plt
import scipy.stats as ss

path_in = "C:\Mirabela\Projects\AtlasComparison\Results\Comparisons\\33" 

prfx_l = ["","_old","_D","_N"]
atls_l = ["Dis3_vs_Nor3","Dis3_vs_Nor3","Dis3_vs_Dis3","Nor3_vs_Nor3"]
regn_l = ["CG","Prostate"]


path_out = 'C:\Mirabela\Projects\AtlasComparison\Results\DistanceComparisonAtlases.txt'
png_path_out = 'C:\Mirabela\Projects\AtlasComparison\Results\DistanceComparisonRegionAtlases.png'
eps_path_out = 'C:\Mirabela\Projects\AtlasComparison\Results\DistanceComparisonRegionAtlases.eps'

results = open(path_out,'w')
results.write('{},{},{},{},{},{},{}\n'.format('SubjectID', 'DiffVol', 'volOld', 
                          'volNew', 'DicePy', 'diceItk', 'haussdorffDist'))
                              

plt.rc('font', weight='bold')
plt.rc('lines', lw=3, color='g')
plt.rc('xtick.major', size=5)
plt.rc('ytick.major', size=5)
plt.rc('xtick', labelsize=12)
plt.rc('ytick', labelsize=17)
boxprops = dict(linestyle='-', linewidth=3, color='blue')

medianprops = dict(linestyle='-.', linewidth=2.5, color='firebrick')
meanlineprops = dict(linestyle='-', linewidth=2.5, color='firebrick')
medianprops = dict(linestyle='None', linewidth=0.5, color='white')
whiskerprops = dict(linestyle='--', linewidth=2.5, color='blue')
capprops = dict(linestyle='-', linewidth=2.5, color='blue')

fig, axes = plt.subplots(nrows=4, ncols=2, figsize=(15, 30))
fs = 25
axes[0,0].set_title(regn_l[0], fontsize=fs)
axes[0,1].set_title(regn_l[1], fontsize=fs)

ylimMax = 5

dist = [[],[]]
dist_apx = [[],[]]
dist_md  = [[],[]]
dist_bs  = [[],[]]
labels = ['R1: C- vs C+','R2: C- vs C+', 'C+: R1 vs R2', 'C-: R1 vs R2']

 
for idx, prfx in enumerate(prfx_l):
    atls = atls_l[idx]
    for regn_idx, regn in enumerate(regn_l):
        
        
        fn1 = atls+'_atlas_1'+regn+".mha"
        fn2 = atls+'_atlas_2'+regn+".mha"
        
        r1_exp_path_in = os.path.join(path_in+prfx, fn1)
        r2_exp_path_in = os.path.join(path_in+prfx, fn2)
        
        r1_img = sitk.ReadImage(r1_exp_path_in);
        r2_img = sitk.ReadImage(r2_exp_path_in);
    
        r1_img_a = sitk.GetArrayFromImage(r1_img)
        r2_img_a = sitk.GetArrayFromImage(r2_img)
        
        idx2 = np.where(r2_img_a==0)
        minIdx  = np.min(idx2[0])
        deltaIdx = (np.max(idx2[0])-minIdx)/3
        
        idxApx          = np.where(idx2[0] < (minIdx+deltaIdx) )
        idxMidgland     = np.where(abs(idx2[0]-minIdx-deltaIdx*3/2) <=deltaIdx/2)
        idxBase         = np.where(idx2[0]>minIdx+2*deltaIdx)
        #print (len(idxApx[0]),len(idxMidgland[0]),len(idxBase[0]))
        #print (idx2[0][idxApx],idx2[0][idxMidgland],idx2[0][idxBase])
        
        
        dist1_l = r1_img_a[idx2]
        
        dist1_apx_l = abs(r1_img_a[(idx2[0][idxApx],idx2[1][idxApx],idx2[2][idxApx])])
        dist1_md_l  = abs(r1_img_a[(idx2[0][idxMidgland],idx2[1][idxMidgland],idx2[2][idxMidgland])])
        dist1_bs_l  = abs(r1_img_a[(idx2[0][idxBase],idx2[1][idxBase],idx2[2][idxBase])])
        

        dist2_l = r2_img_a[np.where(r1_img_a==0)]
        dist_l =  abs(dist1_l)#np.concatenate((abs(dist1_l),abs(dist2_l)), axis=0)        
        
        dist[regn_idx].append(dist_l)
        dist_apx[regn_idx].append(dist1_apx_l)
        dist_md[regn_idx].append(dist1_md_l)
        dist_bs[regn_idx].append(dist1_bs_l)
       
        print(atls, regn, 'mean', np.mean(dist_l),len(idx2[0]))
        print(atls, regn, 'std ', np.std(dist_l))
    

axes[0,0].boxplot(dist[0],labels = labels, showmeans=True, showfliers=False, 
    boxprops=boxprops,meanprops=meanlineprops,meanline=True, medianprops = medianprops,
    capprops=capprops, whiskerprops=whiskerprops)
axes[0,0].set_ylim(-0.1,ylimMax)
axes[0,0].set_ylabel("Surface Distance (mm)",fontsize=20)
axes[0,1].boxplot(dist[1],labels = labels,showmeans=True, showfliers=False, 
    boxprops=boxprops,meanprops=meanlineprops,meanline=True, medianprops = medianprops,
    capprops=capprops, whiskerprops=whiskerprops)
axes[0,1].set_ylim(-0.1, ylimMax)
axes[0,1].set_ylabel("Surface Distance (mm)",fontsize=20)

axes[1,0].boxplot(dist_apx[0],labels = labels, showmeans=True, showfliers=False, 
    boxprops=boxprops,meanprops=meanlineprops,meanline=True, medianprops = medianprops,
    capprops=capprops, whiskerprops=whiskerprops)
axes[1,0].set_ylim(-0.1,ylimMax)
axes[1,0].set_ylabel("Surface Distance (mm)",fontsize=20)
axes[1,1].boxplot(dist_apx[1],labels = labels,showmeans=True, showfliers=False, 
    boxprops=boxprops,meanprops=meanlineprops,meanline=True, medianprops = medianprops,
    capprops=capprops, whiskerprops=whiskerprops)
axes[1,1].set_ylim(-0.1, ylimMax)
axes[1,1].set_ylabel("Surface Distance (mm)",fontsize=20)


axes[2,0].boxplot(dist_md[0],labels = labels, showmeans=True, showfliers=False, 
    boxprops=boxprops,meanprops=meanlineprops,meanline=True, medianprops = medianprops,
    capprops=capprops, whiskerprops=whiskerprops)
axes[2,0].set_ylim(-0.1,ylimMax)
axes[2,0].set_ylabel("Surface Distance (mm)",fontsize=20)
axes[2,1].boxplot(dist_md[1],labels = labels,showmeans=True, showfliers=False, 
    boxprops=boxprops,meanprops=meanlineprops,meanline=True, medianprops = medianprops,
    capprops=capprops, whiskerprops=whiskerprops)
axes[2,1].set_ylim(-0.1, ylimMax)
axes[2,1].set_ylabel("Surface Distance (mm)",fontsize=20)



axes[3,0].boxplot(dist_bs[0],labels = labels, showmeans=True, showfliers=False, 
    boxprops=boxprops,meanprops=meanlineprops,meanline=True, medianprops = medianprops,
    capprops=capprops, whiskerprops=whiskerprops)
axes[3,0].set_ylim(-0.1,ylimMax)
axes[3,0].set_ylabel("Surface Distance (mm)",fontsize=20)
axes[3,1].boxplot(dist_bs[1],labels = labels,showmeans=True, showfliers=False, 
    boxprops=boxprops,meanprops=meanlineprops,meanline=True, medianprops = medianprops,
    capprops=capprops, whiskerprops=whiskerprops)
axes[3,1].set_ylim(-0.1, ylimMax)
axes[3,1].set_ylabel("Surface Distance (mm)",fontsize=20)

plt.savefig(png_path_out, dpi=fig.dpi)
plt.savefig(eps_path_out, dpi=fig.dpi)
plt.show() 

print(ss.ranksums(dist[0][0],dist[0][1]), ss.ranksums(dist_apx[0][0],dist_apx[0][1]),
ss.ranksums(dist_md[0][0],dist_md[0][1]), ss.ranksums(dist_bs[0][0],dist_bs[0][1]))

       
results.close()