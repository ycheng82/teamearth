#include <vtkVersion.h>
#include <vtkPolyDataReader.h>  
#include "vtkPolyData.h"  
#include "vtkSmartPointer.h"  
#include <vtkMassProperties.h>
#include <vtkTriangleFilter.h>
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
  
  
int main( int argc, char* argv[] ){  
  
    // read in the .vtk file  
    vtkSmartPointer<vtkPolyDataReader> reader = vtkSmartPointer<vtkPolyDataReader>::New();  
    reader->SetFileName(argv[1]);  
    reader->Update();  
  
    vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();  
    polyData = reader->GetOutput();  

    vtkSmartPointer<vtkTriangleFilter> tri = vtkSmartPointer<vtkTriangleFilter>::New();

    tri->SetInputData(polyData);
    tri->Update();

    vtkSmartPointer<vtkPolyData> pd = tri->GetOutput();
  
    // evaluate the volume  
    vtkMassProperties *mass = vtkMassProperties::New();  
  
    mass->SetInputData(pd);  
  
    cout << "---- the volume of this vtk polydata is: " << mass->GetVolume() << endl;  
    cout << "---- the SA of this vtk polydata is: " << mass->GetSurfaceArea() << endl;

    
}  