#include <sstream>
	using std::ostringstream;

#include "GeometricProgram.h"

// [TODO] Criar templete pra createXXX();
// [TODO] Verificar se nome já existe.
// [TODO] Destructors. Free memory!
// [TODO] Verificar se os nomes são válidos. Reserver os iniando com underline
//        para variáveis internas.

// -----------------------------------------------------------------------------

GeometricProgram::GeometricProgram() {
	clsObjective = NULL;
	clsInternalVariableCounter = 0;
} // end constructor

// -----------------------------------------------------------------------------

Variable * GeometricProgram::createInternalVariable() {
	ostringstream oss;
	oss << "_var" << clsInternalVariableCounter;
	clsInternalVariableCounter++;

	return createVariable( oss.str() );
} // end constructor

// -----------------------------------------------------------------------------

Constant * GeometricProgram::createConstant( const string &name, const double value ) {
	Constant * element = new Constant( name, value );
	registerPosynomialType( element );
	return element;
} // end method

// -----------------------------------------------------------------------------

Variable * GeometricProgram::createVariable( const string &name ) {
	if ( name == "" )
		throw GeometricProgramException( "Variables must be named." );

	Variable * element = new Variable( name );
	registerPosynomialType( element );
	return element;
} // end method

// -----------------------------------------------------------------------------

Monomial * GeometricProgram::createMonomial( const string &name ) {
	Monomial * element = new Monomial( name );
	registerPosynomialType( element );
	return element;
} // end method

// -----------------------------------------------------------------------------

Monomial * createMonomial( const string &name, Constant * coefficient, Variable * variable, double expoent ) {
	Monomial * element = new Monomial( name );
	element->setCoefficient( coefficient );
	element->addTerm( variable, expoent );

	registerPosynomialType( element );
	return element;
} // end method

// -----------------------------------------------------------------------------

Posynomial * GeometricProgram::createPosynomial( const string &name ) {
	Posynomial * element = new Posynomial( name );
	registerPosynomialType( element );
	return element;
} // end method

// -----------------------------------------------------------------------------

Sum * GeometricProgram::createSum( const string &name ) {
	Sum * element = new Sum( name );
	registerPosynomialType( element );
	return element;
} // end method

// -----------------------------------------------------------------------------

Mul * GeometricProgram::createMul( const string &name ) {
	Mul * element = new Mul( name );
	registerPosynomialType( element );
	return element;
} // end method

// -----------------------------------------------------------------------------

Sum * GeometricProgram::createSum( const string &name, PosynomialType *op1, PosynomialType * op2 ) {
	Sum * element = new Sum( name );
	element->addTerm( op1 );
	element->addTerm( op2 );
	
	registerPosynomialType( element );
	return element;
} // end method

// -----------------------------------------------------------------------------

Mul * GeometricProgram::createMul( const string &name, PosynomialType *op1, PosynomialType * op2 ) {
	Mul * element = new Mul( name );
	element->addTerm( op1 );
	element->addTerm( op2 );

	registerPosynomialType( element );
	return element;
} // end method


// -----------------------------------------------------------------------------

Max * GeometricProgram::createMax( const string &name, PosynomialType *op1, PosynomialType * op2 ) {
	Max * element = new Max( name );
	element->addTerm( op1 );
	element->addTerm( op2 );

	registerPosynomialType( element );
	return element;
} // end method

// -----------------------------------------------------------------------------

Max * GeometricProgram::createMax( const string &name ) {
	Max * element = new Max( name );
	registerPosynomialType( element );
	return element;
} // end method

// -----------------------------------------------------------------------------

Div * GeometricProgram::createDiv( const string &name ) {
	Div * element = new Div( name );
	registerPosynomialType( element );
	return element;
} // end method

// -----------------------------------------------------------------------------

Div * GeometricProgram::createDiv( const string &name, PosynomialType * numerator, MonomialType * denominator ) {
	Div * element = new Div( name );
	element->setNumerator( numerator );
	element->setDenomiator( denomiantor );

	registerPosynomialType( element );
	return element;
} // end method

// -----------------------------------------------------------------------------

void GeometricProgram::print( ostream &out ) const {
	const int numElements = clsElements.size();
	for ( int i = 0; i < numElements; i++ ) {
		const PosynomialType * e = clsElements[i];

		if ( typeid(*e) == typeid( Variable ) )
			continue;

		if ( e->getName() != "" ) {
			out << e->getName() << " = ";
			e->print( out, true );
			out << "\n";
		} // end if
	} // end for

	out << "\n";

	out << "minimize ";
	clsObjective->print(out);
	out << "\n";
	out << "subject to\n";

	const int numConstraints = clsConstraints.size();
	for ( int i = 0; i < numConstraints; i++ ) {
		PosynomialType * posynomial = clsConstraints[i].first;
		MonomialType * monomial = clsConstraints[i].second;

		out << "\t";
		posynomial->print(out);
		out << " <= ";
		monomial->print(out);
		out << "\n";
	} // end if

} // end method

// -----------------------------------------------------------------------------

Constant * GeometricProgram::createConstant( const double value ) { return createConstant( "", value ); }
Monomial * GeometricProgram::createMonomial() { return createMonomial(""); }
Posynomial * GeometricProgram::createPosynomial() { return createPosynomial(""); }

Sum * GeometricProgram::createSum() { return createSum(""); }
Mul * GeometricProgram::createMul() { return createMul(""); }
Div * GeometricProgram::createDiv() { return createDiv(""); }
Max * GeometricProgram::createMax() { return createMax(""); }

Monomial * GeometricProgram::createMonomial( Constant * coefficient, Variable * variable, double expoent ) { return createMonomial( coefficient, variable, expoent ); }

Sum * GeometricProgram::createSum( PosynomialType *op1, PosynomialType * op2 ) { return createSum( "", op1, op2 ); }
Mul * GeometricProgram::createMul( PosynomialType *op1, PosynomialType * op2 ) { return createMul( "", op1, op2 ); }
Max * GeometricProgram::createMax( PosynomialType *op1, PosynomialType * op2 ) { return createMax( "", op1, op2 ); }

Div * GeometricProgram::createDiv( PosynomialType * numerator, MonomialType * denominator ) { return createDiv( "", numerator, denominator ); }

// -----------------------------------------------------------------------------

Constant * GeometricProgram::requestConstant( const string &name ) const {
	map<string,Constant*>::const_iterator it = clsConstants.find( name );
	if ( it == clsConstants.end() )
		throw GeometricProgramException( "Constant not found." );
	return it->second;
} // end method

// -----------------------------------------------------------------------------

Variable * GeometricProgram::requestVariable( const string &name ) const {
	map<string,Variable*>::const_iterator it = clsVariables.find( name );
	if ( it == clsVariables.end() )
		throw GeometricProgramException( "Variable not found." );
	return it->second;
} // end method

// -----------------------------------------------------------------------------

Monomial * GeometricProgram::requestMonomial( const string &name ) const {
	map<string,Monomial*>::const_iterator it = clsMonomials.find( name );
	if ( it == clsMonomials.end() )
		throw GeometricProgramException( "Monomial not found." );
	return it->second;
} // end method

// -----------------------------------------------------------------------------

Posynomial * GeometricProgram::requestPosynomial( const string &name ) const {
	map<string,Posynomial*>::const_iterator it = clsPosynomials.find( name );
	if ( it == clsPosynomials.end() )
		throw GeometricProgramException( "Posynomial not found." );
	return it->second;
} // end method

// -----------------------------------------------------------------------------

PosynomialType * GeometricProgram::requestPosynomialType( const string &name ) const {
	map<string,PosynomialType*>::const_iterator it = clsAny.find( name );
	if ( it == clsAny.end() )
		throw GeometricProgramException( "PosynomialType not found." );
	return it->second;
} // end method

// -----------------------------------------------------------------------------

void GeometricProgram::addInequalityConstraint( PosynomialType * left, MonomialType * right ) {
	clsConstraints.push_back( make_pair( left, right ) );
} // end method

// -----------------------------------------------------------------------------

void GeometricProgram::setObjective( PosynomialType * objective ) {
	clsObjective = objective;
} // end method

// -----------------------------------------------------------------------------

void GeometricProgram::ungeneralize() {
	if ( clsObjective )
		clsObjective = clsObjective->ungeneralize(*this);
	
	const int numConstraints = clsConstraints.size();
	for ( int i = 0; i < numConstraints; i++ ) {
		clsConstraints[i].first->ungeneralize(*this);
	} // end if
} // end method

// -----------------------------------------------------------------------------

void GeometricProgram::standardize( StandardGeometricProgram &gp ) {
	gp.setObjective( clsObjective->toStandardPosynomial(gp) );

	const int numConstraints = clsConstraints.size();
	for ( int i = 0; i < numConstraints; i++ ) {
		PosynomialType * posynomial = clsConstraints[i].first;
		MonomialType * monomial = clsConstraints[i].second;

		Div * constraint = createDiv();
		constraint->setNumerator( posynomial );
		constraint->setDenomiator( monomial );
		gp.addInequalityConstraint( constraint->toStandardPosynomial(gp) );
	} // end if
} // end method

// =============================================================================
// Ungeneralize
// =============================================================================

PosynomialType * Max::ungeneralize( GeometricProgram &gp ) {
	for ( int i = 0; i < clsTerms.size(); i++ )
		clsTerms[i] = clsTerms[i]->ungeneralize(gp);

	Variable * variable = gp.createInternalVariable();
	for ( int i = 0; i < clsTerms.size(); i++ )
		gp.addInequalityConstraint( clsTerms[i], variable );

	return variable;
} // end method

// =============================================================================
// Print
// =============================================================================

void Constant::print( ostream &out, const bool unroll ) const {
	if ( !unroll && hasName() )
		out << getName();
	else
		out << clsValue;
} // end method

// -----------------------------------------------------------------------------

void Variable::print( ostream &out, const bool unroll ) const {
	out << getName();
} // end method

// -----------------------------------------------------------------------------

void Monomial::print( ostream &out, const bool unroll ) const {
	if ( !unroll && hasName() ) {
		out << getName();
	} else {
		if ( clsCoefficient ) {
			clsCoefficient->print(out);
			if ( clsTerms.size() > 0 )
				out << "*";
		} // end if

		for ( int i = 0; i < clsTerms.size(); i++ ) {
			if ( i > 0 )
				out << "*";
			printTerm( clsTerms[i], out );
		} // end for
	} // end else
} // end method

// -----------------------------------------------------------------------------

void Posynomial::print( ostream &out, const bool unroll ) const {
	if ( !unroll && hasName() ) {
		out << getName();
	} else {
		for ( int i = 0; i < clsMonomials.size(); i++ ) {
			if ( i > 0 )
				out << " + ";
			clsMonomials[i]->print(out);
		} // end for
	} // end else
} // end method

// -----------------------------------------------------------------------------

void Div::print( ostream &out, const bool unroll ) const {
	if ( !unroll && hasName() ) {
		out << getName();
	} else {
		out << "( ";
		clsNumerator->print(out);
		out << " ) / ( ";
		clsDenominator->print(out);
		out << " )";
	} // end else
} // end method

// -----------------------------------------------------------------------------

void Sum::print( ostream &out, const bool unroll ) const {
	if ( !unroll && hasName() ) {
		out << getName();
	} else {
		for ( int i = 0; i < clsTerms.size(); i++ ) {
			if ( i > 0 )
				out << " + ";
			out << "(";
			clsTerms[i]->print(out);
			out << ")";
		} // end for
	} // end else
} // end method

// -----------------------------------------------------------------------------

void Mul::print( ostream &out, const bool unroll ) const {
	if ( !unroll && hasName() ) {
		out << getName();
	} else {
		for ( int i = 0; i < clsTerms.size(); i++ ) {
			if ( i > 0 )
				out << " * ";
			out << "(";
			clsTerms[i]->print(out);
			out << ")";
		} // end for
	} // end else
} // end method

// -----------------------------------------------------------------------------

void Max::print( ostream &out, const bool unroll ) const {
	if ( !unroll && hasName() ) {
		out << getName();
	} else {
		if ( clsTerms.size() > 1 )
			out << "max( ";

		for ( int i = 0; i < clsTerms.size(); i++ ) {
			if ( i > 0 )
				out << ", ";
			clsTerms[i]->print(out);
		} // end for

		if ( clsTerms.size() > 1 )
			out << " )";
	} // end else
} // end method

// =============================================================================
// Standardize to StandardMonomial
// =============================================================================

StandardMonomial * Constant::toStandardMonomial( StandardGeometricProgram &gp ) const {
	StandardMonomial * monomial = gp.createMonomial();
	monomial->setCoefficient( clsValue );
	return monomial;
} // end method

// -----------------------------------------------------------------------------

StandardMonomial * Variable::toStandardMonomial( StandardGeometricProgram &gp ) const {
	StandardMonomial * monomial = gp.createMonomial();
	monomial->setCoefficient( 1 );
	monomial->addTerm( getName(), 1 );
	return monomial;
} // end method

// -----------------------------------------------------------------------------

StandardMonomial * Monomial::toStandardMonomial( StandardGeometricProgram &gp ) const {
	StandardMonomial * monomial = gp.createMonomial();
	monomial->setCoefficient( clsCoefficient->getValue() );
	for ( int i = 0; i < clsTerms.size(); i++ ) {
		const Term &term = clsTerms[i];
		monomial->addTerm( term.propVariable->getName(), term.propExpoent );
	} // end for
	return monomial;
} // end method

// =============================================================================
// Standardize to StandardPosynomial
// =============================================================================

StandardPosynomial * MonomialType::toStandardPosynomial( StandardGeometricProgram &gp ) const {
	StandardPosynomial * posynomial = gp.createPosynomial();
	posynomial->addTerm( toStandardMonomial(gp) );
	return posynomial;
} // end method

// -----------------------------------------------------------------------------

StandardPosynomial * Posynomial::toStandardPosynomial( StandardGeometricProgram &gp ) const {
	StandardPosynomial * posynomial = gp.createPosynomial();
	for ( int i = 0; i < clsMonomials.size(); i++ )
		posynomial->addTerm( clsMonomials[i]->toStandardMonomial( gp ) );
	return posynomial;
} // end method

// -----------------------------------------------------------------------------

StandardPosynomial * Sum::toStandardPosynomial( StandardGeometricProgram &gp ) const {
	StandardPosynomial * posynomial = gp.createPosynomial();
	for ( int i = 0; i < clsTerms.size(); i++ )
		posynomial->composeBySum( clsTerms[i]->toStandardPosynomial( gp ) );
	return posynomial;
} // end method

// -----------------------------------------------------------------------------

StandardPosynomial * Mul::toStandardPosynomial( StandardGeometricProgram &gp ) const {
	const int numTerms = getNumTerms();
	
	if ( numTerms == 0 )
		throw GeometricProgramException( "Empty mul!" );

	StandardPosynomial * posynomial = clsTerms[0]->toStandardPosynomial( gp ) ;
	for ( int i = 1; i < numTerms; i++ )
		posynomial = gp.composeByMul( posynomial, clsTerms[i]->toStandardPosynomial( gp ) );
	return posynomial;
} // end method

// -----------------------------------------------------------------------------

StandardPosynomial * Div::toStandardPosynomial( StandardGeometricProgram &gp ) const {
	return gp.composeByDiv(
		clsNumerator->toStandardPosynomial(gp),
		clsDenominator->toStandardMonomial(gp) );
} // end method

// -----------------------------------------------------------------------------

StandardPosynomial * Max::toStandardPosynomial( StandardGeometricProgram &gp ) const {
	throw GeometricProgramException( "Max could not be translated into a StandardPosynomial." );
} // end method

// =============================================================================
// Standard eometric Program Descriptor
// =============================================================================

void StandardGeometricProgram::setObjective( StandardPosynomial * objective ) {
	clsObjective = objective;
} // end method

// -----------------------------------------------------------------------------

void StandardGeometricProgram::addInequalityConstraint( StandardPosynomial * left ) {
	clsConstraints.push_back( left );
} // end method

// -----------------------------------------------------------------------------

StandardPosynomial * StandardGeometricProgram::createPosynomial() {
	StandardPosynomial * element = new StandardPosynomial;
	clsElements.push_back( element );
	return element;
} // end method

// -----------------------------------------------------------------------------

StandardMonomial * StandardGeometricProgram::createMonomial() {
	StandardMonomial * element = new StandardMonomial;
	clsElements.push_back( element );
	return element;
} // end method

// -----------------------------------------------------------------------------

void StandardGeometricProgram::print( ostream &out ) const {
	out << "minimize ";
	clsObjective->print(out);
	out << "\n";
	out << "subject to\n";

	const int numConstraints = clsConstraints.size();
	for ( int i = 0; i < numConstraints; i++ ) {
		out << "\t";
		clsConstraints[i]->print(out);
		out << " <= 1\n";
	} // end if

} // end method