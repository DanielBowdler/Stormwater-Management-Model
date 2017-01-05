//-----------------------------------------------------------------------------
//   toolkitAPI.c
//
//   Project: EPA SWMM5
//   Version: 5.1
//   Date:    08/30/2016
//   Author:  B. McDonnell (EmNet LLC)
//
//   Exportable Functions for Project Definition API.
//
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "headers.h"

#include "swmm5.h"                     // declaration of exportable functions
#include "toolkitAPI.h"
#include "hash.h"


//-----------------------------------------------------------------------------
//  Extended API Functions
//-----------------------------------------------------------------------------

int DLLEXPORT swmm_getSimulationDateTime(int timetype, char *dtimestr)
//
// Input: 	timetype = time type to return
// Output: 	DateTime String 
// Purpose: Get the simulation start, end and report date times
{
	// Check if Open
	if(swmm_IsOpenFlag() == FALSE) return(902);
	
	char     theDate[12];
    char     theTime[9];
	char     _DTimeStr[22];

	DateTime _dtime;
	
	switch(timetype)
	{
		//StartDateTime (globals.h)
		case 0: _dtime = StartDateTime; break;
		//EndDateTime (globals.h)
		case 1: _dtime = EndDateTime;  break;
		//ReportStart (globals.h)
		case 2: _dtime = ReportStart;  break;
		default: return(901);
	}
	datetime_dateToStr(_dtime, theDate);
	datetime_timeToStr(_dtime, theTime);

	strcpy(_DTimeStr, theDate);
	strcat(_DTimeStr, " ");
	strcat(_DTimeStr, theTime);
	
	strcpy(dtimestr, _DTimeStr);
	
	return (0);
}

int DLLEXPORT  swmm_getSimulationUnit(int type, int *value)
//
// Input: 	type = simulation unit type
// Output: 	enum representation of units
// Returns: error code
// Purpose: get simulation unit types
{
	// Check if Open
	if(swmm_IsOpenFlag() == FALSE) return(902);
	
	// Output unit
	switch(type)
	{
		// System Unit (enum.h UnitsType)
		case 0:  *value = UnitSystem; break;
		// Flow Unit (enum.h FlowUnitsType)
		case 1:  *value = FlowUnits; break;
		// Concentration Unit
		//case 2:  *value = UnitSystem; break;
		// Type not available
		default: return(901); 
	}
	
	return (0);
}

int DLLEXPORT  swmm_getSimulationAnalysisSetting(int type, int *value)
//
// Input: 	type = analysis type
// Output: 	setting True or False
// Returns: error code
// Purpose: get simulation analysis setting
{
	// Check if Open
	if(swmm_IsOpenFlag() == FALSE) return(902);
	
	// Output  setting
	switch(type)
	{
		// No ponding at nodes (True or False)
		case 0:  *value = AllowPonding; break;
		// Do flow routing in steady state periods  (True or False)
		case 1:  *value = SkipSteadyState; break;
		// Analyze rainfall/runoff  (True or False)
		case 2:  *value = IgnoreRainfall; break;
		// Analyze RDII (True or False)
		case 3:  *value = IgnoreRDII; break;
		// Analyze snowmelt (True or False)
		case 4:  *value = IgnoreSnowmelt; break;
		// Analyze groundwater (True or False)
		case 5:  *value = IgnoreGwater; break;
		// Analyze flow routing (True or False)
		case 6:  *value = IgnoreRouting; break;
		// Analyze water quality (True or False)
		case 7:  *value = IgnoreQuality; break;	
		// Type not available
		default: return(901); 
	}
	
	return (0);	
}

int DLLEXPORT  swmm_getSimulationParam(int type, double *value)
//
// Input: 	type = analysis type
// Output: 	Simulation Parameter
// Returns: error code
// Purpose: Get simulation analysis parameter
{
	// Check if Open
	if(swmm_IsOpenFlag() == FALSE) return(902);

	// Output  setting
	switch(type)
	{
		// Routing time step (sec)
		case 0: *value = RouteStep; break;
		// Minimum variable time step (sec)
		case 1: *value = MinRouteStep; break;
		// Time step for lengthening (sec)
		case 2: *value = LengtheningStep; break;
		// Antecedent dry days 
		case 3: *value = StartDryDays; break;
		// Courant time step factor
		case 4: *value = CourantFactor; break;
		// Minimum nodal surface area
		case 5: *value = MinSurfArea; break;
		// Minimum conduit slope
		case 6: *value = MinSlope; break;
		// Runoff continuity error
		case 7: *value = RunoffError; break;
		// Groundwater continuity error	
		case 8: *value = GwaterError; break;
		// Flow routing error
		case 9: *value = FlowError; break;
		// Quality routing error
		case 10: *value = QualError; break;
		// DW routing head tolerance (ft)
		case 11: *value = HeadTol; break;
		// Tolerance for steady system flow	
		case 12: *value = SysFlowTol; break;
		// Tolerance for steady nodal inflow
		case 13: *value = LatFlowTol; break;    
		// Type not available
		default: return(901); 
	}
	return (0);
}

int DLLEXPORT  swmm_countObjects(int type, int *count)
//
// Input: 	type = object type (Based on ObjectType enum)
//			count = pointer to integer
// Output: 	returns API Error
// Purpose: uses Object Count table to find number of elements of an object
{	
	if(type >= MAX_OBJ_TYPES)return 901;
	*count = Nobjects[type];
	return (0);
}

int DLLEXPORT swmm_getObjectId(int type, int index, char *id)
//
// Input: 	type = object type (Based on ObjectType enum)
//			index = Index of desired ID
//			id = pointer to id pass by reference
// Output: 	returns API Error
// Purpose: Gets ID for any object
{
	//Provide Empty Character Array 
	strcpy(id,"");
	
	// Check if Open
	if(swmm_IsOpenFlag() == FALSE) return(902);
	// Check if object index is within bounds
	if (index < 0 || index >= Nobjects[type]) return(901);
	switch (type)
	{
		case GAGE:
			strcpy(id,Gage[index].ID); break;
		case SUBCATCH:
			strcpy(id,Subcatch[index].ID); break;
		case NODE:
			strcpy(id,Node[index].ID); break;
		case LINK:
			strcpy(id,Link[index].ID); break;
		case POLLUT:
			strcpy(id,Pollut[index].ID); break;
		case LANDUSE:
			strcpy(id,Landuse[index].ID); break;
		case TIMEPATTERN:
			strcpy(id,Pattern[index].ID); break;
		//case CURVE:
			//strcpy(id,Curve[index].ID); break;
		//case TSERIES:
			//strcpy(id,Tseries[index].ID); break;
		//case CONTROL:
			//strcpy(id,Rules[index].ID); break;
		case TRANSECT:
			strcpy(id,Transect[index].ID); break;
		case AQUIFER:
			strcpy(id,Aquifer[index].ID); break;
		case UNITHYD:
			strcpy(id,UnitHyd[index].ID); break;
		case SNOWMELT:
			strcpy(id,Snowmelt[index].ID); break;
		//case SHAPE:
		//strcpy(id,Shape[index].ID); break;
		//case LID:
		//	strcpy(id,LidProcs[index].ID); break;
		// Type not available
		default: return(901);
   }
   return(0);
}

int DLLEXPORT swmm_getNodeType(int index, int *Ntype)
//
// Input: 	index = Index of desired ID
//			Ntype = Node type (Based on enum NodeType)
// Output: 	returns API Error
// Purpose: Gets Node Type
{
	// Check if Open
	if(swmm_IsOpenFlag() == FALSE) return(902);
	// Check if object index is within bounds
	if (index < 0 || index >= Nobjects[NODE]) return(901);
	*Ntype = Node[index].type;
	return(0);
}

int DLLEXPORT swmm_getLinkType(int index, int *Ltype)
//
// Input: 	index = Index of desired ID
//			Ltype = Link type (Based on enum NodeType)
// Output: 	returns API Error
// Purpose: Gets Link Type
{
	// Check if Open
	if(swmm_IsOpenFlag() == FALSE) return(902);
	// Check if object index is within bounds
	if (index < 0 || index >= Nobjects[LINK]) return(901);
	*Ltype = Link[index].type;
	return(0);
}

int DLLEXPORT swmm_getLinkConnections(int index, int *Node1, int *Node2)
//
// Input: 	type = object type (Based on ObjectType enum)
//			index = Index of desired ID
// Output: 	Node1 and Node2
// Purpose: Gets link Connection ID Indeces
{
	// Check if Open
	if(swmm_IsOpenFlag() == FALSE) return(902);
	// Check if object index is within bounds
	if (index < 0 || index >= Nobjects[LINK]) return(901);
	*Node1 = Link[index].node1;
	*Node2 = Link[index].node2;
	return(0);
}

int DLLEXPORT swmm_getNodeParam(int index, int Param, double *value)
//
// Input: 	index = Index of desired ID
//			param = Parameter desired (Perhaps define enum )
//			id = pointer to id pass by reference
// Output: 	returns API Error
// Purpose: Gets Node Parameter
{
	// Check if Open
	if(swmm_IsOpenFlag() == FALSE) return(902);
	// Check if object index is within bounds
	if (index < 0 || index >= Nobjects[NODE]) return(901);
	
	switch(Param)
	{
		// invertElev
		case 0: *value = Node[index].invertElev * UCF(LENGTH); break;
		// fullDepth
		case 1: *value = Node[index].fullDepth * UCF(LENGTH); break;
		// surDepth
		case 2: *value = Node[index].surDepth * UCF(LENGTH); break;	
		// pondedArea
		case 3: *value = Node[index].pondedArea * UCF(LENGTH) * UCF(LENGTH); break;
		// initDepth
		case 4: *value = Node[index].initDepth * UCF(LENGTH); break;
		// Type not available
		default: return(901);
	}
	return(0);
}


int DLLEXPORT swmm_getLinkParam(int index, int Param, double *value)
//
// Input: 	index = Index of desired ID
//			param = Parameter desired (Perhaps define enum )
//			id = pointer to id pass by reference
// Output: 	returns API Error
// Purpose: Gets Link Parameter
{
	// Check if Open
	if(swmm_IsOpenFlag() == FALSE) return(902);
	// Check if object index is within bounds
	if (index < 0 || index >= Nobjects[LINK]) return(901);
	
	switch(Param)
	{
		// offset1
		case 0: *value = Link[index].offset1 * UCF(LENGTH); break;
		// offset2
		case 1: *value = Link[index].offset2 * UCF(LENGTH); break;
		// q0
		case 2: *value = Link[index].q0  * UCF(FLOW); break;
		// qLimit
		case 3: *value = Link[index].qLimit * UCF(FLOW); break;
		// cLossInlet
		case 4: *value = Link[index].cLossInlet; break;
		// cLossOutlet
		case 5: *value = Link[index].cLossOutlet; break;
		// cLossAvg
		case 6: *value = Link[index].cLossAvg; break;
		// seepRate
		//case 7: *value = Link[index].seepRate * UCF(FLOW); break;
		// Type not available
		default: return(901);
	}
	return(0);
}


int DLLEXPORT swmm_getLinkDirection(int index, signed char *value)
//
// Input: 	index = Index of desired ID
// Output: 	Link Direction (Only changes is slope < 0)
// Purpose: Gets Link Direction
{
	// Check if Open
	if(swmm_IsOpenFlag() == FALSE) return(902);
		// Check if object index is within bounds
	if (index < 0 || index >= Nobjects[LINK]) return(901);
	*value = Link[index].direction;
	return(0);
}



int DLLEXPORT swmm_getSubcatchParam(int index, int Param, double *value)
//
// Input: 	index = Index of desired ID
//			param = Parameter desired (Perhaps define enum )
//			id = pointer to id pass by reference
// Output: 	returns API Error
// Purpose: Gets Subcatchment Parameter
{
	// Check if Open
	if(swmm_IsOpenFlag() == FALSE) return(902);
		// Check if object index is within bounds
	if (index < 0 || index >= Nobjects[SUBCATCH]) return(901);
	
	switch(Param)
	{
		// width
		case 0: *value = Subcatch[index].width * UCF(LENGTH); break;
		// area
		case 1: *value = Subcatch[index].area * UCF(LANDAREA); break;
		// fracImperv
		case 2: *value = Subcatch[index].fracImperv; break;	
		// slope
		case 3: *value = Subcatch[index].slope; break;	
		// curbLength
		case 4: *value = Subcatch[index].curbLength * UCF(LENGTH); break;
		// initBuildup
		//case 5: *value = Subcatch[index].initBuildup; break;
		// Type not available
		default: return(901);
	}
	return(0);
}

int DLLEXPORT swmm_getSubcatchOutConnection(int index, int *type, int *Index )
//
// Input: 	type = object type (Based on ObjectType enum) (Subcatchments can load to Node or another Subcatchment)
//			index = Index of desired ID
// Output: 	Node1 and Node2
// Purpose: Gets Subcatchment Connection ID Indeces for either Node or Subcatchment
{
	// Check if Open
	if(swmm_IsOpenFlag() == FALSE) return(902);
		// Check if object index is within bounds
	if (index < 0 || index >= Nobjects[SUBCATCH]) return(901);
	
	if (Subcatch[index].outNode == -1 && Subcatch[index].outSubcatch == -1)
	{
		*Index = index; // Case of self Loading subcatchment
		*type = SUBCATCH;
	}
	if (Subcatch[index].outNode >= 0)
	{
		*Index = Subcatch[index].outNode;
		*type = NODE;
	}
	if (Subcatch[index].outSubcatch >= 0)
	{
		*Index = Subcatch[index].outSubcatch;
		*type = SUBCATCH;
	}
	
	return(0);
} 

//-------------------------------
// Active Simulation Results API
//-------------------------------

int DLLEXPORT swmm_getCurrentDateTimeStr(char *dtimestr)
//
// Output: 	DateTime String 
// Purpose: Get the current simulation time
{
	// Check if Simulation is Running
	if(swmm_IsStartedFlag() == FALSE) return(903);
	
	//Provide Empty Character Array 
	
    char     theDate[12];
    char     theTime[9];
	char     _DTimeStr[22];
	
	DateTime currentTime;
	// Fetch Current Time
	currentTime = getDateTime(NewRoutingTime);
	
	// Convert To Char
	datetime_dateToStr(currentTime, theDate);
	datetime_timeToStr(currentTime, theTime);
	
	strcpy(_DTimeStr, theDate);
	strcat(_DTimeStr, " ");
	strcat(_DTimeStr, theTime);
	
	strcpy(dtimestr, _DTimeStr);
	return(0);
}


int DLLEXPORT swmm_getNodeResult(int index, int type, double *result)
//
// Input: 	index = Index of desired ID	
//			type = Result Type
// Output: 	result = result data desired (byref)
// Return: 	
// Purpose: Gets Node Simulated Value
{
	// Check if Simulation is Running
	if(swmm_IsStartedFlag() == FALSE) return(903);
		// Check if object index is within bounds
	if (index < 0 || index >= Nobjects[NODE]) return(901);
	
	switch (type)
	{
		// Total Inflow
		case 0: *result = Node[index].inflow * UCF(FLOW); break; 
		// Total Outflow
		case 1: *result = Node[index].outflow * UCF(FLOW); break; 
		// Losses (evap + exfiltration loss)
		case 2: *result = Node[index].losses * UCF(FLOW); break; 
		// Current Volume
		case 3: *result = Node[index].newVolume * UCF(VOLUME); break; 
		// overflow
		case 4: *result = Node[index].overflow * UCF(FLOW); break; 
		// Current water depth
		case 5: *result = Node[index].newDepth * UCF(LENGTH); break; 
		// Current water head
		case 6: *result = (Node[index].newDepth + Node[index].invertElev) * UCF(LENGTH); break;
		// Current Lateral Inflow
		case 7: *result = Node[index].newLatFlow * UCF(FLOW); break;
		// Type not available
		default: return(901);
	}
	return(0);
}

int DLLEXPORT swmm_getLinkResult(int index, int type, double *result)
//
// Input: 	index = Index of desired ID	
//			type = Result Type
// Output: 	result = result data desired (byref)
// Return: 	
// Purpose: Gets Node Simulated Value
{
	// Check if Simulation is Running
	if(swmm_IsStartedFlag() == FALSE) return(903);
		// Check if object index is within bounds
	if (index < 0 || index >= Nobjects[LINK]) return(901);
	
	switch (type)
	{
		// Current Flow
		case 0: *result = Link[index].newFlow * UCF(FLOW) ; break; 
		// Current Depth
		case 1: *result = Link[index].newDepth * UCF(LENGTH); break; 
		// Current Volume
		case 2: *result = Link[index].newVolume * UCF(VOLUME); break; 
		// Upstream Surface Area
		case 3: *result = Link[index].surfArea1 * UCF(LENGTH) * UCF(LENGTH); break; 
		// Downstream Surface Area
		case 4: *result = Link[index].surfArea2 * UCF(LENGTH) * UCF(LENGTH); break; 
		// Current Setting
		case 5: *result = Link[index].setting; break; 
		// Target Setting
		case 6: *result = Link[index].targetSetting; break;
		// Froude number
		case 7: *result = Link[index].froude; break;
		// Type not available
		default: return(901);
	}
	return(0);
}

int DLLEXPORT swmm_getSubcatchResult(int index, int type, double *result)
//
// Input: 	index = Index of desired ID	
//			type = Result Type
// Output: 	result = result data desired (byref)
// Return: 	
// Purpose: Gets Node Simulated Value
{
	// Check if Simulation is Running
	if(swmm_IsStartedFlag() == FALSE) return(903);
	// Check if object index is within bounds	
	if (index < 0 || index >= Nobjects[SUBCATCH]) return(901);
	
	switch (type)
	{
		// Current Rainfall
		case 0: *result = Subcatch[index].rainfall * UCF(RAINFALL); break; 
		// Current Evaporation Loss
		case 1: *result = Subcatch[index].evapLoss * UCF(EVAPRATE); break; 
		// Current Infiltration Loss
		case 2: *result = Subcatch[index].infilLoss * UCF(RAINFALL); break; 
		// Subcatchment Runon
		case 3: *result = Subcatch[index].runon * UCF(FLOW); break; 
		// Current Runoff
		case 4: *result = Subcatch[index].newRunoff * UCF(FLOW); break; 
		// Current Snow Depth
		case 5: *result = Subcatch[index].newSnowDepth * UCF(RAINDEPTH); break;
		// Type not available
		default: return(901);
	}
	return(0);
}





//-------------------------------
// Setters API
//-------------------------------

int DLLEXPORT swmm_setLinkSetting(int index, double targetSetting)
//
// Input: 	index = Index of desired ID
//			value = New Target Setting		
// Output: 	returns API Error
// Purpose: Sets Link open fraction (Weir, Orifice, Pump, and Outlet)
{
	// Check if Simulation is Running
	if(swmm_IsStartedFlag() == FALSE) return(903);
	// Check if object index is within bounds	
	if (index < 0 || index >= Nobjects[LINK]) return(901);
	
	int l_type, errcode;
	
	// Get Link Type
	// errcode = swmm_getLinkType(index, &l_type);
	// WEIR, ORIFICES, PUMPS can have any value between [0,1]
	// CONDUIT can be only 0 or 1 * BEM 11/4/2016 investigate this...	
	
	Link[index].targetSetting  = targetSetting; 
	// Use internal function to apply the new setting
	link_setSetting(index, 0.0);
	
	// Add control action to RPT file if desired flagged
	if (RptFlags.controls)
	{
		DateTime currentTime;
		currentTime = getDateTime(NewRoutingTime);
		char _rule_[11] = "ToolkitAPI";
		report_writeControlAction(currentTime, Link[index].ID, targetSetting, _rule_);
	}
	return(0);
}


int DLLEXPORT swmm_setNodeInflow(int index, double flowrate)
//
// Input: 	index = Index of desired ID
//			value = New Inflow Rate 		
// Output: 	returns API Error
// Purpose: Sets new node inflow rate and holds until set again
{
	// Check if Simulation is Running
	if(swmm_IsStartedFlag() == FALSE) return(903);
	// Check if object index is within bounds
	if (index < 0 || index >= Nobjects[NODE]) return(901);
	
	// Check to see if node has an assigned inflow object
	TExtInflow* inflow;
	inflow = Node[index].extInflow;
	if (!inflow)
	{
		// add inflow object then assign flow rate
		// nodeID	FLOW	""	FLOW	1.0	1	0   
		char *line[7];
		int Ntokens = 7;
		// Get Node ID
		
		swmm_getObjectId(NODE, index, line[0]);
		// Insert Node ID into line
		line[1] = "FLOW";
		line[2] = "";
		line[3] = "FLOW";
		line[4] = "1.0";
		line[5] = "1";
		line[6] = "0";
		
		// Add external inflow to linked list
		inflow_readExtInflow(line, Ntokens);
		
		// Get inflow
		inflow = Node[index].extInflow;
	}

	// Assign new flow rate
	inflow -> extIfaceInflow = flowrate;
	
	return(0);
}

