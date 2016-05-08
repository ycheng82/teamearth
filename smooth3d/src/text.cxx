#include "itkImage.h"
#include "itkGDCMImageIO.h"
#include "itkGDCMSeriesFileNames.h"
#include "itkImageSeriesReader.h"
#include "itkImageFileWriter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkDiscreteGaussianImageFilter.h"
// Software Guide : EndCodeSnippet
int main(int argc, char* argv[])
{
	if (argc < 5)
	{
		std::cerr << "Usage: " << std::endl;
		std::cerr << argv[0] << " DicomDirectory  outputFileName  [seriesName]  gaussianVariance  maxKernelWidth "
			<< std::endl;
		return EXIT_FAILURE;
	}

	typedef signed short    PixelType;
	const unsigned int      Dimension = 3;
	typedef itk::Image< PixelType, Dimension >         ImageType;

	typedef itk::ImageSeriesReader< ImageType >        ReaderType;
	ReaderType::Pointer reader = ReaderType::New();

	typedef itk::GDCMImageIO       ImageIOType;
	ImageIOType::Pointer dicomIO = ImageIOType::New();
	reader->SetImageIO(dicomIO);

	typedef itk::GDCMSeriesFileNames NamesGeneratorType;
	NamesGeneratorType::Pointer nameGenerator = NamesGeneratorType::New();
	nameGenerator->SetUseSeriesDetails(true);
	nameGenerator->AddSeriesRestriction("0008|0021");
	nameGenerator->SetDirectory(argv[1]);
	// Software Guide : EndCodeSnippet
	try
	{
		std::cout << std::endl << "The directory: " << std::endl;
		std::cout << std::endl << argv[1] << std::endl << std::endl;
		std::cout << "Contains the following DICOM Series: ";
		std::cout << std::endl << std::endl;
		// Software Guide : BeginLatex
		//
		// The GDCMSeriesFileNames object first identifies the list of DICOM series
		// present in the given directory. We receive that list in a reference
		// to a container of strings and then we can do things like print out all
		// the series identifiers that the generator had found. Since the process of
		// finding the series identifiers can potentially throw exceptions, it is
		// wise to put this code inside a \code{try/catch} block.
		//
		// Software Guide : EndLatex
		// Software Guide : BeginCodeSnippet
		typedef std::vector< std::string >    SeriesIdContainer;
		const SeriesIdContainer & seriesUID = nameGenerator->GetSeriesUIDs();
		SeriesIdContainer::const_iterator seriesItr = seriesUID.begin();
		SeriesIdContainer::const_iterator seriesEnd = seriesUID.end();
		while (seriesItr != seriesEnd)
		{
			std::cout << seriesItr->c_str() << std::endl;
			++seriesItr;
		}
		// Software Guide : EndCodeSnippet
		// Software Guide : BeginLatex
		//
		// Given that it is common to find multiple DICOM series in the same directory,
		// we must tell the GDCM classes what specific series we want to read. In
		// this example we do this by checking first if the user has provided a series
		// identifier in the command line arguments. If no series identifier has been
		// passed, then we simply use the first series found during the exploration of
		// the directory.
		//
		// Software Guide : EndLatex
		// Software Guide : BeginCodeSnippet
		std::string seriesIdentifier;
		
		
			seriesIdentifier = seriesUID.begin()->c_str();
		
		// Software Guide : EndCodeSnippet
		std::cout << std::endl << std::endl;
		std::cout << "Now reading series: " << std::endl << std::endl;
		std::cout << seriesIdentifier << std::endl;
		std::cout << std::endl << std::endl;
		// Software Guide : BeginLatex
		//
		// We pass the series identifier to the name generator and ask for all the
		// filenames associated to that series. This list is returned in a container of
		// strings by the \code{GetFileNames()} method.
		//
		// \index{itk::GDCMSeriesFileNames!GetFileNames()}
		//
		// Software Guide : EndLatex
		// Software Guide : BeginCodeSnippet
		typedef std::vector< std::string >   FileNamesContainer;
		FileNamesContainer fileNames;
		fileNames = nameGenerator->GetFileNames(seriesIdentifier);
		// Software Guide : EndCodeSnippet
		// Software Guide : BeginLatex
		//
		//
		// The list of filenames can now be passed to the \doxygen{ImageSeriesReader}
		// using the \code{SetFileNames()} method.
		//
		//  \index{itk::ImageSeriesReader!SetFileNames()}
		//
		// Software Guide : EndLatex
		// Software Guide : BeginCodeSnippet
		reader->SetFileNames(fileNames);
		// Software Guide : EndCodeSnippet
		// Software Guide : BeginLatex
		//
		// Finally we can trigger the reading process by invoking the \code{Update()}
		// method in the reader. This call as usual is placed inside a \code{try/catch}
		// block.
		//
		// Software Guide : EndLatex
		// Software Guide : BeginCodeSnippet
		try
		{
			reader->Update();
		}
		catch (itk::ExceptionObject &ex)
		{
			std::cout << ex << std::endl;
			return EXIT_FAILURE;
		}
					//-------------------------------------------------------
		typedef    short    InputPixelType;
		typedef    short   OutputPixelType;
		typedef itk::Image< InputPixelType, Dimension >   InputImageType;
		typedef itk::Image< OutputPixelType, Dimension >   OutputImageType;
		typedef itk::DiscreteGaussianImageFilter<InputImageType, OutputImageType >  FilterType;
		FilterType::Pointer filter = FilterType::New();
		filter->SetInput(reader->GetOutput());
		// Software Guide : EndCodeSnippet
		const double gaussianVariance = atof(argv[3]);
		const unsigned int maxKernelWidth = atoi(argv[4]);
		
		filter->SetVariance(gaussianVariance);
		filter->SetMaximumKernelWidth(maxKernelWidth);
		// Software Guide : EndCodeSnippet
		//  Software Guide : BeginLatex
		//
		//  Finally, the filter is executed by invoking the \code{Update()} method.
		//
		//  \index{itk::DiscreteGaussianImageFilter!Update()}
		//
		//  Software Guide : EndLatex
		// Software Guide : BeginCodeSnippet
		filter->Update();
		//------------------------
		typedef itk::ImageFileWriter< ImageType > WriterType;
		WriterType::Pointer writer = WriterType::New();
		writer->SetFileName(argv[2]);
		writer->SetInput(filter->GetOutput());
		// Software Guide : EndCodeSnippet
		std::cout << "Writing the image as " << std::endl << std::endl;
		std::cout << argv[2] << std::endl << std::endl;
		// Software Guide : BeginLatex
		//
		// The process of writing the image is initiated by invoking the
		// \code{Update()} method of the writer.
		//
		// Software Guide : EndLatex
		try
		{
			// Software Guide : BeginCodeSnippet
			writer->Update();
			// Software Guide : EndCodeSnippet
		}
		catch (itk::ExceptionObject &ex)
		{
			std::cout << ex << std::endl;
			return EXIT_FAILURE;
		}
	}
	catch (itk::ExceptionObject &ex)
	{
		std::cout << ex << std::endl;
		return EXIT_FAILURE;
	}
	// Software Guide : BeginLatex
	//
	// Note that in addition to writing the volumetric image to a file we could
	// have used it as the input for any 3D processing pipeline. Keep in mind that
	// DICOM is simply a file format and a network protocol. Once the image data
	// has been loaded into memory, it behaves as any other volumetric dataset that
	// you could have loaded from any other file format.
	//
	// Software Guide : EndLatex
	return EXIT_SUCCESS;
}