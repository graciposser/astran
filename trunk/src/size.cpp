/*
 *  size.cpp
 *  ICPD
 *
 *  Created by Adriel Mota Ziesemer Jr. on 5/18/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */


#include "size.h"
#include "Stopwatch.h"
#include <iostream>
	using std::cout;
	using std::cerr;
#include <vector>
	using std::vector;
#include <iomanip>
	using std::setprecision;
#include <iostream>
#include <fstream>
//#include "RCTranslator.h"
#include "elmoredelay.h"
#include <map>
#include <math.h>

using namespace std;

//template para transformar dados em String
template< typename T >
inline string ToString( const T &data ) {
    ostringstream out;
    out << data;
    return out.str();
} // end function

// -----------------------------------------------------------------------------

void Size::printFinalGP( string &D, ofstream &file, RCTranslator &rc, double constrArea, string instanceName) {
		//=========Imprime Função Objetivo=================
		file << endl << "%Função objetivo é minimizar o delay" << endl;
		file << "delay_" << instanceName << " = max(" + D + ");\n" << endl;
} // end method


// -----------------------------------------------------------------------------

void Size::printFinalCircuit(ofstream &file, RCTranslator &rc, double constrArea, string instanceName, string sizingType, string optimize) {


		if (sizingType == "transistor"){
			file << "Afinal_" << instanceName << " <= " << constrArea << " * Abase_" << instanceName << ";" << endl;
			for ( int i=0; i<rc.getNumTransistors(); i++){
				file << " Xmin <= X" + ToString(i) << "_" << instanceName << ";" << endl;
				if (optimize == "delay") {
					file << " X" + ToString(i) << "_" << instanceName << "<= Xmax;" << endl;
				}
			}//end for
		}//end if
		else{
			file << " Xmin <= " << instanceName << ";" << endl;
			if (optimize == "delay") {
				file << instanceName << "<= Xmax;" << endl;
			} // end if
		}
} // end method

// -----------------------------------------------------------------------------

void Size::printRC( const RCTranslator &rc, string &D, const int cont, ofstream &file, double constrArea, string instanceName, int contInstance, string sizingType, double Cload, string technology) {
	//values for 350nm technology
	double csbdb_pmos = 2.2087E-15;
	double csbdb_nmos = 2.4025E-15;
	double Req_pmos = 2.0717E+04;
	double Req_nmos = 9.1691E+03;
	double Xmin = 1;
	double Xmax = 15;
	double Xn = 1;
	double Xp = 1.6;

	//values for 45nm technology
	/*
	double csbdb_pmos_45 = 6.4541E-17;
	double csbdb_nmos_45 = 7.1513E-17;
	double Req_pmos_45 = 5.4987E+04;
	double Req_nmos_45 = 1.5586E+04;
	double Xmin_45 = 1;
	double Xmax_45 = 32;
	double Xn_45 = 0.09; 
	double Xp_45 = 0.135;
	*/
	
	const double pnratio = 1.5;

	double csbdb_pmos_45 = 6.4541E-17 * pnratio;
	double csbdb_nmos_45 = 7.1513E-17;
	double Req_pmos_45 = 5.4987E+04 / pnratio;
	double Req_nmos_45 = 1.5586E+04;
	double Xmin_45 = 1;
	double Xmax_45 = 32;
	double Xn_45 = 0.09; 
	double Xp_45 = 0.135;

    //cerr << "================================================================================\n";
	//cerr << "Input Vector: " << rc.getInputVector().toString() << "\n";
	//cerr << "================================================================================\n";

    //================================================================================
    //                                Imprimindo a Saída
    //================================================================================
    if (cont == 0){

		if (sizingType != "gate"){
			file << "%Vari·veis GP" << endl;
			file << "gpvar";
			for ( int i=0; i<rc.getNumTransistors(); i++){
				file << " X" + ToString(i) + "_" + instanceName;
			}//end for
			file << ";" << endl;
		}//end if


		if (contInstance == 0){
			if (technology == "350nm"){
				file << endl << "%Problem data for all Components" << endl;
				file << "Xmax = " << Xmax << ";" << endl;
				file << "Xmin = " << Xmin << ";" << endl;
				file << "CsbP = " << csbdb_pmos << ";" << endl;
				file << "CsbN = " << csbdb_nmos << ";" << endl;
				file << "ReqP = " << Req_pmos << ";" << endl;
				file << "ReqN = " << Req_nmos << ";" << endl;
				file << "Cload = " << Cload << ";" << endl;
				file << "constrArea = " << constrArea << ";" << endl;
				file << "Xn = " << Xn << ";" << endl;
				file << "Xp = " << Xp << ";\n" << endl;
			}//end if
			if (technology == "45nm") {
				file << endl << "%Problem data for all Components using 45nm technology" << endl;
				file << "Xmax = " << Xmax_45 << ";" << endl;
				file << "Xmin = " << Xmin_45 << ";" << endl;
				file << "CsbP = " << csbdb_pmos_45 << ";" << endl;
				file << "CsbN = " << csbdb_nmos_45 << ";" << endl;
				file << "ReqP = " << Req_pmos_45 << ";" << endl;
				file << "ReqN = " << Req_nmos_45 << ";" << endl;
				file << "Cload = " << Cload << ";" << endl;
				file << "constrArea = " << constrArea << ";" << endl;
				file << "Xn = " << Xn_45 << ";" << endl;
				file << "Xp = " << Xp_45 << ";\n" << endl;
			}

		}
		if (technology == "350nm"){
			file << "%INSTANCE " << instanceName << endl;
			file << "Xn_" << instanceName << " = " << Xn << " * " << instanceName << ";" << endl;
			file << "Xp_" << instanceName << " = " << Xp << " * " << instanceName << ";" << endl;

		}//end if
		if (technology == "45nm") {
			file << "%INSTANCE " << instanceName << endl;
			file << "Xn_" << instanceName << " = " << Xn_45 << " * " << instanceName << ";" << endl;
			file << "Xp_" << instanceName << " = " << Xp_45 << " * " << instanceName << ";" << endl;

		}//end if



	//}//end if


		file << endl << "%Problem data for each Component" << endl;
		//===========Imprime o Ron de cada transistor=========

		for ( int i=0; i<rc.getNumTransistors(); i++){
			switch(rc.getTransistorType(i)){
				case 0: {
					if (sizingType == "gate") {
						file << "Rtrans" + ToString(i) << "_" << instanceName << " = " << "ReqP / " << instanceName << ";" << endl; break;
					}
					else {
						file << "Rtrans" + ToString(i) << "_" << instanceName << " = " << "ReqP / X" + ToString(i) << "_" << instanceName << ";" << endl; break;
					}
				}//end case
				case 1: {
					if (sizingType == "gate"){
						file << "Rtrans" + ToString(i) << "_" << instanceName << " = " << "ReqN / " << instanceName << ";" << endl; break;
					}
					else{
						file << "Rtrans" + ToString(i) << "_" << instanceName << " = " << "ReqN / X" + ToString(i) << "_" << instanceName << ";" << endl; break;
					}
				}//end case
			}
		}//end for

		//===========Imprime a Capacitância de entrada de cada transistor=========
		for ( int i=0; i<rc.getNumTransistors(); i++){
			switch(rc.getTransistorType(i)){
			file << rc.getTransistorType(i) << endl;
				case 0:
					if (sizingType == "gate"){
						file << "Ctrans" + ToString(i) << "_" << instanceName << " = " << "CsbP * " << instanceName << ";" << endl; break;
					}//end if
					else {
						file << "Ctrans" + ToString(i) << "_" << instanceName << " = " << "CsbP * X" +  ToString(i) << "_" << instanceName << ";" << endl; break;
					}//end else
				case 1:
					if (sizingType == "gate"){
						file << "Ctrans" + ToString(i) << "_" << instanceName << " = " << "CsbN * " << instanceName << ";" << endl; break;
					}
					else {
						file << "Ctrans" + ToString(i) << "_" << instanceName << " = " << "CsbN * X" +  ToString(i) << "_" << instanceName << ";" << endl; break;
					}
				}//end case
		}//end for
		}//end if
		
		
		
		if (sizingType == "gate" && cont == 0){
			file << "Abase_" << instanceName << " = ";
			for ( int i=0; i<rc.getNumTransistors(); i++){
				switch(rc.getTransistorType(i)){
				file << rc.getTransistorType(i) << endl;
					case 0:
						if (i == (rc.getNumTransistors() -1)){
							file << "Xp"; break;
						}
						else
						{
							file << "Xp +"; break;
						}
					case 1:
						if (i == (rc.getNumTransistors() -1)){
							file << "Xn"; break;
						}
						else
						{
							file << "Xn +"; break;
						}
				}
			}//end for
			file << ";" << endl;
			file << "Afinal_" << instanceName << " = ";
			for ( int i=0; i<rc.getNumTransistors(); i++){
				switch(rc.getTransistorType(i)){
				file << rc.getTransistorType(i) << endl;
					case 0:
						if (i == (rc.getNumTransistors() -1)){
							file << "Xp_" << instanceName; break;
						}
						else
						{
							file << "Xp_" << instanceName << "+"; break;
						}
					case 1:
						if (i == (rc.getNumTransistors() -1)){
							file << "Xn_" << instanceName; break;
						}
						else
						{
							file << "Xn_" << instanceName << "+"; break;
						}
				}
			}//end for
			file << ";" << endl;
		}//end if
		if (sizingType == "transistor"){
			file << "Abase" << "_" << instanceName << " = " << rc.getNumTransistors() << ";" << endl;
			string Afinal = "Afinal_" + instanceName + " = ";
			for (int i=0; i<rc.getNumTransistors(); i++)
			{
				if (i == (rc.getNumTransistors()-1))
					Afinal += "X" + ToString(i) + "_" + instanceName;
				else
					Afinal += "X" + ToString(i) + "_" + instanceName + " + ";
			}
			Afinal += ";";
			file << Afinal << endl;
		}//end else
		


    file << "\n%INPUT: " << cont << endl;


	file << "\n%";
	for ( int i = 0; i < rc.getNumNodes(); i++ )
		if ( rc.isInput(i) )
			file << rc.getNodeName(i) << " ";
	file << "\n%" << rc.getInputVector().toString();
	file << "\n";

	// cerr << "Driver transistor of each nodes:\n";
	for ( int n = 0; n < rc.getNumNodes(); n++ ) {
		if ( rc.getDriverTransistor( n ) != -1 ) {
			//cerr << "\t" << rc.getNodeName(n) << "\t" << rc.getTransistorName( rc.getDriverTransistor(n) ) << "\n";
		} // end if
	} // end for

	//cerr << "Transistos connected to each node (via source or drain):\n";
	for ( int n = 0; n < rc.getNumNodes(); n++ ) {
		if ( rc.getDriverTransistor( n ) == -1 )
			continue;

		//cerr << "\t" << rc.getNodeName(n) << "\t";
		file << "C" + ToString(cont) + ToString(n) + "_" + instanceName << " = ";						//Imprime a capacitância de cada nodo
		const vector<int> &trans = rc.getConnectedTransistors(n);
		for ( int i = 0; i < trans.size(); i++ ){
			//cerr << " " << rc.getTransistorName( trans[i] );
			if (i<(trans.size()-1)) {								//Testa se não for o último nodo vai somando, senão finaliza com ;
				file << "Ctrans" << trans[i] << "_" << instanceName << " + ";
			}
			else
				file << "Ctrans" << trans[i] << "_" << instanceName;
		}

		if ( n == rc.getOutputNode()  )
			file << " + Cload_" << instanceName;
		
		file << ";\n"; 

		//cerr << "\n";
	} // end for

	//=============Imprime a capacitância de Downstream de cada nodo
	//cerr << "Downstream nodes of each node:\n";
	for ( int n = 0; n < rc.getNumNodes(); n++ ) {
		if ( rc.getDriverTransistor( n ) == -1 )
			continue;

		//cerr << "\t" << rc.getNodeName(n) << "\t";
		file << "Cdown" + ToString(cont) << n << "_" + instanceName << " = ";
		
		const vector<int> &nodes = rc.getDownstreamNodes(n);
		for ( int i = 0; i < nodes.size(); i++ ){
			if ( i > 0 )
				file << " + "; 
			file << "C" + ToString(cont) << nodes[i] << "_" << instanceName;
		} // end for
		
		file << ";\n";
	} // end for

	//=============Imprime a capacitância de Upstream de cada nodo +++++++++++++++++++++

	//cerr << "Upstream nodes of each node:\n";
	const vector<int> &upnodes = rc.getUpstreamNodes(rc.getOutputNode());
	for ( int i = 0; i < upnodes.size(); i++ ){
		// cerr << " " << rc.getNodeName( upnodes[i] );
	}//end for
	int j = upnodes.back();

	if ( cont > 0 )
		D += ", ";
	
	file << "D" + ToString(cont) << j << "_" << instanceName << " = Rtrans" << rc.getDriverTransistor(j) << "_" << instanceName << " * (Cdown" + ToString(cont) << j << "_" << instanceName;
	if (upnodes.size() <= 1){
	   file << ");" << endl;
	   D += "D" + ToString(cont) + ToString(j) + "_" + instanceName;
	} else {
		file << ");" << endl;
		for ( int k = (upnodes.size()-2); k >= 0; k-- ) {
			int m = upnodes[k];
			file << "D" + ToString(cont) << m << "_" << instanceName << " = D" + ToString(cont) << j << "_" << instanceName << " + Rtrans" << rc.getDriverTransistor(m) << "_" << instanceName << " * (Cdown" + ToString(cont) << m << "_" << instanceName << ");" << endl;
			
			if ( k == 0 )
				D += "D" + ToString(cont) + ToString(m) + "_" + instanceName;	
			j = m;
		} // end for
	} // end else	
	
	file << flush;

	cerr << "\n";
} // end function

// -----------------------------------------------------------------------------

void Size::printGP_Header( GeometricProgram &gp, const string &technology, const double parameterCload, const double parameterMaxArea ) {
	//values for 350nm technology
	double csbdb_pmos = 2.2087E-15;
	double csbdb_nmos = 2.4025E-15;
	double Req_pmos = 2.0717E+04;
	double Req_nmos = 9.1691E+03;
	double Xmin = 1;
	double Xmax = 15;
	double Xn = 1;
	double Xp = 1.6;

	//values for 45nm technology
	const double pnratio = 1.5;

	double csbdb_pmos_45 = 6.4541E-17 * pnratio;
	double csbdb_nmos_45 = 7.1513E-17;
	double Req_pmos_45 = 5.4987E+04 / pnratio;
	double Req_nmos_45 = 1.5586E+04;
	double Xmin_45 = 1;
	double Xmax_45 = 32;
	double Xn_45 = 0.09;
	double Xp_45 = 0.135;

	// Create variables.
	// [TODO]

	// Create constants.
	gp.createConstant( "Cload", parameterCload );
	gp.createConstant( "constrArea", parameterMaxArea );
	
	if (technology == "350nm"){
		gp.createConstant( "Xmax", Xmax );
		gp.createConstant( "Xmin", Xmin );
		gp.createConstant( "CsbP", csbdb_pmos );
		gp.createConstant( "CsbN", csbdb_nmos );
		gp.createConstant( "ReqP", Req_pmos );
		gp.createConstant( "ReqN", Req_nmos );
		gp.createConstant( "Xn", Xn );
		gp.createConstant( "Xp", Xp );
	} else if (technology == "45nm") {
		gp.createConstant( "Xmax", Xmax_45 );
		gp.createConstant( "Xmin", Xmin_45 );
		gp.createConstant( "CsbP", csbdb_pmos_45 );
		gp.createConstant( "CsbN", csbdb_nmos_45 );
		gp.createConstant( "ReqP", Req_pmos_45 );
		gp.createConstant( "ReqN", Req_nmos_45 );
		gp.createConstant( "Xn", Xn_45 );
		gp.createConstant( "Xp", Xp_45 );
	} else {
		throw string( "Invalid technology!" );
	} // end else
} // end method

// -----------------------------------------------------------------------------

void Size::printGP_InstanceHeader( GeometricProgram &gp, const RCTranslator &rc, const string &instanceName ) {
	Variable * instvar = gp.requestVariable( instanceName );

	gp.createMonomial( "Xn_" + instanceName, gp.requestConstant( "Xn" ), instvar );
	gp.createMonomial( "Xp_" + instanceName, gp.requestConstant( "Xp" ), instvar );

	//===========Imprime o Ron de cada transistor=========

	for ( int i=0; i<rc.getNumTransistors(); i++){
		const string Rtrans = "Rtrans" + ToString(i) + "_" + instanceName;

		switch ( rc.getTransistorType(i) ) {
			case RCTranslator::PMOS:
				gp.createMonomial( Rtrans, gp.requestConstant("ReqP"), instvar, -1 );
				break;
			case RCTranslator::NMOS:
				gp.createMonomial( Rtrans, gp.requestConstant("ReqN"), instvar, -1 );
				break;
		} // end swtich
	} // end for

	//===========Imprime a Capacitância de entrada de cada transistor=========
	for ( int i=0; i<rc.getNumTransistors(); i++){
		const string Ctrans = "Ctrans" + ToString(i) + "_" + instanceName;

		switch ( rc.getTransistorType(i) ) {
			case RCTranslator::PMOS:
				gp.createMonomial( Ctrans, gp.requestConstant("CsbP"), instvar );
				break;
			case RCTranslator::NMOS:
				gp.createMonomial( Ctrans, gp.requestConstant("CsbN"), instvar );
				break;
			} // end switch
	} // end for

	//===========Imprime a area da instancia =========
	int numPMOS = 0;
	int numNMOS = 0;

	for ( int i = 0; i < rc.getNumTransistors(); i++ ) {
		switch(rc.getTransistorType(i)) {
			case RCTranslator::PMOS: numPMOS++; break;
			case RCTranslator::NMOS: numNMOS++; break;
		} // end switch
	} // end for

	// Abase = Xn*numPMOS + Xp*numNMOS
	Sum * sumBase = gp.createSum( "Abase_" + instanceName,
		gp.createMul( gp.requestConstant("Xn"), gp.createConstant( numNMOS ) ),
		gp.createMul( gp.requestConstant("Xp"), gp.createConstant( numPMOS ) ) );

	// Afinal = Abase * X_Instance
	gp.createMul( "Afinal_" + instanceName, sumBase, instvar );
} // end method

// -----------------------------------------------------------------------------

void Size::printGP_InstanceRC( const RCTranslator &rc, string &D, const int cont, GeometricProgram &gp, double constrArea, string instanceName, int contInstance, string sizingType, double Cload, string technology) {
	Variable * instvar = gp.requestVariable( instanceName );

	//=============Imprime a capacitância atrelada a cada nodo
	//cerr << "Transistos connected to each node (via source or drain):\n";
	for ( int n = 0; n < rc.getNumNodes(); n++ ) {
		if ( rc.getDriverTransistor( n ) == -1 )
			continue;

		Sum * sumCap = gp.createSum( "C" + ToString(cont) + ToString(n) + "_" + instanceName );

		//Imprime a capacitância de cada nodo
		const vector<int> &trans = rc.getConnectedTransistors(n);
		for ( int i = 0; i < trans.size(); i++ )
			sumCap->addTerm( gp.requestPosynomialType( "Ctrans" + ToString(trans[i]) + "_" + instanceName ) );

		if ( n == rc.getOutputNode() )
			sumCap->addTerm( gp.requestPosynomialType( "Cload_" + instanceName ) );
	} // end for

	//=============Imprime a capacitância de Downstream de cada nodo
	//cerr << "Downstream nodes of each node:\n";
	for ( int n = 0; n < rc.getNumNodes(); n++ ) {
		if ( rc.getDriverTransistor( n ) == -1 )
			continue;

		Sum * sumDown = gp.createSum( "Cdown" + ToString(cont) + ToString(n) + "_" + instanceName );

		const vector<int> &nodes = rc.getDownstreamNodes(n);
		for ( int i = 0; i < nodes.size(); i++ )
			sumDown->addTerm( gp.requestPosynomialType( "C" + ToString(cont) + ToString(nodes[i]) + "_" + instanceName ) );
	} // end for

	//=============Imprime Elmore Delay de cada nodo +++++++++++++++++++++
	const vector<int> &upnodes = rc.getUpstreamNodes(rc.getOutputNode());

	PosynomialType * prevDelay = gp.createMul( "D" + ToString(cont) + ToString(upnodes.back()) + "_" + instanceName );
	for ( int k = (upnodes.size()-2); k >= 0; k-- ) {

		const int m = upnodes[k];

		Sum * delay = gp.createSum(  "D" + ToString(cont) + ToString(m) + "_" + instanceName );
		delay->addTerm( prevDelay );
		delay->addTerm( gp.createMul(
			gp.requestPosynomialType( "Rtrans" + ToString(rc.getDriverTransistor(m)) + "_" + instanceName ),
			gp.requestPosynomialType( "Cdown" + ToString(cont) + ToString(m) + "_" + instanceName )	) );

		prevDelay = delay;
	} // end for
} // end function

// -----------------------------------------------------------------------------

void Size::addInstances(queue<string> &instances, const t_net &net){
    const list<t_inst> &insts = net.insts;
    for (list<t_inst>::const_iterator it = insts.begin(); it != insts.end(); it++){
		instances.push(it->targetCellInst);
		//cerr << "\t\tNeighbour: " << it->targetCellInst << "\t\tTargetPin: "<< it->targetPin << "\n";
	}//end for
}//end function



//------------------------------------------------------------------------------

void Size::circuitSize( Circuit *c, CellNetlst *topNetlist, CellNetlst *netlist, string instanceName, Inst instance, ofstream &file) {
	string netOut;

	vector<int> &ports = instance.ports;
	for (int i=0; i<ports.size(); i++)
	{
		t_net &net = topNetlist->getNet(ports[i]);
		//cout << "Netname: " << net.name << endl;
		if ( net.name != c->getVddNet() && net.name != c->getGndNet() ) {
			if (netlist->getIOType(i) == IOTYPE_OUTPUT)
				netOut =  net.name;
		} // end if
	}//end for

	for (int i=0; i<ports.size(); i++)
	{
		t_net &net = topNetlist->getNet(ports[i]);
		if ( net.name != c->getVddNet() && net.name != c->getGndNet() ) {
			if (netlist->getIOType(i) == IOTYPE_INPUT)
				file << " " << net.name << " + delay_" << instanceName << " <= " << netOut << ";" << endl;
		} // end if
	}//end for

}//end function



//------------------------------------------------------------------------------

void Size::findCircuitInputs( Circuit *c, CellNetlst *topNetlist, CellNetlst *netlist, string instanceName, Inst instance, ofstream &file, map<string,bool> inputs, double restrCin) {
	vector<int> &ports = instance.ports;
	for (int l=0; l<ports.size(); l++)
	{
		t_net &net = topNetlist->getNet(ports[l]);
		if ( net.name != c->getVddNet() && net.name != c->getGndNet() ) {
			if (netlist->getIOType(l) == IOTYPE_INPUT){
				if (inputs[net.name] == true){
					file << "Cin_" << instanceName << " <= " << restrCin << " * " << instance.cinPort <<";" << endl;
					return;
				}//end if
			}//end if
		}// end if
	}//end for
}//end function

//------------------------------------------------------------------------------

void Size::calculateCload( Circuit *c, CellNetlst *topNetlist, CellNetlst *netlist, string instanceName, Inst instance, ofstream &file, string netOutput, double Cload, map<string,bool> outputs) {
	string netOut;
	vector< pair<int,double> > &cins = netlist->getCins();

	file << setprecision(32);

	vector<int> &ports = instance.ports;
	for (int l=0; l<ports.size(); l++)
	{
		t_net &net = topNetlist->getNet(ports[l]);
		if ( net.name != c->getVddNet() && net.name != c->getGndNet() ) {
			//cout << "\t\t" << net.name << endl;
			//cout << "tipo netlist:" << netlist->getIOType(l) << endl;
			if (netlist->getIOType(l) == IOTYPE_OUTPUT){
				//cout << "Netname: " << net.name << endl;
				//cout << "NetOutput: " << netOutput << endl;
				//for (int j=0; j<outputs.size(); j++){
					if (outputs[net.name] == true){
						//cout << "outputs j: " << outputs[j] << endl;
						file << "Cload_" << instanceName << " = " << Cload << ";" << endl;
					}//end if
					else{
						list<t_inst> insts = net.insts;
						cout << "INSTANCE NAME: " << instanceName << endl;
						file << "Cload_" << instanceName << " = ";
						list<Inst*> sinks = findSinks(c, topNetlist, net );
						for (list<Inst*>::iterator it = sinks.begin(); it != sinks.end(); it++){ 
								Inst *inst = *it;
								if (it != sinks.begin()){
									file <<  " + ";
								}
								file << inst->cinPort << " * " << inst->name;
						}// end for
						/*
						for (list<t_inst>::const_iterator it = insts.begin(); it != insts.end(); it++){
							if (it->targetCellInst != instanceName){
								if (it != insts.begin()){
									file <<  " + ";
								}
								file << cins[it->targetPin].second << " * " << it->targetCellInst;
								cont++;
							}//end if
						}//end for
						*/
						file << ";" << endl;
					}//end else
			}//end if
		}// end if
	}//end for
}//end function

//------------------------------------------------------------------------------

void Size::transistorSizing( Circuit *circuit, CellNetlst *netlist, ofstream &file, string instanceName, int contInstance, string sizingType, double constrArea, double Cload, string technology) {
	/*if ( netlist->getInstances().size() > 0 ) {
		cerr << "[ERROR] Currently we are not able to size hierarchical cells.\n";
		return;
	} // end if
	*/

	// Discover cell ios.
	netlist->calcIOs( circuit->getGndNet(), circuit->getVddNet() );

	// Warm-up the RCTranslator.
	RCTranslator rc;

	try {
		rc.SetGND(circuit->getGndNet());
		rc.SetVDD(circuit->getVddNet());

		// Define cell ios.
		vector<int> &inouts = netlist->getInouts();
		for ( int i = 0; i < inouts.size(); i++ ) {

			switch (netlist->getIOType(inouts[i])) {
				case IOTYPE_INPUT:
					//cerr << "Input: " << netlist->getNetName(inouts[i]) << "\n";
					rc.AddInput( netlist->getNetName(inouts[i]));
					break;
				case IOTYPE_OUTPUT:
					//cerr << "Output: " << netlist->getNetName(inouts[i]) << "\n";
					rc.AddOutput( netlist->getNetName(inouts[i]));
					break;
				default:
					break;
			} // end switch
		} // end for

		// Define cell transistors.
		vector<Trans> &trans = netlist->getTransistors();
		for (vector<Trans>::iterator trans_it = trans.begin(); trans_it != trans.end(); trans_it++){
			if (trans_it->type == 0){
				rc.AddTransistor( ToString(trans_it->name), RCTranslator::PMOS, ToString(netlist->getNetName(trans_it->source)), ToString(netlist->getNetName(trans_it->gate)) , ToString(netlist->getNetName(trans_it->drain)), trans_it->width  );
			} else if (trans_it->type == 1){
				rc.AddTransistor( ToString(trans_it->name), RCTranslator::NMOS, ToString(netlist->getNetName(trans_it->source)), ToString(netlist->getNetName(trans_it->gate)) , ToString(netlist->getNetName(trans_it->drain)), trans_it->width  );
			} else{
				cout << "Transistor type is not defined" << endl;
			} // end else
		} // end for

		// Run
		//ofstream file( "gp.txt" );

		rc.initialize();

		string D;
		int cont = 0;


		while ( rc.next() ) {
			printRC( rc, D, cont, file, constrArea, instanceName, contInstance, sizingType, Cload, technology);
			cont++;
		} // end while
		printFinalGP(D, file, rc, constrArea, instanceName);
	} catch ( RCTranslatorException &e ) {
		cerr << "[EXCEPTION] " << e.what() << "\n";
		return;
	} // end catch

} // end method


//------------------------------------------------------------------------------

void Size::printConstraints( Circuit *circuit, CellNetlst *netlist, ofstream &file, string instanceName, int contInstance, string sizingType, string optimize) {

	// Discover cell ios.
	netlist->calcIOs( circuit->getGndNet(), circuit->getVddNet() );

	// Warm-up the RCTranslator.
	RCTranslator rc;

	try {
		rc.SetGND(circuit->getGndNet());
		rc.SetVDD(circuit->getVddNet());

		// Define cell ios.
		vector<int> &inouts = netlist->getInouts();
		for ( int i = 0; i < inouts.size(); i++ ) {

			switch (netlist->getIOType(inouts[i])) {
				case IOTYPE_INPUT:
					//cerr << "Input: " << netlist->getNetName(inouts[i]) << "\n";
					rc.AddInput( netlist->getNetName(inouts[i]));
					break;
				case IOTYPE_OUTPUT:
					//cerr << "Output: " << netlist->getNetName(inouts[i]) << "\n";
					rc.AddOutput( netlist->getNetName(inouts[i]));
					break;
				default:
					break;
			} // end switch
		} // end for

		// Define cell transistors.
		vector<Trans> &trans = netlist->getTransistors();
		for (vector<Trans>::iterator trans_it = trans.begin(); trans_it != trans.end(); trans_it++){
			if (trans_it->type == 0){
				rc.AddTransistor( ToString(trans_it->name), RCTranslator::PMOS, ToString(netlist->getNetName(trans_it->source)), ToString(netlist->getNetName(trans_it->gate)) , ToString(netlist->getNetName(trans_it->drain)), trans_it->width  );
			} else if (trans_it->type == 1){
				rc.AddTransistor( ToString(trans_it->name), RCTranslator::NMOS, ToString(netlist->getNetName(trans_it->source)), ToString(netlist->getNetName(trans_it->gate)) , ToString(netlist->getNetName(trans_it->drain)), trans_it->width  );
			} else{
				cout << "Transistor type is not defined" << endl;
			} // end else
		} // end for

		// Run
		//ofstream file( "gp.txt" );

		rc.initialize();

		double constrArea = 1.0001;

		printFinalCircuit(file, rc, constrArea, instanceName, sizingType, optimize);
		//calculateElmoreDelay(circuit, rc);
	} catch ( RCTranslatorException &e ) {
		cerr << "[EXCEPTION] " << e.what() << "\n";
		return;
	} // end catch
} // end method

//------------------------------------------------------------------------------

Inst *Size::findDriver (Circuit *circuit, CellNetlst *topNetlist, t_net net){

	list<t_inst> &insts = net.insts;
	for (list<t_inst>::iterator insts_it = insts.begin() ; insts_it != insts.end(); insts_it++) {
		Inst &inst = topNetlist->getInstance( insts_it->targetCellInst );
		CellNetlst* netlist = circuit->getCellNetlst(inst.subCircuit);

		IOType type = netlist->getIOType( insts_it->targetPin );
		if ( type == IOTYPE_OUTPUT )
			return &inst;
	}

	return NULL;

} // end function

//------------------------------------------------------------------------------

list<Inst*> Size::findSinks(Circuit *circuit, CellNetlst *topNetlist, t_net net) {
	list<Inst*> sinks;
	
	list<t_inst> &insts = net.insts;
	for (list<t_inst>::iterator insts_it = insts.begin() ; insts_it != insts.end(); insts_it++) {
		Inst &inst = topNetlist->getInstance( insts_it->targetCellInst );
		CellNetlst* netlist = circuit->getCellNetlst(inst.subCircuit);

		IOType type = netlist->getIOType( insts_it->targetPin );
		if ( type == IOTYPE_INPUT )
			sinks.push_back(&inst);
	} // end for
	
	return sinks;
} // end method

//------------------------------------------------------------------------------

void Size::writeElmoreDelay( Circuit *c, CellNetlst *topNetlist, ofstream &file, list<Inst*> lst) {
	/*
    for ( list<Inst*>::iterator instances_it = lst.begin(); instances_it != lst.end(); instances_it++ ){
        Inst *inst = *instances_it;
   
        file << "D_" << inst->name << " = (delay_"  << inst->name;
		int cont = 0;
        vector<int> &ports = inst->ports;
        for (int i=0; i<ports.size(); i++) {
            t_net &net = topNetlist->getNet(ports[i]);
            if ( net.name != c->getVddNet() && net.name != c->getGndNet() ) {
                Inst *driver = findDriver(c, topNetlist, net);
                if ( inst != driver ) {
                    //cout << ", ";
                   
                    if ( driver == NULL ){
                        //cout << 0;
					}
                    else{
                        if (cont == 0) {
							file << ") + max(D_" << driver->name;
							cont ++;
						}
						else
							file << ", D_" << driver->name;
					} // enf else
                } // end if
               
            } // end if
        } // end for
		file << ");\n";	
	} // end for
	*/
} // end function

// -----------------------------------------------------------------------------

bool Size::fanout4(Circuit* c){
    int k;
    float outcap = 1.0792e-15;
    float cinport=0;
    float cap;
	double Xn_350 = 1;
	double Xp_350 = 1.6;
	//values for 45nm
	double Xn = 0.09;
	double Xp = 0.135;
	double cellArea = 0;
    queue<string> instancesFIFO;
	string technology = "45nm";
    string top = c->getTopCell();
    if ( top == "" ) {
        cout << "Top Cell not defined!\n";
        return false;
    }

    CellNetlst *netlist = c->getCellNetlst( c->getTopCell() );
    map<string,Inst> &instances = netlist->getInstances();
    vector<Trans>& trans = netlist->getTransistors();

    cout << "\n\n\n";

    //Name and size of the cells in a given circuit and circuit area
    map<string,Inst>::iterator it;
    for ( it = instances.begin(); it != instances.end(); it++ ) {
            Inst instance = it->second;
            cout << instance.subCircuit << "\t" << instance.m << "\t" << instance.cinPort << endl;
    }

    //FAZENDO M=1 PARA TODAS AS CELULAS E CALCULANDO A CAPACITANCIA

    map<string, CellNetlst>::iterator cells_it;
    for ( cells_it = c->getCellNetlsts()->begin(); cells_it != c->getCellNetlsts()->end(); cells_it++ ){ //for all netlist cells

     //To set the same transistor size for all cells - it based in commercial library size 1 for 350nm
    for(int x=0; x<cells_it->second.size(); x++){       // for all cell transistors
        if(cells_it->second.getTrans(x).type==PMOS)     //Sizing different for P and N transistors
			cells_it->second.getTrans(x).width = Xp;
		else
			cells_it->second.getTrans(x).width = Xn;
	}


     //Calculate IO type for each pin
    cells_it->second.calcIOs(c->getGndNet(), c->getVddNet());
    //Calculate input capacitance of each cell
    cells_it->second.calculateInputCapacitances();

        cinport = 0;
        //cout << cells_it->first << "\n";
    vector< pair<int,double> > &cins = cells_it->second.getCins();
        for( k=0; k< cins.size(); k++)
           cinport += cins[k].second;

        cinport = cinport/k;
        //cout << "\ncinport:" << cinport << endl;

        for ( it = instances.begin(); it != instances.end(); it++ ) {
            it->second.m = 1;
            if(cells_it->first.compare(it->second.subCircuit) == 0)
                it->second.cinPort = cinport;
        }
    }

    cout << "\n\n\n";

    for ( it = instances.begin(); it != instances.end(); it++ ) {
            Inst instance = it->second;
            cout << instance.subCircuit << "\t" << instance.m << "\t" << instance.cinPort << endl;
    }

    map<string, Interface> *interfaces = c->getInterfaces();
    map<string, Interface>::iterator interfaces_it;
    for ( interfaces_it = interfaces->begin(); interfaces_it != interfaces->end(); interfaces_it++ ) {

		if ( interfaces_it->second.ioType != IOTYPE_OUTPUT ) continue;

		//cout << "\nOut:" << interfaces_it->first << "\n";
		t_net &net = netlist->getNet(interfaces_it->first);
		addInstances(instancesFIFO, net);
		
		const list<t_inst> &insts = net.insts;
		list<t_inst>::const_iterator it;
		for (it = insts.begin(); it != insts.end(); it++){
			Inst &insx = netlist->getInstance(it->targetCellInst);
		   // cerr << "\t\tNeighbour: " << it->targetCellInst << "\t\tTargetPin: "<< IOTypeName[it->targetPin] << "\n";
			//cap = cap/4;
			double dou = outcap/insx.cinPort;
			int roundd = (dou*10)+0.5;
			double f = roundd;
			dou = f/10;
			insx.m = dou;
			insx.cinPort = insx.cinPort*insx.m;
			cout << "Cin_" << insx.subCircuit << " = " << insx.cinPort << endl;
			insx.instanceSized = true;
	
		}

		
    }

    map<string,Inst>::iterator instances_it;
    for (instances_it = instances.begin(); instances_it != instances.end(); instances_it++){
    instances_it->second.instanceVisited = false; // set all instances as false, because they weren't visited
        instances_it->second.instanceSized = false;
    }

   cap = outcap;
	double dou = 0;
	int roundd = 0;
   //FAZENDO O DIMENSIONAMENTO DAS INSTANCIAS

    while( !instancesFIFO.empty() ) {
		string sn = instancesFIFO.front();
		instancesFIFO.pop();

		Inst &inst = netlist->getInstance(sn);
			   //Trans &trans = netlist->getTrans(sn); //pega todos os transistores dessa instância
		if ( inst.instanceVisited )
			continue;

		if ( !inst.instanceSized){
				//cap = cap/4;
				dou = cap/(inst.cinPort*4);
				roundd = (dou*10)+0.5;
				double f = roundd;
				dou = f/10;
				inst.m = dou;
				inst.cinPort = inst.cinPort*inst.m;
				cout << "Cin_" << inst.subCircuit << " = " << inst.cinPort << endl;
				inst.instanceSized = true;
		 }

		//cout << "\nInstance: " << sn << "\t" << inst.subCircuit << "\t" << inst.m << "\t" << inst.cinPort << "\n";

		inst.instanceVisited = true;

		cap = inst.cinPort/4;

		vector<int> &ports = inst.ports;
		for (int i=0; i<ports.size(); i++) {
			t_net &net = netlist->getNet(ports[i]);

				if ( net.name != c->getVddNet() && net.name != c->getGndNet() ) {
					//cerr << "\tNet: " << net.name << "\n";

					const list<t_inst> &insts = net.insts;
					list<t_inst>::const_iterator it;
					for (it = insts.begin(); it != insts.end(); it++){
						Inst &insx = netlist->getInstance(it->targetCellInst);
					   // cerr << "\t\tNeighbour: " << it->targetCellInst << "\t\tTargetPin: "<< IOTypeName[it->targetPin] << "\n";

						if(!insx.instanceSized){
							//cap = cap/4;
							dou = cap/insx.cinPort;
							roundd = (dou*10)+0.5;
							double f = roundd;
							dou = f/10;
							insx.m = dou;
							insx.cinPort = insx.cinPort*insx.m;
							cout << "Cin_" << insx.subCircuit << " = " << insx.cinPort << endl;
							insx.instanceSized = true;
						}

					}

					addInstances(instancesFIFO, netlist->getNet(ports[i]));

				}

		}
    }


     cout << "\n***** TERMINADO O DIMENSIONAMENTO! *****\n" << endl;

    //Name and size of the cells in a given circuit and circuit area
     for ( it = instances.begin(); it != instances.end(); it++ ) {
            Inst instance = it->second;
            cout << instance.subCircuit << "\t" << instance.m << "\t" << instance.cinPort << endl;
    }

	elmoredelay ed;
	ed.elmoreFO4(c);

    return true;
}

/*
IOType getIOType(int net){return inouts_type[net];};
string& getNetName(int n){return nets[n].name;};  // Return the name of a net
t_net& getNet(int n){return nets[n];};            // Return the data structure of a net (list of places where it connects)
vector<t_net>& getNets(){return nets;};           // Return the list of nets of the cell
vector<int> ports;                                // Ordered list of nets that connect to the I/Os of the cell
vector<int> inouts;                               // Ordered List of cell I/Os
string getInout(int n);                           // Return the name of a I/O port
vector<int>& getInouts(){return inouts;};         // Return the list of I/O ports
*/


// -----------------------------------------------------------------------------

struct FunctorImprime {
	void operator()( Inst * inst ) {
		cerr << inst->name << "\n";
	} 
}; // struct

struct FunctorElmoreDelay {
	void operator()( Inst * inst ) {
		cerr << "\t" <<inst->name << "\n";
	} 
}; // struct


struct FunctorReverseListing {
	list<Inst*> lst;
	void operator()( Inst *inst ) {
		lst.push_front(inst);
	}
};

	
bool Size::gp(Circuit* c){

	//transitorSizing(c,  c->getCellNetlst( c->getTopCell() ), file );
	//return true;
	double constrArea =  3.6;
	double constrDelay = 1.04057e-10;
	string technology = "45nm";
	string optimize = "delay";
	string sizingType = "gate";
	double Cload = 10*1.87367e-16;
	double restrCin = 4;
	
	ofstream file( "gp.m" );

	
	ofstream script( "script.bat" );
	ofstream simulate( "simulate.sh" );
	ofstream copyarq( "copyarq.bat" );
	string top = c->getTopCell();
	ofstream subckt;
	string filename = "subckt_" + top + ".sp";
	subckt.open(filename.c_str()); // Write
	
	int contInstance = 0;
	map<string,bool> outputs;
	map<string,bool> inputs;
    //CellNetlst flatten = c->getFlattenCell(c->getTopCell());
    //CellNetlst *netlist = &flatten;
	//float circuitArea =0;
    queue<string> instancesFIFO;
	string netOut;
	//values for 350nm
	double Xn_350 = 1;
	double Xp_350 = 1.6;
	//values for 45nm
	double Xn = 1;
	double Xp = 1;
	double areaCircuit = 0;
	

    if ( top == "" ) {
        cout << "Top Cell has not been set!\n";
        return false;
    } // end if
    else
	cout << "Top Cell: " << top << endl;

    CellNetlst *netlist = c->getCellNetlst( c->getTopCell() );
    map<string,Inst> &instances = netlist->getInstances();
	//vector<Trans> &trans = netlist->getTransistors();
	
	//calculate execution time
	Stopwatch watch;

	watch.start();

	//SET ALL NETS FALSE (INPUT)
	vector<t_net>& nets=c->getCellNetlst(c->getTopCell())->getNets();
    for(vector<t_net>::iterator nets_it=nets.begin(); nets_it!=nets.end(); nets_it++){
		outputs.insert(pair<string,bool> (nets_it->name, false));
		inputs.insert(pair<string,bool> (nets_it->name, false));
    }//end for

    //Passe all cells of the netlist
    map<string, CellNetlst>::iterator cells_it;
    for ( cells_it = c->getCellNetlsts()->begin(); cells_it != c->getCellNetlsts()->end(); cells_it++ ){ //for all netlist cells
		//To set the same transistor size for all cells - it based in commercial library size 1 for 350nm
		for(int x=0; x<cells_it->second.size(); x++){       // for all cell transistors
			if(cells_it->second.getTrans(x).type==PMOS)     //Sizing different for P and N transistors
				cells_it->second.getTrans(x).width = 0.135;
			else
				cells_it->second.getTrans(x).width = 0.09;
			cells_it->second.getTrans(x).length = 0.05;
			//cout << cells_it->second.getTrans(x).name << " " << " L=" << cells_it->second.getTrans(x).length << "U W=" << cells_it->second.getTrans(x).width << "U"<< endl;

		}

		//Calculate IO type for each pin
		cells_it->second.calcIOs(c->getGndNet(), c->getVddNet());


		//Calculate input capacitance of each cell
		cells_it->second.calculateInputCapacitances();
		cout << cells_it->first << "\n";
		vector< pair<int,double> > &cins = cells_it->second.getCins();
		for( int l=0; l< cins.size(); l++) {
			cout << "\t"<< cins[l].second << endl;
		} // end for
		int k;
		double cinport = 0;
        //cout << cells_it->first << "\n";
        for(k=0; k< cins.size(); k++){
			cinport += cins[k].second;
		}
		cinport = cinport/k;
        //cout << "\ncinport:" << cinport << endl;
        for ( map<string,Inst>::iterator it = instances.begin(); it != instances.end(); it++ ) {
            it->second.m = 1;
            if(cells_it->first.compare(it->second.subCircuit) == 0)
                it->second.cinPort = cinport;
        }

    }
	netlist->calcIOs(c->getGndNet(), c->getVddNet());
	
	string netOutput;
    map<string, Interface> *interfaces = c->getInterfaces();
    for ( map<string, Interface>::iterator interfaces_it = interfaces->begin(); interfaces_it != interfaces->end(); interfaces_it++ ) {

		if ( interfaces_it->second.ioType != IOTYPE_OUTPUT ) continue;

		t_net &net = netlist->getNet(interfaces_it->first);
		netOutput = net.name;
		outputs[net.name] = true;
		addInstances(instancesFIFO, net);


    } // end for
	
	for ( map<string, Interface>::iterator interfaces_it = interfaces->begin(); interfaces_it != interfaces->end(); interfaces_it++ ) {

		if ( interfaces_it->second.ioType == IOTYPE_OUTPUT ) continue;

		t_net &net = netlist->getNet(interfaces_it->first);
		inputs[net.name] = true;
    } // end for
	
	

	file << "gpvar";
	for (map<string,Inst>::iterator instances_it = instances.begin(); instances_it != instances.end(); instances_it++){
		file << " " << instances_it->first;
    }//end for
	file << ";" << endl;

    cout << "Imprimindo interfaces...\n";

	for (map<string,Inst>::iterator instances_it = instances.begin(); instances_it != instances.end(); instances_it++){
		calculateCload(c, netlist, c->getCellNetlst( instances_it->second.subCircuit ), instances_it->first, instances_it->second, file, netOutput, Cload, outputs);
		file << "Cin_" << instances_it->first << " = " << instances_it->second.cinPort << " * " << instances_it->first << ";" << endl;
	}//end for

    for (map<string,Inst>::iterator instances_it = instances.begin(); instances_it != instances.end(); instances_it++){
		instances_it->second.instanceVisited = false; // set all instances as false, because they weren't visited
		transistorSizing(c,  c->getCellNetlst( instances_it->second.subCircuit ), file, instances_it->first, contInstance, sizingType, constrArea, Cload, technology);
		contInstance++;
    }//end for

	if (optimize == "delay"){
		file << "gpvar";
		for(vector<t_net>::iterator nets_it=nets.begin(); nets_it!=nets.end(); nets_it++){
			if(nets_it->name!=c->getVddNet() && nets_it->name!=c->getGndNet()){
				file << " " << nets_it->name;
			}//end if
		}//end for
		file << ";" << endl;
	}// end if
	
	if (sizingType == "gate"){
		vector< pair<int,double> > &cins = netlist->getCins();
		file << "Power = (";
		for (map<string,Inst>::iterator instances_it = instances.begin(); instances_it != instances.end(); instances_it++){
			if (instances_it != instances.begin() )
				file <<  " + ";
			file << "Cload_" << instances_it->first;
		}//end for
		
		
		
		
		netlist->calcIOs(c->getGndNet(), c->getVddNet());
		for ( vector<int>::iterator inouts_it=netlist->getInouts().begin(); inouts_it != netlist->getInouts().end(); inouts_it++ ){
			cout << "Tipe pin " << netlist->getNetName(*inouts_it) << " = " << netlist->getIOType(*inouts_it) << endl;
			if (netlist->getIOType(*inouts_it) == IOTYPE_INPUT){
				cout << "Pin: " << *inouts_it << " Cin: " << cins[*inouts_it].second << endl;
				
			} // end if
			
		} // end for
		file << ") * 1.1^2 * 0.5;" << endl; //Faz o somatório dos Cloads + Cin (do circuito) * Vdd^2 * 0.5 (considera que as portas chaveiam em 50% do tempo
		
		
		file << "Abase = " ;
		for (map<string,Inst>::iterator instances_it = instances.begin(); instances_it != instances.end(); instances_it++){
			if (instances_it != instances.begin() )
				file <<  " + ";
			file << "Abase_" << instances_it->first;
		}//end for
		file << ";" << endl;

		file << "Afinal = " ;
		for (map<string,Inst>::iterator instances_it = instances.begin(); instances_it != instances.end(); instances_it++){
			if (instances_it != instances.begin() )
				file <<  " + ";
			file << "Afinal_" << instances_it->first;
		}//end for
		file << ";" << endl;
	}//end if
	
	
	//Print objective function
	if (optimize == "area"){
		//
		// Clean-up;
		//
		for (map<string,Inst>::iterator instances_it = instances.begin(); instances_it != instances.end(); instances_it++)
			instances_it->second.instanceSized = false;	

		//
		// Add last level instances to the queue (the ones which driver output
		// nets).
		//
		for ( map<string, Interface>::iterator interfaces_it = interfaces->begin(); interfaces_it != interfaces->end(); interfaces_it++ ) {

			if ( interfaces_it->second.ioType != IOTYPE_OUTPUT )
				continue;
			
			t_net &net = netlist->getNet(interfaces_it->first);
			walkElmoreDelay( c, findDriver(c, netlist, net ), file );
		} // end for
	} // end if
	
	//Print objective function
	if (optimize == "power" || optimize == "area_delay"){
		//
		// Clean-up;
		//
		for (map<string,Inst>::iterator instances_it = instances.begin(); instances_it != instances.end(); instances_it++)
			instances_it->second.instanceSized = false;	

		//
		// Add last level instances to the queue (the ones which driver output
		// nets).
		//
		for ( map<string, Interface>::iterator interfaces_it = interfaces->begin(); interfaces_it != interfaces->end(); interfaces_it++ ) {

			if ( interfaces_it->second.ioType != IOTYPE_OUTPUT )
				continue;
			
			t_net &net = netlist->getNet(interfaces_it->first);
			walkElmoreDelay( c, findDriver(c, netlist, net ), file );
		} // end for
	} // end if
	
	
	int cont =0;
	file << "delay = max(" ;
	if (optimize == "delay"){
	
		for (map<string,bool>::iterator outputs_it = outputs.begin(); outputs_it != outputs.end(); outputs_it++){
				if (outputs_it->second == true){
					cont ++;
					if (cont > 1){
						file <<  ", ";
					}
					file << outputs_it->first;
				}//end if
		}//end for
	} //end if
	
	if ( optimize == "area" || optimize == "power" || optimize == "area_delay"){
		
		for (map<string,bool>::iterator outputs_it = outputs.begin(); outputs_it != outputs.end(); outputs_it++){
				Inst *instance;
				if (outputs_it->second == true){
					cont ++;
					if (cont > 1){
						file <<  ", ";
					}
					instance = findDriver(c, netlist, netlist->getNet(outputs_it->first));
					file << "D_" << instance->name;
				}//end if
		}//end for
		
	} // end if
	
	file << ");" << endl;
	if ( optimize == "area_delay"){
		file << "Area_delay = Afinal*delay;";
	}// end if
	
	file << endl << "%Conjunto de constraints" << endl;
	file << "\nconstr = [ " << endl;



	if (sizingType == "gate" && optimize=="delay")
	{
		file << "Afinal <= " << constrArea << " * Abase;" << endl;
		//file << "Afinal <= 3.1302;" << endl;

	}//end if
	if (sizingType == "gate" && optimize == "area"){
		file << "delay <= " << constrDelay << ";" << endl; 
	}// end if
	
	if (sizingType == "gate" && optimize == "power"){
		file << "delay <= " << constrDelay << ";" << endl;
		file << "Afinal <= " << constrArea << ";" << endl; 
	}// end if
	
	if (sizingType == "gate" && optimize == "area_delay"){
		file << "delay <= " << constrDelay << ";" << endl;
		file << "Afinal <= " << constrArea << " * Abase;" << endl; 
	}// end if
	
	for (map<string,Inst>::iterator instances_it = instances.begin(); instances_it != instances.end(); instances_it++){
		findCircuitInputs( c, netlist, c->getCellNetlst( instances_it->second.subCircuit ), instances_it->first, instances_it->second, file, inputs, restrCin);
	}//end for
	
	for (map<string,Inst>::iterator instances_it = instances.begin(); instances_it != instances.end(); instances_it++){
		printConstraints(c,  c->getCellNetlst( instances_it->second.subCircuit ), file, instances_it->first, contInstance, sizingType, optimize);
	}//end for

	if (optimize == "delay"){
		for (map<string,Inst>::iterator instances_it = instances.begin(); instances_it != instances.end(); instances_it++){
			circuitSize(c, netlist, c->getCellNetlst( instances_it->second.subCircuit ), instances_it->first, instances_it->second, file);
		}//end for
	}//end if

	file << "];" << endl;

	file << endl << "% Resolver a GP"<< endl;
	
	if (optimize == "area"){
		file << "[min_Afinal,solution,status] = gpsolve(Afinal, constr, 'min')" << endl;
	}//end if
	if (optimize == "delay"){
		file << "[min_delay,solution,status] = gpsolve(delay, constr, 'min')" << endl;
	}//end if
	
	if (optimize == "power"){
		file << "[min_Power,solution,status] = gpsolve(Power, constr, 'min')" << endl;
	}//end if
	
	if (optimize == "area_delay"){
		file << "[min_Area_delay,solution,status] = gpsolve(Area_delay, constr, 'min')" << endl;
	}//end if
	
    file << endl << "% Converte as vari·veis GP para doubles" << endl;
    file << "assign(solution);" << endl;
	
	if (sizingType == "gate"){
		if (optimize == "delay"){
			file << "a = eval( Afinal, solution);" << endl;
			file << "p = eval( Power, solution);" << endl;
			
		}
		if (optimize == "area"){
			file << "a = eval( delay, solution);" << endl;
			file << "p = eval( Power, solution);" << endl;
		} // end if
		if (optimize == "power"){
			file << "a = eval( Afinal, solution);" << endl;
			file << "d = eval( delay, solution);" << endl;
		} // end if
		
		if (optimize == "area_delay"){
			file << "a = eval( Afinal, solution);" << endl;
			file << "d = eval( delay, solution);" << endl;
			file << "p = eval( Power, solution);" << endl;
		} // end if	
		
		//script << "cd Desktop/Linux/graci/astran/snv/ICPD1/build/Debug/" << endl;
		//script << "echo \"ENTROU NO SCRIPT\" " << endl;
		script << "matlab -nodisplay -nosplash -nodesktop -wait -r gp" << endl;
		file << "save arq.txt ";
		for (map<string,Inst>::iterator instances_it = instances.begin(); instances_it != instances.end(); instances_it++){
			file << instances_it->first << " ";
		}//end for
		if (optimize == "area"){
			file << "min_Afinal a p -ascii;" << endl;
		}// end if
		if (optimize == "delay"){
			file << "min_delay a p -ascii;" << endl;
		} // end if
		if (optimize == "power"){
			file << "min_Power a d -ascii;" << endl;
		} // end if
		
		if (optimize == "area_delay"){
			file << "min_Area_delay a d p -ascii;" << endl;
		} // end if
		
		file << endl;
		file << "exit;" << endl;
		file << "exit" << endl;

		//script << "open arq.txt" << endl;
	}//end if
	script.close();
	file.close();

	//USING WINDOWS
	//cout << "passou 1" << endl;
	system("chmod 755 script.bat");
	//cout << "passou 2" << endl;
	system("script.bat");

	//USING MAC
	system("chmod 755 /Users/gposser/Desktop/astran/xcode/build/Debug/script.bat");
	system("/Users/gposser/Desktop/astran/xcode/build/Debug/script.bat");
	
	

	//cout << "passou 3" << endl;
	while( !instancesFIFO.empty() ) {
		string e = instancesFIFO.front();
		instancesFIFO.pop();

		Inst &inst = netlist->getInstance(e);
		//Trans &trans = netlist->getTrans(e); //pega todos os transistores dessa instância
		if ( inst.instanceVisited )
			continue;

		//cerr << "Instance: " << e << "\t" << inst.subCircuit << "\n";

		inst.instanceVisited = true;
		vector<int> &ports = inst.ports;
		for (int i=0; i<ports.size(); i++) {
			t_net &net = netlist->getNet(ports[i]);
			if ( net.name != c->getVddNet() && net.name != c->getGndNet() ) {
				addInstances(instancesFIFO, netlist->getNet(ports[i]));

			} // end if
		} // end for

    } // end while
	string entrada;
	//const int MAX=80;
	//char buff[MAX];
	vector<double> gateSize;
	double dou = 0;
	ifstream cin("arq.txt");
	int roundd = 0;
	double minDelay;
	double powerCircuit;
	double produto;
	if (sizingType == "gate"){
	//ifstream fin("arq.txt");

/*
		while (getline(cin, entrada)) {
			//fin.getline(buff, MAX);
			string aux = "";
            int pos = 3;
            while ( entrada[pos] != ' '){
                aux += entrada[pos];
                pos++;
            }
            //nodos = atoi(aux.c_str());
			dou= atof(aux.c_str());
			minDelay = dou;             //para manter o valor do delay no formato double
			//roundd = (dou*10)+0.5;
			//double f = roundd;
			//dou = f/10;
			//cout << "ROUND: " << roundd << endl;
			//dou = round(dou, 1);
			gateSize.push_back(dou);
		}
*/

		for ( int i = 0; i < instances.size(); i++ ) {
			cin >> dou;
			gateSize.push_back(dou);
		} // end for
		if (optimize == "area_delay"){
			cin >> produto;
			cin >> areaCircuit;
			cin >> minDelay;
			cin >> powerCircuit;
		} // end if
		else{
			cin >> minDelay;
			cin >> areaCircuit;
			cin >> powerCircuit;
		} // end if
		
		int cont = 0;
		for (map<string,Inst>::iterator instances_it = instances.begin(); instances_it != instances.end(); instances_it++){
			instances_it->second.m = gateSize[cont];
			cout << "M " << instances_it->first << " " << instances_it->second.m << endl;
			cont++;
		}//end for

		if (optimize == "area"){ 
			cout << "Area GP: " << minDelay << endl;
			cout << "Delay do Circuito: " << areaCircuit << endl;
			cout << "Potência do Circuito: " << powerCircuit << endl;
		}// end if
		if (optimize == "delay"){
			cout << "Timing GP: " << minDelay << endl;
			cout << "Area do Circuito: " << areaCircuit << endl;
			cout << "Potência do Circuito: " << powerCircuit << endl;
		} // end if
		
		if (optimize == "power"){
			cout << "Mínima Potência do Circuito: " << minDelay << endl;
			cout << "Area do Circuito: " << areaCircuit << endl;
			cout << "Timing GP: " << powerCircuit << endl;
		} // end if
	
		if (optimize == "area_delay"){
			cout << "Area*delay: " << produto << endl;
			cout << "Area do Circuito: " << areaCircuit << endl;
			cout << "Timing GP: " << minDelay << endl;
			cout << "Potência do Circuito: " << powerCircuit << endl;
		} // end if

	}//end if
	
	watch.stop();

	cerr << "Elapsed time: " << watch.getElapsedTime() << "s\n";
	
	elmoredelay ed;
	ed.elmoreFO4(c);
	
	/*-------------------------------------*************************************
	
	
	//READ THE CRITICAL PATH FILE
	vector<string> criticalSubCircuit;
	ifstream  arq("crit.txt");
	string netInitial;
	string netFinal;
	string aux;
	arq >> netInitial;
	arq >> netFinal;
	
	//cout << "Net Entrada:" << netInitial << endl;
	//cout << "Net Saida:" << netFinal << endl;
	while (arq) {
		arq >> aux;
		criticalSubCircuit.push_back(aux);
	}
	
	t_net netEntrada = netlist->getNet(netInitial);
	t_net netSaida = netlist->getNet(netFinal);
	
	simulate << "rm -r " << top << endl;
	simulate << "mkdir " << top << endl;
	
	simulate << "source /opt/ferramentas/scripts/setup.synopsys" << endl;
	
	copyarq << "scp ";
	for (int i=0; i<(criticalSubCircuit.size()-1); i++){
		//cout << criticalSubCircuit[i] << endl;
		Inst instance = netlist->getInstance(criticalSubCircuit[i]);
		printSpice(*c, instance, simulate, copyarq, top);
	}
	
	copyarq << " simulate.sh 143.54.10.45:~/Desktop/spices45nm/." << endl;
	
	*/
	
			
	for (map<string,Inst>::iterator instances_it = instances.begin(); instances_it != instances.end(); instances_it++){
		printSpiceCarac(*c, instances_it->second, simulate, copyarq, top, subckt);
	}//end for
	subckt.close();
	simulate << "source /opt/ferramentas/scripts/setup.cadence" << endl;
	simulate << "source /opt/ferramentas/scripts/setup.synopsys" << endl;
	simulate << "elc -S script_" << top << endl;
	simulate << "sh clear.sh" << endl;
	simulate.close();
	
	copyarq << "scp setup_" << top << ".txt subckt_" << top << ".sp script_" << top;  
	copyarq << " simulate.sh 143.54.10.45:~/Desktop/carac/carac_45nm/." << endl;
	copyarq.close();
	printSetupCarac(*c, simulate, copyarq, top); 
	printScriptCarac(*c, top);
	
    return true;
}

// -----------------------------------------------------------------------------
bool Size::printSpiceCarac(Circuit& circuit, Inst &instance, ofstream &simulate, ofstream &copyarq, string &top, ofstream &subckt){

	string netOut;
	if (!subckt)
		return false;
		
	CellNetlst *netlist = circuit.getCellNetlst( instance.subCircuit );
	
	subckt << ".SUBCKT " <<  instance.subCircuit << "_" << instance.name;
	vector<int> &ports = instance.ports;
	for ( vector<int>::iterator inouts_it=netlist->getInouts().begin(); inouts_it != netlist->getInouts().end(); inouts_it++ )
		subckt << " " << netlist->getNetName(*inouts_it);
	//if(instance.m!=1) file << " M=" << instance.m;
	subckt << endl;
	vector<Trans> trans = netlist->getTransistors();
	for (int x = 0; x < trans.size(); x++){
		subckt << trans[x].name << " " << 
		netlist->getNetName(trans[x].drain) << " " << 
		netlist->getNetName(trans[x].gate) << " " << 
		netlist->getNetName(trans[x].source) << " ";
		if(trans[x].type==PMOS) 
			subckt << circuit.getVddNet() << " PMOS_VTG";
		else
			subckt << circuit.getGndNet() << " NMOS_VTG";
		subckt << " L=" << trans[x].length << "E-6 W=" << trans[x].width*instance.m << "E-6"<< endl;	
	} // end if
	subckt << ".ENDS " << instance.subCircuit << "_" << instance.name << endl << endl;
	
	for ( vector<int>::iterator inouts_it=netlist->getInouts().begin(); inouts_it != netlist->getInouts().end(); inouts_it++ ){
		if (netlist->getIOType(*inouts_it) == IOTYPE_OUTPUT)
			netOut = netlist->getNetName(*inouts_it);
	} // end for
	
	return true;
}

// -----------------------------------------------------------------------------
bool Size::printSetupCarac(Circuit& circuit, ofstream &simulate, ofstream &copyarq, string &top){
		
	//CellNetlst *netlist = circuit.getCellNetlst( instance.subCircuit );
	
	ofstream setup;
	string filename = "setup_" + top + ".txt";
	setup.open(filename.c_str()); // Write
	CellNetlst *netlist = circuit.getCellNetlst( circuit.getTopCell() );
    map<string,Inst> &instances = netlist->getInstances();
	
	
	setup << "//Setup file for simulation in 45nm\n" << endl;
	 
	setup << "Process typical {" << endl;
	setup << "\tvoltage = 1.1; // as voltage" << endl;
	setup << "\ttemp = 25 ; /* as temperature */" << endl; 
	setup << "\tVtn = 0.15 ; // nmos Vt" << endl;
	setup << "\tVtp = 0.15 ; // pmos Vt" << endl;
	setup << "};\n" << endl;
	
	
	setup << "Signal std_cell {" << endl;
	setup << "\tunit = REL ; // relative value" << endl;
	setup << "\tVh = 1.0 1.0 ; // 100% rise/fall" << endl;
	setup << "\tVl = 0.0 0.0 ; " << endl;
	setup << "\tVth = 0.5 0.5 ; // 50% rise/fall" << endl;
	setup << "\tVsh = 0.8 0.8 ;" << endl;
	setup << "\tVsl = 0.2 0.2 ;" << endl;
	setup << "\ttsmax = 2.0n ; // maximum output slew rate" << endl;
	setup << "};\n" << endl;
	
	setup << "Simulation std_cell {" << endl; 
	setup << "\ttransient = 0.2n 80n 10p ;" << endl;
	setup << "\tbisec = 4.0n 4.0n 10ps ; // binary search" << endl; 
	setup << "\tresistance = 10K;" << endl; 
	setup << "};\n" << endl;
	
	for (map<string,Inst>::iterator instances_it = instances.begin(); instances_it != instances.end(); instances_it++){
		setup << "Index " <<  instances_it->second.subCircuit << "_" << instances_it->second.name << " {" << endl;
		setup << "\tslew = 0.007500N 0.018750N 0.037500N 0.075000N 0.150000N 0.300000N 0.600000N ;" << endl;
		setup << "\tload = 0.000400P " << instances_it->second.Cload << " " << instances_it->second.Cload*2 << " ;" << endl;
		setup << "} ;\n" << endl;
	}//end for
	
	for (map<string,Inst>::iterator instances_it = instances.begin(); instances_it != instances.end(); instances_it++){
		setup << "Group " << instances_it->second.subCircuit << "_" << instances_it->second.name << " {" << endl;
		setup << "\tCELL = *" << instances_it->second.subCircuit << "_" << instances_it->second.name << " ;" << endl;
		setup << "} ;\n" << endl;
	} // end for
	
	setup << "Margin m0 {" << endl;
	setup << "\tsetup = 1.0 0.0 ;" << endl;
	setup << "\thold = 1.0 0.0 ;" << endl;
	setup << "\trelease = 1.0 0.0 ;" << endl;
	setup << "\tremoval = 1.0 0.0 ;" << endl;
	setup << "\trecovery = 1.0 0.0 ;" << endl;
	setup << "\twidth = 1.0 0.0 ;" << endl;
	setup << "\tdelay = 1.0 0.0 ; " << endl;
	setup << "\tpower = 1.0 0.0 ; " << endl;
	setup << "\tcap = 1.0 0.0 ;" << endl;
	setup << "} ;\n" << endl;
	
	setup << "Nominal n0 {" << endl;
	setup << "\tcap     = 0.0:0.5:1.0 0.0:0.5:1.0 ;  // rise / fall" << endl;
	setup << "\tcheck   = 1.0:1.0:1.0 1.0:1.0:1.0 ;  // rise / fall" << endl;
	setup << "\tcurrent = 0.0:0.5:1.0 0.0:0.5:1.0 ;  // rise / fall" << endl;
	setup << "\tpower   = 0.0:0.5:1.0 0.0:0.5:1.0 ;  // rise / fall" << endl;
	setup << "\tslew    = 0.0:0.5:1.0 0.0:0.5:1.0 ;  // rise / fall" << endl;
	setup << "\tdelay   = 0.0:0.5:1.0 0.0:0.5:1.0 0.0:0.5:1.0;  // rise / fall / Z" << endl;
	setup << "\tdelay = 0.5 0.5 ; // as rise fall" << endl; 
	setup << "\tpower = 0.5 0.5 ; " << endl;
	setup << "\tcap = 0.5 0.5 ;" << endl;
	setup << "} ;\n" << endl;
	
	setup << "set process (best,typical,worst) {" << endl;
	setup << "\tsimulation = std_cell  ;" << endl; 
	setup << "\tindex = X1 ;" << endl; 
	setup << "\tsignal = std_cell ;" << endl; 
	setup << "\tmargin = m0 ;" << endl; 
	setup << "\tnominal = n0 ;" << endl;
	setup << "} ;\n" << endl;
	
	setup << "set index (best,typical,worst) {" << endl;
	for (map<string,Inst>::iterator instances_it = instances.begin(); instances_it != instances.end(); instances_it++){
		setup << "Group (" << instances_it->second.subCircuit << "_" << instances_it->second.name << ") = " << instances_it->second.subCircuit << "_" << instances_it->second.name << " ;" << endl;
	} // end for
	setup << "} ;\n" << endl;
	
	setup.close();
	return true;
}

// -----------------------------------------------------------------------------
bool Size::printScriptCarac(Circuit& circuit, string &top){
		
	//CellNetlst *netlist = circuit.getCellNetlst( instance.subCircuit );
	
	ofstream scriptcarac;
	string filename = "script_" + top;
	scriptcarac.open(filename.c_str()); // Write
	
	scriptcarac << "set_var EC_SIM_USE_LSF 1 " << endl;
	scriptcarac << "set_var EC_SIM_LSF_CMD \" \" " << endl;
	scriptcarac << "set_var EC_SIM_LSF_PARALLEL 10 " << endl;
	scriptcarac << "set_var EC_SIM_TYPE \"hspice\" " << endl;
	scriptcarac << "set_var EC_SIM_NAME \"hspice\" " << endl;
	scriptcarac << "set_var EC_SPICE_SIMPLIFY 1 " << endl;
	scriptcarac << "set_var EC_DISABLE_BUNDLING 1 \n" << endl;
	
	scriptcarac << "set_var SUBCKT \"subckt_" << top << ".sp\"" << endl;
	scriptcarac << "set_var MODEL \"mos_45.sp\"" << endl;
	scriptcarac << "set_var SETUP \"setup_" << top << ".txt\"" << endl;
	scriptcarac << "set_var PROCESS \"typical\"" << endl;
	
	scriptcarac << "db_open basic_char" << endl; 
	scriptcarac << "db_prepare" << endl;  
	scriptcarac << "db_spice -s hspice -keep_log\n" << endl;

	scriptcarac << "db_output -process typical -alf carac.alf -lib carac.lib -report carac.report -state\n" << endl;
	
	scriptcarac << "alf2html -alf carac.alf -dir ./html\n" << endl;

	scriptcarac << "db_output -process typical -lib lib_" << top << ".lib -state\n" << endl;
	
	scriptcarac << "db_close" << endl; 
	scriptcarac << "exit" << endl;
	
	scriptcarac.close();
	return true;
}



// -----------------------------------------------------------------------------
bool Size::printSpiceSimulation(Circuit& circuit, Inst &instance, ofstream &simulate, ofstream &copyarq, string &top){
	ofstream file;
	string filename = instance.subCircuit + "_" + instance.name + ".sp";

	string netOut;
	file.open(filename.c_str()); // Write
	if (!file)
		return false;
		
	CellNetlst *netlist = circuit.getCellNetlst( instance.subCircuit );
	printHeader (file, "* ", "");
	
	simulate << "hspice " << filename << " > out.txt" << endl;
	simulate << "sh clear.sh" << endl;
	simulate << "sh maxdelay.sh > " << top << "/delay_" << instance.name << ".txt" << endl;
	
	file << ".include mos_45.sp\n"<< endl;
	file << ".param vvdd = 1.1" << endl;
	file << ".param vgnd = 0.0\n" << endl;
	file << ".global VCC" << endl;
	file << ".global GND\n" << endl;
	file << "Vsrc vcc 0 1.1\n" << endl;
	file << ".SUBCKT " <<  instance.subCircuit;
	vector<int> &ports = instance.ports;
	for ( vector<int>::iterator inouts_it=netlist->getInouts().begin(); inouts_it != netlist->getInouts().end(); inouts_it++ )
		file << " " << netlist->getNetName(*inouts_it);
	//if(instance.m!=1) file << " M=" << instance.m;
	file << endl;
	vector<Trans> trans = netlist->getTransistors();
	for (int x = 0; x < trans.size(); x++){
		file << trans[x].name << " " << 
		netlist->getNetName(trans[x].drain) << " " << 
		netlist->getNetName(trans[x].gate) << " " << 
		netlist->getNetName(trans[x].source) << " ";
		if(trans[x].type==PMOS) 
			file << circuit.getVddNet() << " PMOS_VTG";
		else
			file << circuit.getGndNet() << " NMOS_VTG";
		file << " L=" << trans[x].length << "U W=" << trans[x].width << "U"<< endl;	
	} // end if
	file << ".ENDS " << instance.subCircuit << endl << endl;
	
	for ( vector<int>::iterator inouts_it=netlist->getInouts().begin(); inouts_it != netlist->getInouts().end(); inouts_it++ ){
		if (netlist->getIOType(*inouts_it) == IOTYPE_OUTPUT)
			netOut = netlist->getNetName(*inouts_it);
	} // end for
	
	file << "cLoad " << netOut << " 0 " << instance.Cload << "\n" << endl;
	file << instance.name; 
	for ( vector<int>::iterator inouts_it=netlist->getInouts().begin(); inouts_it != netlist->getInouts().end(); inouts_it++ )
		file << " " << netlist->getNetName(*inouts_it);
	file << " " << instance.subCircuit;
	if(instance.m!=1) file << " M=" << instance.m;
	file << endl;
	
	printAlter(instance, file, circuit);
	
	file << ".end" << endl;
	file.close();
	copyarq << filename << " ";
	
	return true;
}

// -----------------------------------------------------------------------------
void Size::printAlter(Inst &instance, ofstream &file, Circuit &circuit ){
	int numInputs = 0;
	map<string,bool> outputs;
	vector<string> inputs;
	string netOut;
	Combination combination;
	CellNetlst *netlist = circuit.getCellNetlst( instance.subCircuit );

	netlist->calcIOs(circuit.getGndNet(), circuit.getVddNet());
	for ( vector<int>::iterator inouts_it=netlist->getInouts().begin(); inouts_it != netlist->getInouts().end(); inouts_it++ ){
		//cout << netlist->getNetName(*inouts_it) << " = " << netlist->getIOType(*inouts_it) << endl;
		if (netlist->getIOType(*inouts_it) == IOTYPE_INPUT){
			numInputs++;
			inputs.push_back(netlist->getNetName(*inouts_it));
		}
		if (netlist->getIOType(*inouts_it) == IOTYPE_OUTPUT)
			netOut = netlist->getNetName(*inouts_it);
	} // end for
	int counter = 1;
	for ( int i = 0; i < inputs.size(); i++ ) {
			file << ".measure tran tpdf" << i << " trig v(" << inputs[i] <<") val=`vvdd/2` rise=1 targ v(" << netOut << ") val=`vvdd/2` fall=1" << endl;
			file << ".measure tran tpdr" << i << " trig v(" << inputs[i] <<") val=`vvdd/2` fall=1 targ v(" << netOut << ") val=`vvdd/2` rise=1\n" << endl;
			//file << ".measure diff" << i << " param='tpdr" << i << " -tpdf" << i << " ' goal = 0\n" << endl;
	} // end for
	file << ".option post\n" << endl;
	file << ".tran 1e-13 12n\n" << endl;
	
	combination.initialize( numInputs * 2 );
	do {	
		//cout << "Test Vector " << counter << "\n";
		
		for ( int i = 0; i < inputs.size(); i++ ) {
			const int i0 = i*2;
			const int i1 = i0 + 1;
			//cout << "*Input " << i << ": ";
			//cout << combination.getElement(i0) << " -> " << combination.getElement(i1) << endl;
			if (combination.getElement(i0) == 0)
				file << "Vdt" << inputs[i] << " " << inputs[i] << " 0 pulse(vgnd ";
			else
				file << "Vdt" << inputs[i] << " " << inputs[i] << " 0 pulse(vvdd ";
				
			if (combination.getElement(i1) == 0)
				file << "vgnd 10p 0p 0p 2n 4n)" << endl;
			else
				file << "vvdd 10p 0p 0p 2n 4n)" << endl;
		} // end for
		file << "\n.alter" << endl;
		counter++;
	} while ( combination.next() );	

}

// -----------------------------------------------------------------------------

void Size::walkElmoreDelay( Circuit * circuit, Inst *inst, ofstream &file ) {
	CellNetlst *topNetlist = circuit->getTopNetlist();
	CellNetlst *netlist = circuit->getCellNetlst(inst->subCircuit);
	vector<int> &ports = inst->ports;
	
	if ( !inst->instanceSized ) {
		vector<Inst*> drivers;
		for ( int l=0; l<ports.size(); l++ ) {
			t_net &net = topNetlist->getNet(ports[l]);
			if ( net.name != circuit->getVddNet() && net.name != circuit->getGndNet() ) {
				if (netlist->getIOType(l) == IOTYPE_INPUT){
					Inst *driver = findDriver(circuit, topNetlist, net);
				
					if ( driver ) {
						drivers.push_back(driver);
						walkElmoreDelay(circuit, driver, file );
					} // end if
				}//end if
			}// end if
		}//end for
		
		string delay = "D_" + inst->name + " = delay_" + inst->name;
		if ( drivers.size() > 0 ) {
			if ( drivers.size() == 1 )
				delay += " + D_" + drivers.front()->name;
			else {
				delay += " + max( ";
				delay += "D_" + drivers.front()->name;
				for ( int i = 1; i < drivers.size(); i++ )
					delay += ", D_" + drivers[i]->name;
				delay += " )";
			} // end else
		} // end if
		
		inst->instanceSized = true;
		
		file << delay << ";\n";
	} // end if
} // end method

// -----------------------------------------------------------------------------

double Size::FO4Sizing( Circuit * circuit, Inst * inst, double Cload, map<string,bool> &outputs ) {
	CellNetlst *topNetlist = circuit->getTopNetlist();
	CellNetlst *netlist = circuit->getCellNetlst(inst->subCircuit);
	vector<int> &ports = inst->ports;
	
	if ( !inst->instanceSized ) {
		double cloadTotal = 0;
		
		for (int l=0; l<ports.size(); l++)	{
			t_net &net = topNetlist->getNet(ports[l]);
			if ( net.name != circuit->getVddNet() && net.name != circuit->getGndNet() ) {
				if (netlist->getIOType(l) == IOTYPE_OUTPUT){
					if ( outputs[net.name] == true ) {
						cloadTotal += Cload;
					} else {
						list<t_inst> insts = net.insts;
						for (list<t_inst>::const_iterator it = insts.begin(); it != insts.end(); it++){
							if (it->targetCellInst != inst->name){
								Inst targetInstan = topNetlist->getInstance(it->targetCellInst);
								cloadTotal += FO4Sizing( circuit, &targetInstan, Cload, outputs );
							}//end if
						}//end for
					}//end else
				}//end if
			}// end if
		}//end for
		
		inst->m = cloadTotal / (4*inst->cinPort);
		cout << inst->name << " = " << inst->m << endl;
		inst->instanceSized = true;
	} // end if

	return inst->cinPort * inst->m;
} // end method

// -----------------------------------------------------------------------------

bool Size::fo4(Circuit *circuit) {
	CellNetlst *netlist = circuit->getCellNetlst(circuit->getTopCell());
	//values for 350nm
	double Xn_350 = 1;
	double Xp_350 = 1.6;
	//values for 45nm
	double Xn = 0.09;
	double Xp = 0.135;
	double Cload = 2*1.87367e-16;
	int k;
	double cinport = 0;
	map<string,bool> outputs;
	
	map<string,Inst> &instances = netlist->getInstances();
	map<string, CellNetlst>::iterator cells_it;
	
	//SET ALL NETS FALSE (INPUT)
	vector<t_net>& nets=circuit->getCellNetlst(circuit->getTopCell())->getNets();
    for(vector<t_net>::iterator nets_it=nets.begin(); nets_it!=nets.end(); nets_it++){
		outputs.insert(pair<string,bool> (nets_it->name, false));
    }//end for
	
    for ( cells_it = circuit->getCellNetlsts()->begin(); cells_it != circuit->getCellNetlsts()->end(); cells_it++ ){ //for all netlist cells

		//To set the same transistor size for all cells - it based in commercial library size 1 for 350nm
		for(int x=0; x<cells_it->second.size(); x++){       // for all cell transistors
			if(cells_it->second.getTrans(x).type==PMOS)     //Sizing different for P and N transistors
				cells_it->second.getTrans(x).width = Xp;
			else
				cells_it->second.getTrans(x).width = Xn;
		}
		//Calculate IO type for each pin
		cells_it->second.calcIOs(circuit->getGndNet(), circuit->getVddNet());
		//Calculate input capacitance of each cell
		cells_it->second.calculateInputCapacitances();
        cinport = 0;
		vector< pair<int,double> > &cins = cells_it->second.getCins();
        for( k=0; k< cins.size(); k++)
           cinport += cins[k].second;

        cinport = cinport/k;

        for ( map<string,Inst>::iterator it = instances.begin(); it != instances.end(); it++ ) {
            it->second.m = 1;
            if(cells_it->first.compare(it->second.subCircuit) == 0)
                it->second.cinPort = cinport;
        }
		
    } // end for
	
	map<string, Interface> *interfaces = circuit->getInterfaces();
    for ( map<string, Interface>::iterator interfaces_it = interfaces->begin(); interfaces_it != interfaces->end(); interfaces_it++ ) {

		if ( interfaces_it->second.ioType != IOTYPE_OUTPUT ) continue;

		t_net &net = netlist->getNet(interfaces_it->first);
		outputs[net.name] = true;
    } // end for
	
	//
	// Clean-up;
	//
	
	for (map<string,Inst>::iterator instances_it = instances.begin(); instances_it != instances.end(); instances_it++)
		instances_it->second.instanceSized = false;	

	//
	// Add last level instances to the queue (the ones which driver output
	// nets).
	//

    for ( map<string, Interface>::iterator interfaces_it = interfaces->begin(); interfaces_it != interfaces->end(); interfaces_it++ ) {

		if ( interfaces_it->second.ioType != IOTYPE_INPUT )
			continue;
		
		t_net &net = netlist->getNet(interfaces_it->first);

		list<Inst*> sinks = findSinks(circuit, netlist, net );
		for ( list<Inst*>::iterator it = sinks.begin(); it != sinks.end(); it++ )
			FO4Sizing( circuit, *it, Cload, outputs );
    } // end for
	
	elmoredelay ed;
	ed.elmoreFO4(circuit);
	
	return true;
	
} // end method
