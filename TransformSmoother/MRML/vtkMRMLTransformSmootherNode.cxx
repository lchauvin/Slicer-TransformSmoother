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

// TransformSmoother MRML includes
#include "vtkMRMLTransformSmootherNode.h"

// Other MRML includes
#include "vtkMRMLLinearTransformNode.h"

// VTK includes
#include <vtkNew.h>
#include <vtkIntArray.h>
#include <vtkCommand.h>

// Other includes
#include <sstream>

// Constants
static const char* INPUT_TRANSFORM_ROLE = "inputTransformNode";
static const char* FILTERED_TRANSFORM_ROLE = "filteredTransformNode";

//-----------------------------------------------------------------------------
vtkMRMLNodeNewMacro(vtkMRMLTransformSmootherNode);

vtkMRMLTransformSmootherNode
::vtkMRMLTransformSmootherNode()
{
  this->HideFromEditorsOff();
  this->SetSaveWithScene( true );

  vtkNew<vtkIntArray> events;
  events->InsertNextValue( vtkCommand::ModifiedEvent );

  this->AddNodeReferenceRole( INPUT_TRANSFORM_ROLE, NULL, events.GetPointer() );
  this->AddNodeReferenceRole( FILTERED_TRANSFORM_ROLE, NULL, events.GetPointer() );

  this->CutOffFrequency = 7.5;
  this->FilterActivated = false;
}

//-----------------------------------------------------------------------------
vtkMRMLTransformSmootherNode
::~vtkMRMLTransformSmootherNode()
{
}

//-----------------------------------------------------------------------------
void vtkMRMLTransformSmootherNode
::WriteXML( ostream& of, int nIndent )
{
  Superclass::WriteXML(of, nIndent);
  vtkIndent indent(nIndent);

  of << indent << " cutoffFrequency=\"" << this->CutOffFrequency << "\"";
  of << indent << " filterActivated=\"" << ( this->FilterActivated ? "true" : "false" ) << "\"";
}

//-----------------------------------------------------------------------------
void vtkMRMLTransformSmootherNode
::ReadXMLAttributes( const char** atts )
{
  Superclass::ReadXMLAttributes(atts);

  // Read all MRML node attributes from two arrays of names and values
  const char* attName;
  const char* attValue;
  
  while (*atts != NULL)
    {
    attName = *(atts++);
    attValue = *(atts++);

    if (!strcmp(attName, "cutoffFrequency"))
      {
      std::stringstream ss;
      ss << attValue;
      double val;
      ss >> val;
      this->CutOffFrequency = val;
      }
    else if(!strcmp(attName, "filterActivated"))
      {
      if (!strcmp(attValue,"true"))
	{
	this->FilterActivated = true;
	}
      else
	{
	this->FilterActivated = false;
	}
      }
    }
}

//-----------------------------------------------------------------------------
void vtkMRMLTransformSmootherNode
::Copy( vtkMRMLNode *anode )
{
  Superclass::Copy( anode );

  vtkMRMLTransformSmootherNode *node = ( vtkMRMLTransformSmootherNode* ) anode;

  this->CutOffFrequency = node->CutOffFrequency;
  this->FilterActivated = node->FilterActivated;

  this->Modified();
}

//-----------------------------------------------------------------------------
void vtkMRMLTransformSmootherNode
::PrintSelf( ostream& os, vtkIndent indent )
{
  vtkMRMLNode::PrintSelf(os, indent);

  os << indent << "InputTransformNodeID: " << this->GetInputTransformNode()->GetID() << std::endl;
  os << indent << "FilteredTransformNodeID: " << this->GetFilteredTransformNode()->GetID() << std::endl;
  os << indent << "CutOff Frequency: " << this->CutOffFrequency << std::endl;
  os << indent << "Filter Activated: " << this->FilterActivated << std::endl;
}

//-----------------------------------------------------------------------------
vtkMRMLLinearTransformNode* vtkMRMLTransformSmootherNode
::GetInputTransformNode()
{
  vtkMRMLLinearTransformNode* inputNode = vtkMRMLLinearTransformNode::SafeDownCast(
    this->GetNodeReference( INPUT_TRANSFORM_ROLE ) );
  return inputNode;
}

//-----------------------------------------------------------------------------
void vtkMRMLTransformSmootherNode
::SetAndObserveInputTransformNodeID( const char* inputNodeId )
{
  // SetAndObserveNodeReferenceID does not handle nicely setting of the same
  // node (it should simply ignore the request, but it adds another observer instead)
  // so check for node equality here.

  const char* currentNodeId = this->GetNodeReferenceID(INPUT_TRANSFORM_ROLE);
  if (inputNodeId != NULL && currentNodeId != NULL)
    {
    if (strcmp(inputNodeId, currentNodeId) == 0)
      {
      // not changed
      return;
      }
    }
  vtkNew<vtkIntArray> events;
  events->InsertNextValue( vtkCommand::ModifiedEvent );
  this->SetAndObserveNodeReferenceID( INPUT_TRANSFORM_ROLE, inputNodeId, events.GetPointer() );
}

//-----------------------------------------------------------------------------
vtkMRMLLinearTransformNode* vtkMRMLTransformSmootherNode
::GetFilteredTransformNode()
{
  vtkMRMLLinearTransformNode* filteredNode = vtkMRMLLinearTransformNode::SafeDownCast(
    this->GetNodeReference( FILTERED_TRANSFORM_ROLE ) );
  return filteredNode;
}

//-----------------------------------------------------------------------------
void vtkMRMLTransformSmootherNode
::SetAndObserveFilteredTransformNodeID( const char* filteredNodeId )
{
  // SetAndObserveNodeReferenceID does not handle nicely setting of the same
  // node (it should simply ignore the request, but it adds another observer instead)
  // so check for node equality here.

  const char* currentNodeId = this->GetNodeReferenceID(FILTERED_TRANSFORM_ROLE);
  if (filteredNodeId != NULL && currentNodeId != NULL)
    {
    if (strcmp(filteredNodeId, currentNodeId) == 0)
      {
      // not changed
      return;
      }
    }
  vtkNew<vtkIntArray> events;
  events->InsertNextValue( vtkCommand::ModifiedEvent );
  this->SetAndObserveNodeReferenceID( FILTERED_TRANSFORM_ROLE, filteredNodeId, events.GetPointer() );
}

//-----------------------------------------------------------------------------
void vtkMRMLTransformSmootherNode
::ProcessMRMLEvents( vtkObject *caller, unsigned long event, void* callData )
{
  Superclass::ProcessMRMLEvents( caller, event, callData );
}
