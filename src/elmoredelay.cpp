/*
 *  elmoredelay.cpp
 *  ICPD
 *
 *  Created by Graci Posser on 9/15/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "elmoredelay.h"

#include "size.h"

#include <iostream>
	using std::cout;
	using std::cerr;
#include <vector>
	using std::vector;
#include <iostream>
#include <fstream>
#include "RCTranslator.h"
#include <map>
#include <math.h>
#include <algorithm>

using std::max;
using namespace std;

//template para transformar dados em String
template< typename T >
inline string ToString( const T &data ) {
    ostringstream out;
    out << data;
    return out.str();
} // end function
 
double elmoredelay::calculateDelay(const RCTranslator &rc, double Xn, double Xp, double m, double CloadInst){
	double csbdb_pmos_350 = 2.2087E-15;
	double csbdb_nmos_350 = 2.4025E-15;
	double Req_pmos_350 = 2.0717E+04;
	double Req_nmos_350 = 9.1691E+03;

	//values for 45nm
	const double pnratio = 1.5;
	
	//double cgateP_45 = 6.5592E-17  * pnratio; //*1.5, pois � a rela��o P/N para a tecnologia 45n
	double cgateP = 72.88E-17; //capacitancia do transistor P calculada considerando w=1um
    //double cgateN_45 = 8.8979E-17;
	double cgateN = 98.8656E-17; //capacitancia do transistor N calculada considerando w=1um
	//double csbdb_pmos_45 = 6.4541E-17 * pnratio;
	double csbdb_pmos = 71.7122E-17; //capacitancia do transistor P calculada considerando w=1um
	//double csbdb_nmos_45 = 7.1513E-17;
	double csbdb_nmos = 79.4589E-17; //capacitancia do transistor N calculada considerando w=1um
	//double Req_pmos_45 = 5.4987E+04 / pnratio;
	double Req_pmos = 4948.83; //resistencia do transistor P para w=1um
	//double Req_nmos_45 = 1.5586E+04;
	double Req_nmos = 1402.74; //resistencia do transistor N para w=1um

	/*
	double csbdb_pmos = 6.4541E-17;
	double csbdb_nmos = 7.1513E-17;
	double Req_pmos = 5.4987E+04;
	double Req_nmos = 1.5586E+04;
	*/
	
	double delaycomb = 0;
	vector<double> Ctrans;
	vector<double> Rtrans;
	vector<double> C;
	vector<double> Cdown;

	//cout << "CLoad = " << CloadInst << endl;
	for ( int i=0; i<rc.getNumTransistors(); i++){
			switch(rc.getTransistorType(i)){
				case 0: {
						Rtrans.push_back(Req_pmos / (rc.getTransistorWidth(i) * m));
						//cout << "Rtrans_" << i << " = " << Rtrans[i] << endl;
						Ctrans.push_back(csbdb_pmos * (rc.getTransistorWidth(i) * m));
						//cout << "Ctrans_" << i << " = " << Ctrans[i] << endl;
						break;
						}
				case 1:	{
						Rtrans.push_back(Req_nmos / (rc.getTransistorWidth(i) * m));
						//cout << "Rtrans_" << i << " = " << Rtrans[i] << endl;
						Ctrans.push_back(csbdb_nmos * (rc.getTransistorWidth(i) * m));
						//cout << "Ctrans_" << i << " = " << Ctrans[i] << endl;
						break;
						}
			}//end switch
	}//end for

	//==================CALCULATE CAPACITANCE FOR EACH NODE=====================

	for ( int n = 0; n < rc.getNumNodes(); n++ ) {
		C.push_back(0);
		if ( rc.getDriverTransistor( n ) == -1 )
			continue;

		//cout << "C" + ToString(n) + " = ";						//Imprime a capacitância de cada nodo
		const vector<int> &trans = rc.getConnectedTransistors(n);
		for ( int i = 0; i < trans.size(); i++ ){
			//cerr << " " << rc.getTransistorName( trans[i] );
			if (i<(trans.size()-1)) {								//Testa se não for o último nodo vai somando, senão finaliza com ;
				//cout << "Ctrans" << trans[i] << " + ";
				C[n] = C[n] + Ctrans[trans[i]];
				//cout << "Ctrans[" << trans[i] << "] = " << Ctrans[trans[i]] << endl;
			}
			else{
				//cout << "Ctrans" << trans[i] << ";" << endl;
				C[n] = C[n] + Ctrans[trans[i]];
			}

		}
		//cout << "C" << n << " = " << C[n] << endl;
	} //end for nodes

	C[rc.getOutputNode()] += CloadInst;

	//==========CALCULATE DOWNSTREAM CAPACITANCE FOR EACH NODE==================
	for ( int j = 0; j < rc.getNumNodes(); j++ ) {
		Cdown.push_back(0);
		if ( rc.getDriverTransistor( j ) == -1 )
			continue;

		//cout << "Cdown" << j << " = Cload + ";
		Cdown[j] = 0;
		const vector<int> &nodes = rc.getDownstreamNodes(j);
		for ( int i = 0; i < nodes.size(); i++ ){
			//cerr << " " << rc.getNodeName( nodes[i] );
			Cdown[j] = Cdown[j] + C[nodes[i]];
		}//end for

		//cout << "Cdown" << j << " = " << Cdown[j] << endl;
	} // end for

	//=============CALCULATE UPSTREAM Delay FOR EACH NODE=================

	const vector<int> &upnodes = rc.getUpstreamNodes(rc.getOutputNode());
	int k = upnodes.back();
	
	delaycomb = 0;
	for ( int l = (upnodes.size()-1); l >= 0; l-- ){
		int m = upnodes[l];
		delaycomb += Rtrans[rc.getDriverTransistor(m)] * (Cdown[m]);
	}	
	
	/*
	//cout << "D" << k << " = " << "Rtrans" << rc.getDriverTransistor(k) << " * Cdown" << k << ";" << endl;
	delaycomb = Rtrans[rc.getDriverTransistor(k)] * Cdown[k];
	//cout << "RTRANS " << rc.getDriverTransistor(k) << " = " << Rtrans[rc.getDriverTransistor(k)] << endl;
	//cout << "Cdown" << k << " = " << Cdown[k] << endl;
	//cout << "D" << k << " = " << delaycomb << endl;


		//cout << "D" + ToString(cont) << j << "_" << instanceName << " = Rtrans" << rc.getDriverTransistor(j) << " * Cdown" << j << ";" << endl;
        if (upnodes.size() <= 1){
			//cout << "D" << k << " = " << "Rtrans" << rc.getDriverTransistor(k) << " * (Cdown[" << k << "] + " << CloadInst << endl;
			delaycomb = Rtrans[rc.getDriverTransistor(k)] * (Cdown[k] + CloadInst);
			//cout << "D" << k << " = " << delaycomb << endl;
    		//D += ", D" + ToString(cont) + ToString(j) + "_" + instanceName;
        }
        else{
            for ( int l = (upnodes.size()-2); l >= 0; l-- ){
    			int m = upnodes[l];
    			//cout << "D" << m << " = D" << k << " + Rtrans" << rc.getDriverTransistor(m) << " * (Cdown" << m << "+ " << CloadInst << ");" << endl;
				if (l == (upnodes.size()-2)){
					delaycomb = delaycomb + Rtrans[rc.getDriverTransistor(m)] * (Cdown[m]);
				} // end if
				else{
					delaycomb = delaycomb + Rtrans[rc.getDriverTransistor(m)] * (Cdown[m]);
				}
				//cout << "D" << m << " = " << delaycomb << endl;
				//D += ", D" + ToString(cont) + ToString(m) + "_" + instanceName;
    			k = m;
    		}
      }
	  
	  */
	  
	return delaycomb;


}

double elmoredelay::createRC( Circuit *circuit, CellNetlst *netlist, string instanceName, double m, double Xn, double Xp, double CloadInst, Inst instan) {
	// Discover cell ios.
	netlist->calcIOs( circuit->getGndNet(), circuit->getVddNet() );
	//Inst instan = netlist->getInstance(instanceName);
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
				rc.AddTransistor( ToString(trans_it->name), RCTranslator::PMOS, ToString(netlist->getNetName(trans_it->source)), ToString(netlist->getNetName(trans_it->gate)) , ToString(netlist->getNetName(trans_it->drain)), (trans_it->width)  );
			} else if (trans_it->type == 1){
				rc.AddTransistor( ToString(trans_it->name), RCTranslator::NMOS, ToString(netlist->getNetName(trans_it->source)), ToString(netlist->getNetName(trans_it->gate)) , ToString(netlist->getNetName(trans_it->drain)), (trans_it->width)  );
			} else{
				cout << "Transistor type is not defined" << endl;
			} // end else
		} // end for

		// Run

		rc.initialize();
		double delay =0;
		double delayOld =0;
		double delayFinal = 0;
		int cont = 0;
		vector<double> Rtrans;

		while ( rc.next() ) {
			//printRC( rc, D, cont, file, constrArea, instanceName, contInstance, sizingType, Cload, technology);

			delay = calculateDelay (rc, Xn, Xp, m, CloadInst);
			if (delayOld > delay){
				//cout << "Delay Old = " << delayOld << endl;
				//cout << "Delay agora = " << delay << endl;
				delayFinal = delayOld;
				//cout << "Delay Final = " << delayFinal << endl;
			}
			else{
				delayFinal = delay;
				//cout << "Delay Final = " << delayFinal << endl;
			}
			delayOld = delayFinal;

			cont++;
			
		} // end while
		//printFinalGP(D, file, rc, constrArea, instanceName);
		//cout << "Delay Final FInal: " << delayFinal << endl;
		//instan.cellArea = calculateArea(rc, m);
		return delayFinal;
	} catch ( RCTranslatorException &e ) {
		cerr << "[EXCEPTION] " << e.what() << "\n";
		return 0;
	} // end catch

} // end method

//------------------------------------------------------------------------------

Inst *elmoredelay::findDriver (Circuit *circuit, CellNetlst *topNetlist, t_net net){

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

list<Inst*> elmoredelay::findSinks(Circuit *circuit, CellNetlst *topNetlist, t_net net) {
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

double elmoredelay::calculateCload( Circuit *c, CellNetlst *topNetlist, CellNetlst *netlist, Inst instance, double Cload, map<string,bool> &outputs, double m, string instanceName) {
	string netOut;
	vector< pair<int,double> > &cins = netlist->getCins();
	vector<int> &ports = instance.ports;
	Inst targetInstan;
	
	double cloadTotal = 0;
	
	for (int l=0; l<ports.size(); l++)
	{
		t_net &net = topNetlist->getNet(ports[l]);
		if ( net.name != c->getVddNet() && net.name != c->getGndNet() ) {
			//cout << "\t\t" << net.name << endl;
			//cout << "tipo netlist:" << netlist->getIOType(l) << endl;
			if (netlist->getIOType(l) == IOTYPE_OUTPUT){
					//cout << "Outputs net: " << outputs[net.name] << endl;
					//cout << "Netname: " << net.name << endl;
					if (outputs[net.name] == true){
						//cout << "Cout load : " << Cload << endl;
						cloadTotal += Cload;
					}//end if
					else{
						//cout << "Cload Funcao = " << Cload << endl;
						list<t_inst> insts = net.insts;
						//cout << "INSTANCE NAME: " << instanceName << endl;
						//file << "Cload_" << instanceName << " = ";
						
						list<Inst*> sinks = findSinks(c, topNetlist, net );
						for (list<Inst*>::iterator it = sinks.begin(); it != sinks.end(); it++){ 
								Inst *inst = *it;
								cloadTotal += inst->cinPort * inst->m;
						}// end for
						
						/*
						for (list<t_inst>::const_iterator it = insts.begin(); it != insts.end(); it++){
							if (it->targetCellInst != instanceName){
								//cout << "Cout .... : " << (cins[it->targetPin].second * m) << " \t" << m << endl;
								targetInstan = topNetlist->getInstance(it->targetCellInst);
								
								//file << cins[it->targetPin].second << " * " << it->targetCellInst;
								cloadTotal += (cins[it->targetPin].second * targetInstan.m);
								cout << "Cin_" << it->targetCellInst << " = " << cins[it->targetPin].second << endl;
							}//end if
						}//end for
						*/
					}//end else
			}//end if
		}// end if
	}//end for
	instance.Cload = cloadTotal;
	return cloadTotal;
	
}//end function


//------------------------------------------------------------------------------

double elmoredelay::circuitDelay( Circuit *c, CellNetlst *topNetlist, const t_net &net) {
	Inst * inst = findDriver(c, topNetlist, net);
	if ( inst == NULL )
		return 0;

	double delayGate = inst->elmoreDelay;
    double delayFinal = 0;

	//cout << "Delay Inst " << inst->subCircuit << " = " << delayGate << endl ;

	CellNetlst *netlist = c->getCellNetlst(inst->subCircuit);

	vector<int> &ports = inst->ports;
	for (int i=0; i<ports.size(); i++) {
		//cout << " \t" << netlist->getIOType(i) << "\n";

		if ( netlist->getIOType(i) != IOTYPE_INPUT )
			continue;

		t_net &netInt = topNetlist->getNet(ports[i]);
		if ( netInt.name == c->getVddNet() || netInt.name == c->getGndNet() )
			continue;

		delayFinal = max(delayFinal, circuitDelay(c, topNetlist, netInt ) );

		//cout << "\tNet " << topNetlist->getNet(ports[i]).name << endl;
		//cout << "\tDELAY FINAL= " << delayFinal << endl;
	} // end for

	//cout << "Delay Gate " << inst->name << delayGate << " + Delay Final " << delayFinal << endl;
	//cout << "### Inst delay: " << (delayGate + delayFinal) << "\n";
	if ((delayGate+delayFinal) == 4.21893e-10) {
		exit(0);
	}
	return delayGate + delayFinal;

}//end function

// -----------------------------------------------------------------------------

void elmoredelay::addInstances(queue<string> &instances, const t_net &net){
    const list<t_inst> &insts = net.insts;
    for (list<t_inst>::const_iterator it = insts.begin(); it != insts.end(); it++){
		instances.push(it->targetCellInst);
		//cerr << "\t\tNeighbour: " << it->targetCellInst << "\t\tTargetPin: "<< it->targetPin << "\n";
	}//end for
}//end function


bool elmoredelay::elmoreSame(Circuit* c){
	cout << "\n\nINICIA CÁLCULO DO ELMORE" << endl;
	double Xn_350 = 1;
	double Xp_350 = 1.6;
	//values for 45nm
	double Xn = 1;
	double Xp = 1;
	
	double CloadInst;
	double Cload = 1.0792e-15;
	double delayCircuit;
	double areaCircuit = 0;
	double delayOld = 0;
	map<string,bool> outputs;
	CellNetlst *netlist = c->getCellNetlst( c->getTopCell() );
	queue<string> instancesFIFO;
	map<string,Inst> &instances = netlist->getInstances();

	vector<t_net>& nets=c->getCellNetlst(c->getTopCell())->getNets();
    for(vector<t_net>::iterator nets_it=nets.begin(); nets_it!=nets.end(); nets_it++){
		outputs.insert(pair<string,bool> (nets_it->name, false));
    }//end for

	//Passe all cells of the netlist
    map<string, CellNetlst>::iterator cells_it;
	for ( cells_it = c->getCellNetlsts()->begin(); cells_it != c->getCellNetlsts()->end(); cells_it++ ){ //for all netlist cells
		//To set the same transistor size for all cells - it based in commercial library size 1 for 350nm
		for(int x=0; x<cells_it->second.size(); x++){       // for all cell transistors
			if(cells_it->second.getTrans(x).type==PMOS)     //Sizing different for P and N transistors
				cells_it->second.getTrans(x).width = 1;
			else
				cells_it->second.getTrans(x).width = 1;
		}

		//Calculate IO type for each pin
		cells_it->second.calcIOs(c->getGndNet(), c->getVddNet());

		//Calculate input capacitance of each cell
		cells_it->second.calculateInputCapacitances();
	} // end for

	for (map<string,Inst>::iterator instances_it = instances.begin(); instances_it != instances.end(); instances_it++){
		instances_it->second.m = 7.9;
		//cout << "M " << instances_it->first << " " << instances_it->second.m << endl;
	}//end for

	map<string, Interface> *interfaces = c->getInterfaces();
    for ( map<string, Interface>::iterator interfaces_it = interfaces->begin(); interfaces_it != interfaces->end(); interfaces_it++ ) {
		if ( interfaces_it->second.ioType != IOTYPE_OUTPUT ) continue;
		t_net &net = netlist->getNet(interfaces_it->first);
		outputs[net.name] = true;
    } // end for

	for (map<string,Inst>::iterator instances_it = instances.begin(); instances_it != instances.end(); instances_it++){
		CloadInst = calculateCload(c, netlist, c->getCellNetlst( instances_it->second.subCircuit ), instances_it->second, Cload, outputs, instances_it->second.m, instances_it->first);
		instances_it->second.Cload = CloadInst;
		cout << "Cload " << instances_it->first << " = " << CloadInst << endl;
		//cout << "Delay ANTES: " << instances_it->first << " = " << instances_it->second.elmoreDelay << endl;
		instances_it->second.elmoreDelay = createRC(c,  c->getCellNetlst( instances_it->second.subCircuit ), instances_it->first, instances_it->second.m, Xn, Xp, CloadInst, instances_it->second);
		cout << "DELAY "<< instances_it->first << " = " << instances_it->second.elmoreDelay << endl;
		c->getCellNetlst( instances_it->second.subCircuit )->calcIOs(c->getGndNet(), c->getVddNet());
		
    }//end for

    for ( map<string, Interface>::iterator interfaces_it = interfaces->begin(); interfaces_it != interfaces->end(); interfaces_it++ ) {
		if ( interfaces_it->second.ioType != IOTYPE_OUTPUT ) continue;
		t_net &net = netlist->getNet(interfaces_it->first);
		//cout << "Net passada: " << net.name << endl;
		delayCircuit = max(delayOld, circuitDelay(c, netlist, net));
		delayOld = delayCircuit;
    } // end for
	
	for ( cells_it = c->getCellNetlsts()->begin(); cells_it != c->getCellNetlsts()->end(); cells_it++ ){ //for all netlist cells
		//To set the same transistor size for all cells - it based in commercial library size 1 for 350nm
		for(int x=0; x<cells_it->second.size(); x++){       // for all cell transistors
			if(cells_it->second.getTrans(x).type==PMOS)     //Sizing different for P and N transistors
				cells_it->second.getTrans(x).width = 0.135;
			else
				cells_it->second.getTrans(x).width = 0.09;
		}
	}

	for (map<string,Inst>::iterator instances_it = instances.begin(); instances_it != instances.end(); instances_it++){
		double cellArea = 0;
		//calculate AREA for each cell
		vector<Trans> &trans = c->getCellNetlst( instances_it->second.subCircuit )->getTransistors();
		for (vector<Trans>::iterator trans_it = trans.begin(); trans_it != trans.end(); trans_it++){
			cellArea = cellArea + (trans_it->width * instances_it->second.m);
		} // end for
		instances_it->second.cellArea = cellArea;
		areaCircuit = areaCircuit + instances_it->second.cellArea;
    }//end for

	cout << "Delay Circuit: " << delayCircuit << endl;
	cout << "Area do Circuito: " << areaCircuit << endl;
	while( !instancesFIFO.empty() ) {
		string e = instancesFIFO.front();
		//cout << "Instances: " << e << endl;
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


	return true;
}


bool elmoredelay::elmoreFO4(Circuit* c){
	cout << "\n\nINICIA CÁLCULO DO ELMORE" << endl;
	double Xn_350 = 1;
	double Xp_350 = 1.6;
	//values for 45nm
	double Xn = 1;
	double Xp = 1;
	double areaCircuit = 0;
	double areaBase = 0;
	double CloadInst;
	double Cload = 4*1.87367e-16;
	double delayCircuit;
	double delayOld = 0;
	map<string,bool> outputs;
	CellNetlst *netlist = c->getCellNetlst( c->getTopCell() );
	queue<string> instancesFIFO;
	map<string,Inst> &instances = netlist->getInstances();

	vector<t_net>& nets=c->getCellNetlst(c->getTopCell())->getNets();
    for(vector<t_net>::iterator nets_it=nets.begin(); nets_it!=nets.end(); nets_it++){
		outputs.insert(pair<string,bool> (nets_it->name, false));
    }//end for

	//Passe all cells of the netlist
    map<string, CellNetlst>::iterator cells_it;
	for ( cells_it = c->getCellNetlsts()->begin(); cells_it != c->getCellNetlsts()->end(); cells_it++ ){ //for all netlist cells
		//To set the same transistor size for all cells - it based in commercial library size 1 for 350nm
		/*
		for(int x=0; x<cells_it->second.size(); x++){       // for all cell transistors
			if(cells_it->second.getTrans(x).type==PMOS)     //Sizing different for P and N transistors
				cells_it->second.getTrans(x).width = 1;
			else
				cells_it->second.getTrans(x).width = 1;
		}*/
		
		//Calculate IO type for each pin
		cells_it->second.calcIOs(c->getGndNet(), c->getVddNet());

		//Calculate input capacitance of each cell
		cells_it->second.calculateInputCapacitances();
	} // end for

	map<string, Interface> *interfaces = c->getInterfaces();
    for ( map<string, Interface>::iterator interfaces_it = interfaces->begin(); interfaces_it != interfaces->end(); interfaces_it++ ) {
		if ( interfaces_it->second.ioType != IOTYPE_OUTPUT ) continue;
		t_net &net = netlist->getNet(interfaces_it->first);
		outputs[net.name] = true;
    } // end for

	for (map<string,Inst>::iterator instances_it = instances.begin(); instances_it != instances.end(); instances_it++){
		CloadInst = calculateCload(c, netlist, c->getCellNetlst( instances_it->second.subCircuit ), instances_it->second, Cload, outputs, instances_it->second.m, instances_it->first);
		instances_it->second.Cload = CloadInst;
		cout << "Cload " << instances_it->first << " = " << CloadInst << endl;
		//cout << "Delay ANTES: " << instances_it->first << " = " << instances_it->second.elmoreDelay << endl;
		//cout << instances_it->first << " = " << instances_it->second.m << endl;
		instances_it->second.elmoreDelay = createRC(c,  c->getCellNetlst( instances_it->second.subCircuit ), instances_it->first, instances_it->second.m, Xn, Xp, CloadInst, instances_it->second);
		//cout << "DELAY "<< instances_it->first << " = " << instances_it->second.elmoreDelay << endl;
		c->getCellNetlst( instances_it->second.subCircuit )->calcIOs(c->getGndNet(), c->getVddNet());
    }//end for

    for ( map<string, Interface>::iterator interfaces_it = interfaces->begin(); interfaces_it != interfaces->end(); interfaces_it++ ) {
		if ( interfaces_it->second.ioType != IOTYPE_OUTPUT ) continue;
		t_net &net = netlist->getNet(interfaces_it->first);
		//cout << "Net passada: " << net.name << endl;
		delayCircuit = max(delayOld, circuitDelay(c, netlist, net));
		delayOld = delayCircuit;
		//cout << "Delay Circuit 2: " << delayCircuit << endl;
    } // end for
	
	for ( map<string, Interface>::iterator interfaces_it = interfaces->begin(); interfaces_it != interfaces->end(); interfaces_it++ ) {
		if ( interfaces_it->second.ioType != IOTYPE_OUTPUT ) continue;
		t_net &net = netlist->getNet(interfaces_it->first);
		//cout << "Net passada: " << net.name << endl;
		delayCircuit = max(delayOld, circuitDelay(c, netlist, net));
		delayOld = delayCircuit;
    } // end for
	
	/*
	for ( cells_it = c->getCellNetlsts()->begin(); cells_it != c->getCellNetlsts()->end(); cells_it++ ){ //for all netlist cells
		//To set the same transistor size for all cells - it based in commercial library size 1 for 350nm
		
		for(int x=0; x<cells_it->second.size(); x++){       // for all cell transistors
			if(cells_it->second.getTrans(x).type==PMOS)     //Sizing different for P and N transistors
				cells_it->second.getTrans(x).width = 0.135;
			else
				cells_it->second.getTrans(x).width = 0.09;
		}
	}
	*/
	for (map<string,Inst>::iterator instances_it = instances.begin(); instances_it != instances.end(); instances_it++){
		
				
		double cellArea = 0;
		double cellBase = 0;
		//calculate AREA for each cell
		vector<Trans> &trans = c->getCellNetlst( instances_it->second.subCircuit )->getTransistors();
		for (vector<Trans>::iterator trans_it = trans.begin(); trans_it != trans.end(); trans_it++){
			cellArea = cellArea + (trans_it->width * instances_it->second.m);
			cellBase = cellBase + trans_it->width;
		} // end for
		instances_it->second.cellArea = cellArea;
		areaCircuit = areaCircuit + instances_it->second.cellArea;
		areaBase = areaBase + cellBase;
    }//end for

	cout << "Delay Circuit: " << delayCircuit << endl;
	cout << "Area Base do Circuito: " << areaBase << endl;
	cout << "Area do Circuito: " << areaCircuit << endl;
	while( !instancesFIFO.empty() ) {
		string e = instancesFIFO.front();
		//cout << "Instances: " << e << endl;
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

	return true;
}


double elmoredelay::computeElmoreDelay( Circuit *circuit ) {
	CellNetlst *topNetlist = circuit->getCellNetlst( circuit->getTopCell() );

	// Setup the iotypes.
	map<string, CellNetlst> &netlists = *circuit->getCellNetlsts();
	for ( map<string, CellNetlst>::iterator it = netlists.begin(); it != netlists.end(); it++ )
		it->second.calcIOs(circuit->getGndNet(), circuit->getVddNet());

	// Compute Elmore Delay :)
	double delay = 0;

	map<string, Interface> *interfaces = circuit->getInterfaces();
    for ( map<string, Interface>::iterator interfaces_it = interfaces->begin(); interfaces_it != interfaces->end(); interfaces_it++ ) {
		if ( interfaces_it->second.ioType != IOTYPE_OUTPUT )
			continue;
		t_net &net = topNetlist->getNet(interfaces_it->first);
		delay = max( delay, circuitDelay( circuit, topNetlist, net ) );
    } // end for

	return delay;
} // end method

