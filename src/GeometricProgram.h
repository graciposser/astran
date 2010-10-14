#ifndef GEOMETRIC_PROGRAM_H
#define GEOMETRIC_PROGRAM_H

#include <typeinfo>
#include <exception>
	using std::exception;
#include <iostream>
	using std::ostream;
#include <string>
	using std::string;
#include <vector>
	using std::vector;
#include <map>
	using std::map;
#include <utility>
	using std::pair;
	using std::make_pair;

class Posynomial;
class GeometricProgram;
class StandardGeometricProgram;

// [TODO] Repensar friendship :)
// [TODO] Criar uma maneira uniformizada de tratar tipos compostos.

class StandardPosynomialType {
public:

	virtual void print( ostream &out ) const = 0;

}; // end class

class StandardMonomial : public StandardPosynomialType {
public:
	struct Term {
		Term(){}
		Term( const string &variable, const double expoent ) {
			propVariable = variable;
			propExpoent = expoent;
		} // end constructor

		string propVariable;
		double propExpoent;
	};

private:

	vector<Term> clsTerms;
	double clsCoefficient;

public:

	StandardMonomial() {
		clsCoefficient = 1;
	} // end constructor

	void addTerm( const string &variable, const double expoent ) {
		// Avoid duplicate terms.
		for ( int i = 0; i < clsTerms.size(); i++ ) {
			Term &term = clsTerms[i];
			if ( term.propVariable == variable ) {
				term.propExpoent += expoent;
				return;
			} // end if
		} // end for

		// If the variables does not exists in this monimial, add it.
		clsTerms.push_back( Term(variable, expoent) );
	} // end method

	void setCoefficient( const double coefficient ) {
		clsCoefficient = coefficient;
	} // end method

	double getCoefficient() const { return clsCoefficient; }
	int getNumTerms() const { return clsTerms.size(); }
	const Term &getTerm( const int i ) const { return clsTerms[i]; }

	// Multiply this monomial by another one.
	void compose( StandardMonomial * monomial ) {
		clsCoefficient *= monomial->getCoefficient();

		const int numTerms = monomial->getNumTerms();
		for ( int i = 0; i < numTerms; i++ ) {
			const Term &term = monomial->getTerm(i);
			addTerm( term.propVariable, term.propExpoent );
		} // end for
	} // end method

	virtual void print( ostream &out ) const {
		if ( clsCoefficient != 1 ) {
			out << clsCoefficient;
			if ( clsTerms.size() > 0 )
				out << "*";
		} // end if

		for ( int i = 0; i < clsTerms.size(); i++ ) {
			if ( i > 0 )
				out << "*";

			const Term &term = clsTerms[i];
			out << term.propVariable;
			if ( term.propExpoent != 1.0 )
				out << "^" << term.propExpoent;
		} // end for
	} // end method


}; // end class

// -----------------------------------------------------------------------------

class StandardPosynomial : public StandardPosynomialType {
private:

	vector<StandardMonomial *> clsMonomials;


public:

	void addTerm( StandardMonomial * monomial ) {
		clsMonomials.push_back( monomial );
	} // end method

	void composeBySum( StandardPosynomial * posynomial ) {
		for ( int i = 0; i < posynomial->getNumMonomials(); i++ )
			addTerm( posynomial->getMonomial(i) );
	} // end method

	int getNumMonomials() const { return clsMonomials.size(); }
	StandardMonomial * getMonomial( const int i ) { return clsMonomials[i]; }

	virtual void print( ostream &out ) const {
		for ( int i = 0; i < clsMonomials.size(); i++ ) {
			if ( i > 0 )
				out << " + ";
			clsMonomials[i]->print(out);
		} // end for
	} // end method

}; // end class

// =============================================================================
// Types
// =============================================================================

class PosynomialType {
friend class GeometricProgram; friend class StandardGeometricProgram;
private:
	string clsName;

public:

	PosynomialType( const string &name ) : clsName( name ) {
	} // end constructor

	const string &getName() const { return clsName; }
	bool hasName() const { return !clsName.empty(); }

	virtual void print( ostream &out, const bool unroll = false ) const = 0;
	virtual StandardPosynomial * toStandardPosynomial( StandardGeometricProgram &gp ) const = 0;
	virtual PosynomialType * ungeneralize( GeometricProgram &gp ) = 0;

}; // end class

// -----------------------------------------------------------------------------

class MonomialType : public PosynomialType {
friend class GeometricProgram; friend class StandardGeometricProgram;
public:
	MonomialType( const string &name ) : PosynomialType( name ) {
	} // end constructor

	virtual StandardMonomial * toStandardMonomial( StandardGeometricProgram &gp ) const = 0;
	virtual StandardPosynomial * toStandardPosynomial( StandardGeometricProgram &gp ) const;
	virtual PosynomialType * ungeneralize( GeometricProgram &gp ) { return this; }

}; // end class

// -----------------------------------------------------------------------------

class GeneralizedPosynomialType : public PosynomialType {
friend class GeometricProgram; friend class StandardGeometricProgram;
public:
	GeneralizedPosynomialType( const string &name ) : PosynomialType( name ) {
	} // end constructor
}; // end class

// -----------------------------------------------------------------------------

class ConstantType : public MonomialType {
public:

	ConstantType( const string &name ) : MonomialType( name ) {
	} // end constructor

	virtual double computeValue() const = 0;
	virtual StandardMonomial * toStandardMonomial( StandardGeometricProgram &gp ) const;
};

// -----------------------------------------------------------------------------

class Constant : public ConstantType {
friend class GeometricProgram; friend class StandardGeometricProgram;
private:
	double clsValue;

	Constant( const string &name, const double value ) : ConstantType( name ) {
		clsValue = value;
	} // end constructor

public:

	virtual void print( ostream &out, const bool unroll = false ) const;
	virtual double computeValue() const { return clsValue; }
}; // end class

// -----------------------------------------------------------------------------

class ConstantSum : public ConstantType {
friend class GeometricProgram; friend class StandardGeometricProgram;
private:
	vector<ConstantType*> clsTerms;

	ConstantSum( const string &name ) : ConstantType( name ) {
	} // end constructor

public:
	void addTerm( ConstantType * constant ) {
		clsTerms.push_back( constant );
	} // end method

	virtual void print( ostream &out, const bool unroll = false ) const;

	virtual double computeValue() const {
		double value = 0;
		for ( int i = 0; i < clsTerms.size(); i++ )
			value += clsTerms[i]->computeValue();
		return value;
	} // end method

}; // end class

// -----------------------------------------------------------------------------

class ConstantMul : public ConstantType {
friend class GeometricProgram; friend class StandardGeometricProgram;
private:
	vector<ConstantType*> clsTerms;

	ConstantMul( const string &name ) : ConstantType( name ) {
	} // end constructor

public:
	void addTerm( ConstantType * constant ) {
		clsTerms.push_back( constant );
	} // end method

	virtual void print( ostream &out, const bool unroll = false ) const;

	virtual double computeValue() const {
		double value = 1;
		for ( int i = 0; i < clsTerms.size(); i++ )
			value *= clsTerms[i]->computeValue();
		return value;
	} // end method

}; // end class

// -----------------------------------------------------------------------------

class Variable : public MonomialType {
friend class GeometricProgram; friend class StandardGeometricProgram;
private:
	Variable( const string &name ) : MonomialType( name ) {
	} // end constructor

public:

	virtual void print( ostream &out, const bool unroll = false ) const;
	virtual StandardMonomial * toStandardMonomial( StandardGeometricProgram &gp ) const;

}; // end class

// -----------------------------------------------------------------------------

class Monomial : public MonomialType {
friend class GeometricProgram; friend class StandardGeometricProgram;
private:

	struct Term {
		Term(){}
		Term( Variable * variable, double expoent ) {
			propVariable = variable;
			propExpoent = expoent;
		} // end constructor

		Variable * propVariable;
		double propExpoent;
	};

	vector<Term> clsTerms;
	ConstantType * clsCoefficient;

	Monomial( const string &name ) : MonomialType( name ) {
		clsCoefficient = NULL;
	} // end constructor

	void printTerm( const Term &term, ostream &out ) const {
		out << term.propVariable->getName();
		if ( term.propExpoent != 1.0 )
			out << "^" << term.propExpoent;
	} // end method

public:

	void setCoefficient( ConstantType * coefficient ) {
		clsCoefficient = coefficient;
	} // end method

	void addTerm( Variable * variable, const double expoent ) {
		// Avoid duplicate terms.
		for ( int i = 0; i < clsTerms.size(); i++ ) {
			Term &term = clsTerms[i];
			if ( term.propVariable == variable ) {
				term.propExpoent += expoent;
				return;
			} // end if
		} // end for

		// If the variables does not exists in this monimial, add it.
		clsTerms.push_back( Term(variable, expoent) );
	} // end method

	virtual void print( ostream &out, const bool unroll = false ) const;
	virtual StandardMonomial * toStandardMonomial( StandardGeometricProgram &gp ) const;

}; // end class

// -----------------------------------------------------------------------------

class Posynomial : public PosynomialType {
friend class GeometricProgram; friend class StandardGeometricProgram;
private:
	vector<Monomial *> clsMonomials;

	Posynomial( const string &name ) : PosynomialType( name ) {
	} // end constructor

public:

	void addMonomial( Monomial * monomial ) {
		clsMonomials.push_back(monomial);
	} // end method

	virtual void print( ostream &out, const bool unroll = false ) const;
	virtual StandardPosynomial * toStandardPosynomial( StandardGeometricProgram &gp ) const;
	virtual PosynomialType * ungeneralize( GeometricProgram &gp ) { return this; }

}; // end class

// -----------------------------------------------------------------------------

class Sum : public PosynomialType {
friend class GeometricProgram; friend class StandardGeometricProgram;
private:
	vector<PosynomialType *> clsTerms;

	Sum( const string &name ) : PosynomialType( name ) {
	} // end constructor

public:

	void addTerm( PosynomialType * term ) {
		clsTerms.push_back(term);
	} // end method

	virtual void print( ostream &out, const bool unroll = false ) const;
	virtual StandardPosynomial * toStandardPosynomial( StandardGeometricProgram &gp ) const;
	
	virtual PosynomialType * ungeneralize( GeometricProgram &gp ) {
		for ( int i = 0; i < clsTerms.size(); i++ )
			clsTerms[i] = clsTerms[i]->ungeneralize(gp);
		return this;
	} // end method

}; // end class

// -----------------------------------------------------------------------------

class Mul : public PosynomialType {
friend class GeometricProgram; friend class StandardGeometricProgram;
private:
	vector<PosynomialType *> clsTerms;

	Mul( const string &name ) : PosynomialType( name ) {
	} // end constructor

public:
	void addTerm( PosynomialType * term ) {
		clsTerms.push_back(term);
	} // end method

	int getNumTerms() const { return clsTerms.size(); }
	PosynomialType * getTerm( const int i ) { return clsTerms[i]; }

	virtual void print( ostream &out, const bool unroll = false ) const;
	virtual StandardPosynomial * toStandardPosynomial( StandardGeometricProgram &gp ) const;

	virtual PosynomialType * ungeneralize( GeometricProgram &gp ) {
		for ( int i = 0; i < clsTerms.size(); i++ )
			clsTerms[i] = clsTerms[i]->ungeneralize(gp);
		return this;
	} // end method

}; // end class

// -----------------------------------------------------------------------------

class Div : public PosynomialType {
friend class GeometricProgram; friend class StandardGeometricProgram;
private:
	PosynomialType * clsNumerator;
	MonomialType * clsDenominator;

	Div( const string &name ) : PosynomialType( name ) {
		clsNumerator = NULL;
		clsDenominator = NULL;
	} // end constructor

public:

	void setNumerator( PosynomialType * numerator ) { clsNumerator = numerator; }
	void setDenomiator( MonomialType * denominator ) { clsDenominator = denominator; }

	virtual void print( ostream &out, const bool unroll = false ) const;
	virtual StandardPosynomial * toStandardPosynomial( StandardGeometricProgram &gp ) const;

	virtual PosynomialType * ungeneralize( GeometricProgram &gp ) {
		// [NOTICE] We don't need to deal with the denominator, since a
		// MonomialType is always non-generalized.

		clsNumerator = clsNumerator->ungeneralize(gp);
		return this;
	} // end method

}; // end class

// -----------------------------------------------------------------------------

class Max : public GeneralizedPosynomialType {
friend class GeometricProgram; friend class StandardGeometricProgram;
private:
	vector<PosynomialType *> clsTerms;

	Max( const string &name ) : GeneralizedPosynomialType( name ) {
	} // end constructor

public:
	void addTerm( PosynomialType * term ) {
		clsTerms.push_back(term);
	} // end method

	virtual void print( ostream &out, const bool unroll = false ) const;
	virtual StandardPosynomial * toStandardPosynomial( StandardGeometricProgram &gp ) const;
	virtual PosynomialType * ungeneralize( GeometricProgram &gp );
}; // end class

// =============================================================================
// Geometric Program Exception
// =============================================================================

class GeometricProgramException : public std::exception {
	private:
		string clsMsg;
	public:

		GeometricProgramException( const string &msg ) : clsMsg(msg) {}
		virtual ~GeometricProgramException() throw() {}

		virtual const char* what() const throw() {
			return clsMsg.c_str();
		} // end method
}; // end class

// =============================================================================
// Geometric Program Descriptor
// =============================================================================

class StandardGeometricProgram {
private:
	// Objective
	StandardPosynomial * clsObjective;

	// Constraints: posy < 1.
	vector< StandardPosynomial * > clsConstraints;

	// All elements: useful for free memory.
	vector< StandardPosynomialType * > clsElements;

	// Constraints: monomial = 1.
	// [TODO]

public:

	StandardPosynomial * createPosynomial();
	StandardMonomial * createMonomial();

	void setObjective( StandardPosynomial * objective );
	void addInequalityConstraint( StandardPosynomial * left );

	// Multiplies two monomials.
	StandardMonomial * composeByMul( StandardMonomial * left, StandardMonomial * right ) {
		StandardMonomial * monomial = createMonomial();
		monomial->compose( left );
		monomial->compose( right );
		return monomial;
	} // end method

	// Sum two posynomial.
	StandardPosynomial * composeBySum( StandardPosynomial * left, StandardPosynomial * right ) {
		StandardPosynomial * posynomial = createPosynomial();

		for ( int i = 0; i < left->getNumMonomials(); i++ )
			posynomial->addTerm( left->getMonomial(i) );

		for ( int i = 0; i < right->getNumMonomials(); i++ )
			posynomial->addTerm( right->getMonomial(i) );

		return posynomial;
	} // end method

	// Multiplies two posynomials.
	StandardPosynomial * composeByMul( StandardPosynomial * left, StandardPosynomial * right ) {
		StandardPosynomial * posynomial = createPosynomial();

		for ( int i = 0; i < left->getNumMonomials(); i++ )
			for ( int j = 0; j < right->getNumMonomials(); j++ )
				posynomial->addTerm( composeByMul( left->getMonomial(i), right->getMonomial(j) ) );

		return posynomial;
	} // end method

	// Divide a posynomial by a monomial.
	StandardPosynomial * composeByDiv( StandardPosynomial * numerator, StandardMonomial * denominator ) {
		StandardMonomial * monomial = createMonomial();
		StandardPosynomial * posynomial = createPosynomial();

		monomial->setCoefficient( 1 / denominator->getCoefficient() );

		const int numTerms = denominator->getNumTerms();
		for ( int i = 0; i < numTerms; i++ ) {
			const StandardMonomial::Term &term = denominator->getTerm(i);
			monomial->addTerm( term.propVariable, -term.propExpoent );
		} // end for

		posynomial->addTerm( monomial );
		return composeByMul( numerator, posynomial );
	} // end method

	// Prints in a human-readable way.
	void print( ostream &out ) const;

}; // public

// =============================================================================
// Geometric Program Descriptor
// =============================================================================

class GeometricProgram {
private:
	int clsInternalVariableCounter;

	// Objective
	PosynomialType * clsObjective;

	// Constraints.
	vector< pair<PosynomialType *, MonomialType *> > clsConstraints;

	// Basic types.
	map<string,Variable*> clsVariables;
	map<string,Monomial*> clsMonomials;
	map<string,Posynomial*> clsPosynomials;

	// Generic types.
	map<string,ConstantType*> clsConstantTypes;
	map<string,PosynomialType*> clsAny;
	
	vector<string> clsNames;
	vector<PosynomialType*> clsElements;

	void registerPosynomialType( PosynomialType * e ) {
		if ( e->getName() != "" ) {
			clsNames.push_back( e->getName() );
			clsAny[ e->getName() ] = e;

			if ( typeid(*e) == typeid( Constant ) )
				clsConstantTypes[e->getName()] = ( ConstantType * ) e;
			else if ( typeid(*e) == typeid( ConstantSum ) )
				clsConstantTypes[e->getName()] = ( ConstantType * ) e;
			else if ( typeid(*e) == typeid( ConstantMul ) )
				clsConstantTypes[e->getName()] = ( ConstantType * ) e;
			else if ( typeid(*e) == typeid( Variable ) )
				clsVariables[e->getName()] = ( Variable * ) e;
			else if ( typeid(*e) == typeid( Monomial ) )
				clsMonomials[e->getName()] = ( Monomial * ) e;
			else if ( typeid(*e) == typeid( Posynomial ) )
				clsPosynomials[e->getName()] = ( Posynomial * ) e;
		} // end if

		clsElements.push_back(e);
	} // end method

public:
	GeometricProgram();

	Variable * createInternalVariable();

	Constant * createConstant( const string &name, const double value );
	ConstantSum * createConstantSum( const string &name );
	ConstantMul * createConstantMul( const string &name );
	Variable * createVariable( const string &name );
	Monomial * createMonomial( const string &name );
	Posynomial * createPosynomial( const string &name );
	Sum * createSum( const string &name );
	Mul * createMul( const string &name );
	Max * createMax( const string &name );
	Div * createDiv( const string &name );

	Monomial * createMonomial( const string &name, ConstantType * coefficient, Variable * variable, double expoent = 1 );

	ConstantSum * createConstantSum( const string &name, ConstantType *op1, ConstantType * op2 );
	ConstantMul * createConstantMul( const string &name, ConstantType *op1, ConstantType * op2 );

	Sum * createSum( const string &name, PosynomialType *op1, PosynomialType * op2 );
	Mul * createMul( const string &name, PosynomialType *op1, PosynomialType * op2 );
	Max * createMax( const string &name, PosynomialType *op1, PosynomialType * op2 );

	Div * createDiv( const string &name, PosynomialType * numerator, MonomialType * denominator );

	Constant * createConstant( const double value );
	ConstantSum * createConstantSum();
	ConstantMul * createConstantMul();
	Monomial * createMonomial();
	Monomial * createMonomial( ConstantType * coefficient, Variable * variable, double expoent = 1 );
	Posynomial * createPosynomial();
	Sum * createSum();
	Mul * createMul();
	Max * createMax();
	Div * createDiv();

	ConstantSum * createConstantSum( ConstantType *op1, ConstantType * op2 );
	ConstantMul * createConstantMul( ConstantType *op1, ConstantType * op2 );

	Sum * createSum( PosynomialType *op1, PosynomialType * op2 );
	Mul * createMul( PosynomialType *op1, PosynomialType * op2 );
	Max * createMax( PosynomialType *op1, PosynomialType * op2 );

	Div * createDiv( PosynomialType * numerator, MonomialType * denominator );

	ConstantType * requestConstantType( const string &name ) const;
	Variable * requestVariable( const string &name ) const;
	Monomial * requestMonomial( const string &name ) const;
	Posynomial * requestPosynomial( const string &name ) const;
	PosynomialType * requestPosynomialType( const string &name ) const;

	void addInequalityConstraint( PosynomialType * left, MonomialType * right );
	void setObjective( PosynomialType * objective );

	// Replace any generalized posynomial by a normal posynomial. This may
	// create new variables and constraints.
	//
	// [NOTICE] You should call this method before converting this geometric
	// program to a standard one.
	void ungeneralize();

	// Build a standard version of this geometric program.
	void standardize( StandardGeometricProgram &gp );

	// Prints in a human-readable way.
	void print( ostream &out ) const;
	
}; // end class

#endif