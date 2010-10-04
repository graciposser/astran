#include "wxcircuit.h"

WxCircuit::WxCircuit( IcpdFrm* frmwork ):View_Circuit( frmwork ){
	currentFrmwork=frmwork;
	refresh();
}

void WxCircuit::PressedKey( wxKeyEvent& event ){
	if (event.GetKeyCode() == WXK_ESCAPE)
		cancel();
	event.Skip();
}

void WxCircuit::hide( wxCloseEvent& event ){
    Show(false);
}

void WxCircuit::okButtonEvt( wxCommandEvent& event ){
	ok();
}

void WxCircuit::cancelButtonEvt( wxCommandEvent& event ){
	cancel();
}

void WxCircuit::ok(){
	wxString cmd;

	// compare if current Design Name is different from textbox value
	if(string(design_name->GetValue()) != currentFrmwork->getDesign()->getName().c_str()){
		cmd = "set designName " + design_name->GetValue();
		currentFrmwork->readCommand(cmd.c_str());
	}
	
	// compare if current Top Cell is different from selection box
	if (cell_choice->GetStringSelection().c_str() != currentFrmwork->getDesign()->getCircuit()->getTopCell()){
		cmd="set topcell " + cell_choice->GetStringSelection();
		currentFrmwork->readCommand(cmd.c_str());
	}

	// compare if current horizontal grid is different from textbox value
	if(horizontalgrid->GetValue() != wxString::Format(_T("%f"), currentFrmwork->getDesign()->getCircuit()->getHPitch())){
		cmd = "set hgrid " + horizontalgrid->GetValue();
		currentFrmwork->readCommand(cmd.c_str());
	}

	// compare if current vertical grid is different from textbox value
	if(verticalgrid->GetValue() != wxString::Format(_T("%f"), currentFrmwork->getDesign()->getCircuit()->getVPitch())){
		cmd = "set vgrid " + verticalgrid->GetValue();
		currentFrmwork->readCommand(cmd.c_str());
	}

	// compare if current Vdd Net Name is different from textbox value
	if(string(vddnetname->GetValue()) != currentFrmwork->getDesign()->getCircuit()->getVddNet().c_str()){
		wxString cmd="set vddnet " + vddnetname->GetValue();
		currentFrmwork->readCommand(cmd.c_str());
	}

	// compare if current Gnd Net Name is different from textbox value
	if(string(gndnetname->GetValue()) != currentFrmwork->getDesign()->getCircuit()->getGndNet().c_str()){
		wxString cmd="set gndnet " + gndnetname->GetValue();
		currentFrmwork->readCommand(cmd.c_str());
	}

	// compare if current Row Height is different from textbox value
	if(string(rowheight->GetValue()) != intToStr(currentFrmwork->getDesign()->getCircuit()->getRowHeight())){
		wxString cmd="set rowheight " + rowheight->GetValue();
		currentFrmwork->readCommand(cmd.c_str());
	}

	// compare if current Supply Size is different from textbox value
	if(supplysize->GetValue() != wxString::Format(_T("%f"), currentFrmwork->getDesign()->getCircuit()->getSupplyVSize())){
		wxString cmd="set supplysize " + supplysize->GetValue();
		currentFrmwork->readCommand(cmd.c_str());
	}

    Show(false);
}

void WxCircuit::cancel(){
	refresh();
    Show(false);
}

void WxCircuit::refresh(){
	
	// append the itens (cells) for the selection box
	cell_choice->Clear();
	for(map<string, CellNetlst>::iterator cells_it=currentFrmwork->getDesign()->getCircuit()->getCellNetlsts()->begin(); cells_it!=currentFrmwork->getDesign()->getCircuit()->getCellNetlsts()->end(); cells_it++)
		cell_choice->Append(cells_it->first.c_str());
    cell_choice->Select(0);
	
	// set the current values for the textboxes
	design_name->SetValue(wxT(currentFrmwork->getDesign()->getName().c_str()));
	horizontalgrid->SetValue(wxString::Format(_T("%f"), currentFrmwork->getDesign()->getCircuit()->getHPitch()));
	verticalgrid->SetValue(wxString::Format(_T("%f"), currentFrmwork->getDesign()->getCircuit()->getVPitch()));
	vddnetname->SetValue(wxT(currentFrmwork->getDesign()->getCircuit()->getVddNet().c_str()));
	gndnetname->SetValue(wxT(currentFrmwork->getDesign()->getCircuit()->getGndNet().c_str()));
	rowheight->SetValue(wxString::Format(_T("%d"), currentFrmwork->getDesign()->getCircuit()->getRowHeight()));
	supplysize->SetValue(wxString::Format(_T("%f"), currentFrmwork->getDesign()->getCircuit()->getSupplyVSize()));
}