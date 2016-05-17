
#include <iostream>
#include <cstdlib>
// ITK IO includes
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
// ITK Registration includes
#include "itkMultiResolutionPDEDeformableRegistration.h"
#include "itkMultiResolutionImageRegistrationMethod.h"
#include "itkHistogramMatchingImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkWarpImageFilter.h"
unsigned int RmsCounter = 0;
double MaxRmsE[4] = {0.8,  0.75,  0.4, 0.2};
//
//  The following section of code implements a Command observer
//  that will monitor the evolution of the registration process.
//  This observer has a layer of intelligence, for deciding what
//  MaximumRMS convergence criteria to use at every resolution level.
//
class CommandIterationUpdate : public itk::Command
{
public:
  typedef  CommandIterationUpdate   Self;
  typedef  itk::Command             Superclass;
  typedef  itk::SmartPointer<Self>  Pointer;
  itkNewMacro( Self );
protected:
  CommandIterationUpdate() {};
  // define ITK short-hand types
  typedef short                                PixelType;
  typedef float                                InternalPixelType;
  typedef itk::Image< PixelType, 3 >           ImageType;
  typedef itk::Image< InternalPixelType, 3 >   InternalImageType;
  typedef itk::Vector< float, 3 >              VectorPixelType;
  typedef itk::Image< VectorPixelType, 3 >     DisplacementFieldType;
  typedef itk::DemonsRegistrationFilter< InternalImageType,
    InternalImageType, DisplacementFieldType>  RegistrationFilterType;
public:
  void Execute(const itk::Object *, const itk::EventObject & )
    {
    std::cout << "Warning: The const Execute method shouldn't be called" << std::endl;
    }
  void Execute(itk::Object *caller, const itk::EventObject & event)
    {
       RegistrationFilterType * filter = static_cast<  RegistrationFilterType * >( caller );
       if( !(itk::IterationEvent().CheckEvent( &event )) )
        {
        return;
        }
      if(filter)
        {
        filter->SetMaximumRMSError(MaxRmsE[RmsCounter]);
        std::cout << filter->GetMetric() <<  "  RMS Change: " << filter->GetRMSChange() << std::endl;
         std::cout << "Level Tolerance=  "<<filter->GetMaximumRMSError ()<<std::endl;
    }
}
};
//
// The following command observer reports the progress of the registration
// inside a given resolution level.
//
class CommandResolutionLevelUpdate : public itk::Command
{
public:
  typedef  CommandResolutionLevelUpdate   Self;
  typedef  itk::Command                   Superclass;
  typedef  itk::SmartPointer<Self>        Pointer;
  itkNewMacro( Self );
protected:
  CommandResolutionLevelUpdate() {};
public:
  void Execute(itk::Object *caller, const itk::EventObject & event)
    {
    Execute( (const itk::Object *)caller, event);
    }
  void Execute(const itk::Object *, const itk::EventObject & )
    {
    std::cout << "----------------------------------" << std::endl;
    RmsCounter = RmsCounter + 1;
    std::cout << "----------------------------------" << std::endl;
    }
};
int main( int argc, char * argv [] )
{
  // Verify the number of parameters in the command line
  if( argc != 5 )
    {
    std::cerr << "usage: " << std::endl;
    std::cerr << argv[0] << " fixedImage movingImage registeredImage deformationField" << std::endl;
    return EXIT_FAILURE;
    }
  // define ITK short-hand types
  const unsigned int Dimension = 3;
  typedef short                                                PixelType;
  typedef float                                                InternalPixelType;
  typedef itk::Image< PixelType, Dimension >                   ImageType;
  typedef itk::Image< InternalPixelType, Dimension >           InternalImageType;
  typedef itk::CastImageFilter< ImageType, InternalImageType > ImageCasterType;
  // setup input file readers
  typedef itk::ImageFileReader< ImageType >  ReaderType;
  ReaderType::Pointer targetReader = ReaderType::New();
  targetReader->SetFileName( argv[1] );
  targetReader->Update();
  ReaderType::Pointer sourceReader = ReaderType::New();
  sourceReader->SetFileName( argv[2] );
  sourceReader->Update();
  // cast target and source to float
  ImageCasterType::Pointer targetImageCaster = ImageCasterType::New();
  ImageCasterType::Pointer sourceImageCaster = ImageCasterType::New();
  targetImageCaster->SetInput( targetReader->GetOutput() );
  sourceImageCaster->SetInput( sourceReader->GetOutput() );
  // match the histograms between source and target
  typedef itk::HistogramMatchingImageFilter<
    InternalImageType, InternalImageType >            MatchingFilterType;
  MatchingFilterType::Pointer matcher = MatchingFilterType::New();
  matcher->SetInput( sourceImageCaster->GetOutput() );
  matcher->SetReferenceImage( targetImageCaster->GetOutput() );
  matcher->SetNumberOfHistogramLevels( 1024 );
  matcher->SetNumberOfMatchPoints( 7 );
  matcher->ThresholdAtMeanIntensityOn();
  // setup the deformation field and filter
  typedef itk::Vector< float, Dimension > VectorPixelType;
  typedef itk::Image< VectorPixelType, Dimension > DisplacementFieldType;
  typedef itk::DemonsRegistrationFilter<
    InternalImageType,
    InternalImageType,
    DisplacementFieldType>       RegistrationFilterType;
  RegistrationFilterType::Pointer filter = RegistrationFilterType::New();
  filter->SetStandardDeviations( 1.0 );
  //
  // Create the Command observer and register it with the registration filter.
  //
  CommandIterationUpdate::Pointer observer = CommandIterationUpdate::New();
  filter->AddObserver( itk::IterationEvent(), observer );
  // use multiresolution scheme
  typedef itk::MultiResolutionPDEDeformableRegistration<
    InternalImageType,
    InternalImageType,
    DisplacementFieldType >      MultiResRegistrationFilterType;
  MultiResRegistrationFilterType::Pointer multires =
    MultiResRegistrationFilterType::New();
  multires->SetRegistrationFilter( filter );
  multires->SetNumberOfLevels( 4 );
  multires->SetFixedImage( targetImageCaster->GetOutput() );
  multires->SetMovingImage( matcher->GetOutput() );
  unsigned int nIterations[4] = {40, 40, 32, 32 };
  multires->SetNumberOfIterations( nIterations );
  //
  // Create the Command observer and register it with the registration filter.
  //
  CommandResolutionLevelUpdate::Pointer levelobserver = CommandResolutionLevelUpdate::New();
  multires->AddObserver( itk::IterationEvent(), levelobserver );
  // apply the registration filter
  try
    {
    multires->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }
  // compute the output (warped) image
  typedef itk::WarpImageFilter< ImageType, ImageType, DisplacementFieldType > WarperType;
  typedef itk::LinearInterpolateImageFunction< ImageType, double > InterpolatorType;
  WarperType::Pointer warper = WarperType::New();
  InterpolatorType::Pointer interpolator = InterpolatorType::New();
  ImageType::Pointer targetImage = targetReader->GetOutput();
  warper->SetInput( sourceReader->GetOutput() );
  warper->SetInterpolator( interpolator );
  warper->SetOutputSpacing( targetImage->GetSpacing() );
  warper->SetOutputOrigin( targetImage->GetOrigin() );
  warper->SetOutputDirection( targetImage->GetDirection() );
  warper->SetDisplacementField( multires->GetOutput() );
  typedef itk::ImageFileWriter< ImageType >  WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName( argv[3] );
  writer->SetInput( warper->GetOutput() );
  try
    {
    writer->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }
  // write the deformation field
  typedef itk::ImageFileWriter< DisplacementFieldType >  DeformationWriterType;
  DeformationWriterType::Pointer defwriter = DeformationWriterType::New();
  defwriter->SetFileName( argv[4] );
  defwriter->SetInput( multires->GetOutput() );
  try
    {
    defwriter->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }
  return EXIT_SUCCESS;
}