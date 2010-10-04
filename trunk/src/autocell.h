/***************************************************************************
*   Copyright (C) 2005 by Adriel Mota Ziesemer Jr., Cristiano Lazzari     *
*   (amziesemerj,clazz)@inf.ufrgs.br                                      *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
***************************************************************************/
#ifndef AUTOCELL_H
#define AUTOCELL_H

/**
Draw a Cell automatically

@author Adriel Mota Ziesemer Jr.
*/

#include <vector>
#include <sstream>
#include "clayout.h"
#include "draw.h"
#include "rules.h"
#include "circuit.h"
#include "pathfinder2.h"
#include "compaction.h"
#include "util.h"

using namespace std;

class Element{
	public:
	unsigned int transN,transP;
	unsigned int posCntsN, posCntsP;
	unsigned int outPolN, outPolP;
	t_net2 linkN, linkP;
	vector<unsigned int> diffCntsN,diffCntsP;
	vector<unsigned int> interMet, interPol;
	unsigned int ativeCntN,ativeCntP;
	unsigned int inoutCnt;
	bool gapP, gapN;
	
	void print(){
		if(gapP) cout << "G";
		else cout << " ";
		cout << outPolP <<"(<"  << ativeCntP << ">";
		for(int x=0; x<diffCntsP.size(); x++)
			cout << "," << diffCntsP[x];
		cout << ") <"  << inoutCnt << ">,";
		for(unsigned int x=0;x<interMet.size();x++)
			cout << "[" << interMet[x] << "," << interPol[x] << "] ";
		if(gapN) cout << "G";
		else cout << " ";
		cout << outPolN <<" (<"  << ativeCntN << ">";
		for(int x=0; x<diffCntsN.size(); x++)
			cout << "," << diffCntsN[x];
		cout << ") -> " << inoutCnt << endl;
	}
};

class AutoCell{
	protected:

	unsigned int hGrid, vGrid, supplySize, height;
	int vdd,gnd,poly_priority,metal_priority,width_priority,gatespacing_priority, nrTracks, posIOs; 

	int supWidth,btDifWidth,pSize,nSize;
	unsigned int nrCntsP,nrCntsN;
	unsigned int pDif_iniY, pDif_endY, nDif_iniY, nDif_endY;
	vector<unsigned int> trackPos, posCntsDiffP, posCntsDiffN;
	int state;
	
	list<Element> elements;
	map<string,unsigned int> inoutPins;
//	void eletricalOtimization(Cell& currentCell, Pathfinder2 &rt);
 
	Element* createElement(int vcost);

	CellNetlst* currentCell;
	Rules* currentRules;
	Circuit* currentCircuit;

	Pathfinder2* rt;
	CellNetlst currentNetList;
	CLayout currentLayout;
	public:
	AutoCell();
	~AutoCell();
	void clear();
	bool calcArea(Circuit* c);
	bool foldTrans();
	bool placeTrans(bool ep, int saquality, int nrAttempts, int wC, int gmC, int rC, int congC, int ngC);

	bool route(int mCost, int pCost, int cCost, int ioCost);
	bool compact(int mPriority, int pPriority, int gsPriority, int wPriority, string lpSolverFile);

	bool selectCell(string c);
	void setNrTracks(int n) {nrTracks=n;};
	int getNrTracks() {return nrTracks;};
	void setPosTracks(int n) {posIOs=n;};
	int getPosTracks() {return posIOs;};
	void setMetPriority(int x) {metal_priority=x;};
	int getMetPriority() {return metal_priority;};
	void setPolPriority(int x) {poly_priority=x;};
	int setPolPriority() {return poly_priority;};
	void setWidthPriority(int x) {width_priority=x;};
	int getWidthPriority() {return width_priority;};
	void setGateSpacingPriority(int x) {gatespacing_priority=x;};
	int getGateSpacingPriority() {return gatespacing_priority;};
	int getState(){return state;};
	float getPSize(){return float(pSize/currentCircuit->getRules()->getScale());};
	float getNSize(){return float(nSize/currentCircuit->getRules()->getScale());};
	CellNetlst* getTmpNetlist(){return &currentNetList;};
	CLayout getLayout(){return currentLayout;};
	Pathfinder2* getRouting(){return rt;};
	void showIOCost();

};

#endif
