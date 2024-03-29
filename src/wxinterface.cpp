///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 15 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wxinterface.h"

///////////////////////////////////////////////////////////////////////////

ICPD_frm::ICPD_frm( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	barra_menu = new wxMenuBar( 0 );
	menu_file = new wxMenu();
	file_new = new wxMenu();
	wxMenuItem* new_design;
	new_design = new wxMenuItem( file_new, wxID_ANY, wxString( wxT("Design") ) , wxEmptyString, wxITEM_NORMAL );
	file_new->Append( new_design );
	
	wxMenuItem* new_cellnetlist;
	new_cellnetlist = new wxMenuItem( file_new, wxID_ANY, wxString( wxT("Cell Netlist") ) , wxEmptyString, wxITEM_NORMAL );
	file_new->Append( new_cellnetlist );
	
	wxMenuItem* new_equation;
	new_equation = new wxMenuItem( file_new, wxID_ANY, wxString( wxT("Equation") ) , wxEmptyString, wxITEM_NORMAL );
	file_new->Append( new_equation );
	
	wxMenuItem* new_layout;
	new_layout = new wxMenuItem( file_new, wxID_ANY, wxString( wxT("Layout") ) , wxEmptyString, wxITEM_NORMAL );
	file_new->Append( new_layout );
	
	menu_file->Append( -1, wxT("New"), file_new );
	
	file_load = new wxMenu();
	wxMenuItem* load_project;
	load_project = new wxMenuItem( file_load, wxID_ANY, wxString( wxT("Project") ) , wxEmptyString, wxITEM_NORMAL );
	file_load->Append( load_project );
	
	file_load->AppendSeparator();
	
	load_technology = new wxMenuItem( file_load, wxID_ANY, wxString( wxT("Technology") ) , wxEmptyString, wxITEM_NORMAL );
#ifdef __WXMSW__
	load_technology->SetBitmaps( wxBitmap( wxT("../Images/load_tec.png"), wxBITMAP_TYPE_ANY ) );
#elif defined( __WXGTK__ )
	load_technology->SetBitmap( wxBitmap( wxT("../Images/load_tec.png"), wxBITMAP_TYPE_ANY ) );
#endif
	file_load->Append( load_technology );
	
	wxMenuItem* load_netlist;
	load_netlist = new wxMenuItem( file_load, wxID_ANY, wxString( wxT("Netlist") ) , wxEmptyString, wxITEM_NORMAL );
#ifdef __WXMSW__
	load_netlist->SetBitmaps( wxBitmap( wxT("../Images/load_net.png"), wxBITMAP_TYPE_ANY ) );
#elif defined( __WXGTK__ )
	load_netlist->SetBitmap( wxBitmap( wxT("../Images/load_net.png"), wxBITMAP_TYPE_ANY ) );
#endif
	file_load->Append( load_netlist );
	
	wxMenuItem* load_layouts;
	load_layouts = new wxMenuItem( file_load, wxID_ANY, wxString( wxT("Layouts") ) , wxEmptyString, wxITEM_NORMAL );
#ifdef __WXMSW__
	load_layouts->SetBitmaps( wxBitmap( wxT("../Images/load_lay.png"), wxBITMAP_TYPE_ANY ) );
#elif defined( __WXGTK__ )
	load_layouts->SetBitmap( wxBitmap( wxT("../Images/load_lay.png"), wxBITMAP_TYPE_ANY ) );
#endif
	file_load->Append( load_layouts );
	
	wxMenuItem* load_placement;
	load_placement = new wxMenuItem( file_load, wxID_ANY, wxString( wxT("Placement") ) , wxEmptyString, wxITEM_NORMAL );
#ifdef __WXMSW__
	load_placement->SetBitmaps( wxBitmap( wxT("../Images/load_pl.png"), wxBITMAP_TYPE_ANY ) );
#elif defined( __WXGTK__ )
	load_placement->SetBitmap( wxBitmap( wxT("../Images/load_pl.png"), wxBITMAP_TYPE_ANY ) );
#endif
	file_load->Append( load_placement );
	
	wxMenuItem* load_routing;
	load_routing = new wxMenuItem( file_load, wxID_ANY, wxString( wxT("Routing Rotdl") ) , wxEmptyString, wxITEM_NORMAL );
	file_load->Append( load_routing );
	
	menu_file->Append( -1, wxT("Load"), file_load );
	
	file_save = new wxMenu();
	wxMenuItem* save_project;
	save_project = new wxMenuItem( file_save, wxID_ANY, wxString( wxT("Project") ) , wxEmptyString, wxITEM_NORMAL );
	file_save->Append( save_project );
	
	file_save->AppendSeparator();
	
	wxMenuItem* save_tech;
	save_tech = new wxMenuItem( file_save, wxID_ANY, wxString( wxT("Technology") ) , wxEmptyString, wxITEM_NORMAL );
	file_save->Append( save_tech );
	
	wxMenuItem* save_netlist;
	save_netlist = new wxMenuItem( file_save, wxID_ANY, wxString( wxT("Netlist") ) , wxEmptyString, wxITEM_NORMAL );
#ifdef __WXMSW__
	save_netlist->SetBitmaps( wxBitmap( wxT("../Images/save_net.png"), wxBITMAP_TYPE_ANY ) );
#elif defined( __WXGTK__ )
	save_netlist->SetBitmap( wxBitmap( wxT("../Images/save_net.png"), wxBITMAP_TYPE_ANY ) );
#endif
	file_save->Append( save_netlist );
	
	wxMenuItem* save_layouts;
	save_layouts = new wxMenuItem( file_save, wxID_ANY, wxString( wxT("Layouts") ) , wxEmptyString, wxITEM_NORMAL );
#ifdef __WXMSW__
	save_layouts->SetBitmaps( wxBitmap( wxT("../Images/save_lay.png"), wxBITMAP_TYPE_ANY ) );
#elif defined( __WXGTK__ )
	save_layouts->SetBitmap( wxBitmap( wxT("../Images/save_lay.png"), wxBITMAP_TYPE_ANY ) );
#endif
	file_save->Append( save_layouts );
	
	wxMenuItem* save_placement;
	save_placement = new wxMenuItem( file_save, wxID_ANY, wxString( wxT("Placement") ) , wxEmptyString, wxITEM_NORMAL );
#ifdef __WXMSW__
	save_placement->SetBitmaps( wxBitmap( wxT("../Images/save_pl.png"), wxBITMAP_TYPE_ANY ) );
#elif defined( __WXGTK__ )
	save_placement->SetBitmap( wxBitmap( wxT("../Images/save_pl.png"), wxBITMAP_TYPE_ANY ) );
#endif
	file_save->Append( save_placement );
	
	wxMenuItem* save_routing;
	save_routing = new wxMenuItem( file_save, wxID_ANY, wxString( wxT("Routing Rotdl") ) , wxEmptyString, wxITEM_NORMAL );
	file_save->Append( save_routing );
	
	menu_file->Append( -1, wxT("Save"), file_save );
	
	file_import = new wxMenu();
	wxMenuItem* import_netlist;
	import_netlist = new wxMenuItem( file_import, wxID_ANY, wxString( wxT("Cadence Netlist") ) , wxEmptyString, wxITEM_NORMAL );
	file_import->Append( import_netlist );
	
	wxMenuItem* import_lef;
	import_lef = new wxMenuItem( file_import, wxID_ANY, wxString( wxT("LEF") ) , wxEmptyString, wxITEM_NORMAL );
	file_import->Append( import_lef );
	
	menu_file->Append( -1, wxT("Import"), file_import );
	
	file_export = new wxMenu();
	wxMenuItem* export_cellLayout;
	export_cellLayout = new wxMenuItem( file_export, wxID_ANY, wxString( wxT("Layout") ) , wxEmptyString, wxITEM_NORMAL );
	file_export->Append( export_cellLayout );
	
	wxMenuItem* export_cellsizes;
	export_cellsizes = new wxMenuItem( file_export, wxID_ANY, wxString( wxT("Cell Sizes") ) , wxEmptyString, wxITEM_NORMAL );
	file_export->Append( export_cellsizes );
	
	wxMenuItem* export_placement;
	export_placement = new wxMenuItem( file_export, wxID_ANY, wxString( wxT("Placement") ) , wxEmptyString, wxITEM_NORMAL );
	file_export->Append( export_placement );
	
	menu_file->Append( -1, wxT("Export"), file_export );
	
	menu_file->AppendSeparator();
	
	wxMenuItem* read_script;
	read_script = new wxMenuItem( menu_file, wxID_ANY, wxString( wxT("Read Script") ) , wxEmptyString, wxITEM_NORMAL );
#ifdef __WXMSW__
	read_script->SetBitmaps( wxBitmap( wxT("../Images/read_script.png"), wxBITMAP_TYPE_ANY ) );
#elif defined( __WXGTK__ )
	read_script->SetBitmap( wxBitmap( wxT("../Images/read_script.png"), wxBITMAP_TYPE_ANY ) );
#endif
	menu_file->Append( read_script );
	
	wxMenuItem* save_history;
	save_history = new wxMenuItem( menu_file, wxID_ANY, wxString( wxT("Save Script") ) , wxEmptyString, wxITEM_NORMAL );
	menu_file->Append( save_history );
	
	menu_file->AppendSeparator();
	
	wxMenuItem* view_preferences;
	view_preferences = new wxMenuItem( menu_file, wxID_ANY, wxString( wxT("Preferences...") ) , wxEmptyString, wxITEM_NORMAL );
#ifdef __WXMSW__
	view_preferences->SetBitmaps( wxBitmap( wxT("../Images/view_pref.png"), wxBITMAP_TYPE_ANY ) );
#elif defined( __WXGTK__ )
	view_preferences->SetBitmap( wxBitmap( wxT("../Images/view_pref.png"), wxBITMAP_TYPE_ANY ) );
#endif
	menu_file->Append( view_preferences );
	
	menu_file->AppendSeparator();
	
	wxMenuItem* exit;
	exit = new wxMenuItem( menu_file, wxID_ANY, wxString( wxT("Exit") ) , wxEmptyString, wxITEM_NORMAL );
#ifdef __WXMSW__
	exit->SetBitmaps( wxBitmap( wxT("../Images/close.png"), wxBITMAP_TYPE_ANY ) );
#elif defined( __WXGTK__ )
	exit->SetBitmap( wxBitmap( wxT("../Images/close.png"), wxBITMAP_TYPE_ANY ) );
#endif
	menu_file->Append( exit );
	
	barra_menu->Append( menu_file, wxT("File") );
	
	menu_circuit = new wxMenu();
	wxMenuItem* config_circuit;
	config_circuit = new wxMenuItem( menu_circuit, wxID_ANY, wxString( wxT("Configure...") ) , wxEmptyString, wxITEM_NORMAL );
#ifdef __WXMSW__
	config_circuit->SetBitmaps( wxBitmap( wxT("../Images/view_cir.png"), wxBITMAP_TYPE_ANY ) );
#elif defined( __WXGTK__ )
	config_circuit->SetBitmap( wxBitmap( wxT("../Images/view_cir.png"), wxBITMAP_TYPE_ANY ) );
#endif
	menu_circuit->Append( config_circuit );
	
	wxMenuItem* view_rules;
	view_rules = new wxMenuItem( menu_circuit, wxID_ANY, wxString( wxT("Rules...") ) , wxEmptyString, wxITEM_NORMAL );
#ifdef __WXMSW__
	view_rules->SetBitmaps( wxBitmap( wxT("../Images/view_rules.png"), wxBITMAP_TYPE_ANY ) );
#elif defined( __WXGTK__ )
	view_rules->SetBitmap( wxBitmap( wxT("../Images/view_rules.png"), wxBITMAP_TYPE_ANY ) );
#endif
	menu_circuit->Append( view_rules );
	
	menu_size = new wxMenu();
	wxMenuItem* size_fo4;
	size_fo4 = new wxMenuItem( menu_size, wxID_ANY, wxString( wxT("FO4 Method") ) , wxEmptyString, wxITEM_NORMAL );
	menu_size->Append( size_fo4 );
	
	wxMenuItem* size_gp;
	size_gp = new wxMenuItem( menu_size, wxID_ANY, wxString( wxT("GP Method") ) , wxEmptyString, wxITEM_NORMAL );
	menu_size->Append( size_gp );
	
	wxMenuItem* m_menuItem47;
	m_menuItem47 = new wxMenuItem( menu_size, wxID_ANY, wxString( wxT("Elmore Delay") ) , wxEmptyString, wxITEM_NORMAL );
	menu_size->Append( m_menuItem47 );
	
	menu_circuit->Append( -1, wxT("Size"), menu_size );
	
	barra_menu->Append( menu_circuit, wxT("Circuit") );
	
	menu_cells = new wxMenu();
	wxMenuItem* netlist_cellgen;
	netlist_cellgen = new wxMenuItem( menu_cells, wxID_ANY, wxString( wxT("CellGen...") ) , wxEmptyString, wxITEM_NORMAL );
#ifdef __WXMSW__
	netlist_cellgen->SetBitmaps( wxBitmap( wxT("../Images/view_cellgen.png"), wxBITMAP_TYPE_ANY ) );
#elif defined( __WXGTK__ )
	netlist_cellgen->SetBitmap( wxBitmap( wxT("../Images/view_cellgen.png"), wxBITMAP_TYPE_ANY ) );
#endif
	menu_cells->Append( netlist_cellgen );
	
	wxMenuItem* netlist_calcPinsPos;
	netlist_calcPinsPos = new wxMenuItem( menu_cells, wxID_ANY, wxString( wxT("Calculate Pins' Position") ) , wxEmptyString, wxITEM_NORMAL );
	menu_cells->Append( netlist_calcPinsPos );
	
	barra_menu->Append( menu_cells, wxT("Cells") );
	
	menu_place = new wxMenu();
	wxMenuItem* place_spec_fp;
	place_spec_fp = new wxMenuItem( menu_place, wxID_ANY, wxString( wxT("Specify Floorplan...") ) , wxEmptyString, wxITEM_NORMAL );
	menu_place->Append( place_spec_fp );
	
	wxMenuItem* place_terminals;
	place_terminals = new wxMenuItem( menu_place, wxID_ANY, wxString( wxT("Terminals") ) , wxEmptyString, wxITEM_NORMAL );
	menu_place->Append( place_terminals );
	
	wxMenuItem* place_mobral;
	place_mobral = new wxMenuItem( menu_place, wxID_ANY, wxString( wxT("zPlace") ) , wxEmptyString, wxITEM_NORMAL );
	menu_place->Append( place_mobral );
	
	wxMenuItem* place_verify;
	place_verify = new wxMenuItem( menu_place, wxID_ANY, wxString( wxT("Verify") ) , wxEmptyString, wxITEM_NORMAL );
	menu_place->Append( place_verify );
	
	wxMenuItem* place_autoflip;
	place_autoflip = new wxMenuItem( menu_place, wxID_ANY, wxString( wxT("Auto Flip") ) , wxEmptyString, wxITEM_NORMAL );
	menu_place->Append( place_autoflip );
	
	wxMenuItem* place_inc_placement;
	place_inc_placement = new wxMenuItem( menu_place, wxID_ANY, wxString( wxT("Incremental Placement") ) , wxEmptyString, wxITEM_NORMAL );
	menu_place->Append( place_inc_placement );
	
	wxMenuItem* place_checkwl;
	place_checkwl = new wxMenuItem( menu_place, wxID_ANY, wxString( wxT("Check WL") ) , wxEmptyString, wxITEM_NORMAL );
	menu_place->Append( place_checkwl );
	
	barra_menu->Append( menu_place, wxT("Place") );
	
	menu_route = new wxMenu();
	wxMenuItem* route_rotdl;
	route_rotdl = new wxMenuItem( menu_route, wxID_ANY, wxString( wxT("Rotdl") ) , wxEmptyString, wxITEM_NORMAL );
	menu_route->Append( route_rotdl );
	
	wxMenuItem* route_nbrouter;
	route_nbrouter = new wxMenuItem( menu_route, wxID_ANY, wxString( wxT("NBRouter") ) , wxEmptyString, wxITEM_NORMAL );
	menu_route->Append( route_nbrouter );
	
	wxMenuItem* route_optimize;
	route_optimize = new wxMenuItem( menu_route, wxID_ANY, wxString( wxT("Optimize") ) , wxEmptyString, wxITEM_NORMAL );
	menu_route->Append( route_optimize );
	
	wxMenuItem* route_compact;
	route_compact = new wxMenuItem( menu_route, wxID_ANY, wxString( wxT("Compact") ) , wxEmptyString, wxITEM_NORMAL );
	menu_route->Append( route_compact );
	
	wxMenuItem* route_clear;
	route_clear = new wxMenuItem( menu_route, wxID_ANY, wxString( wxT("Clear") ) , wxEmptyString, wxITEM_NORMAL );
	menu_route->Append( route_clear );
	
	barra_menu->Append( menu_route, wxT("Route") );
	
	menu_help = new wxMenu();
	wxMenuItem* help_about;
	help_about = new wxMenuItem( menu_help, wxID_ANY, wxString( wxT("About ICPD") ) , wxEmptyString, wxITEM_NORMAL );
#ifdef __WXMSW__
	help_about->SetBitmaps( wxBitmap( wxT("../Images/help.png"), wxBITMAP_TYPE_ANY ) );
#elif defined( __WXGTK__ )
	help_about->SetBitmap( wxBitmap( wxT("../Images/help.png"), wxBITMAP_TYPE_ANY ) );
#endif
	menu_help->Append( help_about );
	
	wxMenuItem* help_homepage;
	help_homepage = new wxMenuItem( menu_help, wxID_ANY, wxString( wxT("Home Page") ) , wxEmptyString, wxITEM_NORMAL );
	menu_help->Append( help_homepage );
	
	barra_menu->Append( menu_help, wxT("Help") );
	
	this->SetMenuBar( barra_menu );
	
	toolbar = this->CreateToolBar( wxTB_FLAT|wxTB_HORIZONTAL, wxID_ANY ); 
	toolbar->AddTool( ID_readscript, wxT("tool_readscript"), wxBitmap( wxT("../Images/read_script.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Read Script"), wxEmptyString );
	toolbar->AddSeparator();
	toolbar->AddTool( ID_opentech, wxT("tool_opentech"), wxBitmap( wxT("../Images/load_tec.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Load Technology"), wxEmptyString );
	toolbar->AddTool( ID_opennetlist, wxT("tool_opennetlist"), wxBitmap( wxT("../Images/load_net.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Load Netlist"), wxEmptyString );
	toolbar->AddTool( ID_openlibrary, wxT("tool_openlibrary"), wxBitmap( wxT("../Images/load_lay.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Load Layouts"), wxEmptyString );
	toolbar->AddTool( ID_openplacement, wxT("tool_openplacement"), wxBitmap( wxT("../Images/load_pl.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Load Placement"), wxEmptyString );
	toolbar->AddSeparator();
	toolbar->AddTool( ID_savenetlist, wxT("tool_savenetlist"), wxBitmap( wxT("../Images/save_net.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Save Netlist"), wxEmptyString );
	toolbar->AddTool( ID_savelayout, wxT("tool_savelayouts"), wxBitmap( wxT("../Images/save_lay.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Save Layouts"), wxEmptyString );
	toolbar->AddTool( ID_savepl, wxT("tool_savepl"), wxBitmap( wxT("../Images/save_pl.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Save Placement"), wxEmptyString );
	toolbar->AddSeparator();
	toolbar->AddTool( ID_rules, wxT("tool_rules"), wxBitmap( wxT("../Images/view_rules.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("View Rules"), wxEmptyString );
	toolbar->AddTool( ID_circuit, wxT("tool_circuit"), wxBitmap( wxT("../Images/view_cir.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("View Circuit"), wxEmptyString );
	toolbar->AddTool( ID_cellgen, wxT("tool_cellgen"), wxBitmap( wxT("../Images/view_cellgen.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("View CellGen"), wxEmptyString );
	toolbar->AddTool( ID_preferences, wxT("tool_preferences"), wxBitmap( wxT("../Images/view_pref.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("View Preferences"), wxEmptyString );
	toolbar->AddSeparator();
	toolbar->AddTool( ID_refresh, wxT("tool_refresh"), wxBitmap( wxT("../Images/refresh.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Refresh"), wxEmptyString );
	toolbar->AddSeparator();
	toolbar->AddTool( ID_help, wxT("tool_help"), wxBitmap( wxT("../Images/help.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Help"), wxEmptyString );
	toolbar->Realize();
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText6 = new wxStaticText( this, wxID_ANY, wxT("Layouts:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	bSizer4->Add( m_staticText6, 0, wxALL, 5 );
	
	wx_layouts = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	bSizer4->Add( wx_layouts, 1, wxALL|wxEXPAND, 5 );
	
	bSizer3->Add( bSizer4, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText7 = new wxStaticText( this, wxID_ANY, wxT("Instances Tree:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	bSizer5->Add( m_staticText7, 0, wxALL, 5 );
	
	wx_instances = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	bSizer5->Add( wx_instances, 1, wxALL|wxEXPAND, 5 );
	
	bSizer3->Add( bSizer5, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText8 = new wxStaticText( this, wxID_ANY, wxT("Cells:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	bSizer6->Add( m_staticText8, 0, wxALL, 5 );
	
	wx_cells = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	bSizer6->Add( wx_cells, 1, wxALL|wxEXPAND, 5 );
	
	bSizer3->Add( bSizer6, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText9 = new wxStaticText( this, wxID_ANY, wxT("Nets:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText9->Wrap( -1 );
	bSizer7->Add( m_staticText9, 0, wxALL, 5 );
	
	wx_nets = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	bSizer7->Add( wx_nets, 1, wxALL|wxEXPAND, 5 );
	
	bSizer3->Add( bSizer7, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText10 = new wxStaticText( this, wxID_ANY, wxT("Interfaces:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText10->Wrap( -1 );
	bSizer8->Add( m_staticText10, 0, wxALL, 5 );
	
	wx_interfaces = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	bSizer8->Add( wx_interfaces, 1, wxALL|wxEXPAND, 5 );
	
	bSizer3->Add( bSizer8, 1, wxEXPAND, 5 );
	
	bSizer2->Add( bSizer3, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxVERTICAL );
	
	wx_log = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY );
	bSizer9->Add( wx_log, 1, wxALL|wxEXPAND, 5 );
	
	bSizer2->Add( bSizer9, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	wxCommand = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	bSizer2->Add( wxCommand, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL, 0 );
	
	this->SetSizer( bSizer2 );
	this->Layout();
	statusBar = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( ICPD_frm::OnClose ) );
	this->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( ICPD_frm::PressedKey ) );
	barra_menu->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( ICPD_frm::PressedKey ), NULL, this );
	this->Connect( new_design->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::newDesign ) );
	this->Connect( new_cellnetlist->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::newCell ) );
	this->Connect( new_layout->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::newCell ) );
	this->Connect( load_project->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::loadProject ) );
	this->Connect( load_technology->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::loadTech ) );
	this->Connect( load_netlist->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::loadSpiceNetlist ) );
	this->Connect( load_layouts->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::loadLayouts ) );
	this->Connect( load_placement->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::loadPlacement ) );
	this->Connect( load_routing->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::loadRouting ) );
	this->Connect( save_project->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::saveProject ) );
	this->Connect( save_tech->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::saveTech ) );
	this->Connect( save_netlist->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::saveSpiceNetlist ) );
	this->Connect( save_layouts->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::saveLayouts ) );
	this->Connect( save_placement->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::savePlacement ) );
	this->Connect( save_routing->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::saveRouting ) );
	this->Connect( import_netlist->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::importSpiceNetlist ) );
	this->Connect( import_lef->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::importLEF ) );
	this->Connect( export_cellLayout->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::exportLayout ) );
	this->Connect( export_cellsizes->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::exportCellSizes ) );
	this->Connect( export_placement->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::exportPlacement ) );
	this->Connect( read_script->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::flReadScript ) );
	this->Connect( save_history->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::saveHistory ) );
	this->Connect( view_preferences->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::showPreferences ) );
	this->Connect( exit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::OnFileExit ) );
	this->Connect( config_circuit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::showCircuit ) );
	this->Connect( view_rules->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::showTech ) );
	this->Connect( size_fo4->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::sizeFO4 ) );
	this->Connect( size_gp->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::sizeGP ) );
	this->Connect( m_menuItem47->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::elmoreDelay ) );
	this->Connect( netlist_cellgen->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::showCellgen ) );
	this->Connect( netlist_calcPinsPos->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::calcPinsPos ) );
	this->Connect( place_spec_fp->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::showFP ) );
	this->Connect( place_terminals->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::plTerminals ) );
	this->Connect( place_mobral->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::fastp ) );
	this->Connect( place_verify->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::plVerify ) );
	this->Connect( place_autoflip->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::plAutoFlip ) );
	this->Connect( place_inc_placement->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::plIncremental ) );
	this->Connect( place_checkwl->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::plWL ) );
	this->Connect( route_rotdl->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::rotdl ) );
	this->Connect( route_nbrouter->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::pfinder ) );
	this->Connect( route_optimize->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::rtOptimize ) );
	this->Connect( route_compact->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::rtCompact ) );
	this->Connect( route_clear->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::rtClear ) );
	this->Connect( help_about->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::HelpaboutClick ) );
	this->Connect( help_homepage->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::helpHP ) );
	toolbar->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( ICPD_frm::PressedKey ), NULL, this );
	this->Connect( ID_readscript, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ICPD_frm::flReadScript ) );
	this->Connect( ID_opentech, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ICPD_frm::loadTech ) );
	this->Connect( ID_opennetlist, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ICPD_frm::loadSpiceNetlist ) );
	this->Connect( ID_openlibrary, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ICPD_frm::loadLayouts ) );
	this->Connect( ID_openplacement, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ICPD_frm::loadPlacement ) );
	this->Connect( ID_savenetlist, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ICPD_frm::saveSpiceNetlist ) );
	this->Connect( ID_savelayout, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ICPD_frm::saveLayouts ) );
	this->Connect( ID_savepl, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ICPD_frm::savePlacement ) );
	this->Connect( ID_rules, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ICPD_frm::showTech ) );
	this->Connect( ID_circuit, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ICPD_frm::showCircuit ) );
	this->Connect( ID_cellgen, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ICPD_frm::showCellgen ) );
	this->Connect( ID_preferences, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ICPD_frm::showPreferences ) );
	this->Connect( ID_refresh, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ICPD_frm::refreshIcon ) );
	this->Connect( ID_help, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ICPD_frm::HelpaboutClick ) );
	wx_layouts->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( ICPD_frm::PressedKey ), NULL, this );
	wx_layouts->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( ICPD_frm::layoutListEvent ), NULL, this );
	wx_layouts->Connect( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler( ICPD_frm::viewLayout ), NULL, this );
	wx_instances->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( ICPD_frm::PressedKey ), NULL, this );
	wx_instances->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( ICPD_frm::openInstancewindow ), NULL, this );
	wx_instances->Connect( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler( ICPD_frm::printInstance ), NULL, this );
	wx_cells->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( ICPD_frm::PressedKey ), NULL, this );
	wx_cells->Connect( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler( ICPD_frm::printCell ), NULL, this );
	wx_nets->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( ICPD_frm::PressedKey ), NULL, this );
	wx_nets->Connect( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler( ICPD_frm::printNet ), NULL, this );
	wx_interfaces->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( ICPD_frm::PressedKey ), NULL, this );
	wx_interfaces->Connect( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler( ICPD_frm::printInterface ), NULL, this );
	wx_log->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( ICPD_frm::PressedKey ), NULL, this );
	wxCommand->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( ICPD_frm::CmdKeyDown ), NULL, this );
	wxCommand->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ICPD_frm::command ), NULL, this );
	statusBar->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( ICPD_frm::PressedKey ), NULL, this );
}

ICPD_frm::~ICPD_frm()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( ICPD_frm::OnClose ) );
	this->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( ICPD_frm::PressedKey ) );
	barra_menu->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( ICPD_frm::PressedKey ), NULL, this );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::newDesign ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::newCell ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::newCell ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::loadProject ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::loadTech ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::loadSpiceNetlist ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::loadLayouts ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::loadPlacement ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::loadRouting ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::saveProject ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::saveTech ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::saveSpiceNetlist ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::saveLayouts ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::savePlacement ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::saveRouting ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::importSpiceNetlist ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::importLEF ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::exportLayout ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::exportCellSizes ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::exportPlacement ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::flReadScript ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::saveHistory ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::showPreferences ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::OnFileExit ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::showCircuit ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::showTech ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::sizeFO4 ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::sizeGP ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::elmoreDelay ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::showCellgen ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::calcPinsPos ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::showFP ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::plTerminals ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::fastp ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::plVerify ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::plAutoFlip ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::plIncremental ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::plWL ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::rotdl ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::pfinder ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::rtOptimize ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::rtCompact ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::rtClear ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::HelpaboutClick ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::helpHP ) );
	toolbar->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( ICPD_frm::PressedKey ), NULL, this );
	this->Disconnect( ID_readscript, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ICPD_frm::flReadScript ) );
	this->Disconnect( ID_opentech, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ICPD_frm::loadTech ) );
	this->Disconnect( ID_opennetlist, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ICPD_frm::loadSpiceNetlist ) );
	this->Disconnect( ID_openlibrary, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ICPD_frm::loadLayouts ) );
	this->Disconnect( ID_openplacement, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ICPD_frm::loadPlacement ) );
	this->Disconnect( ID_savenetlist, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ICPD_frm::saveSpiceNetlist ) );
	this->Disconnect( ID_savelayout, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ICPD_frm::saveLayouts ) );
	this->Disconnect( ID_savepl, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ICPD_frm::savePlacement ) );
	this->Disconnect( ID_rules, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ICPD_frm::showTech ) );
	this->Disconnect( ID_circuit, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ICPD_frm::showCircuit ) );
	this->Disconnect( ID_cellgen, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ICPD_frm::showCellgen ) );
	this->Disconnect( ID_preferences, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ICPD_frm::showPreferences ) );
	this->Disconnect( ID_refresh, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ICPD_frm::refreshIcon ) );
	this->Disconnect( ID_help, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ICPD_frm::HelpaboutClick ) );
	wx_layouts->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( ICPD_frm::PressedKey ), NULL, this );
	wx_layouts->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( ICPD_frm::layoutListEvent ), NULL, this );
	wx_layouts->Disconnect( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler( ICPD_frm::viewLayout ), NULL, this );
	wx_instances->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( ICPD_frm::PressedKey ), NULL, this );
	wx_instances->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( ICPD_frm::openInstancewindow ), NULL, this );
	wx_instances->Disconnect( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler( ICPD_frm::printInstance ), NULL, this );
	wx_cells->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( ICPD_frm::PressedKey ), NULL, this );
	wx_cells->Disconnect( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler( ICPD_frm::printCell ), NULL, this );
	wx_nets->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( ICPD_frm::PressedKey ), NULL, this );
	wx_nets->Disconnect( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler( ICPD_frm::printNet ), NULL, this );
	wx_interfaces->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( ICPD_frm::PressedKey ), NULL, this );
	wx_interfaces->Disconnect( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler( ICPD_frm::printInterface ), NULL, this );
	wx_log->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( ICPD_frm::PressedKey ), NULL, this );
	wxCommand->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( ICPD_frm::CmdKeyDown ), NULL, this );
	wxCommand->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ICPD_frm::command ), NULL, this );
	statusBar->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( ICPD_frm::PressedKey ), NULL, this );
}

CellGen::CellGen( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer6;
	fgSizer6 = new wxFlexGridSizer( 2, 5, 0, 0 );
	fgSizer6->SetFlexibleDirection( wxBOTH );
	fgSizer6->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	static_cellnet = new wxStaticText( this, wxID_ANY, wxT("Cell Netlist:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_cellnet->Wrap( -1 );
	fgSizer6->Add( static_cellnet, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxArrayString cellnet_choiceChoices;
	cellnet_choice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, cellnet_choiceChoices, 0 );
	cellnet_choice->SetSelection( 0 );
	fgSizer6->Add( cellnet_choice, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	fgSizer6->Add( 0, 0, 1, wxEXPAND, 5 );
	
	generate_button = new wxButton( this, wxID_ANY, wxT("&Generate Cell"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( generate_button, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	select_button = new wxButton( this, wxID_ANY, wxT("&Select"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( select_button, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	static_trpos = new wxStaticText( this, wxID_ANY, wxT("Tracks Position:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_trpos->Wrap( -1 );
	fgSizer6->Add( static_trpos, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	s_TP = new wxTextCtrl( this, wxID_ANY, wxT("5"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( s_TP, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	static_trnum = new wxStaticText( this, wxID_ANY, wxT("Tracks Number:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_trnum->Wrap( -1 );
	fgSizer6->Add( static_trnum, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	s_RN = new wxTextCtrl( this, wxID_ANY, wxT("2"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( s_RN, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	fold_button = new wxButton( this, wxID_ANY, wxT("&Fold Tr."), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( fold_button, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	static_widcost = new wxStaticText( this, wxID_ANY, wxT("Width Cost:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_widcost->Wrap( -1 );
	fgSizer6->Add( static_widcost, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	p_WC = new wxTextCtrl( this, wxID_ANY, wxT("3"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( p_WC, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	static_gatemmcost = new wxStaticText( this, wxID_ANY, wxT("Gate Miss Match Cost:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_gatemmcost->Wrap( -1 );
	fgSizer6->Add( static_gatemmcost, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	p_GMC = new wxTextCtrl( this, wxID_ANY, wxT("4"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( p_GMC, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	fgSizer6->Add( 0, 0, 1, wxEXPAND, 5 );
	
	static_rtcost = new wxStaticText( this, wxID_ANY, wxT("Routing Cost:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_rtcost->Wrap( -1 );
	fgSizer6->Add( static_rtcost, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	p_RC = new wxTextCtrl( this, wxID_ANY, wxT("1"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( p_RC, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	static_rtdenscost = new wxStaticText( this, wxID_ANY, wxT("Rt Density Cost:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_rtdenscost->Wrap( -1 );
	fgSizer6->Add( static_rtdenscost, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	p_RTDC = new wxTextCtrl( this, wxID_ANY, wxT("4"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( p_RTDC, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	fgSizer6->Add( 0, 0, 1, wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );
	
	static_gapcost = new wxStaticText( this, wxID_ANY, wxT("# Gaps Cost:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_gapcost->Wrap( -1 );
	fgSizer6->Add( static_gapcost, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	p_NGC = new wxTextCtrl( this, wxID_ANY, wxT("2"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( p_NGC, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	fgSizer6->Add( 0, 0, 1, wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	fgSizer6->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	fgSizer6->Add( 0, 0, 1, wxEXPAND, 5 );
	
	static_taqual = new wxStaticText( this, wxID_ANY, wxT("TA Quality:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_taqual->Wrap( -1 );
	fgSizer6->Add( static_taqual, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	p_TAQ = new wxTextCtrl( this, wxID_ANY, wxT("150"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( p_TAQ, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	static_numatt = new wxStaticText( this, wxID_ANY, wxT("# Attempts:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_numatt->Wrap( -1 );
	fgSizer6->Add( static_numatt, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	p_NRA = new wxTextCtrl( this, wxID_ANY, wxT("3"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( p_NRA, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	place_button = new wxButton( this, wxID_ANY, wxT("&Place Tr."), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( place_button, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	static_metal = new wxStaticText( this, wxID_ANY, wxT("Metal:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_metal->Wrap( -1 );
	fgSizer6->Add( static_metal, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	rc_M = new wxTextCtrl( this, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( rc_M, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	static_poly1 = new wxStaticText( this, wxID_ANY, wxT("Poly:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_poly1->Wrap( -1 );
	fgSizer6->Add( static_poly1, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	rc_P = new wxTextCtrl( this, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( rc_P, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	fgSizer6->Add( 0, 0, 1, wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );
	
	static_contacts = new wxStaticText( this, wxID_ANY, wxT("Contacts:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_contacts->Wrap( -1 );
	fgSizer6->Add( static_contacts, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	rc_C = new wxTextCtrl( this, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( rc_C, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	static_ioports = new wxStaticText( this, wxID_ANY, wxT("I/O Ports:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_ioports->Wrap( -1 );
	fgSizer6->Add( static_ioports, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	rc_IOP = new wxTextCtrl( this, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( rc_IOP, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	route_button = new wxButton( this, wxID_ANY, wxT("&Route"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( route_button, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	static_width = new wxStaticText( this, wxID_ANY, wxT("Width:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_width->Wrap( -1 );
	fgSizer6->Add( static_width, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	cp_W = new wxTextCtrl( this, wxID_ANY, wxT("10"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( cp_W, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	static_gatespac = new wxStaticText( this, wxID_ANY, wxT("Gate Spacing:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_gatespac->Wrap( -1 );
	fgSizer6->Add( static_gatespac, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	cp_GS = new wxTextCtrl( this, wxID_ANY, wxT("5"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( cp_GS, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	fgSizer6->Add( 0, 0, 1, wxEXPAND, 5 );
	
	static_metals = new wxStaticText( this, wxID_ANY, wxT("Metals:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_metals->Wrap( -1 );
	fgSizer6->Add( static_metals, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	cp_M = new wxTextCtrl( this, wxID_ANY, wxT("2"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( cp_M, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	static_poly2 = new wxStaticText( this, wxID_ANY, wxT("Poly:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_poly2->Wrap( -1 );
	fgSizer6->Add( static_poly2, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	cp_P = new wxTextCtrl( this, wxID_ANY, wxT("3"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( cp_P, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	compact_button = new wxButton( this, wxID_ANY, wxT("&Compact Layout"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( compact_button, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	view_button = new wxButton( this, wxID_ANY, wxT("&View Layout"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( view_button, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	this->SetSizer( fgSizer6 );
	this->Layout();
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( CellGen::hide ) );
	cellnet_choice->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	generate_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CellGen::generateCell ), NULL, this );
	select_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CellGen::select ), NULL, this );
	select_button->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	s_TP->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	s_RN->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	fold_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CellGen::fold ), NULL, this );
	fold_button->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	p_WC->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	p_GMC->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	p_RC->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	p_RTDC->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	p_NGC->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	p_TAQ->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	p_NRA->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	place_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CellGen::place ), NULL, this );
	place_button->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	rc_M->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	rc_P->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	rc_C->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	rc_IOP->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	route_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CellGen::route ), NULL, this );
	route_button->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	cp_W->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	cp_GS->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	cp_M->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	cp_P->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	compact_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CellGen::compact ), NULL, this );
	compact_button->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	view_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CellGen::view ), NULL, this );
	view_button->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
}

CellGen::~CellGen()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( CellGen::hide ) );
	cellnet_choice->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	generate_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CellGen::generateCell ), NULL, this );
	select_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CellGen::select ), NULL, this );
	select_button->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	s_TP->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	s_RN->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	fold_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CellGen::fold ), NULL, this );
	fold_button->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	p_WC->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	p_GMC->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	p_RC->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	p_RTDC->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	p_NGC->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	p_TAQ->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	p_NRA->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	place_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CellGen::place ), NULL, this );
	place_button->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	rc_M->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	rc_P->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	rc_C->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	rc_IOP->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	route_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CellGen::route ), NULL, this );
	route_button->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	cp_W->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	cp_GS->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	cp_M->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	cp_P->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	compact_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CellGen::compact ), NULL, this );
	compact_button->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	view_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CellGen::view ), NULL, this );
	view_button->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
}

View_Circuit::View_Circuit( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer3;
	fgSizer3 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer3->SetFlexibleDirection( wxBOTH );
	fgSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	static_design1 = new wxStaticText( this, wxID_ANY, wxT("Design Name:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_design1->Wrap( -1 );
	fgSizer3->Add( static_design1, 0, wxALL, 5 );
	
	design_name = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( design_name, 0, wxALL, 5 );
	
	static_topcell = new wxStaticText( this, wxID_ANY, wxT("Top Cell:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_topcell->Wrap( -1 );
	fgSizer3->Add( static_topcell, 0, wxALL, 5 );
	
	wxArrayString cell_choiceChoices;
	cell_choice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, cell_choiceChoices, 0 );
	cell_choice->SetSelection( 0 );
	fgSizer3->Add( cell_choice, 0, wxALL, 5 );
	
	static_horizontal = new wxStaticText( this, wxID_ANY, wxT("Horizontal Grid:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_horizontal->Wrap( -1 );
	fgSizer3->Add( static_horizontal, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	horizontalgrid = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( horizontalgrid, 0, wxALL, 5 );
	
	static_vertical = new wxStaticText( this, wxID_ANY, wxT("Vertical Grid:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_vertical->Wrap( -1 );
	fgSizer3->Add( static_vertical, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	verticalgrid = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( verticalgrid, 0, wxALL, 5 );
	
	static_vdd = new wxStaticText( this, wxID_ANY, wxT("Vdd Net Name:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_vdd->Wrap( -1 );
	fgSizer3->Add( static_vdd, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	vddnetname = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( vddnetname, 0, wxALL, 5 );
	
	static_gnd = new wxStaticText( this, wxID_ANY, wxT("Gnd Net Name:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_gnd->Wrap( -1 );
	fgSizer3->Add( static_gnd, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	gndnetname = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( gndnetname, 0, wxALL, 5 );
	
	static_row = new wxStaticText( this, wxID_ANY, wxT("Row Height:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_row->Wrap( -1 );
	fgSizer3->Add( static_row, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	rowheight = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( rowheight, 0, wxALL, 5 );
	
	static_supply = new wxStaticText( this, wxID_ANY, wxT("Supply Size:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_supply->Wrap( -1 );
	fgSizer3->Add( static_supply, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	supplysize = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( supplysize, 0, wxALL, 5 );
	
	ok_button = new wxButton( this, wxID_ANY, wxT("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( ok_button, 0, wxALL, 5 );
	
	cancel_button = new wxButton( this, wxID_ANY, wxT("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( cancel_button, 0, wxALL, 5 );
	
	this->SetSizer( fgSizer3 );
	this->Layout();
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( View_Circuit::hide ) );
	this->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Circuit::PressedKey ) );
	design_name->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Circuit::PressedKey ), NULL, this );
	design_name->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( View_Circuit::applyButtonEvt ), NULL, this );
	horizontalgrid->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Circuit::PressedKey ), NULL, this );
	verticalgrid->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Circuit::PressedKey ), NULL, this );
	vddnetname->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Circuit::PressedKey ), NULL, this );
	gndnetname->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Circuit::PressedKey ), NULL, this );
	rowheight->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Circuit::PressedKey ), NULL, this );
	supplysize->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Circuit::PressedKey ), NULL, this );
	ok_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( View_Circuit::okButtonEvt ), NULL, this );
	ok_button->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Circuit::PressedKey ), NULL, this );
	cancel_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( View_Circuit::cancelButtonEvt ), NULL, this );
	cancel_button->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Circuit::PressedKey ), NULL, this );
}

View_Circuit::~View_Circuit()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( View_Circuit::hide ) );
	this->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Circuit::PressedKey ) );
	design_name->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Circuit::PressedKey ), NULL, this );
	design_name->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( View_Circuit::applyButtonEvt ), NULL, this );
	horizontalgrid->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Circuit::PressedKey ), NULL, this );
	verticalgrid->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Circuit::PressedKey ), NULL, this );
	vddnetname->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Circuit::PressedKey ), NULL, this );
	gndnetname->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Circuit::PressedKey ), NULL, this );
	rowheight->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Circuit::PressedKey ), NULL, this );
	supplysize->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Circuit::PressedKey ), NULL, this );
	ok_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( View_Circuit::okButtonEvt ), NULL, this );
	ok_button->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Circuit::PressedKey ), NULL, this );
	cancel_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( View_Circuit::cancelButtonEvt ), NULL, this );
	cancel_button->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Circuit::PressedKey ), NULL, this );
}

View_Preferences::View_Preferences( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer4;
	fgSizer4 = new wxFlexGridSizer( 2, 3, 0, 0 );
	fgSizer4->SetFlexibleDirection( wxBOTH );
	fgSizer4->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	static_plPath = new wxStaticText( this, wxID_ANY, wxT("Placement Tool:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_plPath->Wrap( -1 );
	fgSizer4->Add( static_plPath, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	plPath = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 230,-1 ), 0 );
	fgSizer4->Add( plPath, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	plPath_button = new wxButton( this, wxID_ANY, wxT("Choose"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer4->Add( plPath_button, 0, wxALL, 5 );
	
	static_rotdl = new wxStaticText( this, wxID_ANY, wxT("Rotdl path:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_rotdl->Wrap( -1 );
	fgSizer4->Add( static_rotdl, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	rotdlPath = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 230,-1 ), 0 );
	fgSizer4->Add( rotdlPath, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	rotdlPath_button = new wxButton( this, wxID_ANY, wxT("Choose"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer4->Add( rotdlPath_button, 0, wxALL, 5 );
	
	static_viewer = new wxStaticText( this, wxID_ANY, wxT("CIF viewer path:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_viewer->Wrap( -1 );
	fgSizer4->Add( static_viewer, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	viewerPath = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 230,-1 ), 0 );
	fgSizer4->Add( viewerPath, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	viewerPath_button = new wxButton( this, wxID_ANY, wxT("Choose"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer4->Add( viewerPath_button, 0, wxALL, 5 );
	
	static_lpsolver = new wxStaticText( this, wxID_ANY, wxT("LPSolver path:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_lpsolver->Wrap( -1 );
	fgSizer4->Add( static_lpsolver, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	lpsolverPath = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 230,-1 ), 0 );
	fgSizer4->Add( lpsolverPath, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	lpsolverPath_button = new wxButton( this, wxID_ANY, wxT("Choose"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer4->Add( lpsolverPath_button, 0, wxALL, 5 );
	
	static_logf = new wxStaticText( this, wxID_ANY, wxT("Log File:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_logf->Wrap( -1 );
	fgSizer4->Add( static_logf, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	logPath = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 230,-1 ), 0 );
	fgSizer4->Add( logPath, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	logPath_button = new wxButton( this, wxID_ANY, wxT("Choose"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer4->Add( logPath_button, 0, wxALL, 5 );
	
	ok_button = new wxButton( this, wxID_ANY, wxT("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer4->Add( ok_button, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	cancel_button = new wxButton( this, wxID_ANY, wxT("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer4->Add( cancel_button, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	this->SetSizer( fgSizer4 );
	this->Layout();
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( View_Preferences::hide ) );
	this->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Preferences::PressedKey ) );
	plPath->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Preferences::PressedKey ), NULL, this );
	plPath_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( View_Preferences::plButton ), NULL, this );
	plPath_button->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Preferences::PressedKey ), NULL, this );
	rotdlPath->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Preferences::PressedKey ), NULL, this );
	rotdlPath_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( View_Preferences::rotdlButton ), NULL, this );
	rotdlPath_button->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Preferences::PressedKey ), NULL, this );
	viewerPath->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Preferences::PressedKey ), NULL, this );
	viewerPath_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( View_Preferences::viewerButton ), NULL, this );
	viewerPath_button->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Preferences::PressedKey ), NULL, this );
	lpsolverPath->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Preferences::PressedKey ), NULL, this );
	lpsolverPath_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( View_Preferences::lpsolverButton ), NULL, this );
	lpsolverPath_button->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Preferences::PressedKey ), NULL, this );
	logPath->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Preferences::PressedKey ), NULL, this );
	logPath_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( View_Preferences::logButton ), NULL, this );
	logPath_button->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Preferences::PressedKey ), NULL, this );
	ok_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( View_Preferences::okButtonEvt ), NULL, this );
	ok_button->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Preferences::PressedKey ), NULL, this );
	cancel_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( View_Preferences::cancelButtonEvt ), NULL, this );
	cancel_button->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Preferences::PressedKey ), NULL, this );
}

View_Preferences::~View_Preferences()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( View_Preferences::hide ) );
	this->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Preferences::PressedKey ) );
	plPath->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Preferences::PressedKey ), NULL, this );
	plPath_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( View_Preferences::plButton ), NULL, this );
	plPath_button->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Preferences::PressedKey ), NULL, this );
	rotdlPath->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Preferences::PressedKey ), NULL, this );
	rotdlPath_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( View_Preferences::rotdlButton ), NULL, this );
	rotdlPath_button->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Preferences::PressedKey ), NULL, this );
	viewerPath->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Preferences::PressedKey ), NULL, this );
	viewerPath_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( View_Preferences::viewerButton ), NULL, this );
	viewerPath_button->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Preferences::PressedKey ), NULL, this );
	lpsolverPath->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Preferences::PressedKey ), NULL, this );
	lpsolverPath_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( View_Preferences::lpsolverButton ), NULL, this );
	lpsolverPath_button->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Preferences::PressedKey ), NULL, this );
	logPath->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Preferences::PressedKey ), NULL, this );
	logPath_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( View_Preferences::logButton ), NULL, this );
	logPath_button->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Preferences::PressedKey ), NULL, this );
	ok_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( View_Preferences::okButtonEvt ), NULL, this );
	ok_button->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Preferences::PressedKey ), NULL, this );
	cancel_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( View_Preferences::cancelButtonEvt ), NULL, this );
	cancel_button->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Preferences::PressedKey ), NULL, this );
}

Design_Rules::Design_Rules( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 3, 4, 0, 0 );
	fgSizer2->SetFlexibleDirection( wxBOTH );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	static_technology = new wxStaticText( this, wxID_ANY, wxT("Technology:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_technology->Wrap( -1 );
	fgSizer2->Add( static_technology, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	wxTechName = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer2->Add( wxTechName, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	static_soi = new wxStaticText( this, wxID_ANY, wxT("SOI:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_soi->Wrap( -1 );
	fgSizer2->Add( static_soi, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	wxSOI = new wxCheckBox( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	wxSOI->SetValue(true);
	
	fgSizer2->Add( wxSOI, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	static_mlayers = new wxStaticText( this, wxID_ANY, wxT("Metal Layers:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_mlayers->Wrap( -1 );
	fgSizer2->Add( static_mlayers, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxMLayers = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer2->Add( wxMLayers, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	static_resolution = new wxStaticText( this, wxID_ANY, wxT("Resolution:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_resolution->Wrap( -1 );
	fgSizer2->Add( static_resolution, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxResolution = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer2->Add( wxResolution, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	static_rule = new wxStaticText( this, wxID_ANY, wxT("Rule:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_rule->Wrap( -1 );
	fgSizer2->Add( static_rule, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxArrayString wxSelect_RuleChoices;
	wxSelect_Rule = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSelect_RuleChoices, 0 );
	wxSelect_Rule->SetSelection( 0 );
	fgSizer2->Add( wxSelect_Rule, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	static_rdescription = new wxStaticText( this, wxID_ANY, wxT("Description:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_rdescription->Wrap( -1 );
	fgSizer2->Add( static_rdescription, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	static_RuleDesc = new wxStaticText( this, wxID_ANY, wxT("Rule Description"), wxDefaultPosition, wxDefaultSize, 0 );
	static_RuleDesc->Wrap( -1 );
	fgSizer2->Add( static_RuleDesc, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	static_value = new wxStaticText( this, wxID_ANY, wxT("Value:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_value->Wrap( -1 );
	fgSizer2->Add( static_value, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxRule_Value = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer2->Add( wxRule_Value, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	fgSizer2->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	fgSizer2->Add( 0, 0, 1, wxEXPAND, 5 );
	
	static_layerlabel = new wxStaticText( this, wxID_ANY, wxT("Layer Label:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_layerlabel->Wrap( -1 );
	fgSizer2->Add( static_layerlabel, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	wxArrayString wxSelect_LayerChoices;
	wxSelect_Layer = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSelect_LayerChoices, 0 );
	wxSelect_Layer->SetSelection( 0 );
	fgSizer2->Add( wxSelect_Layer, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	static_ldescript = new wxStaticText( this, wxID_ANY, wxT("Description:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_ldescript->Wrap( -1 );
	fgSizer2->Add( static_ldescript, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	static_LayerDesc = new wxStaticText( this, wxID_ANY, wxT("Layer Description"), wxDefaultPosition, wxDefaultSize, 0 );
	static_LayerDesc->Wrap( -1 );
	fgSizer2->Add( static_LayerDesc, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	static_cif = new wxStaticText( this, wxID_ANY, wxT("CIF:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_cif->Wrap( -1 );
	fgSizer2->Add( static_cif, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	wxCIF = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer2->Add( wxCIF, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	static_gds = new wxStaticText( this, wxID_ANY, wxT("GDSII:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_gds->Wrap( -1 );
	fgSizer2->Add( static_gds, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxGDSII = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer2->Add( wxGDSII, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	static_tech = new wxStaticText( this, wxID_ANY, wxT("Tech:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_tech->Wrap( -1 );
	fgSizer2->Add( static_tech, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	wxTechVal = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer2->Add( wxTechVal, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	fgSizer2->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	fgSizer2->Add( 0, 0, 1, wxEXPAND, 5 );
	
	save_button = new wxButton( this, wxID_ANY, wxT("&Save"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer2->Add( save_button, 0, wxALL, 5 );
	
	close_button = new wxButton( this, wxID_ANY, wxT("&Close"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer2->Add( close_button, 0, wxALL, 5 );
	
	this->SetSizer( fgSizer2 );
	this->Layout();
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( Design_Rules::hide ) );
	this->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Design_Rules::PressedKey ) );
	wxTechName->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Design_Rules::PressedKey ), NULL, this );
	wxSOI->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Design_Rules::PressedKey ), NULL, this );
	wxMLayers->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Design_Rules::PressedKey ), NULL, this );
	wxResolution->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Design_Rules::PressedKey ), NULL, this );
	wxSelect_Rule->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( Design_Rules::selectRule ), NULL, this );
	wxSelect_Rule->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Design_Rules::PressedKey ), NULL, this );
	wxRule_Value->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Design_Rules::PressedKey ), NULL, this );
	wxSelect_Layer->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( Design_Rules::selectLayer ), NULL, this );
	wxSelect_Layer->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Design_Rules::PressedKey ), NULL, this );
	wxCIF->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Design_Rules::PressedKey ), NULL, this );
	wxGDSII->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Design_Rules::PressedKey ), NULL, this );
	wxTechVal->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Design_Rules::PressedKey ), NULL, this );
	save_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Design_Rules::saveButtonEvt ), NULL, this );
	save_button->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Design_Rules::PressedKey ), NULL, this );
	close_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Design_Rules::closeButtonEvt ), NULL, this );
	close_button->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Design_Rules::PressedKey ), NULL, this );
}

Design_Rules::~Design_Rules()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( Design_Rules::hide ) );
	this->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Design_Rules::PressedKey ) );
	wxTechName->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Design_Rules::PressedKey ), NULL, this );
	wxSOI->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Design_Rules::PressedKey ), NULL, this );
	wxMLayers->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Design_Rules::PressedKey ), NULL, this );
	wxResolution->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Design_Rules::PressedKey ), NULL, this );
	wxSelect_Rule->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( Design_Rules::selectRule ), NULL, this );
	wxSelect_Rule->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Design_Rules::PressedKey ), NULL, this );
	wxRule_Value->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Design_Rules::PressedKey ), NULL, this );
	wxSelect_Layer->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( Design_Rules::selectLayer ), NULL, this );
	wxSelect_Layer->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Design_Rules::PressedKey ), NULL, this );
	wxCIF->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Design_Rules::PressedKey ), NULL, this );
	wxGDSII->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Design_Rules::PressedKey ), NULL, this );
	wxTechVal->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Design_Rules::PressedKey ), NULL, this );
	save_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Design_Rules::saveButtonEvt ), NULL, this );
	save_button->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Design_Rules::PressedKey ), NULL, this );
	close_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Design_Rules::closeButtonEvt ), NULL, this );
	close_button->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Design_Rules::PressedKey ), NULL, this );
}

Specify_FP::Specify_FP( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer8;
	fgSizer8 = new wxFlexGridSizer( 2, 4, 0, 0 );
	fgSizer8->SetFlexibleDirection( wxBOTH );
	fgSizer8->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	static_rows = new wxStaticText( this, wxID_ANY, wxT("# Rows:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_rows->Wrap( -1 );
	fgSizer8->Add( static_rows, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	wxNrRows = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer8->Add( wxNrRows, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	fgSizer8->Add( 0, 0, 1, wxEXPAND, 5 );
	
	ok_button1 = new wxButton( this, wxID_ANY, wxT("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer8->Add( ok_button1, 0, wxALL, 5 );
	
	static_coreutil = new wxStaticText( this, wxID_ANY, wxT("Core Utilization:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_coreutil->Wrap( -1 );
	fgSizer8->Add( static_coreutil, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	wxUtilization = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer8->Add( wxUtilization, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	fgSizer8->Add( 0, 0, 1, wxEXPAND, 5 );
	
	cancel_button = new wxButton( this, wxID_ANY, wxT("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer8->Add( cancel_button, 0, wxALL, 5 );
	
	static_margin = new wxStaticText( this, wxID_ANY, wxT("Margins:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_margin->Wrap( -1 );
	fgSizer8->Add( static_margin, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	
	fgSizer8->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	fgSizer8->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	fgSizer8->Add( 0, 0, 1, wxEXPAND, 5 );
	
	static_cleft = new wxStaticText( this, wxID_ANY, wxT("Core to Left:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_cleft->Wrap( -1 );
	fgSizer8->Add( static_cleft, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	wxC2L = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer8->Add( wxC2L, 0, wxALL, 5 );
	
	static_ctop = new wxStaticText( this, wxID_ANY, wxT("Core to Top:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_ctop->Wrap( -1 );
	fgSizer8->Add( static_ctop, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	wxC2T = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer8->Add( wxC2T, 0, wxALL, 5 );
	
	static_cright = new wxStaticText( this, wxID_ANY, wxT("Core to Right:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_cright->Wrap( -1 );
	fgSizer8->Add( static_cright, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	wxC2R = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer8->Add( wxC2R, 0, wxALL, 5 );
	
	static_cbottom = new wxStaticText( this, wxID_ANY, wxT("Core to Bottom:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_cbottom->Wrap( -1 );
	fgSizer8->Add( static_cbottom, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	wxC2B = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer8->Add( wxC2B, 0, wxALL, 5 );
	
	this->SetSizer( fgSizer8 );
	this->Layout();
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( Specify_FP::hide ) );
	this->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Specify_FP::PressedKey ) );
	wxNrRows->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Specify_FP::PressedKey ), NULL, this );
	ok_button1->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Specify_FP::okButtonEvt ), NULL, this );
	ok_button1->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Specify_FP::PressedKey ), NULL, this );
	wxUtilization->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Specify_FP::PressedKey ), NULL, this );
	cancel_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Specify_FP::cancelButtonEvt ), NULL, this );
	cancel_button->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Specify_FP::PressedKey ), NULL, this );
	wxC2L->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Specify_FP::PressedKey ), NULL, this );
	wxC2T->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Specify_FP::PressedKey ), NULL, this );
	wxC2R->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Specify_FP::PressedKey ), NULL, this );
	wxC2B->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Specify_FP::PressedKey ), NULL, this );
}

Specify_FP::~Specify_FP()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( Specify_FP::hide ) );
	this->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Specify_FP::PressedKey ) );
	wxNrRows->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Specify_FP::PressedKey ), NULL, this );
	ok_button1->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Specify_FP::okButtonEvt ), NULL, this );
	ok_button1->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Specify_FP::PressedKey ), NULL, this );
	wxUtilization->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Specify_FP::PressedKey ), NULL, this );
	cancel_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Specify_FP::cancelButtonEvt ), NULL, this );
	cancel_button->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Specify_FP::PressedKey ), NULL, this );
	wxC2L->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Specify_FP::PressedKey ), NULL, this );
	wxC2T->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Specify_FP::PressedKey ), NULL, this );
	wxC2R->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Specify_FP::PressedKey ), NULL, this );
	wxC2B->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Specify_FP::PressedKey ), NULL, this );
}

Help_About::Help_About( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 520,720 ), wxSize( 520,720 ) );
	
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxVERTICAL );
	
	richtxt_about = new wxRichTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxALWAYS_SHOW_SB|wxNO_BORDER|wxWANTS_CHARS );
	richtxt_about->Enable( false );
	
	bSizer11->Add( richtxt_about, 1, wxALL|wxEXPAND, 0 );
	
	this->SetSizer( bSizer11 );
	this->Layout();
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( Help_About::hide ) );
}

Help_About::~Help_About()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( Help_About::hide ) );
}

View_PrintGP::View_PrintGP( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer6;
	fgSizer6 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer6->SetFlexibleDirection( wxBOTH );
	fgSizer6->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	static_design13 = new wxStaticText( this, wxID_ANY, wxT("Optimize:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_design13->Wrap( -1 );
	fgSizer6->Add( static_design13, 0, wxALL, 5 );
	
	wxString optimize_choiceChoices[] = { wxT("delay"), wxT("area") };
	int optimize_choiceNChoices = sizeof( optimize_choiceChoices ) / sizeof( wxString );
	optimize_choice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, optimize_choiceNChoices, optimize_choiceChoices, 0 );
	optimize_choice->SetSelection( 0 );
	fgSizer6->Add( optimize_choice, 0, wxALL, 5 );
	
	static_design1 = new wxStaticText( this, wxID_ANY, wxT("Sizing Type:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_design1->Wrap( -1 );
	fgSizer6->Add( static_design1, 0, wxALL, 5 );
	
	wxString sizingtype_choiceChoices[] = { wxT("gate"), wxT("transistor") };
	int sizingtype_choiceNChoices = sizeof( sizingtype_choiceChoices ) / sizeof( wxString );
	sizingtype_choice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, sizingtype_choiceNChoices, sizingtype_choiceChoices, 0 );
	sizingtype_choice->SetSelection( 0 );
	fgSizer6->Add( sizingtype_choice, 0, wxALL, 5 );
	
	static_design12 = new wxStaticText( this, wxID_ANY, wxT("Technology:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_design12->Wrap( -1 );
	fgSizer6->Add( static_design12, 0, wxALL, 5 );
	
	wxString technology_choiceChoices[] = { wxT("45nm"), wxT("350nm") };
	int technology_choiceNChoices = sizeof( technology_choiceChoices ) / sizeof( wxString );
	technology_choice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, technology_choiceNChoices, technology_choiceChoices, 0 );
	technology_choice->SetSelection( 0 );
	fgSizer6->Add( technology_choice, 0, wxALL, 5 );
	
	
	fgSizer6->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	fgSizer6->Add( 0, 0, 1, wxEXPAND, 5 );
	
	static_design11 = new wxStaticText( this, wxID_ANY, wxT("Parameters:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_design11->Wrap( -1 );
	fgSizer6->Add( static_design11, 0, wxALL, 5 );
	
	
	fgSizer6->Add( 0, 0, 1, wxEXPAND, 5 );
	
	static_design112 = new wxStaticText( this, wxID_ANY, wxT("Cload:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_design112->Wrap( -1 );
	fgSizer6->Add( static_design112, 0, wxALL, 5 );
	
	parameterCload = new wxTextCtrl( this, wxID_ANY, wxT("4*1.434879e-16"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( parameterCload, 0, wxALL, 5 );
	
	static_design111 = new wxStaticText( this, wxID_ANY, wxT("Max Area:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_design111->Wrap( -1 );
	fgSizer6->Add( static_design111, 0, wxALL, 5 );
	
	parameterMaxArea = new wxTextCtrl( this, wxID_ANY, wxT("3.255"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( parameterMaxArea, 0, wxALL, 5 );
	
	static_design1111 = new wxStaticText( this, wxID_ANY, wxT("Delay:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_design1111->Wrap( -1 );
	fgSizer6->Add( static_design1111, 0, wxALL, 5 );
	
	parameterDelay = new wxTextCtrl( this, wxID_ANY, wxT("4.29113e-10"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( parameterDelay, 0, wxALL, 5 );
	
	static_design11111 = new wxStaticText( this, wxID_ANY, wxT("Max Cin:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_design11111->Wrap( -1 );
	fgSizer6->Add( static_design11111, 0, wxALL, 5 );
	
	parameterMaxCin = new wxTextCtrl( this, wxID_ANY, wxT("1.0001"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( parameterMaxCin, 0, wxALL, 5 );
	
	ok_button = new wxButton( this, wxID_ANY, wxT("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( ok_button, 0, wxALL, 5 );
	
	cancel_button = new wxButton( this, wxID_ANY, wxT("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( cancel_button, 0, wxALL, 5 );
	
	this->SetSizer( fgSizer6 );
	this->Layout();
	
	// Connect Events
	parameterCload->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_PrintGP::PressedKey ), NULL, this );
	parameterCload->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( View_PrintGP::applyButtonEvt ), NULL, this );
	parameterMaxArea->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_PrintGP::PressedKey ), NULL, this );
	parameterMaxArea->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( View_PrintGP::applyButtonEvt ), NULL, this );
	parameterDelay->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_PrintGP::PressedKey ), NULL, this );
	parameterDelay->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( View_PrintGP::applyButtonEvt ), NULL, this );
	parameterMaxCin->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_PrintGP::PressedKey ), NULL, this );
	parameterMaxCin->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( View_PrintGP::applyButtonEvt ), NULL, this );
	ok_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( View_PrintGP::okButtonEvt ), NULL, this );
	ok_button->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_PrintGP::PressedKey ), NULL, this );
	cancel_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( View_PrintGP::cancelButtonEvt ), NULL, this );
	cancel_button->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_PrintGP::PressedKey ), NULL, this );
}

View_PrintGP::~View_PrintGP()
{
	// Disconnect Events
	parameterCload->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_PrintGP::PressedKey ), NULL, this );
	parameterCload->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( View_PrintGP::applyButtonEvt ), NULL, this );
	parameterMaxArea->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_PrintGP::PressedKey ), NULL, this );
	parameterMaxArea->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( View_PrintGP::applyButtonEvt ), NULL, this );
	parameterDelay->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_PrintGP::PressedKey ), NULL, this );
	parameterDelay->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( View_PrintGP::applyButtonEvt ), NULL, this );
	parameterMaxCin->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_PrintGP::PressedKey ), NULL, this );
	parameterMaxCin->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( View_PrintGP::applyButtonEvt ), NULL, this );
	ok_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( View_PrintGP::okButtonEvt ), NULL, this );
	ok_button->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_PrintGP::PressedKey ), NULL, this );
	cancel_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( View_PrintGP::cancelButtonEvt ), NULL, this );
	cancel_button->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_PrintGP::PressedKey ), NULL, this );
}
