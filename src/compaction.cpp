/**
 \file compaction.cpp
 Compact layout. 
 \date 25-sep-2006 \author Cristiano Lazzari <clazz@inf.ufrgs.br> 
 */

#include "compaction.h"

/** Constructor. */
compaction::compaction( cp_algo a ) {
	algo = a;
	lp_filename = "compaction";
	variables.clear();
	constraints.clear();
}

/** Insert new variable. */
void compaction::insertVal( string name ) {
	variables[name] = 0;  
	//  cout << "INS VAR " << name << endl;
}

/** Set the LP filename. */
void compaction::setLPFilename( string name ) {
	lp_filename = name;
}

/** Insert new constraint. */
void compaction::insertConstraintBTZ( string v1 ) {
	
	cp_constraint cpc;
	
	cpc.v1 = variables.find( v1 );
	cpc.v2 = variables.end();
	cpc.t  = variables.end();
	
	if ( cpc.v1 == variables.end() ) {
		insertVal( v1 );
		cpc.v1 = variables.find( v1 );
	}
	
	cpc.type = CP_BIG_ZERO;
	
	constraints.push_back( cpc );
}

/** Insert new constraint. */
void compaction::insertConstraintEBTZ( string v1 ) {
	
	cp_constraint cpc;
	
	cpc.v1 = variables.find( v1 );
	cpc.v2 = variables.end();
	cpc.t  = variables.end();
	
	if ( cpc.v1 == variables.end() ) {
		insertVal( v1 );
		cpc.v1 = variables.find( v1 );
	}
	
	cpc.type = CP_BIG_EQ_ZERO;
	
	constraints.push_back( cpc );
}

/** Insert new constraint. */
void compaction::insertConstraintEZ( string v1 ) {
	
	cp_constraint cpc;
	
	cpc.v1 = variables.find( v1 );
	cpc.v2 = variables.end();
	cpc.t  = variables.end();
	
	if ( cpc.v1 == variables.end() ) {
		insertVal( v1 );
		cpc.v1 = variables.find( v1 );
	}
	
	cpc.type = CP_EQ_ZERO;
	
	constraints.push_back( cpc );
}


/** Insert new constraint. */
void compaction::insertConstraint( string v1, string v2, cp_cons_tp type, int val ) {
	
	cp_constraint cpc;
	
	cpc.v1 = variables.find( v1 );
	cpc.v2 = variables.find( v2 );
	cpc.t  = variables.end();
	
	if ( cpc.v1 == variables.end() ) {
		insertVal( v1 );
		//    variables[v1] = 0;  
		cpc.v1 = variables.find( v1 );
	}
	
	if ( cpc.v2 == variables.end() ) {
		insertVal( v2 );
		//    variables[v2] = 0;  
		cpc.v2 = variables.find( v2 );
	}
	
	cpc.type = type;
	cpc.val = val;    
	
	constraints.push_back( cpc );
	
}

/** Insert new constraint. */
void compaction::insertConstraint( string v1, string v2, cp_cons_tp type, string t ) {
	
	cp_constraint cpc;
	
	cpc.v1 = variables.find( v1 );
	cpc.v2 = variables.find( v2 );
	cpc.t  = variables.find( t );
	
	if ( cpc.v1 == variables.end() ) {
		insertVal( v1 );
		cpc.v1 = variables.find( v1 );
	}
	
	if ( cpc.v2 == variables.end() ) {
		insertVal( v2 );
		cpc.v2 = variables.find( v2 );
	}
	if ( cpc.t == variables.end() ) {
		insertVal( t );
		cpc.t = variables.find( t );
	}
	
	if ( type == CP_EQ )
		cpc.type = CP_EQ_VAR;
	else if ( type == CP_MIN )
		cpc.type = CP_MIN_VAR;
	else if ( type == CP_MAX )
		cpc.type = CP_MAX_VAR;
	else
		cpc.type = type;
	
	constraints.push_back( cpc );  
}

/** Insert new constraint. */
void compaction::insertConstraint( string v1, string v2, cp_cons_tp type, string t, int val ) {
	
	cp_constraint cpc;
	
	cpc.v1 = variables.find( v1 );
	cpc.v2 = variables.find( v2 );
	cpc.t  = variables.find( t );
	
	if ( cpc.v1 == variables.end() ) {
		insertVal( v1 );
		cpc.v1 = variables.find( v1 );
	}
	
	if ( cpc.v2 == variables.end() ) {
		insertVal( v2 );
		cpc.v2 = variables.find( v2 );
	}
	if ( cpc.t == variables.end() ) {
		insertVal( t );
		cpc.t = variables.find( t );
	}  
	
	if ( type == CP_EQ )
		cpc.type = CP_EQ_VAR_VAL;
	else if ( type == CP_MIN )
		cpc.type = CP_MIN_VAR_VAL;
	else if ( type == CP_MAX )
		cpc.type = CP_MAX_VAR_VAL;
	else
		cpc.type = type;
	
	cpc.val = val;
	
	constraints.push_back( cpc );  
}

/** Insert Upper Bound. */
void compaction::insertUpperBound( string v1, int val ) {
	
	cp_constraint cpc;
    
	cpc.v1 = variables.find( v1 );
	cpc.v2 = variables.end();
	cpc.t  = variables.end();
	
	if ( cpc.v1 == variables.end() ) {
		insertVal( v1 );
		cpc.v1 = variables.find( v1 );
	}
	
	cpc.type = CP_UPPER_BOUND;
	cpc.val = val;
	
	constraints.push_back( cpc );
}

/** Insert Upper Bound. */
void compaction::insertLowerBound( string v1, int val ) {
	
	cp_constraint cpc;
    
	cpc.v1 = variables.find( v1 );
	cpc.v2 = variables.end();
	cpc.t  = variables.end();
	
	if ( cpc.v1 == variables.end() ) {
		insertVal( v1 );
		cpc.v1 = variables.find( v1 );
	}
	
	cpc.type = CP_LOWER_BOUND;
	cpc.val = val;
	
	constraints.push_back( cpc );
}

/** Force these variavles to be integer. */
void compaction::forceIntegerVar( string v ) {
	int_vars.push_back( v );
}

/** Insert LP variable. These variables are include in the minimization objective function. */
void compaction::insertLPMinVar( string v ) {
	lp_min_var.push_back( v );
	lp_min_val.push_back( 1 );
}

/** Insert LP variable. These variables are include in the minimization objective function. */
void compaction::insertLPMinVar( string v, int i ) {
	lp_min_var.push_back( v );
	lp_min_val.push_back( i );
}

/** Solve compaction constraints with linear programming. */
int compaction::solve(string lpSolverFile) {
	cout << "Calling LP Solver (" 
	<< variables.size() << " variables, " 
	<< constraints.size() << " constraints)" << endl;
	
	string fn = lp_filename + ".lp";
	ofstream f(fn.c_str());
	
	
	if ( !f ) {
		cerr << "ERROR:Cannot create file " << fn << ". Please verify your temporary directory." << endl;
		exit(-1);
	}
	
	f << "min: "; 
	for ( unsigned int i = 0; i < lp_min_var.size(); i++ ) {    
		if ( i != 0 )
			f << " + ";
		if ( lp_min_val[i] != 1 )
			f << lp_min_val[i] << " ";
		
		f << lp_min_var[i];
	}
	f << ";" << endl;
	
	// Constant zero
	variables[ "ZERO" ] = 0;
	f << "Czero: ZERO = 0;" << endl;
	
	for ( unsigned int i = 0; i < constraints.size(); i++ ) {
		
		string v1 = constraints[i].v1->first;
		
		string v2  = "0";
		if ( constraints[i].v2 != variables.end() )
			v2 = constraints[i].v2->first;
		
		string t  = "0";
		if ( constraints[i].t != variables.end() )
			t = constraints[i].t->first;
		
		cp_cons_tp type = constraints[i].type;
		
		int val = constraints[i].val;
		
		if ( type == CP_MIN )
			f << "C" << i << ": " << v2 << " - " << v1 << " >= " << val << ";" << endl;
		if ( type == CP_MAX )
			f << "C" << i << ": " << v2 << " - " << v1 << " <= " << val << ";" << endl;
		else if ( type == CP_EQ )
			f << "C" << i << ": " << v2 << " - " << v1 << " = " << val << ";" << endl;
		else if ( type == CP_MIN_VAR_VAL )
			f << "C" << i << ": " << v2 << " - " << v1 << " >= " << val << " " << t << ";" << endl;
		else if ( type == CP_MAX_VAR_VAL )
			f << "C" << i << ": " << v2 << " - " << v1 << " <= " << val << " " << t << ";" << endl;
		else if ( type == CP_EQ_VAR_VAL )
			f << "C" << i << ": " << v2 << " - " << v1 << " = " << val << " " << t << ";" << endl;
		else if ( type == CP_MIN_VAR )
			f << "C" << i << ": " << v2 << " - " << v1 << " >= " << t << ";" << endl;
		else if ( type == CP_MAX_VAR )
			f << "C" << i << ": " << v2 << " - " << v1 << " <= " << t << ";" << endl;
		else if ( type == CP_EQ_VAR )
			f << "C" << i << ": " << v2 << " - " << v1 << " = " << t << ";" << endl;
		else if ( type == CP_BIG_ZERO )
			f << "C" << i << ": " << v1 << " > 0;" << endl;
		else if ( type == CP_BIG_EQ_ZERO )
			f << "C" << i << ": " << v1 << " >= 0;" << endl;
		else if ( type == CP_EQ_ZERO )
			f << "C" << i << ": " << v1 << " = 0;" << endl;
		else if ( type == CP_UPPER_BOUND )
			f << "C" << i << ": " << v1 << " <= " << val << ";" << endl;
		else if ( type == CP_LOWER_BOUND )
			f << "C" << i << ": " << v1 << " >= " << val << ";" << endl;
		
	}
	
	if ( int_vars.size() > 0 ) {  
		f << "int "; 
		for ( unsigned int i = 0; i < int_vars.size(); i++ ) {
			if ( i != 0 )
				f << ", ";
			f << int_vars[i];
		}
		f << ";" << endl;
	}
	
	f.close();
    string cmd = "\"" + lpSolverFile + "\" " + 	lp_filename + ".lp 2> temp.log";
	cout << "Running command: " << cmd << endl;
	
	FILE *x = _popen(cmd.c_str(), "r");
	
	if(x==NULL){
		cout << "ERROR: Problem to execute lp_solve!" << endl;
		return 0;
	}
	
	char str[150];
	while (fgets(str, 150, x)) {
		
		istringstream s(str);
		
		string n;
		int v;
		
		s >> n;
		if(n=="This"){
			cout << "Error executing LP Solver: " << str << endl;
			return 0;
		}
		s >> v;
		
		
		map<string,int>::iterator i = variables.find( n );
		if ( i != variables.end() ) {
			i->second = v;
		}
	}
	
	_pclose(x);

	/*
	char buffer = NULL;
	ifstream file;
	string filename = lp_filename + ".log";
	file.open(filename.c_str());
	while (!file.eof()){
		file.get(buffer);
		cout << buffer;
	}
	file.close();
	*/
	return 1;
}

/** Get variables values. */
int compaction::getVariableVal( string name ) {
	map<string,int>::iterator i = variables.find( name );
	if ( i != variables.end() ) 
		return i->second;
	return -1;  
}