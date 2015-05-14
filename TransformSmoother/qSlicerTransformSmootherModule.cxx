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

// Qt includes
#include <QtPlugin>

// TransformSmoother Logic includes
#include <vtkSlicerTransformSmootherLogic.h>

// TransformSmoother includes
#include "qSlicerTransformSmootherModule.h"
#include "qSlicerTransformSmootherModuleWidget.h"

//-----------------------------------------------------------------------------
Q_EXPORT_PLUGIN2(qSlicerTransformSmootherModule, qSlicerTransformSmootherModule);

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicerTransformSmootherModulePrivate
{
public:
  qSlicerTransformSmootherModulePrivate();
};

//-----------------------------------------------------------------------------
// qSlicerTransformSmootherModulePrivate methods

//-----------------------------------------------------------------------------
qSlicerTransformSmootherModulePrivate::qSlicerTransformSmootherModulePrivate()
{
}

//-----------------------------------------------------------------------------
// qSlicerTransformSmootherModule methods

//-----------------------------------------------------------------------------
qSlicerTransformSmootherModule::qSlicerTransformSmootherModule(QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qSlicerTransformSmootherModulePrivate)
{
}

//-----------------------------------------------------------------------------
qSlicerTransformSmootherModule::~qSlicerTransformSmootherModule()
{
}

//-----------------------------------------------------------------------------
QString qSlicerTransformSmootherModule::helpText() const
{
  return "This is a loadable module that can be bundled in an extension";
}

//-----------------------------------------------------------------------------
QString qSlicerTransformSmootherModule::acknowledgementText() const
{
  return "This work was partially funded by NIH grant NXNNXXNNNNNN-NNXN";
}

//-----------------------------------------------------------------------------
QStringList qSlicerTransformSmootherModule::contributors() const
{
  QStringList moduleContributors;
  moduleContributors << QString("John Doe (AnyWare Corp.)");
  return moduleContributors;
}

//-----------------------------------------------------------------------------
QIcon qSlicerTransformSmootherModule::icon() const
{
  return QIcon(":/Icons/TransformSmoother.png");
}

//-----------------------------------------------------------------------------
QStringList qSlicerTransformSmootherModule::categories() const
{
  return QStringList() << "Examples";
}

//-----------------------------------------------------------------------------
QStringList qSlicerTransformSmootherModule::dependencies() const
{
  return QStringList();
}

//-----------------------------------------------------------------------------
void qSlicerTransformSmootherModule::setup()
{
  this->Superclass::setup();
}

//-----------------------------------------------------------------------------
qSlicerAbstractModuleRepresentation* qSlicerTransformSmootherModule
::createWidgetRepresentation()
{
  return new qSlicerTransformSmootherModuleWidget;
}

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qSlicerTransformSmootherModule::createLogic()
{
  return vtkSlicerTransformSmootherLogic::New();
}
