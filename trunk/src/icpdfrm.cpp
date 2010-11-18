#include "icpdfrm.h"

#include "images.h"

/** Constructor */
IcpdFrm::IcpdFrm( wxWindow* parent ):ICPD_frm( parent ){
	wxLog::SetActiveTarget( new wxLogTextCtrl(wx_log));
	
    new Redirector( wx_log, cout, false);
    //new Redirector( wx_log, cerr, true);

	printHeader(cout, "", "");

	readCommand("read \"icpd.cfg\""); // [BUG] On debug mode!
		
	
	wxabout = new WxAbout(this);
	wxrules = new WxRules(this);
	wxautocell = new WxAutoCell(this);
	wxcircuit = new WxCircuit(this);
	wxfp = new WxFP(this);
	wxpreferences = new WxPreferences(this);
	wxprintgp = new WxPrintGP(this);
	refresh();

	#ifdef __WXMSW__
	load_technology->SetBitmaps( wxGetBitmapFromMemory(load_tec) );
	#elif defined( __WXGTK__ )
	load_technology->SetBitmap( wxGetBitmapFromMemory(load_tec) );
	#endif	
	
	
	
	
}

// functions that implement events

// Frame events
void IcpdFrm::OnClose(wxCloseEvent& event){
	readCommand("exit");
}

void IcpdFrm::CmdKeyDown( wxKeyEvent& event ){
	pos = designmng.getPosCmdLog();

	if (event.GetKeyCode() == WXK_UP && pos != 0){
		--pos;
		designmng.setPosCmdLog(pos);
		wxCommand->SetValue(designmng.getCommandLog()[pos].c_str());
		wxCommand->SetInsertionPointEnd();
	}
	else if (event.GetKeyCode() == WXK_DOWN && pos != designmng.getCommandLog().size()){
			++pos;
			if (pos == designmng.getCommandLog().size())
				wxCommand->SetValue("");
			else
				wxCommand->SetValue(designmng.getCommandLog()[pos].c_str());
			designmng.setPosCmdLog(pos);
			wxCommand->SetInsertionPointEnd();
	}else if (event.GetKeyCode() == WXK_ESCAPE)
		wxCommand->SetValue("");
	else if (event.GetKeyCode() == WXK_F5)
		refresh();
	else if (event.GetKeyCode() == WXK_F1)
		wxabout->Show();
	else event.Skip();
}

void IcpdFrm::PressedKey( wxKeyEvent& event ){
	if (event.GetKeyCode() == WXK_F5)
		refresh();
	else if (event.GetKeyCode() == WXK_F1)
		wxabout->Show();
	event.Skip();
}

// File->New
void IcpdFrm::newDesign( wxCommandEvent& event ){
	wxString cmd = wxGetTextFromUser(wxT("Enter the name: "),wxT("New Design"),wxT(""));
	if(cmd!=""){
		cmd="new design \"" + cmd + "\"";
		readCommand(cmd.c_str());

		instanceLevel.clear();
		refresh();
	}
}

void IcpdFrm::newCell( wxCommandEvent& event ){
	wxString cmd = wxGetTextFromUser(wxT("Enter the cell name: "),wxT("Add a new cell netlist"),wxT(""));
	if(cmd!=""){
		cmd="new cell " + cmd;
		readCommand(cmd.c_str());
		refresh();
	}
}

// File->Load
void IcpdFrm::loadProject( wxCommandEvent& event ){
	wxFileDialog* tmp =  new wxFileDialog(this, wxT("Open Project"), wxT(""), wxT(""), wxT("ICPD Project File (*.icp)|*.icp|All Files (*.*)|*.*"), wxOPEN);
	if ( tmp->ShowModal() == wxID_OK ){
		wxString cmd = tmp->GetDirectory()+"/"+tmp->GetFilename();
		cmd = "load project \"" + cmd + "\"";
		readCommand(cmd.c_str());
		refresh();
    }
}

void IcpdFrm::loadTech( wxCommandEvent& event ){
	wxFileDialog* tmp =  new wxFileDialog(this, wxT("Open Technology"), wxT(""), wxT(""), wxT("Rules File (*.rul)|*.rul|All Files (*.*)|*.*"), wxOPEN);
	if ( tmp->ShowModal() == wxID_OK ){
		wxString cmd = tmp->GetDirectory()+"/"+tmp->GetFilename();
		cmd = "load technology \"" + cmd + "\"";
		readCommand(cmd.c_str());
    }
}

void IcpdFrm::loadSpiceNetlist( wxCommandEvent& event ){
	wxFileDialog* tmp =  new wxFileDialog(this, wxT("Open Netlist"), wxT(""), wxT(""), wxT("Spice (*.sp)|*.sp|Structural Verilog (*.v)|*.v|All Files (*.*)|*.*"), wxOPEN);
	if ( tmp->ShowModal() == wxID_OK ){
		wxString cmd = tmp->GetDirectory()+"/"+tmp->GetFilename();
		cmd = "load netlist \"" + cmd + "\"";
		readCommand(cmd.c_str());
		refresh();
	}
}

void IcpdFrm::loadLayouts( wxCommandEvent& event ){
	wxFileDialog* tmp =  new wxFileDialog(this, wxT("Open Layouts"), wxT(""), wxT(""), wxT("ICPD Layouts Format (*.lay)|*.lay|All Files (*.*)|*.*"), wxOPEN);
	if ( tmp->ShowModal() == wxID_OK ){
		wxString cmd = tmp->GetDirectory()+"/"+tmp->GetFilename();
		cmd = "load layouts \"" + cmd + "\"";
		readCommand(cmd.c_str());
		refresh();
	}
}

void IcpdFrm::loadPlacement( wxCommandEvent& event ){
	wxFileDialog* tmp =  new wxFileDialog(this, wxT("Open Placement"), wxT(""), wxT(""), wxT("Book Shelf (*.pl)|*.pl|Mango Parrot (*.mpp)|*.mpp|All Files (*.*)|*.*"), wxOPEN);
	if ( tmp->ShowModal() == wxID_OK ){
		wxString cmd = tmp->GetDirectory()+"/"+tmp->GetFilename();
		cmd = "load placement \"" + cmd + "\"";
		readCommand(cmd.c_str());
		refresh();
	}
}

void IcpdFrm::loadRouting( wxCommandEvent& event ){
	wxFileDialog* tmp =  new wxFileDialog(this, wxT("Open Routing"), wxT(""), wxT(""), wxT("Rotdl Routing Format (*.rot)|*.rot|All Files (*.*)|*.*"), wxOPEN);
	if ( tmp->ShowModal() == wxID_OK ){
		wxString cmd = tmp->GetDirectory()+"/"+tmp->GetFilename();
		cmd = "load routing \"" + cmd + "\"";
		readCommand(cmd.c_str());
		refresh();
	}
}

// File->Save
void IcpdFrm::saveProject( wxCommandEvent& event ){
	wxFileDialog* tmp =  new wxFileDialog(this, wxT("Save Project"), wxT(""), wxT(""), wxT("ICPD Project File (*.icp)|*.icp"), wxSAVE|wxOVERWRITE_PROMPT);
	if ( tmp->ShowModal() == wxID_OK ){
		wxString cmd = tmp->GetDirectory()+"/"+tmp->GetFilename();
		cmd = "save project \"" + cmd + "\"";
		readCommand(cmd.c_str());
	}
}

void IcpdFrm::saveTech( wxCommandEvent& event ){
	wxFileDialog* tmp =  new wxFileDialog(this, wxT("Save Rules"), wxT(""), wxT(""), wxT("Rules File (*.rul)|*.rul"), wxSAVE|wxOVERWRITE_PROMPT);
	if ( tmp->ShowModal() == wxID_OK ){
		wxString cmd = tmp->GetDirectory()+"/"+tmp->GetFilename();
		cmd = "save technology \"" + cmd + "\"";
		readCommand(cmd.c_str());
	}
}

void IcpdFrm::saveSpiceNetlist( wxCommandEvent& event ){
	wxFileDialog* tmp =  new wxFileDialog(this, wxT("Save Netlist"), wxT(""), wxT(""), wxT("Spice (*.sp)|*.sp"), wxSAVE|wxOVERWRITE_PROMPT);
	if ( tmp->ShowModal() == wxID_OK ){
		wxString cmd = tmp->GetDirectory()+"/"+tmp->GetFilename();
		cmd = "save netlist \"" + cmd + "\"";
		readCommand(cmd.c_str());
	}
}

void IcpdFrm::saveLayouts( wxCommandEvent& event ){
	wxFileDialog* tmp =  new wxFileDialog(this, wxT("Save Layouts"), wxT(""), wxT("project.lay"), wxT("ICPD Layouts File (*.lay)|*.lay"), wxSAVE|wxOVERWRITE_PROMPT);
	if ( tmp->ShowModal() == wxID_OK ){
		wxString cmd = tmp->GetDirectory()+"/"+tmp->GetFilename();
		cmd = "save layouts \"" + cmd + "\"";
		readCommand(cmd.c_str());
	}
}

void IcpdFrm::savePlacement( wxCommandEvent& event ){
	wxFileDialog* tmp =  new wxFileDialog(this, wxT("Save Placement"), wxT(""), wxT(""), wxT("Book Shelf (*.pl)|*.pl"), wxSAVE|wxOVERWRITE_PROMPT);
	if ( tmp->ShowModal() == wxID_OK ){
		wxString cmd = tmp->GetDirectory()+"/"+tmp->GetFilename();
		cmd = "save placement \"" + cmd + "\"";
		readCommand(cmd.c_str());
	}
}

void IcpdFrm::saveRouting( wxCommandEvent& event ){
	wxFileDialog* tmp =  new wxFileDialog(this, wxT("Save Routing"), wxT(""), wxT(""), wxT("Rotdl Routing Format (*.rot)|*.rot"), wxSAVE|wxOVERWRITE_PROMPT);
	if ( tmp->ShowModal() == wxID_OK ){
		wxString cmd = tmp->GetDirectory()+"/"+tmp->GetFilename();
		cmd = "save routing \"" + cmd + "\"";
		readCommand(cmd.c_str());
	}
}

// File->Import
void IcpdFrm::importSpiceNetlist( wxCommandEvent& event ){
	wxFileDialog* tmp =  new wxFileDialog(this, wxT("Import Netlist"), wxT(""), wxT(""), wxT("All Files (*.*)|*.*|Spice (*.sp)|*.sp"), wxOPEN);
	if ( tmp->ShowModal() == wxID_OK ){
		wxString cmd = tmp->GetDirectory()+"/"+tmp->GetFilename();
		cmd = "import netlist \"" + cmd + "\"";
		readCommand(cmd.c_str());
		refresh();
	}
}

void IcpdFrm::importLEF( wxCommandEvent& event ){
	wxFileDialog* tmp =  new wxFileDialog(this, wxT("Import Cell Library"), wxT(""), wxT(""), wxT("Library Exchange Format (*.lef)|*.lef|All Files (*.*)|*.*"), wxOPEN);
	if ( tmp->ShowModal() == wxID_OK ){
		wxString cmd = tmp->GetDirectory()+"/"+tmp->GetFilename();
		cmd = "import lef \"" + cmd + "\"";
		readCommand(cmd.c_str());
		refresh();
	}
}

// File->Export
void IcpdFrm::exportLayout( wxCommandEvent& event ){
	wxFileDialog* tmp =  new wxFileDialog(this, wxT("Save Layout"), wxT(""), wxT(wx_layouts->GetStringSelection()+".cif"), wxT("Calltech Intermediate Format (*.cif)|*.cif|Graphic Data System II (*.gds)|*.gds|CIF2Cadende Convertion File (*.c2c)|*.c2c|Sight Viewer (*.sight)|*.sight"), wxSAVE|wxOVERWRITE_PROMPT);
	if ( tmp->ShowModal() == wxID_OK ){
		wxString cmd = tmp->GetDirectory()+"/"+tmp->GetFilename();
		cmd = "export layout " + wx_layouts->GetStringSelection() + " \"" + cmd + "\"";
		readCommand(cmd.c_str());
	}
}

void IcpdFrm::exportCellSizes( wxCommandEvent& event ){
	wxFileDialog* tmp =  new wxFileDialog(this, wxT("Save Library Dimensions"), wxT(""), wxT(""), wxT("Mango Parrot Cell estimative File (*.est)|*.est"), wxSAVE|wxOVERWRITE_PROMPT);
	if ( tmp->ShowModal() == wxID_OK ){
		wxString cmd = tmp->GetDirectory()+"/"+tmp->GetFilename();
		cmd = "export cellsizes \"" + cmd + "\"";
		readCommand(cmd.c_str());
	}
}

void IcpdFrm::exportPlacement( wxCommandEvent& event ){
	wxFileDialog* tmp =  new wxFileDialog(this, wxT("Save Placement"), wxT(""), wxT(""), wxT("Cadence Placement File (*.place)|*.place"), wxSAVE|wxOVERWRITE_PROMPT);
	if ( tmp->ShowModal() == wxID_OK ){
		wxString cmd = tmp->GetDirectory()+"/"+tmp->GetFilename();
		cmd = "export placement \"" + cmd + "\"";
		readCommand(cmd.c_str());
	}
}

// File Script
void IcpdFrm::flReadScript( wxCommandEvent& event ){
	wxFileDialog* tmp =  new wxFileDialog(this, wxT("Read Script"), wxT(""), wxT(""), wxT("Script File (*.run)|*.run|All Files (*.*)|*.*"), wxOPEN);
	if ( tmp->ShowModal() == wxID_OK ){
		wxString cmd = tmp->GetDirectory()+"/"+tmp->GetFilename();
		cmd = "read \"" + cmd + "\"";
		readCommand(cmd.c_str());
		refresh();
	}
}

void IcpdFrm::saveHistory( wxCommandEvent& event ){
	wxFileDialog* tmp =  new wxFileDialog(this, wxT("Save History"), wxT(""), wxT(""), wxT("Script File (*.run)|*.run"), wxSAVE|wxOVERWRITE_PROMPT);
	if ( tmp->ShowModal() == wxID_OK ){
		wxString cmd = tmp->GetDirectory()+"/"+tmp->GetFilename();
		cmd = "save history \"" + cmd + "\"";
		readCommand(cmd.c_str());
	}
}

// File Preferences
void IcpdFrm::showPreferences( wxCommandEvent& event ){
	wxpreferences->Show();
}

// File Exit
void IcpdFrm::OnFileExit( wxCommandEvent& event ){
	readCommand("exit");
}

// Circuit
void IcpdFrm::showTech( wxCommandEvent& event ){
	wxrules->Show();
}

void IcpdFrm::showCircuit( wxCommandEvent& event ){
	wxcircuit->Show();
}

// Circuit -> Size
void IcpdFrm::sizeFO4( wxCommandEvent& event ){
	readCommand("size fo4");
}

void IcpdFrm::elmoreDelay( wxCommandEvent& event ){
	readCommand("elmore fo4");
	
}

void IcpdFrm::sizeGP( wxCommandEvent& event ){
	//readCommand("size gp");
	wxprintgp->Show();
}

// Cells
void IcpdFrm::showCellgen( wxCommandEvent& event ){
	wxautocell->Show();
}

void IcpdFrm::calcPinsPos( wxCommandEvent& event ){
	readCommand("calcPinsPos");
}

// Place
void IcpdFrm::showFP( wxCommandEvent& event ){
	wxfp->Show();
}

void IcpdFrm::plTerminals( wxCommandEvent& event ){
	readCommand("place terminals");
}

void IcpdFrm::fastp( wxCommandEvent& event ){
	readCommand("place fplace");
}

void IcpdFrm::plVerify( wxCommandEvent& event ){
	readCommand("place check");
}

void IcpdFrm::plAutoFlip( wxCommandEvent& event ){
	readCommand("place autoflip");
}

void IcpdFrm::plIncremental( wxCommandEvent& event ){
	readCommand("place incremental");
}

void IcpdFrm::plWL( wxCommandEvent& event ){
	readCommand("place getwl");
}

// Route
void IcpdFrm::rotdl( wxCommandEvent& event ){
	readCommand("route rotdl");
	refresh();
}

void IcpdFrm::pfinder( wxCommandEvent& event ){
	wxString cmd = wxGetTextFromUser(wxT("Enter the number of iterations (ex. 1000): "),wxT("Negociation Based Router"),wxT(""));
	if(cmd!=""){
		cmd="route pfinder " + cmd;
		readCommand(cmd.c_str());
		refresh();
	}
}

void IcpdFrm::rtOptimize( wxCommandEvent& event ){
	readCommand("route optimize");
	refresh();
}

void IcpdFrm::rtCompact( wxCommandEvent& event ){
	readCommand("route compact");
	refresh();
}

void IcpdFrm::rtClear( wxCommandEvent& event ){
	readCommand("route clear");
	refresh();
}

// Help
void IcpdFrm::HelpaboutClick( wxCommandEvent& event ){
	wxabout->Show();
}

void IcpdFrm::helpHP( wxCommandEvent& event ){
	wxLaunchDefaultBrowser(wxT("http://www.inf.ufrgs.br/~amziesemerj/icpd"));
}

// Tools
void IcpdFrm::refreshIcon( wxCommandEvent& event ){
	refresh();
}

// List Boxes
void IcpdFrm::layoutListEvent( wxCommandEvent& event ){
	string selectedLayout=wx_layouts->GetStringSelection().c_str();
	instanceLevel.clear();
	instanceLevel.push_back(designmng.getCircuit()->getLayout(selectedLayout));
	refreshInstanceList();
}

void IcpdFrm::viewLayout( wxCommandEvent& event ){
	viewLayout(wx_layouts->GetStringSelection().c_str());
}

void IcpdFrm::viewLayout( string cellName ){
	if(designmng.getViewer()!=""){
		SaveLayout slayout;
		slayout.saveCif("temp.cif", *(designmng.getRules()), *(designmng.getCircuit()->getLayouts()), cellName);
		string cmd = designmng.getViewer() + " temp.cif";
		cout << "Opening viewer: " << cmd << endl;
		icpdExecute(cmd);
	}
}

void IcpdFrm::openInstancewindow( wxCommandEvent& event ){
	//wxinstance->open(wx_layouts->GetStringSelection(), wx_instances->GetStringSelection());
}

void IcpdFrm::printInstance( wxCommandEvent& event ){
	if(wx_instances->GetStringSelection()==".."){
		instanceLevel.pop_back();
		refreshInstanceList();
	}else{
		wxString cmd = "print instance ";
		cmd += instanceLevel.back()->getName().c_str();
		cmd += " " + wx_instances->GetStringSelection();
		readCommand(cmd.c_str());
		CLayout *tmp=designmng.getCircuit()->getLayout(instanceLevel.back()->getInstance(wx_instances->GetStringSelection().c_str())->getTargetCell());
		if(tmp){
			instanceLevel.push_back(tmp);
			refreshInstanceList();
		}
		else cout << "No further hierarchy" << endl;
	}
	wx_layouts->Select(wx_layouts->FindString(instanceLevel.back()->getName().c_str()));
}

void IcpdFrm::printCell( wxCommandEvent& event ){
	wxString cmd = "print cell " + wx_cells->GetStringSelection(); 
	readCommand(cmd.c_str());
}

void IcpdFrm::printNet( wxCommandEvent& event ){
	wxString cmd = "print net " + wx_nets->GetStringSelection();
	readCommand(cmd.c_str());
}

void IcpdFrm::printInterface( wxCommandEvent& event ){
	wxString cmd = "print interface " + wx_interfaces->GetStringSelection();
	readCommand(cmd.c_str());
}

// Shell
void IcpdFrm::command( wxCommandEvent& event ){
	string cmd=wxCommand->GetValue().c_str();
	wxCommand->SetValue(_T(""));
	readCommand(cmd.c_str());
}

void IcpdFrm::readCommand(string cmd){
	statusBar->SetStatusText(wxT("Executing command..."));
	designmng.readCommand(cmd);
	statusBar->SetStatusText(wxT(""));
}

/* sem uso!!
void IcpdFrm::selectCell(wxCommandEvent& event){
	wxString cmd = "select cell" + wx_layouts->GetStringSelection();
	readCommand(cmd.c_str());
}*/

// functions that don't implement events
void IcpdFrm::refresh(){
	wx_layouts->Clear();
	map<string, CLayout >::iterator layouts_it;
	for(layouts_it=designmng.getCircuit()->getLayouts()->begin(); layouts_it!=designmng.getCircuit()->getLayouts()->end(); layouts_it++)
		wx_layouts->Append(layouts_it->first.c_str());
	if (instanceLevel.size())
		wx_layouts->Select(wx_layouts->FindString(instanceLevel.back()->getName().c_str()));

	wx_cells->Clear();
	map<string, CellNetlst>::iterator cells_it;
	for(cells_it=designmng.getCircuit()->getCellNetlsts()->begin(); cells_it!=designmng.getCircuit()->getCellNetlsts()->end(); cells_it++)
		wx_cells->Append(cells_it->first.c_str());
		
	refreshInstanceList();

/*	wx_nets->Clear();
	map<string, list<Net> >::iterator tmp3_it;
	for(tmp3_it=designmng.getCircuit()->getNetList()->begin(); tmp3_it!=designmng.getCircuit()->getNetList()->end(); tmp3_it++)
		wx_nets->Append(tmp3_it->first.c_str());
    //wx_nets->Select(0);
*/
	wx_interfaces->Clear();
	map<string, Interface>::iterator tmp4_it;
	for(tmp4_it=designmng.getCircuit()->getInterfaces()->begin(); tmp4_it!=designmng.getCircuit()->getInterfaces()->end(); tmp4_it++)
		wx_interfaces->Append(tmp4_it->first.c_str());
    //wx_interfaces->Select(0);

	wxautocell->refresh();
	wxcircuit->refresh();
	wxrules->refresh();
	wxfp->refresh();
	wxpreferences->refresh();
	wxprintgp->refresh();
}

void IcpdFrm::refreshInstanceList(){
	wx_instances->Clear();
	if(instanceLevel.size()){
	if(instanceLevel.size()>1) wx_instances->Append("..");
	map<string, Instance>::iterator tmp2_it;
	for(tmp2_it=instanceLevel.back()->getInstances()->begin(); tmp2_it!=instanceLevel.back()->getInstances()->end(); tmp2_it++)
		wx_instances->Append(tmp2_it->first.c_str());
	}
    //wx_instances->Select(0);
}