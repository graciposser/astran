/*
 *  size.h
 *  ICPD
 *
 *  Created by Adriel Mota Ziesemer Jr. on 5/18/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef SIZE_H
#define SIZE_H

/**
Calculate the size of the cells
 
 @author Graci Posser, Luiza Silva Porto Ramos
 */

#include "circuit.h"
#include "cellnetlst.h"
#include "RCTranslator.h"
#include "elmoredelay.h"

using namespace std;
    
class Size{
private:
	void addInstances(queue<string> &instances, const t_net &net);
	void printOutputGP();
	void printRC( const RCTranslator &rc, string &D, const int cont, ofstream &file, double constrArea, string instanceName, int contInstance, string sizingType, double Cload, string technology);
	void printFinalGP( string &D, ofstream &file, RCTranslator &rc, double constrArea, string instanceName);
	void circuitSize( Circuit *circuit, CellNetlst *topNetlist, CellNetlst *netlist,  string instanceName, Inst instance, ofstream &file);
	void transistorSizing( Circuit *circuit, CellNetlst *netlist, ofstream &file, string instanceName, int contInstance, string sizingType, double constrArea, double Cload, string technology);
	void printFinalCircuit(ofstream &file, RCTranslator &rc, double constrArea, string instanceName, string sizingType, string optimize);
	void printConstraints( Circuit *circuit, CellNetlst *netlist, ofstream &file, string instanceName, int contInstance, string sizingType, string optimize);
	void calculateCload( Circuit *c, CellNetlst *topNetlist, CellNetlst *netlist, string instanceName, Inst instance, ofstream &file, string netOutput, double Cload, map<string,bool> outputs);
	void calculateElmoreDelay(Circuit *c, RCTranslator rc);
	void findCircuitInputs( Circuit *c, CellNetlst *topNetlist, CellNetlst *netlist, string instanceName, Inst instance, ofstream &file, map<string,bool> inputs, double restrCin);
	void writeElmoreDelay( Circuit *c, CellNetlst *topNetlist, ofstream &file, list<Inst*> lst);

	bool fanout4(Circuit *c);

	Inst *findDriver (Circuit *circuit, CellNetlst *topNetlist, t_net net);
	list<Inst*> findSinks(Circuit *circuit, CellNetlst *topNetlist, t_net net);

	template<class Functor>
	void backwardWalking( Circuit *circuit, Functor &func );

	double FO4Sizing( Circuit * circuit, Inst * inst, double Cload, map<string,bool> &outputs );
	void walkElmoreDelay( Circuit * circuit, Inst * inst, ofstream &file );

	bool printSpiceSimulation(Circuit& circuit, Inst &instance, ofstream &simulate, ofstream &copyarq, string &top);
	bool printSpiceCarac(Circuit& circuit, Inst &instance, ofstream &simulate, ofstream &copyarq, string &top, ofstream &subckt);
	void printAlter(Inst &instance, ofstream &file, Circuit &circuit);
	bool printSetupCarac(Circuit& circuit, ofstream &simulate, ofstream &copyarq, string &top);
	bool printScriptCarac(Circuit& circuit, string &top);
public:
	bool fo4(Circuit* c);
	bool gp(Circuit* c);
};


//------------------------------------------------------------------------------

template<class Functor>
void Size::backwardWalking( Circuit *circuit, Functor &func ) {
	queue<Inst*> opened;
	CellNetlst *netlist = circuit->getCellNetlst(circuit->getTopCell());
	
	//
	// Clean-up;
	//
	map<string,Inst> &instances = netlist->getInstances();
	for (map<string,Inst>::iterator instances_it = instances.begin(); instances_it != instances.end(); instances_it++)
		instances_it->second.instanceVisited = false;	

	//
	// Add last level instances to the queue (the ones which driver output
	// nets).
	//
    map<string, Interface> *interfaces = circuit->getInterfaces();
    for ( map<string, Interface>::iterator interfaces_it = interfaces->begin(); interfaces_it != interfaces->end(); interfaces_it++ ) {

		if ( interfaces_it->second.ioType != IOTYPE_OUTPUT )
			continue;

		t_net &net = netlist->getNet(interfaces_it->first);
		opened.push( findDriver(circuit, netlist, net) );
    } // end for	
	
	//
	// Transverse the netlist backward and call the functor for each instance.
	//
	while ( !opened.empty() ) {
		Inst *inst = opened.front();
		opened.pop();
		
		// Check if this instance was already processed.
		if ( !inst || inst->instanceVisited )
			continue;	
		inst->instanceVisited = true;
	
		// Call the functor for the instance.
		func(inst);

		// Put the instance neighbours into the queue.
		vector<int> &ports = inst->ports;
		for ( int i=0; i < ports.size(); i++ ) {
			t_net &net = netlist->getNet(ports[i]);
			if ( net.name != circuit->getVddNet() && net.name != circuit->getGndNet() )
				opened.push( findDriver(circuit, netlist, net) );			
		} // end for
	} // end while
} // end method

#endif