#include "wxprintgp.h"

WxPrintGP::WxPrintGP( IcpdFrm* frmwork )
:
View_PrintGP( frmwork )
{
	currentFrmwork=frmwork;
	refresh();
}

void WxPrintGP::PressedKey( wxKeyEvent& event )
{
	if (event.GetKeyCode() == WXK_ESCAPE)
		cancel();
	event.Skip();
}
/*
void WxPrintGP::applyButtonEvt( wxCommandEvent& event )
{
	// TODO: Implement applyButtonEvt
}
*/
void WxPrintGP::okButtonEvt( wxCommandEvent& event )
{
	ok();
}

void WxPrintGP::cancelButtonEvt( wxCommandEvent& event )
{
	cancel();
}

void WxPrintGP::ok(){
	wxString cmd;
	
	// compare if current optimize is different from textbox value	
	//optimize_choice->GetCurrentSelection() // 0 e 1
	if(optimize_choice->GetCurrentSelection() !=currentFrmwork->getDesign()->getCircuit()->getoptimize()){
		//cout << optimize_choice->GetCurrentSelection() << "<<<<"<< currentFrmwork->getDesign()->getCircuit()->getoptimizename()<< "BEFORE" ;

		Circuit * c = currentFrmwork->getDesign()->getCircuit();
		cmd= optimize_choice->GetStringSelection();
		c->setoptimize(cmd.c_str());
		//cout << optimize_choice->GetCurrentSelection() << "<<<<"<< currentFrmwork->getDesign()->getCircuit()->getoptimizename()<< "AFTER" ;
	}
	
	// compare if current sizing type is different from textbox value	
	//sizingtype_choice->GetCurrentSelection() // 0 e 1
	if(sizingtype_choice->GetCurrentSelection() !=currentFrmwork->getDesign()->getCircuit()->getsizingtype()){
		//cout << sizingtype_choice->GetCurrentSelection() << "<<<<"<< currentFrmwork->getDesign()->getCircuit()->getsizingtypename()<< "BEFORE" ;

		Circuit * c = currentFrmwork->getDesign()->getCircuit();
		cmd= sizingtype_choice->GetStringSelection();
		c->setsizingtype(cmd.c_str());
		//cout << sizingtype_choice->GetCurrentSelection() << "<<<<"<< currentFrmwork->getDesign()->getCircuit()->getsizingtypename()<< "AFTER" ;
	}
	
   
	// compare if current technology is different from textbox value	
	 //technology_choice->GetCurrentSelection() // 0 e 1
	if(technology_choice->GetCurrentSelection() !=currentFrmwork->getDesign()->getCircuit()->gettechnology()){
		cmd="set technology " +  technology_choice->GetStringSelection() ;	
		//cout << technology_choice->GetStringSelection() << "<<<<"<< currentFrmwork->getDesign()->getCircuit()->gettechnologyname()<< "BEFORE \n" ;
		currentFrmwork->readCommand(cmd.c_str());
		//cout << technology_choice->GetCurrentSelection() << "<<<<"<< currentFrmwork->getDesign()->getCircuit()->gettechnologyname()<< "AFTER" ;
	}
	
	// compare if current Cload is different from textbox value
	if(parameterCload->GetValue() != wxString::Format(_T("%f"), currentFrmwork->getDesign()->getCircuit()->getCload())){
		cmd = "set cload " + parameterCload->GetValue();
		//cout << parameterCload->GetValue() << "<<<<"<< currentFrmwork->getDesign()->getCircuit()->getCload()<< "BEFORE \n" ;
		currentFrmwork->readCommand(cmd.c_str());
		//cout << parameterCload->GetValue() << "<<<<"<< currentFrmwork->getDesign()->getCircuit()->getCload()<< "END AFTER" ;
	}
	
	// compare if current constrArea is different from textbox value
	if(parameterMaxArea->GetValue() != wxString::Format(_T("%f"), currentFrmwork->getDesign()->getCircuit()->getconstrArea())){
		cmd = "set constrArea " + parameterMaxArea->GetValue();
		//cout << parameterMaxArea->GetValue() << "<<<<"<< currentFrmwork->getDesign()->getCircuit()->getconstrArea()<< "BEFORE \n" ;
		currentFrmwork->readCommand(cmd.c_str());
		//cout << parameterMaxArea->GetValue() << "<<<<"<< currentFrmwork->getDesign()->getCircuit()->getconstrArea()<< "AFTER" ;
	}
	
	// compare if current constrDelay is different from textbox value
	if(parameterDelay->GetValue() != wxString::Format(_T("%f"), currentFrmwork->getDesign()->getCircuit()->getconstrDelay())){
		cmd = "set constrDelay " + parameterDelay->GetValue();
		//cout << parameterDelay->GetValue() << "<<<<"<< currentFrmwork->getDesign()->getCircuit()->getconstrDelay()<< "BEFORE \n" ;
		currentFrmwork->readCommand(cmd.c_str());
		//cout << parameterDelay->GetValue() << "<<<<"<< currentFrmwork->getDesign()->getCircuit()->getconstrDelay()<< "AFTER" ;
	}
	
	// compare if current constrCin is different from textbox value
	if(parameterMaxCin->GetValue() != wxString::Format(_T("%f"), currentFrmwork->getDesign()->getCircuit()->getconstrCin())){
		cmd = "set constrCin " + parameterMaxCin->GetValue();
		//cout << parameterMaxCin->GetValue() << "<<<<"<< currentFrmwork->getDesign()->getCircuit()->getconstrCin()<< "BEFORE \n" ;
		currentFrmwork->readCommand(cmd.c_str());
		//cout << parameterMaxCin->GetValue() << "<<<<"<< currentFrmwork->getDesign()->getCircuit()->getconstrCin() << "AFTER" ;
	}
	
	
	currentFrmwork->readCommand("size gp");

	
    Show(false);
}

void WxPrintGP::cancel(){
	refresh();
    Show(false);
}

void WxPrintGP::refresh(){
	
	if(currentFrmwork->getDesign()->getCircuit()->gettechnologyname() == "45nm"){
		technology_choice->SetSelection( 0 );
	}
	else{//==350nm
		technology_choice->SetSelection( 1 );
	}	
	
	 parameterCload->SetValue (doubleToString(currentFrmwork->getDesign()->getCircuit()->getCload()).c_str());	
     parameterMaxArea->SetValue(doubleToString(currentFrmwork->getDesign()->getCircuit()->getconstrArea()).c_str());
	 parameterDelay->SetValue(doubleToString(currentFrmwork->getDesign()->getCircuit()->getconstrDelay()).c_str());
	 parameterMaxCin->SetValue(doubleToString(currentFrmwork->getDesign()->getCircuit()->getconstrCin()).c_str());
}

string  WxPrintGP::doubleToString(double val)
{
    std::ostringstream out;
    out << val;
    return out.str();
}
