#include "wxPreferences.h"

WxPreferences::WxPreferences( IcpdFrm* frmwork ):View_Preferences( frmwork ){
	currentFrmwork=frmwork;
	refresh();
}

void WxPreferences::PressedKey( wxKeyEvent& event ){
	if (event.GetKeyCode() == WXK_ESCAPE)
		cancel();
	event.Skip();
}

void WxPreferences::hide( wxCloseEvent& event ){
	refresh();
    Show(false);
}

void WxPreferences::okButtonEvt( wxCommandEvent& event ){
	ok();
}

void WxPreferences::cancelButtonEvt( wxCommandEvent& event ){
	cancel();
}

void WxPreferences::plButton( wxCommandEvent& event ){
	wxFileDialog* tmp =  new wxFileDialog(this, wxT("Set Path"), wxT(""), wxT(""), wxT("*.*"), wxOPEN);
	if ( tmp->ShowModal() == wxID_OK ){
		wxString path=tmp->GetDirectory() + "/" + tmp->GetFilename();
		plPath->SetValue(path);
	}
}

void WxPreferences::rotdlButton( wxCommandEvent& event ){
	wxFileDialog* tmp =  new wxFileDialog(this, wxT("Set Path"), wxT(""), wxT(""), wxT("*.*"), wxOPEN);
	if ( tmp->ShowModal() == wxID_OK ){
		wxString path=tmp->GetDirectory() + "/" + tmp->GetFilename();
		rotdlPath->SetValue(path);
	}
}

void WxPreferences::viewerButton( wxCommandEvent& event ){
	wxFileDialog* tmp =  new wxFileDialog(this, wxT("Set Path"), wxT(""), wxT(""), wxT("*.*"), wxOPEN);
	if ( tmp->ShowModal() == wxID_OK ){
		wxString path=tmp->GetDirectory() + "/" + tmp->GetFilename();
		viewerPath->SetValue(path);
	}
}

void WxPreferences::lpsolverButton( wxCommandEvent& event ){
	wxFileDialog* tmp =  new wxFileDialog(this, wxT("Set Path"), wxT(""), wxT(""), wxT("*.*"), wxOPEN);
	if ( tmp->ShowModal() == wxID_OK ){
		wxString path=tmp->GetDirectory() + "/" + tmp->GetFilename();
		lpsolverPath->SetValue(path);
	}
}

void WxPreferences::logButton( wxCommandEvent& event ){
	wxFileDialog* tmp =  new wxFileDialog(this, wxT("Set Path"), wxT(""), wxT(""), wxT("*.*"), wxSAVE|wxOVERWRITE_PROMPT);
	if ( tmp->ShowModal() == wxID_OK ){
		wxString path=tmp->GetDirectory() + "/" + tmp->GetFilename();
		logPath->SetValue(path);
	}
}

void WxPreferences::ok(){
	wxString cmd, path;

	// compare if current placer path is different from textbox value
	if(string(plPath->GetValue())!=currentFrmwork->getDesign()->getPlacerFile()){
		path = "\"" + plPath->GetValue() + "\"";
		cmd="set placer " + path;
		currentFrmwork->readCommand(cmd.c_str());
	}

	// compare if current rotdl path is different from textbox value
	if(string(rotdlPath->GetValue())!=currentFrmwork->getDesign()->getRotDL()){
		path = "\"" + rotdlPath->GetValue() + "\"";
		cmd="set rotdl " + path;
		currentFrmwork->readCommand(cmd.c_str());
	}

	// compare if current viewer path is different from textbox value
	if(string(viewerPath->GetValue())!=currentFrmwork->getDesign()->getViewer()){
		path = "\"" + viewerPath->GetValue() + "\"";
		cmd="set viewer " + path;
		currentFrmwork->readCommand(cmd.c_str());
	}

	// compare if current lpsolver path is different from textbox value
	if(string(lpsolverPath->GetValue())!=currentFrmwork->getDesign()->getlpSolver()){
		path = "\"" + lpsolverPath->GetValue() + "\"";
		cmd="set lpsolve " + path;
		currentFrmwork->readCommand(cmd.c_str());
	}

	// compare if current log path is different from textbox value
	if(string(logPath->GetValue())!=currentFrmwork->getDesign()->getLog()){
		path = "\"" + logPath->GetValue() + "\"";
		cmd="set log " + path;
		currentFrmwork->readCommand(cmd.c_str());
	}

    Show(false);
}

void WxPreferences::cancel(){
	refresh();
    Show(false);
}

void WxPreferences::refresh(){
	// set the current values for the textboxes
	plPath->SetValue(currentFrmwork->getDesign()->getPlacerFile().c_str());
	rotdlPath->SetValue(currentFrmwork->getDesign()->getRotDL().c_str());
	viewerPath->SetValue(currentFrmwork->getDesign()->getViewer().c_str());
	lpsolverPath->SetValue(currentFrmwork->getDesign()->getlpSolver().c_str());
	logPath->SetValue(currentFrmwork->getDesign()->getLog().c_str());
}
