/**************************************************************************
*   Copyright (C) 2005 by Adriel Mota Ziesemer Jr., Renato Hentschke      *
*   (amziesemerj,renato)[at]@inf.ufrgs.br                                 *
***************************************************************************/

#include "spice.h"

#include <iostream>
	using std::cout;
	using std::cerr;

#include "Combination.h"

bool Spice::readFile(string nome, Circuit& netlist, bool reading_cadence)
{
	bool ok=true;
	
	ifstream arq (nome.c_str());
	string linha;
	
	if (!arq.is_open()){
		cout << "\t-> File " << nome << " could not be opened." << endl;
		return false;
	}
	
	vector<string> palavras;
	string palavra;
	
	CellNetlst subcktCell,topCell;
	CellNetlst *currentCell=&topCell;
	topCell.setName(upcase(removeExt(getFileName(nome))));
	string cellName;
	unsigned int lineNr=0;
	while (!arq.eof()){
		lineNr++;
		getline(arq,linha);
		
		palavras.clear();
		
		istrstream clin(linha.c_str());
		
		while (clin>>palavra)
			palavras.push_back(upcase(palavra));
		
		if (palavras.size() == 0) continue;

		if (palavras[0] == "*INTERFACE"){
			if(palavras.size() == 4 || palavras.size() == 6){
				IOType type;
				direction orient;
				switch(palavras[2][palavras[2].size()-1]){
					case 'N': orient=N; break;
					case 'S': orient=S; break;
					case 'E': orient=E; break;
					case 'W': orient=W; break;
					default: 
						cout << "Error on line" << lineNr << " - Interface orientation unknown." << endl;

				}
				switch(palavras[3][0]){
					case 'I': type=IOTYPE_INPUT; break;
					case 'O': type=IOTYPE_OUTPUT; break;
					default: 
						cout << "Error on line" << lineNr << " - Interface type unknown. Use INPUT or OUTPUT." << endl;
				}
				topCell.insertInOut(palavras[1]);
				netlist.insertInterface(palavras[1], orient, type, 0, 0);
			}
			else
				cout << "Error on line" << lineNr << " - Number of parameters for *interface is incorrect." << endl;
				
			continue;
		}

		if (reading_cadence && palavras[0] == "*" && palavras.size() == 5 && palavras[1] == "SUBCIRCUIT"){
			currentCell->clear();
			topCell.setName(palavras[4].substr(0,palavras[4].size()-1));
		}

		if (palavras[0][0] == '*' || palavras[0][0] == 'C' || palavras[0][0] == '+' || palavras[0][0] == 'D' || (palavras[0][0]=='X' && reading_cadence)) // corrigir aqui para ler o '+' e ignorar os parâmetros desnecessários
			continue;

		if (palavras[0] == ".MODEL" || palavras[0] == ".END") break;
		
		if (palavras[0] == ".SUBCKT" && currentCell==&topCell && !reading_cadence){
			// It's a new cell definition. . .
			subcktCell.clear();
			currentCell=&subcktCell;
			cellName=palavras[1];

			// compare if subcircuit name is the same as the top cell name
			if(cellName == topCell.getName()){
				string topname = topCell.getName() + "-TOP";
				topCell.setName(topname);
			}

			for (int p=2; p<palavras.size(); p++)
				currentCell->insertInOut(palavras[p]);

		}
		else if (palavras[0] == string(".INCLUDE")){
			if(!readFile(getPath(nome)+palavras[1],netlist,reading_cadence)) return false;
		}

		// declaring transistor in subcircuit read from Cadence
		else if (palavras[0][0] == 'M' && palavras.size()>=5){

			// insert in and out pins
			if (reading_cadence){
				for (int p=1; p<5; ++p){
					if (!isNumber(palavras[p]) || palavras[p] == "0")
						currentCell->insertInOut(palavras[p]);
				}
			}

			// identify transistor type
			transType type;
			if(palavras[5]=="PMOS" || palavras[5]=="CMOSP" || palavras[5]=="MODP")
				type=PMOS;
			else if(palavras[5]=="NMOS" || palavras[5]=="CMOSN" || palavras[5]=="MODN")
				type=NMOS;
			else {
				cout << "Error on line" << lineNr << " - Parameter " << palavras[5] << " incorrect. Use NMOS or PMOS" << endl;
				ok=false;
			}

			// get parameters' values
			float length=0, width=0;
			for (int p=6; p<palavras.size(); p++){
				int pos=palavras[p].find("=");
				string parm=palavras[p].substr(0,pos++);
				float  tam=atof((palavras[p].substr(pos)).c_str())*getFactor(palavras[p][palavras[p].size()-1]);
				if(parm=="L")
					length=tam;
				else if(parm=="W")
					width=tam;
				else if(parm!="AD" && parm!="PD" && parm!="AS" && parm!="PS" && parm!="NRD" && parm!="NRS" && parm!="M"){
					cout << "Error on line" << lineNr << " - Parameter " << parm << " not supported." << endl;
					ok=false;
				}
			}

			// insert transistor in cell
			if(ok)
				currentCell->insertTrans(palavras[0], palavras[1], palavras[2], palavras[3], type, length, width);
		}

		else if (palavras[0][0] == 'X' && !reading_cadence){
			string instName=palavras[0];
			vector<string> net_ids;
			int p;
			int m=1;
			for (p=1; p<palavras.size(); p++){
				if(palavras[p].substr(0,2)=="M="){
					m=strToInt(palavras[p].substr(2));
					break;
				}
				net_ids.push_back(palavras[p]);
			}
			string subcktName=net_ids.back();
			net_ids.pop_back();
			currentCell->insertInstance(instName, subcktName, net_ids, m);
		}
		else if (currentCell==&subcktCell && palavras[0] == ".ENDS"){
			currentCell->setName(cellName);
			netlist.insertCell(*currentCell);
			currentCell=&topCell;
		}
		else {
			cout << "Parser error on line " << lineNr << endl;
			return false;
		}
	}

	arq.close();

	if(topCell.getNets().size() != 0)
		netlist.insertCell(topCell);

	return ok;
}

bool Spice::saveFile(string filename, Circuit& netList){
	ofstream file;
	file.open(filename.c_str()); // Write
	if (!file)
		return 0;

	printHeader (file, "* ", "");
	
	//file << ".include mos_45.sp\n"<< endl;
	//file << ".param vvdd = 1.1" << endl;
	//file << ".param vgnd = 0.0/n" << endl;
	//file << ".global vdd" << endl;
	//file << ".global gnd/n" << endl;
	//file << "Vsrc vdd 0 1.1/n" << endl;
	map<string, CellNetlst>::iterator cells_it;
	for ( cells_it = netList.getCellNetlsts()->begin(); cells_it != netList.getCellNetlsts()->end(); cells_it++ ){
		file << ".SUBCKT " <<  cells_it->first;
		for ( vector<int>::iterator inouts_it=cells_it->second.getInouts().begin(); inouts_it != cells_it->second.getInouts().end(); inouts_it++ )
			file << " " << cells_it->second.getNetName(*inouts_it);
		file << endl;
		for(map<string,Inst>::iterator tmp=cells_it->second.getInstances().begin(); tmp!=cells_it->second.getInstances().end(); ++tmp){
			file << tmp->first << " ";
			for(vector<int>::iterator tmp2=tmp->second.ports.begin(); tmp2!=tmp->second.ports.end(); ++tmp2)
				file << cells_it->second.getNetName(*tmp2) << " ";
			file << tmp->second.subCircuit;
			if(tmp->second.m!=1) file << " M=" << tmp->second.m;
			file << endl;			
		}
		for(int x=0; x<cells_it->second.size(); x++){
			file << cells_it->second.getTrans(x).name << " " << 
			cells_it->second.getNetName(cells_it->second.getTrans(x).drain) << " " << 
			cells_it->second.getNetName(cells_it->second.getTrans(x).gate) << " " << 
			cells_it->second.getNetName(cells_it->second.getTrans(x).source) << " ";
			if(cells_it->second.getTrans(x).type==PMOS) 
				file << netList.getVddNet() << " PMOS";
			else
				file << netList.getGndNet() << " NMOS";
			file << " L=" << cells_it->second.getTrans(x).length << "U W=" << cells_it->second.getTrans(x).width << "U"<< endl;			
		}
		file << ".ENDS " << cells_it->first << endl << endl;
	}
	//file << "cLoad n1 0 4*1.87367e-16\n" << endl;
	//file << ".measure tran tpdf trig v(in) val=`vvdd/2` rise=1 targ v(out) val=`vvdd/2` fall=1" << endl;
	//file << ".measure tran tpdr trig v(in) val=`vvdd/2` fall=1 targ v(out) val=`vvdd/2` rise=1\n" << endl;
	//file << ".measure diff param='tpdr-tpdf' goal = 0\n" << endl;
	//file << "Vdt gin 0 pulse(vgnd vvdd 0 0p 0p 5n 10n)\n" << endl;
	//file << ".option post\n" << endl;
	//file << ".tran 1e-13 10n\n" << endl;

	//printAlter(netList, file);
	//file << ".end" << endl;
	return true;
}

float Spice::getFactor(char s){
	switch(s){
		case 'U': return 1;
		case 'N': return 0.001;  //fill with the others
		default: return 1000000;
	}
}

void Spice::printAlter(Circuit& circuit, ofstream &file){
	int numInputs = 0;
	map<string,bool> outputs;
	map<string,bool> inputs;

	Combination combination;
	CellNetlst *netlist = circuit.getCellNetlst( circuit.getTopCell() );
    map<string,Inst> &instances = netlist->getInstances();

	//SET ALL NETS FALSE (INPUT)
	vector<t_net>& nets=circuit.getCellNetlst(circuit.getTopCell())->getNets();
    for(vector<t_net>::iterator nets_it=nets.begin(); nets_it!=nets.end(); nets_it++){
		outputs.insert(pair<string,bool> (nets_it->name, false));
		inputs.insert(pair<string,bool> (nets_it->name, false));
    }//end for

    map<string, Interface> *interfaces = circuit.getInterfaces();
    for ( map<string, Interface>::iterator interfaces_it = interfaces->begin(); interfaces_it != interfaces->end(); interfaces_it++ ) {
		if ( interfaces_it->second.ioType != IOTYPE_OUTPUT ) continue;

		t_net &net = netlist->getNet(interfaces_it->first);
		outputs[net.name] = true;
    } // end for
	
	for ( map<string, Interface>::iterator interfaces_it = interfaces->begin(); interfaces_it != interfaces->end(); interfaces_it++ ) {
		if ( interfaces_it->second.ioType == IOTYPE_OUTPUT ) continue;

		t_net &net = netlist->getNet(interfaces_it->first);
		inputs[net.name] = true;
    } // end for

	int counter = 1;
	
	for ( map<string,bool>::iterator inputs_it = inputs.begin(); inputs_it != inputs.end(); inputs_it++ ) {
		if (inputs_it->second == true){
			numInputs++;
		} // end if
	} // end for
	
	combination.initialize( numInputs * 2 );

	do {	
		cout << "Test Vector " << counter << "\n";
		
		for ( int i = 0; i < numInputs; i++ ) {
			const int i0 = i*2;
			const int i1 = i0 + 1;
			file << ".alter" << endl;
			cout << "\tInput " << i << ": ";
			cout << combination.getElement(i0) << " -> " << combination.getElement(i1);
			cout << "\n";
		} // end for
		
		cout << "\n";
		
		counter++;
	} while ( combination.next() );	

}