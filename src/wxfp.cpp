#include "wxfp.h"

WxFP::WxFP( IcpdFrm* frmwork ):Specify_FP( frmwork ){
	currentFrmwork=frmwork;
    refresh();
}

void WxFP::PressedKey( wxKeyEvent& event ){
	if (event.GetKeyCode() == WXK_ESCAPE)
		cancel();
	else if (event.GetKeyCode() == WXK_RETURN)
		ok();
	event.Skip();
}

void WxFP::hide( wxCloseEvent& event ){
    Show(false);
}

void WxFP::okButtonEvt( wxCommandEvent& event ){
	ok();
}

void WxFP::cancelButtonEvt( wxCommandEvent& event ){
	cancel();
}

void WxFP::ok(){
	wxString cmd;

	// set new values for margins
	cmd="set area " +  wxNrRows->GetValue() + " " +  wxUtilization->GetValue();
	currentFrmwork->readCommand(cmd.c_str());

	// compare if current margins are different from textboxes values
	if ((wxC2L->GetValue() != wxString::Format(_T("%f"), currentFrmwork->getDesign()->getCircuit()->getLMargin() * currentFrmwork->getDesign()->getCircuit()->getHPitch())) || 
		(wxC2R->GetValue() != wxString::Format(_T("%f"), currentFrmwork->getDesign()->getCircuit()->getRMargin() * currentFrmwork->getDesign()->getCircuit()->getHPitch())) ||
		(wxC2B->GetValue() != wxString::Format(_T("%f"), currentFrmwork->getDesign()->getCircuit()->getBMargin() * currentFrmwork->getDesign()->getCircuit()->getVPitch())) ||
		(wxC2T->GetValue() != wxString::Format(_T("%f"), currentFrmwork->getDesign()->getCircuit()->getTMargin() * currentFrmwork->getDesign()->getCircuit()->getVPitch()))) {
			cmd="set margins " +  wxC2L->GetValue() + " " +  wxC2R->GetValue() + " " +  wxC2T->GetValue() + " " +  wxC2B->GetValue();
			currentFrmwork->readCommand(cmd.c_str());
	}

	Show(false);
}

void WxFP::cancel(){
	refresh();
    Show(false);
}

void WxFP::refresh(){
	// set the current values for the textboxes
	wxNrRows->SetValue(wxString::Format(_T("%d"), currentFrmwork->getDesign()->getPlacer()->getNrRows()));
	wxUtilization->SetValue(wxString::Format(_T("%f"), currentFrmwork->getDesign()->getPlacer()->getUtilization()));
	wxC2L->SetValue(wxString::Format(_T("%f"), currentFrmwork->getDesign()->getCircuit()->getLMargin() * currentFrmwork->getDesign()->getCircuit()->getHPitch()));	
	wxC2R->SetValue(wxString::Format(_T("%f"), currentFrmwork->getDesign()->getCircuit()->getRMargin() * currentFrmwork->getDesign()->getCircuit()->getHPitch()));	
	wxC2B->SetValue(wxString::Format(_T("%f"), currentFrmwork->getDesign()->getCircuit()->getBMargin() * currentFrmwork->getDesign()->getCircuit()->getVPitch()));	
	wxC2T->SetValue(wxString::Format(_T("%f"), currentFrmwork->getDesign()->getCircuit()->getTMargin() * currentFrmwork->getDesign()->getCircuit()->getVPitch()));	
}