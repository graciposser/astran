/*
 *  elmoredelay.h
 *  ICPD
 *
 *  Created by Graci Posser on 9/15/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef ELMOREDELAY_H
#define ELMOREDELAY_H

/**
Calculate the Elmore Delay

 */

#include "circuit.h"
#include "cellnetlst.h"
#include "RCTranslator.h"

using namespace std;

class elmoredelay{
private:
	void addInstances(queue<string> &instances, const t_net &net);
	void printOutputGP();
	void printRC( const RCTranslator &rc, string &D, const int cont, ofstream &file, double constrArea, string instanceName, int contInstance, string sizingType, double Cload, string technology);
	void printFinalGP( string &D, ofstream &file, RCTranslator &rc, double constrArea, string instanceName);
	double circuitDelay( Circuit *circuit, CellNetlst *topNetlist, const t_net &net);
	double circuitDelayFernanda( Circuit *circuit, CellNetlst *topNetlist, const t_net &net);
	double createRC( Circuit *circuit, CellNetlst *netlist, string instanceName, double m, double Xn, double Xp, double CloadInst, Inst instan);
	void printFinalCircuit(ofstream &file, RCTranslator &rc, double constrArea, string instanceName, string sizingType);
	void printConstraints( Circuit *circuit, CellNetlst *netlist, ofstream &file, string instanceName, int contInstance, string sizingType);
	double calculateCload( Circuit *c, CellNetlst *topNetlist, CellNetlst *netlist, Inst instance, double Cload, map<string,bool> &outputs, double m, string instanceName);
	double calculateDelay(const RCTranslator &rc, double Xn, double Xp, double m, double CloadInst);
	Inst *findDriver (Circuit *circuit, CellNetlst *topNetlist, t_net net);
	list<Inst*> findSinks(Circuit *circuit, CellNetlst *topNetlist, t_net net);
	
public:
	bool elmoreSame(Circuit* c);
	bool elmoreFO4(Circuit* c);
	bool calculateElmoreDelay(Circuit *c);
	double computeElmoreDelay( Circuit *circuit );
	
};

#endif