#include <vtkVersion.h>
#include <vtkSmartPointer.h>
#include <vtkSphere.h>
#include <vtkSampleFunction.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkVolumeProperty.h>
#include <vtkCamera.h>
#include <vtkImageShiftScale.h>
#include <vtkImageData.h>
#include <vtkPointData.h>
#include <vtkDataArray.h>
//#include <vtkXMLImageDataReader.h>
#include <vtkGenericDataObjectReader.h>
#include <vtkMassProperties.h>
#include <vtkPolyData.h>
#include <vtkPolyDataReader.h>
#include <vtkImageDataGeometryFilter.h>
 
static void CreateImageData(vtkImageData* im);
 
int main(int argc, char *argv[])
{
  vtkSmartPointer<vtkImageData> imageData =
    vtkSmartPointer<vtkImageData>::New();

  if (argc < 2)
    {
    cout << "USAGE: smartvolumemapper <filename>" << endl;
    return 0;
    }
  else
    {
    vtkSmartPointer<vtkGenericDataObjectReader> reader = 
      vtkSmartPointer<vtkGenericDataObjectReader>::New();
    reader->SetFileName(argv[1]);
    reader->Update();
    imageData->ShallowCopy(reader->GetOutput());
    }
 
  //setup the 3 different views

  vtkSmartPointer<vtkSmartVolumeMapper> volumeMapper = 
    vtkSmartPointer<vtkSmartVolumeMapper>::New();
  volumeMapper->SetBlendModeToComposite(); // composite first


  vtkSmartPointer<vtkSmartVolumeMapper> volumeMapper2 = 
    vtkSmartPointer<vtkSmartVolumeMapper>::New();
  volumeMapper2->SetBlendModeToComposite(); // composite first


  vtkSmartPointer<vtkSmartVolumeMapper> volumeMapper3 = 
    vtkSmartPointer<vtkSmartVolumeMapper>::New();
  volumeMapper3->SetBlendModeToComposite(); // composite first

  vtkSmartPointer<vtkSmartVolumeMapper> volumeMapper4 = 
    vtkSmartPointer<vtkSmartVolumeMapper>::New();
  volumeMapper4->SetBlendModeToComposite(); // composite first

#if VTK_MAJOR_VERSION <= 5
  volumeMapper->SetInputConnection(imageData->GetProducerPort());
  volumeMapper2->SetInputConnection(imageData->GetProducerPort());
  volumeMapper3->SetInputConnection(imageData->GetProducerPort());
  volumeMapper4->SetInputConnection(imageData->GetProducerPort());
#else
  volumeMapper->SetInputData(imageData);
  volumeMapper2->SetInputData(imageData);
  volumeMapper3->SetInputData(imageData);
  volumeMapper4->SetInputData(imageData);
#endif  
  vtkSmartPointer<vtkVolumeProperty> volumeProperty = 
    vtkSmartPointer<vtkVolumeProperty>::New();
  volumeProperty->ShadeOn();
  


  vtkSmartPointer<vtkVolumeProperty> volumeProperty2 = 
    vtkSmartPointer<vtkVolumeProperty>::New();
  volumeProperty2->ShadeOn();
  


  vtkSmartPointer<vtkVolumeProperty> volumeProperty3 = 
    vtkSmartPointer<vtkVolumeProperty>::New();
  volumeProperty3->ShadeOn();
  

  vtkSmartPointer<vtkVolumeProperty> volumeProperty4 = 
    vtkSmartPointer<vtkVolumeProperty>::New();
  volumeProperty4->ShadeOn();
  
 
  //skin bone
  vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity1 = 
    vtkSmartPointer<vtkPiecewiseFunction>::New();

    compositeOpacity1->AddPoint(0.0,0.0);
    compositeOpacity1->AddPoint(90.0,0.1);
    compositeOpacity1->AddPoint(90.1,0.0);
    compositeOpacity1->AddPoint(1000.0,1.0);

    //bones only
  vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity2 = 
    vtkSmartPointer<vtkPiecewiseFunction>::New();

    compositeOpacity2->AddPoint(255.0,0.0);
    compositeOpacity2->AddPoint(1000.0,1.0);

  //skin only
  vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity3 = 
    vtkSmartPointer<vtkPiecewiseFunction>::New();

    compositeOpacity3->AddPoint(0.0,0.0);
    compositeOpacity3->AddPoint(90.0,1.0);
    compositeOpacity3->AddPoint(90.1,0.0);
    compositeOpacity3->AddPoint(1000.0,0.0);


  //organs
  vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity4 = 
    vtkSmartPointer<vtkPiecewiseFunction>::New();

    //compositeOpacity4->AddPoint(-3000.0,0.0);
    compositeOpacity4->AddPoint(40.0,1.0);
    compositeOpacity4->AddPoint(40.1,0.0);
    compositeOpacity4->AddPoint(90.0,1.0);
    compositeOpacity4->AddPoint(90.1,0.0);
    compositeOpacity4->AddPoint(1000.0,0.0);

  volumeProperty->SetScalarOpacity(compositeOpacity1); // composite first.
  volumeProperty2->SetScalarOpacity(compositeOpacity2); // composite first.
  volumeProperty3->SetScalarOpacity(compositeOpacity3); // composite first.
  volumeProperty4->SetScalarOpacity(compositeOpacity4); // composite first.
 
  vtkSmartPointer<vtkColorTransferFunction> color = 
    vtkSmartPointer<vtkColorTransferFunction>::New();
  //color->AddRGBPoint(-3000.0  ,0.2,0.8,0.2);
  color->AddRGBPoint(0.0  ,0.6,0.8,0.8);
  color->AddRGBPoint(255.0  ,1.0,0.0,0.0);
  color->AddRGBPoint(1000.0,1.0,1.0,1.0);
  volumeProperty->SetColor(color);
  volumeProperty2->SetColor(color);
  volumeProperty3->SetColor(color);
  volumeProperty4->SetColor(color);

  volumeProperty->SetInterpolationTypeToLinear();
  volumeProperty2->SetInterpolationTypeToLinear();
  volumeProperty3->SetInterpolationTypeToLinear();
  volumeProperty4->SetInterpolationTypeToLinear();
 
  vtkSmartPointer<vtkVolume> volume = 
    vtkSmartPointer<vtkVolume>::New();
  vtkSmartPointer<vtkVolume> volume2 = 
    vtkSmartPointer<vtkVolume>::New();
  vtkSmartPointer<vtkVolume> volume3 = 
    vtkSmartPointer<vtkVolume>::New();
  vtkSmartPointer<vtkVolume> volume4 = 
    vtkSmartPointer<vtkVolume>::New();


  volume->SetMapper(volumeMapper);
  volume->SetProperty(volumeProperty);
  volume2->SetMapper(volumeMapper2);
  volume2->SetProperty(volumeProperty2);
  volume3->SetMapper(volumeMapper3);
  volume3->SetProperty(volumeProperty3);
  volume4->SetMapper(volumeMapper4);
  volume4->SetProperty(volumeProperty4);

 
  // Software mode, for coverage. It also makes sure we will get the same
  // regression image on all platforms.
  //volumeMapper->SetRequestedRenderModeToRayCast();
  //volumeMapper2->SetRequestedRenderModeToRayCast();
  //volumeMapper3->SetRequestedRenderModeToRayCast();
  //volumeMapper4->SetRequestedRenderModeToRayCast();
  

  //set up renderer window
  vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->SetSize(800, 801);

  //set up interactor
  vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  interactor->SetRenderWindow(renderWindow);

  //set up the viewports
  double topLeft[4] =     {0.0, 0.0, 0.5, 0.5};
  double topRight[4] =    {0.5, 0.5, 1.0, 1.0};
  double bottomLeft[4] =  {0.0, 0.5, 0.5, 1.0};
  double bottomRight[4] = {0.5, 0.0, 1.0, 0.5};

  //set up the renderers
  vtkSmartPointer<vtkRenderer> topLeftRenderer = vtkSmartPointer<vtkRenderer>::New();
  renderWindow->AddRenderer(topLeftRenderer);
  topLeftRenderer->SetViewport(topLeft);
  topLeftRenderer->SetBackground(.4, .5, .6);

  vtkSmartPointer<vtkRenderer> topRightRenderer = vtkSmartPointer<vtkRenderer>::New();
  renderWindow->AddRenderer(topRightRenderer);
  topRightRenderer->SetViewport(topRight);
  topRightRenderer->SetBackground(.4, .4, .4);

  vtkSmartPointer<vtkRenderer> bottomLeftRenderer = vtkSmartPointer<vtkRenderer>::New();
  renderWindow->AddRenderer(bottomLeftRenderer);
  bottomLeftRenderer->SetViewport(bottomLeft);
  bottomLeftRenderer->SetBackground(.4, .5, .4);

  vtkSmartPointer<vtkRenderer> bottomRightRenderer = vtkSmartPointer<vtkRenderer>::New();
  renderWindow->AddRenderer(bottomRightRenderer);
  bottomRightRenderer->SetViewport(bottomRight);
  bottomRightRenderer->SetBackground(.5, .5, .4);

  topLeftRenderer->AddViewProp(volume);
  topRightRenderer->AddViewProp(volume2);
  bottomLeftRenderer->AddViewProp(volume3);
  bottomRightRenderer->AddViewProp(volume4);

  topRightRenderer->ResetCamera();
  topLeftRenderer->ResetCamera();
  bottomLeftRenderer->ResetCamera();
  bottomRightRenderer->ResetCamera();

  renderWindow->Render();

  interactor->Start();
 
  return EXIT_SUCCESS;
}
 
void CreateImageData(vtkImageData* imageData)
{
  // Create a spherical implicit function.
  vtkSmartPointer<vtkSphere> sphere =
    vtkSmartPointer<vtkSphere>::New();
  sphere->SetRadius(0.1);
  sphere->SetCenter(0.0,0.0,0.0);
 
  vtkSmartPointer<vtkSampleFunction> sampleFunction =
    vtkSmartPointer<vtkSampleFunction>::New();
  sampleFunction->SetImplicitFunction(sphere);
  sampleFunction->SetOutputScalarTypeToDouble();
  sampleFunction->SetSampleDimensions(127,127,127); // intentional NPOT dimensions.
  sampleFunction->SetModelBounds(-1.0,1.0,-1.0,1.0,-1.0,1.0);
  sampleFunction->SetCapping(false);
  sampleFunction->SetComputeNormals(false);
  sampleFunction->SetScalarArrayName("values");
  sampleFunction->Update();
 
  vtkDataArray* a = sampleFunction->GetOutput()->GetPointData()->GetScalars("values");
  double range[2];
  a->GetRange(range);
 
  vtkSmartPointer<vtkImageShiftScale> t = 
    vtkSmartPointer<vtkImageShiftScale>::New();
  t->SetInputConnection(sampleFunction->GetOutputPort());
 
  t->SetShift(-range[0]);
  double magnitude=range[1]-range[0];
  if(magnitude==0.0)
    {
    magnitude=1.0;
    }
  t->SetScale(255.0/magnitude);
  t->SetOutputScalarTypeToUnsignedChar();
 
  t->Update();
 
  imageData->ShallowCopy(t->GetOutput());
}