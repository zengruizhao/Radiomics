# -*- coding: utf-8 -*-
"""
Created on Sat Oct 01 13:49:34 2016

@author: mirabela
"""


#path_in = 'C:\\Mirabela\\Projects\\AtlasComparison\\Results\\img\\NewResults\\images'
#path_out = 'C:\\Mirabela\Projects\\AtlasComparison\\Results\\img\\NewResults\\processed'

path_in = 'C:\\Mirabela\\Projects\\AtlasComparison\\Results\\img\\OldResults\\redone\\images'
path_out = 'C:\\Mirabela\Projects\\AtlasComparison\\Results\\img\\OldResults\\redone\\processed'


import os
import SimpleITK as sitk
import numpy as np
import cv2

def createContour(img):
    
    arr = sitk.GetArrayFromImage(img)[:,:,1]
    arr[arr<50] = 0 
    arr[arr>=50] = 255

    kernel = np.ones([5,5])
    arr = cv2.dilate(arr,kernel)
    arr = cv2.erode(arr,kernel)
    
    kernel = np.ones([9,9])
    arr2 = cv2.erode(arr,kernel)
    
    arr -= arr2
 
    kernel = np.ones([6,6])
    arr3 = cv2.dilate(arr,kernel)
 
    return sitk.GetImageFromArray(arr), sitk.GetImageFromArray(arr3)
    
def addContour(img, cntr, col, cntr2, col2,tr):
    
    s = img.GetSize()
    for x in range(s[0]):
        for y in range(s[1]):
            if cntr2.GetPixel(x,y)>0 and x-tr[0]>=0 and x-tr[0]<s[0] and y-tr[1]>=0 and y-tr[1]<s[1]:
                img.SetPixel (x-tr[0],y-tr[1],col2);
            
            if cntr.GetPixel(x,y)>0 and x-tr[0]>=0 and x-tr[0]<s[0] and y-tr[1]>=0 and y-tr[1]<s[1]:
                img.SetPixel (x-tr[0],y-tr[1],col);
            
    return img
    

for f in os.listdir(path_in):
    
    print f
    img = sitk.ReadImage(os.path.join(path_in,f))
    
    cropPr = sitk.Crop(img,[0,0],[img.GetSize()[0]/2,img.GetSize()[1]/2])
    cropCG = sitk.Crop(img,[img.GetSize()[0]/2,0],[0,img.GetSize()[1]/2])

    cropCGSignificance = sitk.Crop(img,[img.GetSize()[0]/2,img.GetSize()[1]/2],[0,0])
    cropPrSignificance = sitk.Crop(img,[0,img.GetSize()[1]/2],[img.GetSize()[0]/2,0])
    
    cropCGOutline1,cropCGOutline2 = createContour(cropCGSignificance)
    cropPrOutline1,cropPrOutline2 = createContour(cropPrSignificance)
    
    col1 = [255,255,0]
    col2 = [0,0,0]
    tr  = [0,0]
    cropCGOutlined = addContour(cropCG, cropCGOutline1,col1, cropCGOutline2,col2,tr)
    cropPrOutlined = addContour(cropPr, cropPrOutline1,col1, cropPrOutline2,col2,tr)
    
    brdr = [100,6]
    cropPrOutlined = sitk.Crop(cropPrOutlined,brdr,brdr)
    
    brdr = [150,6]
    cropCGOutlined = sitk.Crop(cropCGOutlined,brdr, brdr)


    sitk.WriteImage(cropPrOutlined, os.path.join(path_out,f[:-4]+"_pr.png"))    
    sitk.WriteImage(cropCGOutlined, os.path.join(path_out,f[:-4]+"_cg.png"))    