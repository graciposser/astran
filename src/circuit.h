/***************************************************************************
*   Copyright (C) 2005 by Adriel Mota Ziesemer Jr.                         *
*   amziesemerj[at]inf.ufrgs.br                                            *
****************************************************************************/

#ifndef CIRCUIT_H
#define CIRCUIT_H

/**
Data Structs for the circuit

@author Adriel Mota Ziesemer Jr.
*/

#include <list>
#include <map>
#include <string>
#include <strstream>

#include "cellnetlst.h"
#include "clayout.h"

using namespace std;

struct Interface{
	direction orient;
	IOType ioType;
	Point pos;
};

class Circuit {
	protected:
	map<string, Interface> interfaces;
	map<string, CLayout> layouts;
	map<string, CellNetlst> cellNetlsts;
	map<string, string> equations;

	Rules* currentRules;
	int cellsHeight, c2lMargin, c2rMargin, c2tMargin, c2bMargin;
	float hGrid, vGrid, supplyHSize, supplyVSize;
	string topCell,vddNet, gndNet;
	CellNetlst *topNetlist;
	
	//parameters to PrintGP
	string  technology, optimize, sizingtype ; 
	double Cload ,constrArea, constrDelay,constrCin;
    
    public:
	Circuit();
	~Circuit(){};
	
	//parameters to PrintGP
	int getoptimize(){if(optimize=="delay") return 0;  else return 1;};
	int getsizingtype(){if(sizingtype=="gate") return 0;  else return 1;};
	int gettechnology(){if(technology=="45nm") return 0;  else return 1;};
	string getoptimizename(){return optimize;};
	string getsizingtypename(){return sizingtype;};
	string gettechnologyname(){return technology;};
	double getCload(){return Cload;};
	double getconstrArea(){return constrArea;};
	double getconstrDelay(){return constrDelay;};
	double getconstrCin(){return constrCin;};
	
	void setoptimize(string s){optimize=s;};
	void setsizingtype(string s){sizingtype=s;};
	void settechnology(string s){technology=s;};
	void setCload(double n){ Cload=n;};
	void setconstrArea(double n){ constrArea=n;};
	void setconstrDelay(double n){constrDelay=n;};
	void setconstrCin(double n){ constrCin=n;};

	void clear();
	bool setTopCell(string cellName);
	string getTopCell(){return topCell;}
	CellNetlst *getTopNetlist() { return topNetlist; }
	void setVddNet(string s){vddNet=s;};
	string getVddNet(){return vddNet;};
	void setGndNet(string s){gndNet=s;};
	string getGndNet(){return gndNet;};
	void setRules(Rules* r){currentRules=r;};
	Rules* getRules(){return currentRules;};
	void setHPitch(float x){hGrid=x;};
	float getHPitch(){return hGrid;};
	void setVPitch(float x){vGrid=x;};
	float getVPitch(){return vGrid;};
	void setSupplyVSize(float x){supplyVSize=x;};
	float getSupplyVSize(){return supplyVSize;};
	void setRowHeight(int h){cellsHeight=h;};
	int getRowHeight(){return cellsHeight;};
	bool setMargins(float l, float r, float t, float b);
	int getLMargin(){return c2lMargin;};
	int getRMargin(){return c2rMargin;};
	int getTMargin(){return c2tMargin;};
	int getBMargin(){return c2bMargin;};
	CLayout* getLayout(string n);
	map<string, CLayout>* getLayouts(){return &layouts;};
	bool insertCell(CellNetlst& tmp);
	bool insertLayout(CLayout& tmp);
	bool insertEquation(string cellName, string& tmp);
	bool insertInterface(string name, direction orient, IOType type, int posx, int posy);
	unsigned int nrCells();
	CellNetlst* getCellNetlst(string n);
	string* getEquation(string n);
	Interface* getInterface(string n);
	CellNetlst getFlattenCell(string name);

	map<string, Interface>* getInterfaces(){return &interfaces;};
	map<string, CellNetlst>* getCellNetlsts(){return &cellNetlsts;};
	map<string, string>* getEquations(){return &equations;};
	void setCellsHeight(unsigned int h) { cellsHeight=h;};

	bool writeCellSizesFile(string filename);
	bool printInterface(string net);
	bool printNet(string net);
	bool printInstance(CLayout* l, string instance);
	void calculateCellsPins();

	bool isPrimaryInput( const string &netname );
	bool isPrimaryOutput( const string &netname );
	
	bool isPrimaryInput( const Inst &inst );
	bool isPrimaryOutput( const Inst &inst );
	
//	void printCells();
//	void printNetlist();
//	bool checkCells();


};

#endif

