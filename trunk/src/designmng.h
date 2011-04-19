/**************************************************************************
*   Copyright (C) 2005 by Adriel Mota Ziesemer Jr.                        *
*   amziesemer[at]inf.ufrgs.br                                            *
***************************************************************************/
#ifndef DESIGNMNG_H
#define DESIGNMNG_H

/**
Repository for entire design

@author Adriel Mota Ziesemer Jr.
*/

#include <list>
#include <vector>
#include <string>
#include "clayout.h"
#include "circuit.h"
#include "size.h"
#include "elmoredelay.h"
#include "draw.h"
#include "router.h"
#include "util.h"
#include "placer.h"
#include "spice.h"
#include "rules.h"
#include "autocell.h"
#include "verilog.h"

#include "savelayout.h"
#include "lef.h"
#include "gds.h"

using namespace std;

enum command_name {
	// New - 2
	NEW_DESIGN,NEW_CELL,

	// Load - 6
	LOAD_PROJECT,LOAD_TECHNOLOGY,LOAD_NETLIST,LOAD_LAYOUTS,LOAD_PLACEMENT,LOAD_ROUTING,

	// Save - 7
	SAVE_PROJECT,SAVE_TECHNOLOGY,SAVE_NETLIST,SAVE_LAYOUTS,SAVE_PLACEMENT,SAVE_ROUTING,SAVE_HISTORY,

	// Import - 2
	IMPORT_NETLIST,IMPORT_LEF,

	// Export - 3
	EXPORT_LAYOUT,EXPORT_CELLSIZES,EXPORT_PLACEMENT,

	READ,

	// Place - 7
	PLACE_TERMINALS, PLACE_TERMINALS_OPTIMIZE, PLACE_FPLACE,PLACE_GETWL,PLACE_INSTANCE,PLACE_AUTOFLIP,PLACE_INCREMENTAL,PLACE_CHECK,

	// Route - 6
	ROUTE_ROTDL,ROUTE_PFINDER,ROUTE_OPTIMIZE,ROUTE_COMPACT,ROUTE_TEST,ROUTE_CLEAR,

	// Print - 4
	PRINT_INSTANCE,PRINT_CELL,PRINT_NET,PRINT_INTERFACE,

	// Preferences - 6
	SET_PLACER,SET_ROTDL,SET_VIEWER,SET_LPSOLVE,SET_LOG,SET_VERBOSEMODE,

	// Technology - 8
	SET_TECH_NAME,SET_TECH_MLAYERS,SET_TECH_SOI,SET_TECH_RESOLUTION,SET_TECH_RULE,SET_TECH_CIF,SET_TECH_GDSII,SET_TECH_VALTECH,

	// Circuit - 8
	SET_DESIGNNAME,SET_GRID,SET_HGRID,SET_VGRID,SET_VDDNET,SET_GNDNET,SET_ROWHEIGHT,SET_SUPPLYSIZE,

	// Floorplan - 3
	SET_TOPCELL,SET_AREA,SET_MARGINS,

	CALCPINSPOS,

	// CellGen - 9
	CELLGEN_SETNRTRACKS,CELLGEN_SETPOSTRACKS,CELLGEN_SELECT,CELLGEN_FOLD,CELLGEN_PLACE,CELLGEN_GETARCCOST,CELLGEN_SETARCCOST,CELLGEN_ROUTE,CELLGEN_COMPACT,

	// Size - 2
	SIZE_FO4,SIZE_GP,
	
	//Parameters PrintGP
	SET_TECHNOLOGY,// CHANGE_OPTIMIZE,
    SET_CLOAD, SET_CONSTRAREA, SET_CONSTRDELAY, SET_CONSTRCIN, 
	
	// Elmore - 2
	ELMORE_SAME,ELMORE_FO4,
	
	// Help - 2
	HELP, HELP_PARAM,

	EXEC, EXIT,
	NR_COMMANDS
};

struct commands {
	string name;
	string desc;
};

static commands commands_lst[] = {
	// New - 2
	{"NEW DESIGN <str_Name>", "Cleans current design and start a new one"},
	{"NEW CELL <str_Name>", "Creates a new cell, it will appear in the Cells list box"},

	// Load - 6
	{"LOAD PROJECT <str_Path>", "Opens a project file previously saved from ICPD. Supported extension: .icp (ICPD Project File)"},
	{"LOAD TECHNOLOGY <str_Path>", "Opens the technology file with the rules to set for the design. Supported extension: .rul (Rules File)"},
	{"LOAD NETLIST <str_Path>", "Opens the netlist file with circuit description. Supported extensions: .sp (Spice), .v (Structural Verilog)"},
	{"LOAD LAYOUTS <str_Path>", "Opens the layouts file previously saved from ICPD. Supported extension: .lay (ICPD Layouts Format)"},
	{"LOAD PLACEMENT <str_Path>", "Opens the placement file. Supported extensions: .pl (Book Shelf), .mpp (Mango Parrot)"},
	{"LOAD ROUTING <str_Path>", "Opens the routing file. Supported extension: .rot (Rotdl Routing Format)"},

	// Save - 7
	{"SAVE PROJECT <str_Path>", "Saves the current project and configurations. Supported extension: .icp (ICPD Project File)"},
	{"SAVE TECHNOLOGY <str_Path>", "Saves the current technology rules to file. Supported extension: .rul (Rules File)"},
	{"SAVE NETLIST <str_Path>", "Saves the spice netlist to file. Supported extension: .sp (Spice)"},
	{"SAVE LAYOUTS <str_Path>", "Saves the current layouts to file. Supported extension: .lay (ICPD Layouts Format)"},
	{"SAVE PLACEMENT <str_Path>", "Saves placement to file. Supported extension: .pl (Book Shelf)"},
	{"SAVE ROUTING <str_Path>", "Saves routing to file. Supported extension: .rot (Rotdl Routing Format)"},
	{"SAVE HISTORY <str_Path>", "Saves history log to a script file. Supported extension: .run (Script File)"},

	// Import - 2
	{"IMPORT NETLIST <str_Path>", "Imports netlist file generated by Cadence. Supported extensions: .sp (Spice) and Cadence Netlist Files"},
	{"IMPORT LEF <str_Path>", "Imports LEF file for layouts. Supported extension: .lef (Library Exchange Format)"},

	// Export - 3
	{"EXPORT LAYOUT <str_Layout> <str_Path>", "Exports layout generated by ICPD to open with other applications. Supported extensions: .cif (Calltech Intermediate Format), .gds (Graphic Data System II), .c2c (CIF to Cadence Convertion File)"},
	{"EXPORT CELLSIZES <str_Path>", "Exports cell sizes file. Supported extension: .est (Mango Parrot Cell Estimative)"},
	{"EXPORT PLACEMENT <str_Path>", "Exports placement generated by ICPD to open with Cadence. Supported extension: .place (Cadence Placement File)"},

	{"READ <str_Path>", "Reads script file to run, previously saved from ICPD. Supported extension: .run (Script File)"},

	// Place - 7
	{"PLACE TERMINALS", "Makes placement of interface terminals"},
	{"PLACE TERMINALS_OPTIMIZE", "Optimize the terminals position"},
	{"PLACE FPLACE", ""},
	{"PLACE GETWL", "Checks width and length of placement"},
	{"PLACE INSTANCE <str_Layout> <str_Instance> <int_PosX> <int_PosY> <bool_MX> <bool_MY>", "Makes placement of instance"},
	{"PLACE AUTOFLIP", "Makes cell autoflip"},
	{"PLACE INCREMENTAL", "Makes the incremental placement"},
	{"PLACE CHECK", "Checks cell's placement"},

	// Route - 6
	{"ROUTE ROTDL", "Makes routing using Rotdl"},
	{"ROUTE PFINDER <int_Iterations>", "Makes routing using Pathfinder"},
	{"ROUTE OPTIMIZE", "Optmizes the routing"},
	{"ROUTE COMPACT", "Compacts the routing using LPSolver"},
	{"ROUTE TEST", "Makes a test of routing to compare"},
	{"ROUTE CLEAR", "Clears the current routing"},

	// Print - 4
	{"PRINT INSTANCE <str_Layout> <str_Instance>", "Prints the cell's instance's information on the screen"},
	{"PRINT CELL <str_Cell>", "Prints the cell's information on the screen"},
	{"PRINT NET <str_Net>", "Prints the net's information on the screen"},
	{"PRINT INTERFACE <str_Interface>", "Prints the interface's information on the screen"},

	// Preferences - 6
	{"SET PLACER <str_Path>", "Sets the Mobralplace path"},
	{"SET ROTDL <str_Path>", "Sets the Rotdl path"},
	{"SET VIEWER <str_Path>", "Sets the viewer path"},
	{"SET LPSOLVE <str_Path>", "Sets the LPSolver path"},
	{"SET LOG <str_Path>", "Sets the path to salve the log"},
	{"SET VERBOSE_MODE <int_Mode>", "Sets the verbose mode to able or disable"},

	// Technology - 8
	{"SET TECHNOLOGY NAME <str_Name>", "Sets a new name to the current technology rule"},
	{"SET TECHNOLOGY MLAYERS <int_Number>", "Sets the number of metal layers supported by the current technology"},
	{"SET TECHNOLOGY SOI <str_YES/NO>", "Sets if the current technology supports SOI. Parameter must be \"YES\" or \"NO\""},
	{"SET TECHNOLOGY RESOLUTION <int_Value>", "Sets the resolution of the current technology rule"},
	{"SET TECHNOLOGY RULE <str_Rule> <float_Value>", "Sets a new value for a specific rule"},
	{"SET TECHNOLOGY CIF <str_Layer> <str_Value>", "Sets a new value for a specific CIF layer label"},
	{"SET TECHNOLOGY GDSII <str_Layer> <str_Value>", "Sets a new value for a specific GDSII layer label"},
	{"SET TECHNOLOGY VAL_TECH <str_Layer> <str_Value>", "Sets a new value for a specific ValTech layer label"},

	// Circuit - 8
	{"SET DESIGNNAME <str_Name>", "Sets a new name to the current design"},
	{"SET GRID <float_HPitch> <float_VPitch>", "Sets new values to the grid (horizontal and vertical pitchs)"},
	{"SET HGRID <float_Value>", "Sets a new value to the horizontal pitch grid"},
	{"SET VGRID <float_value>", "Sets a new value to the vertical pitch grid"},
	{"SET VDDNET <str_Name>", "Sets a new name to the VDD Net"},
	{"SET GNDNET <str_Name>", "Sets a new name to the GND Net"},
	{"SET ROWHEIGHT <int_Value>", "Sets a new value to the circuit's row height"},
	{"SET SUPPLYSIZE <float_Value>", "Sets a new value to the circuit's supply size"},

	// Floorplan - 3
	{"SET TOPCELL <str_Cell>", "Sets a new cell as the top cell"},
	{"SET AREA <int_#Rows> <float_Utilization>", "Sets the floorplan area"},
	{"SET MARGINS <float_Left> <float_Right> <float_Top> <float_Bottom>", "Sets new values to the floorplan margins"},

	{"CALCPINSPOS", "Calculates the pins' position"},

	// CellGen - 9
	{"CELLGEN SET NR_TRACKS <int_Value>", "Sets the number of tracks in the Cellgen"},
	{"CELLGEN SET POS_TRACKS <int_Value>", "Sets the tracks position in the Cellgen"},
	{"CELLGEN SELECT <str_Cell>", "Selects a cell to generate the layout using Cellgen"},
	{"CELLGEN FOLD", "Folds the cell's transistors with Cellgen"},
	{"CELLGEN PLACE <int_Saquality> <int_#Attempts> <int_WidthCost> <int_GateMissMatchCost> <int_RoutingCost> <int_RtDensityCost> <int_#GapsCost>", "Places the cell's transistors with Cellgen"},
	{"CELLGEN GETARCCOST <int_Node1> <int_Node2>", "Gets the arc cost between two nodes using Cellgen"},
	{"CELLGEN SETARCCOST <int_Node1> <int_Node2> <int_Cost>", "Sets the arc cost between two nodes using Cellgen"},
	{"CELLGEN ROUTE <int_MetalCost> <int_PolyCost> <int_ContactsCost> <int_IOPortsCost>", "Makes the routing of the cell with Cellgen"},
	{"CELLGEN COMPACT <int_MetalsPriority> <int_PolyPriority> <int_GateSpacingPriority> <int_WidthPriority>", "Compacts the cell's layout with Cellgen"},

	// Size - 2
	{"SIZE FO4", "Size the circuit using the FO4 algorithm"},
	{"SIZE GP", "Size the circuit using the GP algorithm"},

    //PrintGP parameters

	{"SET TECHNOLOGY <int_Value>","Sets a new value to the circuit's technology"},
    {"SET CLOAD <int_Value>","Sets a new value to the circuit's Cload"},
    {"SET CONSTRAREA <int_Value>","Sets a new value to the circuit's constrArea"},
    {"SET CONSTRDELAY <int_Value>","Sets a new value to the circuit's constrDelay"},
    {"SET CONSTRCIN <int_Value>", "Sets a new value to the circuit's constrCin"},
  
	
	// Elmore - 2
	{"ELMORE SAME", "Calculate de delay of the circuit using the Elmore Delay method considering the minimum size for all cells"},
	{"ELMORE FO4", "Calculate de delay of the circuit using the Elmore Delay method considering the circuit sizing using the FO4 algorithm"},
	
	// Help - 2
	{"HELP", "Prints the commands' usage on the screen"},
	{"HELP <str_Command>", "Prints the specified commands with details on the screen. Use double quotes (\"this is an example\") for words in the same parameter"},

	{"EXEC <str_Command>", "Executes an external program"},
	{"EXIT", "Closes the program and kill its process"}
};

class DesignMng{
private:
	string name;
	Circuit* circuit;
	Router* router;
	Rules* rules;	
	Placer* placer;
	AutoCell* autocell;

	int verboseMode, poscmdlog;

	string historyFile, placerFile, rotdlFile, viewerProgram, lpSolverFile;
	vector<string> commandlog;
	bool history(string cmd);
	bool saveHistory(string filename);
	bool saveProjectConfig(string filename, string project_name);

	bool run(string filename);

public:
	DesignMng();
	~DesignMng();

	int readCommand(string cmd);

	int getPosCmdLog(){return poscmdlog;};
	void setPosCmdLog(int pos){poscmdlog = pos;};
	vector<string> getCommandLog(){return commandlog;};

	int getCommandCode(vector<string> shell_cmd);

	string getViewer(){return viewerProgram;};
	string getlpSolver(){return lpSolverFile;};
	string getRotDL(){return rotdlFile;};
	string getPlacerFile(){return placerFile;};
	string getLog(){return historyFile;};
	Rules* getRules(){return rules;};
	Router* getRouter(){return router;};
	Circuit* getCircuit(){return circuit;};
	Placer* getPlacer(){return placer;};
	AutoCell* getAutoCell(){return autocell;};
		
	void setName(string s){name=s;};
	string getName(){return name;};
};
#endif
