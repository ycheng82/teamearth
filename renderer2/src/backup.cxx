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
 
 //renWin Skin and Bone
  vtkSmartPointer<vtkRenderWindow> renWin = 
    vtkSmartPointer<vtkRenderWindow>::New();

  //renWin2 Skin
  vtkSmartPointer<vtkRenderWindow> renWin2 = 
    vtkSmartPointer<vtkRenderWindow>::New();

  //renWin3 Bone
  vtkSmartPointer<vtkRenderWindow> renWin3 = 
    vtkSmartPointer<vtkRenderWindow>::New();

  //ren renWin
  vtkSmartPointer<vtkRenderer> ren1 = 
    vtkSmartPointer<vtkRenderer>::New();
  ren1->SetBackground(0.4,0.4,0.4);

  //ren2 renWin2
  vtkSmartPointer<vtkRenderer> ren2 = 
    vtkSmartPointer<vtkRenderer>::New();
  ren2->SetBackground(0.4,0.4,0.4);

  //ren3 renWin3
  vtkSmartPointer<vtkRenderer> ren3 = 
    vtkSmartPointer<vtkRenderer>::New();
  ren3->SetBackground(0.4,0.4,0.4);
 
  renWin->AddRenderer(ren1);
  renWin2->AddRenderer(ren2);
  renWin3->AddRenderer(ren3);
 
  renWin->SetSize(301,300); // intentional odd and NPOT  width/height
  renWin2->SetSize(301,300);
  renWin3->SetSize(301,300);
 
  vtkSmartPointer<vtkRenderWindowInteractor> iren = 
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  iren->SetRenderWindow(renWin);


  vtkSmartPointer<vtkRenderWindowInteractor> iren2 = 
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  iren2->SetRenderWindow(renWin2);


  vtkSmartPointer<vtkRenderWindowInteractor> iren3 = 
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  iren3->SetRenderWindow(renWin3);
 
  renWin->Render(); // make sure we have an OpenGL context.
  renWin2->Render();
  renWin3->Render();
 
  vtkSmartPointer<vtkSmartVolumeMapper> volumeMapper = 
    vtkSmartPointer<vtkSmartVolumeMapper>::New();
  volumeMapper->SetBlendModeToComposite(); // composite first


  vtkSmartPointer<vtkSmartVolumeMapper> volumeMapper2 = 
    vtkSmartPointer<vtkSmartVolumeMapper>::New();
  volumeMapper2->SetBlendModeToComposite(); // composite first


  vtkSmartPointer<vtkSmartVolumeMapper> volumeMapper3 = 
    vtkSmartPointer<vtkSmartVolumeMapper>::New();
  volumeMapper3->SetBlendModeToComposite(); // composite first

#if VTK_MAJOR_VERSION <= 5
  volumeMapper->SetInputConnection(imageData->GetProducerPort());
  volumeMapper2->SetInputConnection(imageData->GetProducerPort());
  volumeMapper3->SetInputConnection(imageData->GetProducerPort());
#else
  volumeMapper->SetInputData(imageData);
  volumeMapper2->SetInputData(imageData);
  volumeMapper3->SetInputData(imageData);
#endif  
  vtkSmartPointer<vtkVolumeProperty> volumeProperty = 
    vtkSmartPointer<vtkVolumeProperty>::New();
  volumeProperty->ShadeOff();
  volumeProperty->SetInterpolationType(VTK_LINEAR_INTERPOLATION);


  vtkSmartPointer<vtkVolumeProperty> volumeProperty2 = 
    vtkSmartPointer<vtkVolumeProperty>::New();
  volumeProperty2->ShadeOff();
  volumeProperty2->SetInterpolationType(VTK_LINEAR_INTERPOLATION);


  vtkSmartPointer<vtkVolumeProperty> volumeProperty3 = 
    vtkSmartPointer<vtkVolumeProperty>::New();
  volumeProperty3->ShadeOff();
  volumeProperty3->SetInterpolationType(VTK_LINEAR_INTERPOLATION);
 
  vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity1 = 
    vtkSmartPointer<vtkPiecewiseFunction>::New();

    compositeOpacity1->AddPoint(0.0,0.0);
    compositeOpacity1->AddPoint(90.0,0.1);
    compositeOpacity1->AddPoint(90.1,0.0);
    compositeOpacity1->AddPoint(1000.0,1.0);


  vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity2 = 
    vtkSmartPointer<vtkPiecewiseFunction>::New();

    compositeOpacity2->AddPoint(255.0,0.0);
    compositeOpacity2->AddPoint(1000.0,1.0);


  vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity3 = 
    vtkSmartPointer<vtkPiecewiseFunction>::New();

    compositeOpacity3->AddPoint(0.0,0.0);
    compositeOpacity3->AddPoint(90.0,0.1);
    compositeOpacity3->AddPoint(90.1,0.0);
    compositeOpacity3->AddPoint(1000.0,0.0);

  volumeProperty->SetScalarOpacity(compositeOpacity1); // composite first.
  volumeProperty2->SetScalarOpacity(compositeOpacity2); // composite first.
  volumeProperty3->SetScalarOpacity(compositeOpacity3); // composite first.
 
  vtkSmartPointer<vtkColorTransferFunction> color = 
    vtkSmartPointer<vtkColorTransferFunction>::New();
  color->AddRGBPoint(0.0  ,0.6,0.8,0.8);
  color->AddRGBPoint(255.0  ,1.0,0.0,0.0);
  color->AddRGBPoint(1000.0,1.0,1.0,1.0);
  volumeProperty->SetColor(color);
  volumeProperty2->SetColor(color);
  volumeProperty3->SetColor(color);
 
  vtkSmartPointer<vtkVolume> volume = 
    vtkSmartPointer<vtkVolume>::New();
  vtkSmartPointer<vtkVolume> volume2 = 
    vtkSmartPointer<vtkVolume>::New();
  vtkSmartPointer<vtkVolume> volume3 = 
    vtkSmartPointer<vtkVolume>::New();


  volume->SetMapper(volumeMapper);
  volume->SetProperty(volumeProperty);
  volume2->SetMapper(volumeMapper2);
  volume2->SetProperty(volumeProperty2);
  volume3->SetMapper(volumeMapper3);
  volume3->SetProperty(volumeProperty3);


  ren1->AddViewProp(volume);
  ren1->ResetCamera();
  ren2->AddViewProp(volume2);
  ren2->ResetCamera();
  ren3->AddViewProp(volume3);
  ren3->ResetCamera();


 
  // Render composite. In default mode. For coverage.
  renWin->Render();
  renWin2->Render();
  renWin3->Render();
 
  // 3D texture mode. For coverage.
//#if !defined(VTK_LEGACY_REMOVE)
//  volumeMapper->SetRequestedRenderModeToRayCastAndTexture();
//#endif // VTK_LEGACY_REMOVE
  renWin->Render();
  renWin2->Render();
  renWin3->Render();
 
  // Software mode, for coverage. It also makes sure we will get the same
  // regression image on all platforms.
  volumeMapper->SetRequestedRenderModeToRayCast();
  volumeMapper2->SetRequestedRenderModeToRayCast();
  volumeMapper3->SetRequestedRenderModeToRayCast();
  renWin->Render();
  renWin2->Render();
  renWin3->Render();
 
  iren->Start();
  iren2->Start();
  iren3->Start();
 
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