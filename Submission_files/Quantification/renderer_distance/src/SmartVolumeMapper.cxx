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
#include <vtkInteractorStyleImage.h>
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
#include <vtkStdString.h>
#include <vtkDistanceWidget.h>
#include <vtkDistanceRepresentation.h>
 
int main(int argc, char *argv[])
{
  vtkSmartPointer<vtkImageData> imageData =
    vtkSmartPointer<vtkImageData>::New();

  if (argc < 3)
    {
    cout << "USAGE: smartvolumemapper <filename> <mode>\n Modes: -S : locks axis, -V : 3D mode" << endl;
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
 
  //setup the 4 different views
vtkStdString mode = argv[2];
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
//turn off shading to improve performance 
  vtkSmartPointer<vtkVolumeProperty> volumeProperty = 
    vtkSmartPointer<vtkVolumeProperty>::New();
  volumeProperty->ShadeOff();
  


  vtkSmartPointer<vtkVolumeProperty> volumeProperty2 = 
    vtkSmartPointer<vtkVolumeProperty>::New();
  volumeProperty2->ShadeOff();
  


  vtkSmartPointer<vtkVolumeProperty> volumeProperty3 = 
    vtkSmartPointer<vtkVolumeProperty>::New();
  volumeProperty3->ShadeOff();
  

  vtkSmartPointer<vtkVolumeProperty> volumeProperty4 = 
    vtkSmartPointer<vtkVolumeProperty>::New();
  volumeProperty4->ShadeOff();
  
 
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


  //test
  vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity4 = 
    vtkSmartPointer<vtkPiecewiseFunction>::New();

    //compositeOpacity4->AddPoint(-3000.0,0.0);
    compositeOpacity4->AddPoint(0.0,0.0);
    //compositeOpacity4->AddPoint(0.1,0.0);
    compositeOpacity4->AddPoint(100.0,1.0);
    compositeOpacity4->AddPoint(255.1,0.0);
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
  vtkSmartPointer<vtkInteractorStyleImage> style = vtkSmartPointer<vtkInteractorStyleImage>::New();
  if (mode == "-S") //change style based on args
  {
    style->SetInteractionModeToImageSlicing();
  }
  else
  {
    style->SetInteractionModeToImage3D();
  }
  vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  interactor->SetRenderWindow(renderWindow);
  interactor->SetInteractorStyle(style);

//distance measure based on two points
  vtkSmartPointer<vtkDistanceWidget> distanceWidget = vtkSmartPointer<vtkDistanceWidget>::New();
  distanceWidget->SetInteractor(interactor);
  distanceWidget->CreateDefaultRepresentation();
  static_cast<vtkDistanceRepresentation *>(distanceWidget->GetRepresentation())
    ->SetLabelFormat("%-#6.3g mm"); //assuming dicom used mm

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