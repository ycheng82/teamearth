
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImageRegionIterator.h"
#include "itkImage.h"


#include "itkDemonsRegistrationFilter.h"
#include "itkHistogramMatchingImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkWarpImageFilter.h"
#include "itkGDCMImageIO.h"
#include "itkGDCMSeriesFileNames.h"
#include "itkImageSeriesReader.h"
#include "itkImageSeriesWriter.h"
#include <vector>
#include "itksys/SystemTools.hxx"
class CommandIterationUpdate : public itk::Command
{
public:
	typedef  CommandIterationUpdate                     Self;
	typedef  itk::Command                               Superclass;
	typedef  itk::SmartPointer<CommandIterationUpdate>  Pointer;
	itkNewMacro(CommandIterationUpdate);
protected:
	CommandIterationUpdate() {};
	typedef itk::Image< float, 2 >            InternalImageType;
	typedef itk::Vector< float, 2 >           VectorPixelType;
	typedef itk::Image<  VectorPixelType, 2 > DisplacementFieldType;
	typedef itk::DemonsRegistrationFilter<
		InternalImageType,
		InternalImageType,
		DisplacementFieldType>   RegistrationFilterType;
public:
	void Execute(itk::Object *caller, const itk::EventObject & event)
	{
		Execute((const itk::Object *)caller, event);
	}
	void Execute(const itk::Object * object, const itk::EventObject & event)
	{
		const RegistrationFilterType * filter = static_cast< const RegistrationFilterType * >(object);
		if (!(itk::IterationEvent().CheckEvent(&event)))
		{
			return;
		}
		std::cout << filter->GetMetric() << std::endl;
	}
};
int main(int argc, char *argv[])
{
	  //------------------------------------------------------------------------------
	if (argc < 3)
	{
		std::cerr << "Usage: " << argv[0] <<
			" DicomDirectory  OutputDicomDirectory" << std::endl;
		return EXIT_FAILURE;
	}	 
	/*if (argc < 4)
	{
		std::cerr << "Missing Parameters " << std::endl;
		std::cerr << "Usage: " << argv[0];
		std::cerr << " fixedImageFile movingImageFile ";
		std::cerr << " outputImageFile " << std::endl;
		std::cerr << " [outputDisplacementFieldFile] " << std::endl;
		return EXIT_FAILURE;
	}			*/

	typedef signed short    PixelType;
	const unsigned int      Dimension = 3;
	typedef itk::Image< PixelType, Dimension >      ImageType;
	typedef itk::ImageSeriesReader< ImageType >     ReaderType;

	typedef itk::GDCMImageIO                        ImageIOType;
	typedef itk::GDCMSeriesFileNames                NamesGeneratorType;
	ImageIOType::Pointer gdcmIO = ImageIOType::New();
	NamesGeneratorType::Pointer namesGenerator = NamesGeneratorType::New();

	namesGenerator->SetInputDirectory(argv[1]);
	const ReaderType::FileNamesContainer & filenames =
		namesGenerator->GetInputFileNames();

	std::size_t numberOfFileNames = filenames.size();
	std::cout << numberOfFileNames << std::endl;
	for (unsigned int fni = 0; fni < numberOfFileNames; ++fni)
	{
		std::cout << "filename # " << fni << " = ";
		std::cout << filenames[fni] << std::endl;
	}

	ReaderType::Pointer reader = ReaderType::New();
	reader->SetImageIO(gdcmIO);
	reader->SetFileNames(filenames);

	try
	{

		reader->Update();


	}
	catch (itk::ExceptionObject &excp)
	{
		std::cerr << "Exception thrown while writing the image" << std::endl;
		std::cerr << excp << std::endl;
		return EXIT_FAILURE;
	}
	//----------------------------------------------------------
	std::cout << "  file 2 " << std::endl;
	NamesGeneratorType::Pointer namesGenerator2 = NamesGeneratorType::New();
	namesGenerator2->SetInputDirectory(argv[2]);
	const ReaderType::FileNamesContainer & filenames2 =
		namesGenerator2->GetInputFileNames();

	 numberOfFileNames = filenames2.size();
	std::cout << numberOfFileNames << std::endl;
	for (unsigned int fni = 0; fni < numberOfFileNames; ++fni)
	{
		std::cout << "filename # " << fni << " = ";
		std::cout << filenames2[fni] << std::endl;
	}

	ReaderType::Pointer reader2 = ReaderType::New();
	reader2->SetImageIO(gdcmIO);
	reader2->SetFileNames(filenames2);

	try
	{

		reader2->Update();


	}
	catch (itk::ExceptionObject &excp)
	{
		std::cerr << "Exception thrown while writing the image" << std::endl;
		std::cerr << excp << std::endl;
		return EXIT_FAILURE;
	}
	//-------------------------------------------------------------------------------------------------

	std::cout << "  file in part end "<<std::endl;
	const unsigned int Dimension1 = 3;
	typedef unsigned short PixelType1;
	typedef itk::Image< PixelType1, Dimension1 >  FixedImageType;
	typedef itk::Image< PixelType1, Dimension1 >  MovingImageType;

	typedef itk::ImageFileReader< FixedImageType  > FixedImageReaderType;
	typedef itk::ImageFileReader< MovingImageType > MovingImageReaderType;
	FixedImageReaderType::Pointer fixedImageReader = FixedImageReaderType::New();
	MovingImageReaderType::Pointer movingImageReader = MovingImageReaderType::New();
	std::string in1 = ( filenames[0]);
	std::string in2 = ( filenames2[0]);
	std::cout  << in1 << std::endl;
	std::cout << in2 << std::endl;
	fixedImageReader->SetFileName(in1);			    // change it was argv[1]
	movingImageReader->SetFileName(in2);					// argv[2]

	//typedef itk::Image< PixelType1, 2 >        ImageType;
	typedef float                                      InternalPixelType;
	typedef itk::Image< InternalPixelType, Dimension1 > InternalImageType;
	typedef itk::CastImageFilter< FixedImageType,
		InternalImageType >  FixedImageCasterType;
	typedef itk::CastImageFilter< MovingImageType,
		InternalImageType >  MovingImageCasterType;
	FixedImageCasterType::Pointer fixedImageCaster = FixedImageCasterType::New();
	MovingImageCasterType::Pointer movingImageCaster
		= MovingImageCasterType::New();
	fixedImageCaster->SetInput(fixedImageReader->GetOutput());
	movingImageCaster->SetInput(movingImageReader->GetOutput());
	fixedImageCaster->Update();


	FixedImageType::Pointer	Image = fixedImageReader->GetOutput();
	FixedImageType::RegionType region = Image->GetLargestPossibleRegion();

	FixedImageType::SizeType size = region.GetSize();
	int w = region.GetSize()[0];
	int h = region.GetSize()[1];
	std::cout << size << std::endl;

	typedef itk::HistogramMatchingImageFilter<
		InternalImageType,
		InternalImageType >   MatchingFilterType;
	MatchingFilterType::Pointer matcher = MatchingFilterType::New();
	
	matcher->SetInput(movingImageCaster->GetOutput());
	matcher->SetReferenceImage(fixedImageCaster->GetOutput());
	
	matcher->SetNumberOfHistogramLevels(1024);
	matcher->SetNumberOfMatchPoints(7);
	
	matcher->ThresholdAtMeanIntensityOn();
	
	typedef itk::Vector< float, Dimension1 >           VectorPixelType;
	typedef itk::Image<  VectorPixelType, Dimension1 > DisplacementFieldType;
	typedef itk::DemonsRegistrationFilter<
		InternalImageType,
		InternalImageType,
		DisplacementFieldType> RegistrationFilterType;
	RegistrationFilterType::Pointer filter = RegistrationFilterType::New();
	
	CommandIterationUpdate::Pointer observer = CommandIterationUpdate::New();
	filter->AddObserver(itk::IterationEvent(), observer);
	
	filter->SetFixedImage(fixedImageCaster->GetOutput());
	filter->SetMovingImage(matcher->GetOutput());
	
	filter->SetNumberOfIterations(500);
	filter->SetStandardDeviations(5.0);
	
	filter->Update();
	
	typedef itk::WarpImageFilter<
		MovingImageType,
		MovingImageType,
		DisplacementFieldType  >     WarperType;
	typedef itk::LinearInterpolateImageFunction<
		MovingImageType,
		double          >  InterpolatorType;
	WarperType::Pointer warper = WarperType::New();
	InterpolatorType::Pointer interpolator = InterpolatorType::New();
	FixedImageType::Pointer fixedImage = fixedImageReader->GetOutput();
	warper->SetInput(movingImageReader->GetOutput());
	warper->SetInterpolator(interpolator);
	warper->SetOutputSpacing(fixedImage->GetSpacing());
	warper->SetOutputOrigin(fixedImage->GetOrigin());
	warper->SetOutputDirection(fixedImage->GetDirection());
	
	warper->SetDisplacementField(filter->GetOutput());
	
	typedef  unsigned char                           OutputPixelType;
	typedef itk::Image< OutputPixelType, Dimension1 > OutputImageType;
	typedef itk::CastImageFilter<
		MovingImageType,
		OutputImageType >          CastFilterType;
	typedef itk::ImageFileWriter< OutputImageType >  WriterType;
	WriterType::Pointer      writer = WriterType::New();
	CastFilterType::Pointer  caster = CastFilterType::New();
	writer->SetFileName(argv[3]);   // it should be 3
	caster->SetInput(warper->GetOutput());
	writer->SetInput(caster->GetOutput());
	writer->Update();
	
	if (argc > 4) // if a fourth line argument has been provided...
	{
	
		typedef itk::ImageFileWriter< DisplacementFieldType > FieldWriterType;
		FieldWriterType::Pointer fieldWriter = FieldWriterType::New();
		fieldWriter->SetFileName(argv[4]);
		fieldWriter->SetInput(filter->GetOutput());
		fieldWriter->Update();
		
	}
	if (argc > 5) // if a fifth line argument has been provided...
	{
		typedef DisplacementFieldType            VectorImage2DType;
		typedef DisplacementFieldType::PixelType Vector2DType;
		VectorImage2DType::ConstPointer vectorImage2D = filter->GetOutput();
		VectorImage2DType::RegionType  region2D = vectorImage2D->GetBufferedRegion();
		VectorImage2DType::IndexType   index2D = region2D.GetIndex();
		VectorImage2DType::SizeType    size2D = region2D.GetSize();
		typedef itk::Vector< float, 3 >  Vector3DType;
		typedef itk::Image< Vector3DType, 3 >  VectorImage3DType;
		typedef itk::ImageFileWriter< VectorImage3DType > VectorImage3DWriterType;
		VectorImage3DWriterType::Pointer writer3D = VectorImage3DWriterType::New();
		VectorImage3DType::Pointer vectorImage3D = VectorImage3DType::New();
		VectorImage3DType::RegionType  region3D;
		VectorImage3DType::IndexType   index3D;
		VectorImage3DType::SizeType    size3D;
		index3D[0] = index2D[0];
		index3D[1] = index2D[1];
		index3D[2] = 0;
		size3D[0] = size2D[0];
		size3D[1] = size2D[1];
		size3D[2] = 1;
		region3D.SetSize(size3D);
		region3D.SetIndex(index3D);
		vectorImage3D->SetRegions(region3D);
		vectorImage3D->Allocate();
		typedef itk::ImageRegionConstIterator< VectorImage2DType > Iterator2DType;
		typedef itk::ImageRegionIterator< VectorImage3DType > Iterator3DType;
		Iterator2DType  it2(vectorImage2D, region2D);
		Iterator3DType  it3(vectorImage3D, region3D);
		it2.GoToBegin();
		it3.GoToBegin();
		Vector2DType vector2D;
		Vector3DType vector3D;
		vector3D[2] = 0; // set Z component to zero.
		while (!it2.IsAtEnd())
		{
			vector2D = it2.Get();
			vector3D[0] = vector2D[0];
			vector3D[1] = vector2D[1];
			it3.Set(vector3D);
			++it2;
			++it3;
		}
		writer3D->SetInput(vectorImage3D);
		writer3D->SetFileName(argv[5]);
		try
		{
			writer3D->Update();
		}
		catch (itk::ExceptionObject & excp)
		{
			std::cerr << excp << std::endl;
			return EXIT_FAILURE;
		}
	}
	return EXIT_SUCCESS;
}