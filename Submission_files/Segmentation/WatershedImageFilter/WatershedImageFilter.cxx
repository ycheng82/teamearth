#include <iostream>

#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkScalarToRGBPixelFunctor.h"
#include "itkUnaryFunctorImageFilter.h"
#include "itkVectorCastImageFilter.h"
#include "itkVectorGradientAnisotropicDiffusionImageFilter.h"
#include "itkWatershedImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkScalarToRGBColormapImageFilter.h"
#include "itkGradientMagnitudeImageFilter.h"

#include "itkImageFileReader.h"
#include "QuickView.h"
#include "itkTimeProbesCollectorBase.h"

// Run with:
// ./WatershedImageFilter threshold level
// e.g.
// ./WatershedImageFilter 0.005 .5
// (A rule of thumb is to set the Threshold to be about 1 / 100 of the Level.)

typedef itk::Image<unsigned char, 2>       UnsignedCharImageType;
typedef itk::Image<float, 2>               FloatImageType;
typedef itk::RGBPixel<unsigned char>       RGBPixelType;
typedef itk::Image<RGBPixelType, 2>        RGBImageType;
typedef itk::Image<itk::IdentifierType, 2> LabeledImageType;

static void PerformSegmentation(FloatImageType::Pointer image, const float threshold, const float level);

int main( int argc, char *argv[] )
{
	// Verify arguments
	if (argc < 3 )
	{
		std::cerr << "Parameters " << std::endl;
		std::cerr << " threshold level" << std::endl;
		return 1;
	}
	
	typedef itk::Image<unsigned char, 2>  ImageType;
	typedef itk::ImageFileReader<ImageType> ReaderType;
	ReaderType::Pointer reader = ReaderType::New();
	
	// Parse arguments
	std::string strThreshold = argv[1];
	float threshold = 0.0;
	std::stringstream ssThreshold;
	ssThreshold << strThreshold;
	ssThreshold >> threshold;
	
	std::string strLevel = argv[2];
	float level = 0.0;
	std::stringstream ssLevel;
	ssLevel << strLevel;
	ssLevel >> level;
	
	// Output arguments
	threshold = 0.05;
	level=0.18;
	std::cout << "Running with:" << std::endl
	<< "Threshold: " << threshold << std::endl
	<< "Level: " << level << std::endl;
	
	
	typedef itk::GradientMagnitudeImageFilter<
	UnsignedCharImageType, FloatImageType >  GradientMagnitudeImageFilterType;
	GradientMagnitudeImageFilterType::Pointer gradientMagnitudeImageFilter =GradientMagnitudeImageFilterType::New();
	
//	reader->SetFileName("head_neck/000001.dcm");
	reader->SetFileName("test_head_neck.dcm");

	
//	gradientMagnitudeImageFilter->SetInput(image);
	gradientMagnitudeImageFilter->SetInput(reader->GetOutput());
	gradientMagnitudeImageFilter->Update();
	
	// Custom parameters
	itk::TimeProbesCollectorBase timer;
	for (threshold=0.01; threshold<0.02; threshold+=0.001) {
		for (level=0.15; level<0.25; level+=0.01) {
			std::stringstream ss;
			ss << threshold << "_" << level;
			std::cout << "Segment input for threshold_level = " << ss.str() << std::endl;
			timer.Start(ss.str().c_str());
			PerformSegmentation(gradientMagnitudeImageFilter->GetOutput(), threshold, level);
			timer.Stop(ss.str().c_str());
		}
	}
	timer.Report( std::cout );
	
	return EXIT_SUCCESS;
}


void PerformSegmentation(FloatImageType::Pointer image, const float threshold, const float level)
{
	typedef itk::WatershedImageFilter<FloatImageType> WatershedFilterType;
	WatershedFilterType::Pointer watershed = WatershedFilterType::New();
	watershed->SetThreshold(threshold);
	watershed->SetLevel(level);
	watershed->SetInput(image);
	watershed->Update();
	
	typedef itk::ScalarToRGBColormapImageFilter<LabeledImageType, RGBImageType> RGBFilterType;
	RGBFilterType::Pointer colormapImageFilter = RGBFilterType::New();
	colormapImageFilter->SetInput(watershed->GetOutput());
	colormapImageFilter->SetColormap( RGBFilterType::Jet );
	colormapImageFilter->Update();
	
	std::stringstream ss;
	ss << "times_thresh_0.01_0.02_level_0.15_0.25/output_" << threshold << "_" << level << ".png";
	
	typedef itk::ImageFileWriter<RGBImageType> FileWriterType;
	FileWriterType::Pointer writer = FileWriterType::New();
	writer->SetFileName(ss.str());
	writer->SetInput(colormapImageFilter->GetOutput());
	writer->Update();
	
}
