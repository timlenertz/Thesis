#include <itkImageRegistrationMethod.h>
#include <itkAffineTransform.h>
#include <itkMutualInformationImageToImageMetric.h>
#include <itkNormalizedCorrelationImageToImageMetric.h>
#include <itkGradientDescentOptimizer.h>
#include <itkRegularStepGradientDescentOptimizer.h>
#include <itkCastImageFilter.h>
#include <itkMeanSquaresImageToImageMetric.h>
#include <itkImageFileReader.h>
#include <itkTranslationTransform.h>

#include <itkImageFileWriter.h>
#include <itkResampleImageFilter.h>
#include <iostream>

#include "imreg.h"

using namespace itk;

namespace pcf {

namespace {
	using ImageType = Image<unsigned char, 2>;
	using TransformType = AffineTransform<double, 2>;
	using OptimizerType = RegularStepGradientDescentOptimizer;
	using MetricType = MeanSquaresImageToImageMetric<ImageType, ImageType>;
	using InterpolatorType = LinearInterpolateImageFunction<ImageType, double>;
	using RegistrationType = ImageRegistrationMethod<ImageType, ImageType>;

	ImageType::Pointer read_png(const std::string& file_name) {
		using ReaderType = itk::ImageFileReader<ImageType>;
		ReaderType::Pointer reader = ReaderType::New();
		reader->SetFileName(file_name.c_str());
		reader->Update();
		return reader->GetOutput();	
	}
}

Eigen::Affine2f affine_image_registration(const std::string& fixed_file_name, const std::string& loose_file_name) {
	ImageType::Pointer fixed = read_png(fixed_file_name);
	ImageType::Pointer loose = read_png(loose_file_name);
	
	TransformType::Pointer transform = TransformType::New();
	OptimizerType::Pointer optimizer = OptimizerType::New();
	InterpolatorType::Pointer interpolator = InterpolatorType::New();
	RegistrationType::Pointer registration = RegistrationType::New();
	MetricType::Pointer metric = MetricType::New();
	
	ImageType::RegionType fixed_region = fixed->GetLargestPossibleRegion();
	registration->SetFixedImageRegion(fixed_region);

	unsigned int number_of_pixels = fixed_region.GetNumberOfPixels();
	unsigned int number_of_samples = number_of_pixels * 0.01;
	
	std::cout << fixed_region.GetIndex() << std::endl;
	std::cout << fixed_region.GetSize() << std::endl;
	std::cout << loose->GetLargestPossibleRegion().GetIndex() << std::endl;
	std::cout << loose->GetLargestPossibleRegion().GetSize() << std::endl;
	
	//metric->SetFixedImageStandardDeviation(0.4);
	//metric->SetMovingImageStandardDeviation(0.4);
	//metric->SetNumberOfSpatialSamples(number_of_samples);
	
	//optimizer->SetLearningRate(0.1);
	optimizer->SetNumberOfIterations(100);
	//optimizer->MaximizeOn();
	
	optimizer->SetMaximumStepLength(1.0);
	optimizer->SetMinimumStepLength(0.1);
	
	registration->SetOptimizer(optimizer);
	registration->SetMetric(metric);
	registration->SetTransform((MetricType::TransformType*) transform.GetPointer());
	registration->SetInterpolator((MetricType::InterpolatorType*) interpolator.GetPointer());
	
	registration->SetFixedImage(fixed);
	registration->SetMovingImage(loose);
	
	using ParametersType = RegistrationType::ParametersType;
	ParametersType initial_parameters(6);
	initial_parameters[0] = 1.0;
	initial_parameters[1] = 0.0;
	initial_parameters[2] = 0.0;
	initial_parameters[3] = 1.0;
	initial_parameters[4] = 0.0;
	initial_parameters[5] = 0.0;
	
	registration->SetInitialTransformParameters(initial_parameters);
	
	try {
		registration->Update();
		
		ParametersType final_parameters = registration->GetLastTransformParameters();
		
		///
		using ResampleFilterType = ResampleImageFilter<ImageType, ImageType>;
		TransformType::Pointer final_transform = TransformType::New();
		final_transform->SetParameters(final_parameters);
		final_transform->SetFixedParameters(transform->GetFixedParameters());
		ResampleFilterType::Pointer resample = ResampleFilterType::New();
		resample->SetTransform(registration->GetOutput()->Get());
		resample->SetInput(loose);
		resample->SetSize(fixed->GetLargestPossibleRegion().GetSize());
		resample->SetOutputOrigin(fixed->GetOrigin());
		resample->SetOutputSpacing(fixed->GetSpacing());
		resample->SetOutputDirection(fixed->GetDirection());
		resample->SetDefaultPixelValue(0);
		
		
		ImageFileWriter<ImageType>::Pointer writer = ImageFileWriter<ImageType>::New();
		writer->SetFileName("output.png");
		writer->SetInput( resample->GetOutput() );
		writer->Update();
		///
		
		Eigen::Affine2f t = Eigen::Affine2f::Identity();
		t.matrix()(0, 0) = final_parameters[0];
		t.matrix()(0, 1) = final_parameters[1];
		t.matrix()(1, 0) = final_parameters[2];
		t.matrix()(1, 1) = final_parameters[3];
		t.matrix()(0, 2) = final_parameters[4];
		t.matrix()(1, 2) = final_parameters[5];
		return t;
	} catch(const ExceptionObject& err) {
		std::cerr << err.what() << std::endl;
		return Eigen::Affine2f::Identity();
	}
}

}