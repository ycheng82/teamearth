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

static void CreateImage(UnsignedCharImageType::Pointer image);
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
	
	UnsignedCharImageType::Pointer image = UnsignedCharImageType::New();
	CreateImage(image);
	
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
	
	// Fixed parameters
//	PerformSegmentation(gradientMagnitudeImageFilter->GetOutput(), .0025, .25);
//	PerformSegmentation(gradientMagnitudeImageFilter->GetOutput(), .005, .5);
//	PerformSegmentation(gradientMagnitudeImageFilter->GetOutput(), .0075, .75);
//	PerformSegmentation(gradientMagnitudeImageFilter->GetOutput(), .009, .9);
	
	
//	QuickView viewer;
//	reader->SetFileName(name+"0.0025" + "_0.25"+".png");
//	viewer.AddImage(reader->GetOutput());
//	reader->SetFileName(name+"0.005" + "_0.5"+".png");
//	viewer.AddImage(reader->GetOutput());
//	reader->SetFileName(name+"0.0075" + "_0.75"+".png");
//	viewer.AddImage(reader->GetOutput());
//	reader->SetFileName(name+"0.009" + "_0.9"+".png");
//	viewer.AddImage(reader->GetOutput());
	
//	reader->SetFileName(name+ssThreshold.str() + "_"+ssLevel.str()+".png");
//	viewer.AddImage(reader->GetOutput());

//	reader->SetFileName("input.png");
//	viewer.AddImage(reader->GetOutput());
	
//	viewer.Visualize();
	
	return EXIT_SUCCESS;
}


void CreateImage(UnsignedCharImageType::Pointer image)
{
	// Create a white image with 3 dark regions of different values
	
	itk::Index<2> start;
	start.Fill(0);
	
	itk::Size<2> size;
	size.Fill(200);
	
	itk::ImageRegion<2> region(start,size);
	image->SetRegions(region);
	image->Allocate();
	image->FillBuffer(255);
	
	itk::ImageRegionIterator<UnsignedCharImageType> imageIterator(image,region);
 
	while(!imageIterator.IsAtEnd())
	{
		if(imageIterator.GetIndex()[0] > 20 && imageIterator.GetIndex()[0] < 50 &&
		   imageIterator.GetIndex()[1] > 20 && imageIterator.GetIndex()[1] < 50)
			imageIterator.Set(50);
		
		++imageIterator;
	}
	
	imageIterator.GoToBegin();
	
	while(!imageIterator.IsAtEnd())
	{
		if(imageIterator.GetIndex()[0] > 60 && imageIterator.GetIndex()[0] < 80 &&
		   imageIterator.GetIndex()[1] > 60 && imageIterator.GetIndex()[1] < 80)
			imageIterator.Set(100);
		
		++imageIterator;
	}
	
	imageIterator.GoToBegin();
	
	while(!imageIterator.IsAtEnd())
	{
		if(imageIterator.GetIndex()[0] > 100 && imageIterator.GetIndex()[0] < 130 &&
		   imageIterator.GetIndex()[1] > 100 && imageIterator.GetIndex()[1] < 130)
			imageIterator.Set(150);
		
		++imageIterator;
	}
	
	typedef itk::ImageFileWriter<UnsignedCharImageType> FileWriterType;
	FileWriterType::Pointer writer = FileWriterType::New();
	writer->SetFileName("input.png");
	writer->SetInput(image);
	writer->Update();
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
