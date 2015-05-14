/*==============================================================================

  Program: 3D Slicer

  Copyright (c) Kitware Inc.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Jean-Christophe Fillion-Robin, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

// FooBar Widgets includes
#include "qSlicerTransformSmootherFooBarWidget.h"
#include "ui_qSlicerTransformSmootherFooBarWidget.h"

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_TransformSmoother
class qSlicerTransformSmootherFooBarWidgetPrivate
  : public Ui_qSlicerTransformSmootherFooBarWidget
{
  Q_DECLARE_PUBLIC(qSlicerTransformSmootherFooBarWidget);
protected:
  qSlicerTransformSmootherFooBarWidget* const q_ptr;

public:
  qSlicerTransformSmootherFooBarWidgetPrivate(
    qSlicerTransformSmootherFooBarWidget& object);
  virtual void setupUi(qSlicerTransformSmootherFooBarWidget*);
};

// --------------------------------------------------------------------------
qSlicerTransformSmootherFooBarWidgetPrivate
::qSlicerTransformSmootherFooBarWidgetPrivate(
  qSlicerTransformSmootherFooBarWidget& object)
  : q_ptr(&object)
{
}

// --------------------------------------------------------------------------
void qSlicerTransformSmootherFooBarWidgetPrivate
::setupUi(qSlicerTransformSmootherFooBarWidget* widget)
{
  this->Ui_qSlicerTransformSmootherFooBarWidget::setupUi(widget);
}

//-----------------------------------------------------------------------------
// qSlicerTransformSmootherFooBarWidget methods

//-----------------------------------------------------------------------------
qSlicerTransformSmootherFooBarWidget
::qSlicerTransformSmootherFooBarWidget(QWidget* parentWidget)
  : Superclass( parentWidget )
  , d_ptr( new qSlicerTransformSmootherFooBarWidgetPrivate(*this) )
{
  Q_D(qSlicerTransformSmootherFooBarWidget);
  d->setupUi(this);
}

//-----------------------------------------------------------------------------
qSlicerTransformSmootherFooBarWidget
::~qSlicerTransformSmootherFooBarWidget()
{
}
