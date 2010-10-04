/**************************************************************************
*   Copyright (C) 2005 by Adriel Mota Ziesemer Jr., Cristiano Lazzari     *
*   (amziesemerj,clazz)@inf.ufrgs.br                                      *
***************************************************************************/

#ifndef RULES_H
#define RULES_H

/**
Store and manage the design rules

@author Adriel Mota Ziesemer Jr., Cristiano Lazzari
*/

#include <iostream>
#include <fstream>
#include <strstream>
#include <string>
#include <math.h>
#include "util.h"

using namespace std;

enum rule_name {
	S1P1P1,S1DFP1,E1P1DF,E2P1DF,E1DFP1,E1DNP1,E1DDP1,R1P1  ,S1DFP2,S1P1P2,
	W2CT  ,E1M1CT,E2M1CT,E3M1CT,E1DFCT,E1P1CT,E2P1CT,E3P1CT,E1P2CT,S1CTP1,S1CTDP,S1CTDN,S1CTDF,
	S1CTP2,R1M1  ,R2M1  ,W2VI  ,E1M1VI,E1M2VI,E2M1VI,S1M2M2,S1M2M1,
	S1M1M2,R2M2  ,W2P1  ,W2DF  ,W2V2  ,S1M1M1,S2M1M1,E1M2V2,S1CTCT,E1INDF,
	E1IPDF,E1WNDP,S1DNWN, S2M2M2,W1M1  ,W1M2  ,W1M3  ,W1M4  ,W1M5  ,
	W1M6  ,W1M7  ,W1M8  ,W1M9  ,W1M10 ,S1DFDF,S1VIVI,S1V2V2,S1M3M3,
	E1M3V2,E1M3V3,W2V3  ,E1M4V3,S1M4M4,W2V4  ,E1M4V4,S2P1P1,E1M5V4,
	S1M5M5,W2V5  ,S1IPIP,S1ININ,W2V6  ,W2V7  ,W2V8  ,W2V9  ,W2V10,
	N_RULES
};

struct trules {
	string name;
	string desc;
	float val;
};

static trules rules_lst[] = {
	{"S1P1P1","Minimum Gate Spacing",0},
	{"S1DFP1","Minimum POLY1 spacing to DIFF",0},
	{"E1P1DF","Minimum POLY1 extension of GATE",0},
	{"E2P1DF","Minimum POLY1 extension of GATE in L shape diff",0},
	{"E1DFP1","Minimum DIFF extension of GATE",0},
	{"E1DNP1","Minimum NDIFF extension of GATE when butted to PDIFF",0},
	{"E1DDP1","Minimum PDIFF extension of GATE when butted to NDIFF",0},
	{"R1P1"  ,"Maximum ratio of POLY area to touched GATE area",0},
	{"S1DFP2","Minimum POLY2 spacing to DIFF",0},
	{"S1P1P2","Minimum POLY1 spacing to POLY2",0},
	{"W2CT"  ,"Fixed CONT size",0},
	{"E1M1CT","Minimum MET1 enclosure of CONT",0},
	{"E2M1CT","Minimum MET1 extension on CONT on at least 2 opposite sides",0},
	{"E3M1CT","Minimum MET1 enclosure of CONT if E2M1CT is not fulfilled",0},
	{"E1DFCT","Minimum DIFF enclosure of CONT",0},
	{"E1P1CT","Minimum POLY1 enclosure of CONT",0},
	{"E2P1CT","Minimum POLY1 extension on CONT on at least 2 opposite sides",0},
	{"E3P1CT","Minimum POLY1 enclosure of CONT if E2P1CT is not fulfilled",0},
	{"E1P2CT","Minimum POLY2 enclosure of CONT",0},
	{"S1CTP1","Minimum DIFFCON spacing of GATE",0},
	{"S1CTDP","Minimum NDIFFCON spacing of PDIFF",0},
	{"S1CTDN","Minimum PDIFFCON spacing of NDIFF",0},
	{"S1CTDF","Minimum POLY1CON spacing of DIFF",0},
	{"S1CTP2","Minimum POLY1CON spacing of POLY2",0},
	{"R1M1"  ,"Minimum ratio of MET1 area to die area",0},
	{"R2M1"  ,"Maximum ratio of MET1 area to connected GATE and CPOLY area",0},
	{"W2VI"  ,"Fixed VIA size",0},
	{"E1M1VI","Minimum MET1 enclosure of VIA",0},
	{"E1M2VI","Minimum MET2 enclosure of VIA",0},
	{"E2M1VI","Minimum WIDE_MET1 enclosure of VIA",0},
	{"S1M2M2","Minimum MET2 spacing to MET2",0},
	{"S1M2M1","Minimum MET2 spacing to MET1 over CPOLY",0},
	{"S1M1M2","Minimum MET1 spacing to MET2 over CPOLY",0},
	{"R2M2"  ,"Maximum ratio of MET2 area to connected GATE and CPOLY area",0},
	{"W2P1"  ,"Minimum GATE length",0},
	{"W2DF"  ,"Minimum GATE width",0},
	{"W2V2"  ,"Fixed VIA2 size",0},
	{"S1M1M1","Minimum MET1 spacing to MET1",0},
	{"S2M1M1","Minimum Wide MET1 spacing to MET1",0},
	{"E1M2V2","Minimum MET2 enclose of VIA2",0},
	{"S1CTCT","Minumum CONT spacing",0},
	{"E1INDF","Minimum NPLUS extension of DIFF",0},
	{"E1IPDF","Minimum PPLUS extension of DIFF",0},
	{"E1WNDP","Minimum NTUB enclosure of PDIFF",0},
	{"S1DNWN","Minimum NDIFF spacing to NTUB",0},
	{"S2M2M2","Minimum MET2 spacing to WIDE_MET2",0},
	{"W1M1"  ,"Minimum MET1 width",0},
	{"W1M2"  ,"Minimum MET2 width",0},
	{"W1M3"  ,"Minimum MET3 width",0},
	{"W1M4"  ,"Minimum MET4 width",0},
	{"W1M5"  ,"Minimum MET5 width",0},
	{"W1M6"  ,"Minimum MET6 width",0},
	{"W1M7"  ,"Minimum MET7 width",0},
	{"W1M8"  ,"Minimum MET8 width",0},
	{"W1M9"  ,"Minimum MET9 width",0},
	{"W1M10" ,"Minimum MET10 width",0},
	{"S1DFDF","Minimum DIFF spacing to DIFF",0},
	{"S1VIVI","Minimum VIA spacing to VIA",0},
	{"S1V2V2","Minimum VIA2 spacing to VIA2",0},
	{"S1M3M3","Minimum MET3 spacing to MET3",0},
	{"E1M3V2","Minimum MET3 enclosure of VIA2",0},
	{"E1M3V3","Minimum MET3 enclosure of VIA3",0},
	{"W2V3"  ,"Fixed VIA3 size",0},
	{"E1M4V3","Minimum MET4 enclosure of VIA3",0},
	{"S1M4M4","Minimum MET4 spacing to MET4",0},
	{"W2V4"  ,"Fixed VIA4 size",0},
	{"E1M4V4","Minimum MET4 enclosure of VIA4",0},
	{"S2P1P1","Minimum GATE spacing to GATE",0},
	{"E1M5V4","Minimum MET5 enclosure of VIA4",0},
	{"S1M5M5","Minimum MET5 spacing to MET5",0},
	{"W2V5"  ,"Fixed VIA5 size",0},
	{"S1IPIP","PPLUS spacing to PPLUS",0},
	{"S1ININ","NPLUS spacing to NPLUS",0},
	{"W2V6"  ,"Fixed VIA6 size",0},
	{"W2V7"  ,"Fixed VIA7 size",0},
	{"W2V8"  ,"Fixed VIA8 size",0},
	{"W2V9"  ,"Fixed VIA9 size",0},
	{"W2V10" ,"Fixed VIA10 size",0}
};


// Layer names for the CIF file

enum layer_name {NWEL,PWEL,NSEL,PSEL,NDIF,PDIF,POLY,CONT,MET1,MET1P,VIA1,
	MET2,MET2P,VIA2,MET3,MET3P,VIA3,MET4,VIA4,MET5,VIA5,MET6,VIA6,
	MET7,VIA7,MET8,VIA8,MET9,VIA9,MET10,CELLBOX,
	TECHNAME, // NOME DA TECNOLOGIA
	MLAYERS,  // Numero de niveis
	SOI,      // Para definir se a tecnologia e' SOI ou nao
	N_LAYER_NAMES
};

enum layerPurpose {L_DRAWING, L_PIN, L_NET, L_OTHER};

struct layer_labels {
	string name, desc, valCIF, valGDSII, valTech;
	layerPurpose purpose;	
};

static layer_labels layer_labels_lst[]={
	{"NWEL","N Well","","","",L_DRAWING},
	{"PWEL","P Well","","","",L_DRAWING},
	{"NSEL","Select N-Type Diffusion","","","",L_DRAWING},
	{"PSEL","Select P-Type Diffusion","","","",L_DRAWING},
	{"NDIF","N+ Diffusion","","","",L_DRAWING},
	{"PDIF","P+ Diffusion","","","",L_DRAWING},
	{"POLY","Polysilicon","","","",L_DRAWING},
	{"CONT","Contact","","","",L_DRAWING},
	{"MET1","Metal 1","","","",L_DRAWING},
	{"MET1P","Metal 1 Pin","","","",L_PIN},
	{"VIA1","Via 1","","","",L_DRAWING},
	{"MET2","Metal 2","","","",L_DRAWING},
	{"MET2P","Metal 2 Pin","","","",L_PIN},
	{"VIA2","Via 2","","","",L_DRAWING},
	{"MET3","Metal 3","","","",L_DRAWING},
	{"MET3P","Metal 3 Pin","","","",L_PIN},
	{"VIA3","Via 3","","","",L_DRAWING},
	{"MET4","Metal 4","","","",L_DRAWING},
	{"VIA4","Via 4","","","",L_DRAWING},
	{"MET5","Metal 5","","","",L_DRAWING},
	{"VIA5","Via 5","","","",L_DRAWING},
	{"MET6","Metal 6","","","",L_DRAWING},
	{"VIA6","Via 6","","","",L_DRAWING},
	{"MET7","Metal 7","","","",L_DRAWING},
	{"VIA7","Via 7","","","",L_DRAWING},
	{"MET8","Metal 8","","","",L_DRAWING},
	{"VIA8","Via 8","","","",L_DRAWING},
	{"MET9","Metal 9","","","",L_DRAWING},
	{"VIA9","Via 9","","","",L_DRAWING},
	{"MET10","Metal 10","","","",L_DRAWING},
	{"CELLBOX","Cell Box","","","",L_DRAWING},
	{"TECHNAME","","TECHNAME","","",L_OTHER},
	{"MLAYERS","","3","","",L_OTHER},
	{"SOI","","YES","","",L_OTHER}
};

class Rules {
protected:
	unsigned int resolution;
	bool calcGrid();
	unsigned int grid;
	
public:
	Rules();
	~Rules();

	// General methods
	void setTechName(string newTechName);
	void setMLayers(string newMLayers);
	void setSOI(string newSOI);
	void setResolution(int r) {resolution=r;};
	unsigned int getScale() {return resolution;};

	bool readRules(string filename);
	bool saveRules (string filename);
	bool saveCIFLayerTable(string filename);
	bool saveGDSIILayerTable(string filename);
	
	// Rules' methods
	int getRule(rule_name name);
	float getRulef(rule_name name);
	string getRuleName(int i){return rules_lst[i].name;};
	string getRuleDesc(rule_name);
	int findRule(string name);
	void setRule(rule_name name, float val);
	int getRulesSize(){return N_RULES;};
	void listRules();
	int getIntValue(float f);

	// Layers' methods
	string getCIFVal(layer_name name);
	string getGDSIIVal(layer_name name);
	string getTechVal(layer_name name);
	string getLayerName(int i){return layer_labels_lst[i].name;};
	string getLayerDesc(layer_name);
	int findLayerName(string name);
	int findCIFVal(string name);
	int findTechVal(string name);
	void setCIFVal(layer_name name, string val);
	void setGDSIIVal(layer_name name, string val);
	void setTechVal(layer_name name, string val);
	int getLayersSize(){return TECHNAME;};
	void listLayersLabels();
	string getLayerPurposeText(layerPurpose l);
};

#endif