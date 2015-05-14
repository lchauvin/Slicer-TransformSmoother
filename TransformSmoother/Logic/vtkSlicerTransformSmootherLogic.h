/*==============================================================================

  Program: 3D Slicer

  Portions (c) Copyright Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

==============================================================================*/

// .NAME vtkSlicerTransformSmootherLogic - slicer logic class for volumes manipulation
// .SECTION Description
// This class manages the logic associated with reading, saving,
// and changing propertied of the volumes


#ifndef __vtkSlicerTransformSmootherLogic_h
#define __vtkSlicerTransformSmootherLogic_h

// Slicer includes
#include "vtkSlicerModuleLogic.h"

// MRML includes
#include "vtkMRMLLinearTransformNode.h"
#include "vtkMRMLScene.h"
#include "vtkMRMLTransformSmootherNode.h"

// STD includes
#include <cstdlib>

#include "vtkSlicerTransformSmootherModuleLogicExport.h"

class vtkMatrix4x4;

/// \ingroup Slicer_QtModules_ExtensionTemplate
class VTK_SLICER_TRANSFORMSMOOTHER_MODULE_LOGIC_EXPORT vtkSlicerTransformSmootherLogic :
  public vtkSlicerModuleLogic
{
public:

  static vtkSlicerTransformSmootherLogic *New();
  vtkTypeMacro(vtkSlicerTransformSmootherLogic, vtkSlicerModuleLogic);
  void PrintSelf(ostream& os, vtkIndent indent);

  void ProcessMRMLNodesEvents(vtkObject* caller, unsigned long event, void* callData);

  void Filter(vtkMRMLTransformSmootherNode* tsNode);

protected:
  vtkSlicerTransformSmootherLogic();
  virtual ~vtkSlicerTransformSmootherLogic();

  virtual void SetMRMLSceneInternal(vtkMRMLScene* newScene);
  /// Register MRML Node classes to Scene. Gets called automatically when the MRMLScene is attached to this logic class.
  virtual void RegisterNodes();
  virtual void UpdateFromMRMLScene();
  virtual void OnMRMLSceneNodeAdded(vtkMRMLNode* node);
  virtual void OnMRMLSceneNodeRemoved(vtkMRMLNode* node);

  void Slerp(double* result, double t, double* from, double* to, bool adjustSign = true);
  void GetInterpolatedTransform(vtkMatrix4x4* itemAmatrix, vtkMatrix4x4* itemBmatrix,
				double itemAweight, double itemBweight,
				vtkMatrix4x4* interpolatedMatrix);
private:

  vtkSlicerTransformSmootherLogic(const vtkSlicerTransformSmootherLogic&); // Not implemented
  void operator=(const vtkSlicerTransformSmootherLogic&); // Not implemented
};

#endif
