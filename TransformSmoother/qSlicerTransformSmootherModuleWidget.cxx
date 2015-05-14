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
#include <QDebug>
#include <QMessageBox>
#include <QTimer>

// SlicerQt includes
#include "qSlicerTransformSmootherModuleWidget.h"
#include "ui_qSlicerTransformSmootherModuleWidget.h"

#include "vtkSlicerTransformSmootherLogic.h"

#include "vtkMRMLNode.h"
#include "vtkMRMLScene.h"
#include "vtkMRMLTransformSmootherNode.h"

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicerTransformSmootherModuleWidgetPrivate: public Ui_qSlicerTransformSmootherModuleWidget
{
  Q_DECLARE_PUBLIC( qSlicerTransformSmootherModuleWidget );

protected:
  qSlicerTransformSmootherModuleWidget* const q_ptr;

public:
  qSlicerTransformSmootherModuleWidgetPrivate( qSlicerTransformSmootherModuleWidget& object );
  ~qSlicerTransformSmootherModuleWidgetPrivate();
  vtkSlicerTransformSmootherLogic* logic() const;

  QTimer* UpdatingTransformTimer;
};

//-----------------------------------------------------------------------------
// qSlicerTransformSmootherModuleWidgetPrivate methods

//-----------------------------------------------------------------------------
qSlicerTransformSmootherModuleWidgetPrivate::qSlicerTransformSmootherModuleWidgetPrivate( qSlicerTransformSmootherModuleWidget& object ) : q_ptr( &object )
{
  this->UpdatingTransformTimer = new QTimer();
}

//-----------------------------------------------------------------------------
qSlicerTransformSmootherModuleWidgetPrivate::~qSlicerTransformSmootherModuleWidgetPrivate()
{
  if (this->UpdatingTransformTimer)
    {
    this->UpdatingTransformTimer->stop();
    this->UpdatingTransformTimer->deleteLater();
    }
}

//-----------------------------------------------------------------------------
vtkSlicerTransformSmootherLogic* qSlicerTransformSmootherModuleWidgetPrivate::logic() const
{
  Q_Q( const qSlicerTransformSmootherModuleWidget );
  return vtkSlicerTransformSmootherLogic::SafeDownCast( q->logic() );
}

//-----------------------------------------------------------------------------
// qSlicerTransformSmootherModuleWidget methods

//-----------------------------------------------------------------------------
qSlicerTransformSmootherModuleWidget::qSlicerTransformSmootherModuleWidget(QWidget* _parent)
  : Superclass( _parent )
    , d_ptr( new qSlicerTransformSmootherModuleWidgetPrivate( *this ) )
{
}

//-----------------------------------------------------------------------------
qSlicerTransformSmootherModuleWidget::~qSlicerTransformSmootherModuleWidget()
{
}

//-----------------------------------------------------------------------------
void qSlicerTransformSmootherModuleWidget::setup()
{
  Q_D(qSlicerTransformSmootherModuleWidget);
  d->setupUi(this);
  this->Superclass::setup();

  this->setMRMLScene( d->logic()->GetMRMLScene() );

  connect(d->ModuleNodeComboBox, SIGNAL(currentNodeChanged(vtkMRMLNode*)),
	  this, SLOT(onModuleNodeChanged()));

  connect(d->UpdatingTransformTimer, SIGNAL(timeout()),
	  this, SLOT(onUpdatingTransformTimeout()));

  connect(d->ActivateFilterCheckBox, SIGNAL(toggled(bool)),
	  this, SLOT(onFilterToggled(bool)));

  connect(d->InputTransformComboBox, SIGNAL(currentNodeChanged(vtkMRMLNode*)),
	  this, SLOT(onInputNodeChanged()));

  connect(d->OutputTransformComboBox, SIGNAL(currentNodeChanged(vtkMRMLNode*)),
	  this, SLOT(onOutputNodeChanged()));

  connect(d->CutOffFrequencySlider, SIGNAL(valueChanged(double)),
	  this, SLOT(onCutOffFrequencyChanged(double)));

  this->UpdateFromMRMLNode();
}

//-----------------------------------------------------------------------------
void qSlicerTransformSmootherModuleWidget::enter()
{
  Q_D(qSlicerTransformSmootherModuleWidget);

  if ( this->mrmlScene() == NULL )
    {
    qCritical() << "Invalid scene!";
    return;
    }

  // Create a module MRML node if there is none in the scene.
  vtkMRMLNode* node = this->mrmlScene()->GetNthNodeByClass( 0, "vtkMRMLTransformSmootherNode" );
  if ( node == NULL )
    {
    vtkSmartPointer< vtkMRMLTransformSmootherNode > newNode =
      vtkSmartPointer< vtkMRMLTransformSmootherNode >::New();
    this->mrmlScene()->AddNode( newNode );
    }

  node = this->mrmlScene()->GetNthNodeByClass( 0, "vtkMRMLTransformSmootherNode" );
  if ( node == NULL )
    {
    qCritical( "Failed to create module node" );
    return;
    }

  // For convenience, select a default node
  if ( d->ModuleNodeComboBox->currentNode() == NULL )
    {
    d->ModuleNodeComboBox->setCurrentNodeID( node->GetID() );
    }

  // Start timer if not already started
  if ( d->UpdatingTransformTimer->isActive() == false )
    {
    d->UpdatingTransformTimer->start( 50 );
    }

  this->Superclass::enter();
}

//-----------------------------------------------------------------------------
void qSlicerTransformSmootherModuleWidget::setMRMLScene(vtkMRMLScene* scene)
{
  Q_D(qSlicerTransformSmootherModuleWidget);

  this->Superclass::setMRMLScene( scene );
}

//-----------------------------------------------------------------------------
void qSlicerTransformSmootherModuleWidget::onSceneImportedEvent()
{
  this->enter();
}

//-----------------------------------------------------------------------------
void qSlicerTransformSmootherModuleWidget::onModuleNodeChanged()
{
  Q_D(qSlicerTransformSmootherModuleWidget);

  this->UpdateFromMRMLNode();
}

//-----------------------------------------------------------------------------
void qSlicerTransformSmootherModuleWidget::onUpdatingTransformTimeout()
{
  Q_D(qSlicerTransformSmootherModuleWidget);

  if ( this->mrmlScene() == NULL )
    {
    return;
    }

  vtkCollection* filteringNodes = this->mrmlScene()->GetNodesByClass( "vtkMRMLTransformSmootherNode" );
  for (int i = 0; i < filteringNodes->GetNumberOfItems(); ++i)
    {
    vtkMRMLTransformSmootherNode* tsNode =
      vtkMRMLTransformSmootherNode::SafeDownCast( filteringNodes->GetItemAsObject(i) );
    if ( tsNode == NULL )
      {
      return;
      }

    d->logic()->Filter( tsNode );
    }

  filteringNodes->Delete();
}

//-----------------------------------------------------------------------------
void qSlicerTransformSmootherModuleWidget::onFilterToggled(bool filterActivated)
{
  Q_D(qSlicerTransformSmootherModuleWidget);

  vtkMRMLTransformSmootherNode* tsNode =
    vtkMRMLTransformSmootherNode::SafeDownCast( d->ModuleNodeComboBox->currentNode() );
  if ( tsNode == NULL )
    {
    qCritical( "Filter has been toggled with no module node selection" );
    return;
    }

  tsNode->SetFilterActivated( filterActivated );
}

//-----------------------------------------------------------------------------
void qSlicerTransformSmootherModuleWidget::onInputNodeChanged()
{
  Q_D(qSlicerTransformSmootherModuleWidget);

  vtkMRMLTransformSmootherNode* tsNode =
    vtkMRMLTransformSmootherNode::SafeDownCast( d->ModuleNodeComboBox->currentNode() );
  if ( tsNode == NULL )
    {
    qCritical( "Input node has changed with no module node selection" );
    return;
    }

  vtkMRMLLinearTransformNode* inputNode =
    vtkMRMLLinearTransformNode::SafeDownCast( d->InputTransformComboBox->currentNode() );
  tsNode->SetAndObserveInputTransformNodeID( (inputNode!=NULL) ? inputNode->GetID() : NULL );
}

//-----------------------------------------------------------------------------
void qSlicerTransformSmootherModuleWidget::onOutputNodeChanged()
{
  Q_D(qSlicerTransformSmootherModuleWidget);

  vtkMRMLTransformSmootherNode* tsNode =
    vtkMRMLTransformSmootherNode::SafeDownCast( d->ModuleNodeComboBox->currentNode() );
  if ( tsNode == NULL )
    {
    qCritical( "Output node has changed with no module node selection" );
    return;
    }

  vtkMRMLLinearTransformNode* outputNode =
    vtkMRMLLinearTransformNode::SafeDownCast( d->OutputTransformComboBox->currentNode() );

  // Sanity check: If the output transform is already selected as output in another filter
  // with a different input, the output results will be the average of both inputs.
  // User should be warned in this case.

  bool differentFiltersUsingSameOutput = false;
  bool useSameOutputForMultipleFilters = false;
  vtkCollection* filteringNodes = this->mrmlScene()->GetNodesByClass( "vtkMRMLTransformSmootherNode" );
  for (int i = 0; i < filteringNodes->GetNumberOfItems(); ++i)
    {
    vtkMRMLTransformSmootherNode* tmpNode =
      vtkMRMLTransformSmootherNode::SafeDownCast( filteringNodes->GetItemAsObject(i) );
    if ( tmpNode == NULL )
      {
      continue;
      }

    vtkMRMLLinearTransformNode* filterOutputNode = tmpNode->GetFilteredTransformNode();
    if ( ( filterOutputNode != NULL ) && ( filterOutputNode == outputNode ) )
      {
      // Same output node has been found in another filter in the scene
      differentFiltersUsingSameOutput = true;

      QMessageBox warningMsg;
      std::stringstream ss;
      ss << "Another filter (ID: " << tmpNode->GetID() << ") has been found in the scene with the same output transform. Having two or more filters with the same output will result in the average of all inputs of these filters. Are you sure you want to select this transform as output for this filter ?";
      warningMsg.setText( ss.str().c_str() );
      warningMsg.setStandardButtons( QMessageBox::Yes | QMessageBox::No );
      warningMsg.setDefaultButton( QMessageBox::No );
      int ret = warningMsg.exec();

      useSameOutputForMultipleFilters = ( ret == QMessageBox::Yes );

      break;
      }
    }
  filteringNodes->Delete();

  if ( ( differentFiltersUsingSameOutput == true ) && ( useSameOutputForMultipleFilters == false ) )
    {
    tsNode->SetAndObserveFilteredTransformNodeID( "" );
    this->UpdateFromMRMLNode();
    return;
    }

  tsNode->SetAndObserveFilteredTransformNodeID( (outputNode!=NULL) ? outputNode->GetID() : NULL);
}

//-----------------------------------------------------------------------------
void qSlicerTransformSmootherModuleWidget::onCutOffFrequencyChanged(double cutoffFrequency)
{
  Q_D(qSlicerTransformSmootherModuleWidget);

  vtkMRMLTransformSmootherNode* tsNode =
    vtkMRMLTransformSmootherNode::SafeDownCast( d->ModuleNodeComboBox->currentNode() );
  if ( tsNode == NULL )
    {
    qCritical( "Cutoff Frequency changed with no module node selection" );
    return;
    }

  tsNode->SetCutOffFrequency( cutoffFrequency );
}

//-----------------------------------------------------------------------------
void qSlicerTransformSmootherModuleWidget::UpdateFromMRMLNode()
{
  Q_D(qSlicerTransformSmootherModuleWidget);

  vtkMRMLTransformSmootherNode* tsNode =
    vtkMRMLTransformSmootherNode::SafeDownCast( d->ModuleNodeComboBox->currentNode() );
  if ( tsNode == NULL )
    {
    d->InputTransformComboBox->setCurrentNodeID( "" );
    d->OutputTransformComboBox->setCurrentNodeID( "" );
    d->ActivateFilterCheckBox->setChecked( false );
    return;
    }

  d->InputTransformComboBox->setCurrentNode( tsNode->GetInputTransformNode() );
  d->OutputTransformComboBox->setCurrentNode( tsNode->GetFilteredTransformNode() );

  d->ActivateFilterCheckBox->setChecked( tsNode->GetFilterActivated() );
  d->CutOffFrequencySlider->setValue( tsNode->GetCutOffFrequency() );
}
