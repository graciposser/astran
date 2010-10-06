/**************************************************************************
*   Copyright (C) 2005 by Adriel Mota Ziesemer Jr.                        *
*   [amziesemerj[at]@inf.ufrgs.br                                      *
**************************************************************************/
#ifndef CELLNETLST_H
#define CELLNETLST_H

/**
Data Structure of the cell netlist

@author Adriel Mota Ziesemer Jr.
*/

#include <vector>
#include <list>
#include <map>
#include <queue>
#include <string>
#include <iostream>
#include <strstream>
#include <sstream>
#include "math.h"
#include "thresholdaccept.h"
#include "util.h"

using namespace std;

enum transType {PMOS, NMOS, NONE};
const char * const TransistorTypeName[] = { "PMOS", "NMOS", "NONE"}; 

enum t_DGorS {DRAIN, GATE, SOURCE, GAP};

// Auxiliar struct that stores the transistor port where a net is connected.
struct t_net2{
	int link;
	t_DGorS type;
};

// Auxiliar struct that stores the cell pin of a instance where a net is connected.
struct t_inst{
	string targetCellInst;
	int targetPin;
};

// Auxiliar struct that stores the places that each net is connected to
struct t_net{
	list<t_net2> trans;
	list<t_inst> insts;
	string name;
};

// Struct that stores data about a Transistor
struct Trans{
	string name;
	bool visited;
	unsigned int drain, gate, source;
	transType type;
	float length;
	float width;
};

// Struct that stores data about a Cell Instance
struct Inst{
	string subCircuit;      // Name of the cell to be instantiated
	string name;
	vector<int> ports;      // Ordered list of nets that connect to the I/Os of the cell
	double m;                  // Multiplier (M=)
	bool instanceVisited;
	float cinPort;		//Input capacitance of port 
	bool instanceSized;
	double elmoreDelay;
	double cellArea;
	double Cload;
};

// Class that stores the data structure of a .subckt
class CellNetlst{
	protected:
	string name;                      // Cell name
	vector<Trans> trans;              // List of transistors (M)
	map<string,Inst> instances;       // List of instances (X)
	queue<string> instancesFIFO; // Queue of netlist instances
	vector<int> inouts;               // Ordered List of cell I/Os
	vector<IOType> inouts_type;       // Ordered List of cell I/Os types (input, output,...)
	vector<t_net> nets;		          // List of transistors and instances that are connected to each net
	vector<t_net2> orderingP, orderingN, cpP, cpN, tmpP, tmpN; // Transistor placement
	vector< pair<int,double> > cins; // Stores the input capacitance of each input pin. <pin index, pin capacitance>
	int PorN, custo_atual, custo_anterior;
	int wGaps, missMatchGate, wRouting, maxCong, posPN, wCost, gmCost, rCost, congCost, ngCost;

	// Auxiliar Methods
	bool eulerpath();
	bool visit(int nDifs, t_net2& transP, t_net2& transN);
	bool visit(int nDifs);
	bool verifyCnt(int pos);
	t_net2 point(int link, t_DGorS type);
	void move(vector<t_net2>& ref);
	int insertNet(string& name, t_DGorS type, int t); // Insert a new
	void move(const vector<t_net2>& ref, vector<t_net2>& cp);

	public:
	void setName(string n){name=n;};
	string getName(){return name;};
	bool setIOType(string name, IOType t);
	IOType getIOType(int net){return inouts_type[net];};
	void appendNetNameSuffix(string n);               // Append a string in the name of the nets
	string& getNetName(int n){return nets[n].name;};  // Return the name of a net
	void insertInstance(string instName, string subCircuit, vector<string> &ports, int multiplyer); // Insert a new cell instance
	map<string,Inst>& getInstances(){return instances;}; // Return all the instances
	Inst &getInstance( string name );
	void insertTrans(string name, string d, string g, string s, transType t, float l, float w); // Insert a new transistor
	bool insertInOut(string name);                    // Insert a new I/O pin
	string getInout(int n);                           // Return the name of a I/O port
	vector<int>& getInouts(){return inouts;};         // Return the list of I/O ports
	bool calcIOs(  string gndName, string vddName );  // Define the type pin: if it is IN or OUT
	void multiply(int m);                             // Scales the cell by a given factor
	t_net& getNet(int n){return nets[n];};            // Return the data structure of a net (list of places where it connects)
	t_net& getNet( string name );
	vector<t_net>& getNets(){return nets;};           // Return the list of nets of the cell
	bool check();                                     // Verify if the cell structure is OK
	bool folding(float pSize, float nSize);           // Fold the transistors
	int size(){return trans.size();};                 // Return the number of transistors of the cell
	int pSize();                                      // Return the number of P transistors
	int nSize();                                      // Return the number of N transistors
	Trans& getTrans(int ID){ return trans[ID];};      // Return the data structure of a transistor
	vector<Trans>& getTransistors(){return trans;};	  // Return all the transistors
	vector< pair<int,double> > &getCins() {return cins; }
	void print();                                     // Print the current subcircuit to the standard output
	void clear();                                     // Clear the current subcircuit
	bool transPlacement(bool ep, int saquality, int nrattempts, int wC, int gmC, int rC, int congC, int ngC); // Executes the placement algorithm
	vector<t_net2>& getOrderingP(){return orderingP;}; // Return the placement of the P transistors
	vector<t_net2>& getOrderingN(){return orderingN;}; // Return the placement of the N transistors
	bool isIO(string n);                               // Return true if the given net is an I/O net

	//Auxiliar functions to the placement algorithm
	int Perturbation();                                // Retorna o custo da perturbacao
	void UndoPerturbation();                           // Desfaz a ultima perturbacao
	int GetCost();                                     // Retorna o custo do posicionamento atual
	int GetProblemSize();                              // Retorna um numero qualquer que indica o tamanho do problem (ex. numero de transistores)

	void calculateInputCapacitances();                 // Computes the input capacitance of each pin and store them in cins vector.
};

#endif
