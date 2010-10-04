#include "wxautocell.h"

WxAutoCell::WxAutoCell( IcpdFrm* frmwork ):CellGen( frmwork ){
	currentFrmwork=frmwork;
	update();
}

void WxAutoCell::PressedKey( wxKeyEvent& event ){
	if (event.GetKeyCode() == WXK_ESCAPE)
		Show(false);
	event.Skip();
}

void WxAutoCell::hide( wxCloseEvent& event ){
    Show(false);
}

/*void WxAutoCell::generateAllCells( wxCommandEvent& event ){
	for(wxArrayString::iterator cells_it=cellnet_choice->GetStrings().begin(); cells_it!=cellnet_choice->GetStrings().end(); cells_it++)
		generateCell(cells_it->fi
		cellnet_choice->Append(cells_it-first.c_str());
    cellnet_choice->Select(0);
}*/

void WxAutoCell::generateCell( wxCommandEvent& event ){
	cout << "CellGen: Auto generating cell " << cellnet_choice->GetStringSelection() << endl;	

	// Select cell
	wxString cmd="cellgen set pos_tracks " + s_TP->GetValue();
	currentFrmwork->readCommand(cmd.c_str());
	cmd="cellgen set nr_tracks " + s_RN->GetValue();
	currentFrmwork->readCommand(cmd.c_str());
	cmd="cellgen select " + cellnet_choice->GetStringSelection();
	currentFrmwork->readCommand(cmd.c_str());

	// Fold Transistors
	currentFrmwork->readCommand("cellgen fold");

	// Place
	cmd="cellgen place " + p_TAQ->GetValue() + " " + p_NRA->GetValue() + " " + p_WC->GetValue() + " " + p_GMC->GetValue() + " " + p_RC->GetValue() + " " + p_RTDC->GetValue() + " " + p_NGC->GetValue();
	currentFrmwork->readCommand(cmd.c_str());

	// Route
	cmd="cellgen route " + rc_M->GetValue() + " " + rc_P->GetValue() + " " + rc_C->GetValue() + " " + rc_IOP->GetValue();
	currentFrmwork->readCommand(cmd.c_str());

	// Compact Layout
	cmd="cellgen compact " + cp_M->GetValue() + " " + cp_P->GetValue() + " " + cp_GS->GetValue() + " " + cp_W->GetValue();
	currentFrmwork->readCommand(cmd.c_str());
	update();
}

void WxAutoCell::select( wxCommandEvent& event ){
	wxString cmd="cellgen set pos_tracks " + s_TP->GetValue();
	currentFrmwork->readCommand(cmd.c_str());
	cmd="cellgen set nr_tracks " + s_RN->GetValue();
	currentFrmwork->readCommand(cmd.c_str());
	cmd="cellgen select " + cellnet_choice->GetStringSelection();
	currentFrmwork->readCommand(cmd.c_str());
	update();
}

void WxAutoCell::fold( wxCommandEvent& event ){
	currentFrmwork->readCommand("cellgen fold");
	update();
}

void WxAutoCell::place( wxCommandEvent& event ){
	wxString cmd="cellgen place " + p_TAQ->GetValue() + " " + p_NRA->GetValue() + " " + p_WC->GetValue() + " " + p_GMC->GetValue() + " " + p_RC->GetValue() + " " + p_RTDC->GetValue() + " " + p_NGC->GetValue();
	currentFrmwork->readCommand(cmd.c_str());
	update();
}

void WxAutoCell::route( wxCommandEvent& event ){
	wxString cmd="cellgen route " + rc_M->GetValue() + " " + rc_P->GetValue() + " " + rc_C->GetValue() + " " + rc_IOP->GetValue();
	currentFrmwork->readCommand(cmd.c_str());
	update();
}

void WxAutoCell::compact( wxCommandEvent& event ){
	wxString cmd="cellgen compact " + cp_M->GetValue() + " " + cp_P->GetValue() + " " + cp_GS->GetValue() + " " + cp_W->GetValue();
	currentFrmwork->readCommand(cmd.c_str());
	update();
}

void WxAutoCell::view( wxCommandEvent& event ){
	currentFrmwork->viewLayout(cellnet_choice->GetStringSelection().c_str());
}

void WxAutoCell::refresh(){
	// append the itens (cells) for the selection box
	cellnet_choice->Clear();
	for(map<string, CellNetlst>::iterator cells_it=currentFrmwork->getDesign()->getCircuit()->getCellNetlsts()->begin(); cells_it!=currentFrmwork->getDesign()->getCircuit()->getCellNetlsts()->end(); cells_it++)
		cellnet_choice->Append(cells_it->first.c_str());
    cellnet_choice->Select(0);
}

void WxAutoCell::update(){
	// update the states to follow the cellgen's flow
	int state=0;
	if(currentFrmwork->getDesign()->getAutoCell())
		state=currentFrmwork->getDesign()->getAutoCell()->getState();
	fold_button->Enable(state>=2);
	place_button->Enable(state>=3);
	route_button->Enable(state>=4);
	compact_button->Enable(state>=5);
	view_button->Enable(state>=6);
	Update();
}