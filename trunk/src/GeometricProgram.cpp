#include <sstream>
	using std::ostringstream;
#include <set>
	using std::set;

#include "GeometricProgram.h"

// [TODO] Criar templete pra createXXX();
// [TODO] Verificar se nome já existe.
// [TODO] Verificar se os nomes são válidos. Reserver os iniando com underline
//        para variáveis internas.

// -----------------------------------------------------------------------------

GeometricProgram::GeometricProgram() {
	clsObjective = NULL;
	clsInternalVariableCounter = 0;
} // end constructor

// -----------------------------------------------------------------------------

GeometricProgram::~GeometricProgram() {
	for ( int i = 0; i < clsElements.size(); i++ )
		delete clsElements[i];
} // end destructor

// -----------------------------------------------------------------------------

Variable * GeometricProgram::createInternalVariable() {
	ostringstream oss;
	oss << "var" << clsInternalVariableCounter;
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

ConstantSum * GeometricProgram::createConstantSum( const string &name ) {
	ConstantSum * element = new ConstantSum( name );
	registerPosynomialType( element );
	return element;
} // end method

// -----------------------------------------------------------------------------

ConstantMul * GeometricProgram::createConstantMul( const string &name ) {
	ConstantMul * element = new ConstantMul( name );
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

Monomial * GeometricProgram::createMonomial( const string &name, ConstantType * coefficient, Variable * variable, double expoent ) {
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

ConstantSum * GeometricProgram::createConstantSum( const string &name, ConstantType *op1, ConstantType * op2 ) {
	ConstantSum * element = new ConstantSum( name );
	element->addTerm( op1 );
	element->addTerm( op2 );

	registerPosynomialType( element );
	return element;
} // end method

// -----------------------------------------------------------------------------

ConstantMul * GeometricProgram::createConstantMul( const string &name, ConstantType *op1, ConstantType * op2 ) {
	ConstantMul * element = new ConstantMul( name );
	element->addTerm( op1 );
	element->addTerm( op2 );

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
	element->setDenomiator( denominator );

	registerPosynomialType( element );
	return element;
} // end method

// -----------------------------------------------------------------------------

void GeometricProgram::print( ostream &out ) const {
	bool printConstraints = false;
	// Print variables.
	out << "gpvar";
	for ( map<string,Variable*>::const_iterator it = clsVariables.begin(); it != clsVariables.end(); it++ )
		out << " " << it->second->getName();
	out << ";\n\n";

	// Print assignments.
	const int numElements = clsElements.size();
	for ( int i = 0; i < numElements; i++ ) {
		const PosynomialType * e = clsElements[i];

		// Skip variables.
		if ( typeid(*e) == typeid( Variable ) )
			continue;

		// Print the element.
		if ( e->getName() != "" ) {
			out << e->getName() << " = ";
			e->print( out, true );
			out << ";\n";
		} // end if
	} // end for

	out << "\n";
	
	
	// Print constraints.
	if (printConstraints) {
		out << "constr = [";

		const int numConstraints = clsConstraints.size();
		for ( int i = 0; i < numConstraints; i++ ) {
			PosynomialType * posynomial = clsConstraints[i].first;
			MonomialType * monomial = clsConstraints[i].second;

			out << "\n\t";
			posynomial->print(out);
			out << " <= ";
			monomial->print(out);
			out << ";";
		} // end if
		out << "\n];\n\n";

		// Print objective.
		if ( clsObjective ) {
			out << "[ result, solution, status ] = gpsolve( ";
			clsObjective->print(out);
			out << ", constr, 'min' )\n";
		} else {
			out << "[WARNING] Objective function was not set.\n";
		} // end else
	} // end if
	out << std::flush;
} // end method

// -----------------------------------------------------------------------------

Constant * GeometricProgram::createConstant( const double value ) { return createConstant( "", value ); }
ConstantSum * GeometricProgram::createConstantSum() { return createConstantSum( "" ); }
ConstantMul * GeometricProgram::createConstantMul() { return createConstantMul( "" ); }
Monomial * GeometricProgram::createMonomial() { return createMonomial(""); }
Posynomial * GeometricProgram::createPosynomial() { return createPosynomial(""); }

Sum * GeometricProgram::createSum() { return createSum(""); }
Mul * GeometricProgram::createMul() { return createMul(""); }
Div * GeometricProgram::createDiv() { return createDiv(""); }
Max * GeometricProgram::createMax() { return createMax(""); }

Monomial * GeometricProgram::createMonomial( ConstantType * coefficient, Variable * variable, double expoent ) { return createMonomial( coefficient, variable, expoent ); }


ConstantSum * GeometricProgram::createConstantSum( ConstantType *op1, ConstantType * op2 ) { return createConstantSum( "", op1, op2 ); }
ConstantMul * GeometricProgram::createConstantMul( ConstantType *op1, ConstantType * op2 ) { return createConstantMul( "", op1, op2 ); }

Sum * GeometricProgram::createSum( PosynomialType *op1, PosynomialType * op2 ) { return createSum( "", op1, op2 ); }
Mul * GeometricProgram::createMul( PosynomialType *op1, PosynomialType * op2 ) { return createMul( "", op1, op2 ); }
Max * GeometricProgram::createMax( PosynomialType *op1, PosynomialType * op2 ) { return createMax( "", op1, op2 ); }

Div * GeometricProgram::createDiv( PosynomialType * numerator, MonomialType * denominator ) { return createDiv( "", numerator, denominator ); }

// -----------------------------------------------------------------------------

ConstantType * GeometricProgram::requestConstantType( const string &name ) const {
	map<string,ConstantType*>::const_iterator it = clsConstantTypes.find( name );
	if ( it == clsConstantTypes.end() )
		throw GeometricProgramException( "ConstantType '" + name + "' not found." );
	return it->second;
} // end method

// -----------------------------------------------------------------------------

Variable * GeometricProgram::requestVariable( const string &name ) const {
	map<string,Variable*>::const_iterator it = clsVariables.find( name );
	if ( it == clsVariables.end() )
		throw GeometricProgramException( "Variable '" + name + "' not found." );
	return it->second;
} // end method

// -----------------------------------------------------------------------------

Monomial * GeometricProgram::requestMonomial( const string &name ) const {
	map<string,Monomial*>::const_iterator it = clsMonomials.find( name );
	if ( it == clsMonomials.end() )
		throw GeometricProgramException( "Monomial '" + name + "' not found." );
	return it->second;
} // end method

// -----------------------------------------------------------------------------

Posynomial * GeometricProgram::requestPosynomial( const string &name ) const {
	map<string,Posynomial*>::const_iterator it = clsPosynomials.find( name );
	if ( it == clsPosynomials.end() )
		throw GeometricProgramException( "Posynomial '" + name + "' not found." );
	return it->second;
} // end method

// -----------------------------------------------------------------------------

PosynomialType * GeometricProgram::requestPosynomialType( const string &name ) const {
	map<string,PosynomialType*>::const_iterator it = clsAny.find( name );
	if ( it == clsAny.end() )
		throw GeometricProgramException( "PosynomialType '" + name + "' not found." );
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

	// [ADIVICE] Since the constraint vector may be modifieid by addition of
	// new constraints, it may change its internal pointer to element array
	// making a reference to an element becomes invalid.

	const int originalNumConstraints = clsConstraints.size();

	vector<PosynomialType *> ungeneralizedConstraints( originalNumConstraints, (PosynomialType * ) NULL );

	for ( int i = 0; i < originalNumConstraints; i++ )
		ungeneralizedConstraints[i] = clsConstraints[i].first->ungeneralize(*this);

	for ( int i = 0; i < originalNumConstraints; i++ )
		if ( ungeneralizedConstraints[i] )
			clsConstraints[i].first = ungeneralizedConstraints[i];
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

// -----------------------------------------------------------------------------

void GeometricProgram::assignValue( const string &variable, const double value ) {
	Variable * var = requestVariable( variable );
	var->setValue( value );
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

void ConstantSum::print( ostream &out, const bool unroll ) const {
	if ( !unroll && hasName() ) {
		out << getName();
	} else {
		for ( int i = 0; i < clsTerms.size(); i++ ) {
			if ( i > 0 )
				out << " + ";
			clsTerms[i]->print(out);
		} // end for
	} // end else
} // end method

// -----------------------------------------------------------------------------

void ConstantMul::print( ostream &out, const bool unroll ) const {
	if ( !unroll && hasName() ) {
		out << getName();
	} else {
		for ( int i = 0; i < clsTerms.size(); i++ ) {
			if ( i > 0 )
				out << " * ";
			clsTerms[i]->print(out);
		} // end for
	} // end else
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
		for ( int i = 0; i < clsTerms.size(); i++ ) {
			if ( i > 0 )
				out << " + ";
			clsTerms[i]->print(out);
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

StandardMonomial * ConstantType::toStandardMonomial( StandardGeometricProgram &gp ) const {
	StandardMonomial * monomial = gp.createMonomial();
	monomial->setCoefficient( computeValue() );
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
	monomial->setCoefficient( clsCoefficient->computeValue() );
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
	for ( int i = 0; i < clsTerms.size(); i++ )
		posynomial->addTerm( clsTerms[i]->toStandardMonomial( gp ) );
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
	throw GeometricProgramException( "Max could not be translated into a StandardPosynomial. Call ungeneralize()." );
} // end method

// =============================================================================
// Standard Geometric Program Descriptor
// =============================================================================

StandardGeometricProgram::~StandardGeometricProgram() {
	for ( int i = 0; i < clsElements.size(); i++ )
		delete clsElements[i];
} // end destructor

// -----------------------------------------------------------------------------

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
	const int numConstraints = clsConstraints.size();

	// Print variables.
	set<string> variables;
	for ( int i = 0; i < numConstraints; i++ ) {
		StandardPosynomial * posy = clsConstraints[i];

		const int numMonomials = posy->getNumMonomials();
		for ( int j = 0; j < numMonomials; j++ ) {
			StandardMonomial * monomial = posy->getMonomial(j);

			const int numTerms = monomial->getNumTerms();
			for ( int k = 0; k < numTerms; k++ )
				variables.insert( monomial->getTerm(k).propVariable );
		} // end for
	} // end for

	out << "gpvar";
	for ( set<string>::const_iterator it = variables.begin(); it != variables.end(); it++ )
		out << " " << *it;
	out << ";\n\n";

	// Print constraints.
	out << "constr = [";
	for ( int i = 0; i < numConstraints; i++ ) {
		out << "\n\t";
		clsConstraints[i]->print(out);
		out << " <= 1;";
	} // end if
	out << "\n];\n\n";

	// Print objective.
	if ( clsObjective ) {
		out << "[ result, solution, status ] = gpsolve( ";
		clsObjective->print(out);
		out << ", constr, 'min' )\n\n";
	} else {
		out << "[WARNING] Objective function was not set.\n";
	} // end else

	out << std::flush;
} // end method