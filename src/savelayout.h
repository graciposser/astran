/**************************************************************************
*   Copyright (C) 2005 by Adriel Mota Ziesemer Jr., Cristiano Lazzari     *
*   (amziesemerj,clazz)@inf.ufrgs.br                                      *
***************************************************************************/
#ifndef SAVELAYOUT_H
#define SAVELAYOUT_H

/**
Save the layouts

@author Adriel Mota Ziesemer Jr., Cristiano Lazzari
*/

#include <fstream>
#include <iostream>
#include <time.h>
#include <map>
#include <list>
#include "clayout.h"
#include "util.h"
#include "rules.h"

using namespace std;

class SaveLayout{
	private:
	ofstream file;
	Rules *rules;
	int idx;
	int cifmetric;
	map<string, int> instances;
	string filename;
		
	bool cellCif(map<string, CLayout>& layouts, string top);
	bool cellSight(map<string, CLayout>& layouts, string top);

	public:
	SaveLayout();
	~SaveLayout();
	bool saveCif(string fname, Rules &actual_rules, map<string, CLayout>& layouts, string top);
	bool cif2Cadence(string designName, string top);
	bool saveSight(string fname, Rules &actual_rules, map<string, CLayout>& layouts, string top);
};

#endif
