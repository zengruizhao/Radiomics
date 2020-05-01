# Radiomics

1. tool.py 通过pnet_mri函数将dicom格式转化为nii
2. nib2mat.py 通过nib2mat_all_wholeimage函数将nii转化为mat
3. biasFieldcorrection.m
4. resampling.py
5. tool.py 通过dicomInformation函数获取dicom信息
6. information.m 解析information
7. intensity standardization
8. nib2mat.py 通过mat2nib将mat格式的数据转化为nii格式
9. extractFeatures.py 使用[pyradiomics](https://pyradiomics.readthedocs.io/en/latest/)提取特征
