
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src )

SET(TESTS MaskTests)

SET(Test_SRCS
itkDiffeomorphicDemonsRegistrationWithMaskFilterTest.cxx
)

#ADD_EXECUTABLE(DiffeomorphicDemonsRegistrationWithMaskFilterTest itkDiffeomorphicDemonsRegistrationWithMaskFilterTest.cxx)
ADD_EXECUTABLE(MaskTests test.cxx ${Test_SRCS})
TARGET_LINK_LIBRARIES(MaskTests ITKAlgorithms ITKIO)

ADD_TEST(MaskTest1 ${TESTS}  
--compare
    ${PROJECT_SOURCE_DIR}/data/result_with_mask.nii.gz
    ${PROJECT_BINARY_DIR}/test_WithMasks.nii.gz
    --compareNumberOfPixelsTolerance 0
    --compareIntensityTolerance 10
  itkDiffeomorphicDemonsRegistrationWithMaskFilterTest
    ${PROJECT_SOURCE_DIR}/data/subject1_Small.nii.gz 
    ${PROJECT_SOURCE_DIR}/data/subject2_Small.nii.gz
    ${PROJECT_BINARY_DIR}/test_WithMasks.nii.gz
    ${PROJECT_SOURCE_DIR}/data/subject1_Small_mask.nii.gz
    ${PROJECT_SOURCE_DIR}/data/subject2_Small_mask.nii.gz
)

ADD_TEST(MaskTest2 ${TESTS}
--compare
    ${PROJECT_SOURCE_DIR}/data/result_with_mask.nii.gz
    ${PROJECT_BINARY_DIR}/test_WithFixedMask.nii.gz
    --compareNumberOfPixelsTolerance 100 
    --compareIntensityTolerance 10
  itkDiffeomorphicDemonsRegistrationWithMaskFilterTest
    ${PROJECT_SOURCE_DIR}/data/subject1_Small.nii.gz
    ${PROJECT_SOURCE_DIR}/data/subject2_Small.nii.gz
    ${PROJECT_BINARY_DIR}/test_WithFixedMask.nii.gz
    ${PROJECT_SOURCE_DIR}/data/subject1_Small_mask.nii.gz
)

ADD_TEST(WithoutMaskTest ${TESTS}
--compare
    ${PROJECT_SOURCE_DIR}/data/result_without_mask.nii.gz
    ${PROJECT_BINARY_DIR}/test_WithoutMasks.nii.gz
    --compareNumberOfPixelsTolerance 0
    --compareIntensityTolerance 10
  itkDiffeomorphicDemonsRegistrationWithMaskFilterTest
    ${PROJECT_SOURCE_DIR}/data/subject1_Small.nii.gz
    ${PROJECT_SOURCE_DIR}/data/subject2_Small.nii.gz
    ${PROJECT_BINARY_DIR}/test_WithoutMasks.nii.gz
)

