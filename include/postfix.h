﻿#pragma once

#include <string>
#include <vector>
//#include "TStack.h"
#include <stack>

#include <functional>
#include <unordered_map>
#include <sstream>
#include <memory>
#include <iostream>
#include <stdexcept>

#include"tableman.h"
#include"polynom.h"

using namespace std;


// Как сделать переменные-полиномы: хранить как значения переменных в мапе указатели на полиномы? (или лучше значения для удобства?) 
// и при добавлении переменных в базу искать через методы tablemanager полином в таблицах чтобы присвоить его
// Ещё и выполнять действия с миксом полином/число придётся как-то...

enum class LexemeType { Undef, Operand, Operator , ParOpen, ParClose };

class Lexeme
{
	
protected:
	bool defined = false;
	LexemeType type = LexemeType::Undef;
public:
	Lexeme() { throw logic_error("Lexem default constructor should not be called. Invalid container usage."); } // Никогда не должен вызываться, может вызваться при неправильном использовании контейнеров и требуется для сборки

	Lexeme( bool defined) : defined(defined) {};
	explicit Lexeme( LexemeType type, bool defined) : type(type), defined(defined) {};
	LexemeType getType() { return type; }
	virtual int getPriority() const { return -1; } // Приоритет скобок -1
	virtual void define() { defined = true; //cout << defined << " defined " << name << endl; 
	}
	bool isDefined() { return defined; }
	virtual ~Lexeme() {};
	friend ostream& operator<<(ostream&, Lexeme&);
};

void printLex(Lexeme*);

ostream& operator<<(ostream& out, Lexeme& lex)
{
	//cout << " <<" << lex.name << endl;
	return out << "LEX("<<int(lex.type)<<")";
}

//////////////////////////////////////////
//template <class T>
//class Variable : public Lexeme
//{
//	T value;
//public:
//	Variable(string name) : Lexeme(name, LexemeType::var, false) {}
//	void define(T& value_) { value = value_; defined = true; }
//	T& getValue() { return value; }
//	//T* getValuePointer() { return &value; }
//};

enum class OperandType {Number, Polynom};

class Operand : public Lexeme
{
public:
	Operand(bool defined) :Lexeme( LexemeType::Operand, defined) {};
	virtual OperandType getType() const  = 0;
	virtual ~Operand() = default;
};


class Number : public Operand {
	double value;
public:
	Number(double val) : Operand(true), value(val) {};
	OperandType getType() const override { return OperandType::Number; }
	double getValue() const { return value; }
	//void define(double val) { value = val; }
};

class PolynomialRef : public Operand {
	Polynom* polyP;
public:
	PolynomialRef(Polynom* polynomPointer) : Operand(true), polyP(polynomPointer) {};
	OperandType getType() const override { return OperandType::Polynom; }
	Polynom& getPolynom() const {
		return *polyP;
	}
	//void define(Polynom* polynomPointer) { polyP = polynomPointer; }
};

//////////////////////////////////////////
enum class Associativity { Left, Right };
enum class OperatorType{Add, Sub, Mul, Div};

class Operator : public Lexeme
{
	OperatorType func;
	int argCount;
	int priority;
	Associativity associativity;


public:
	explicit Operator(int argCount_, int priority_, OperatorType optorType_, Associativity associativity_ = Associativity::Left)
		: Lexeme(LexemeType::Operator, true), argCount(argCount_), priority(priority_), func(optorType_), associativity(associativity_)
	{
		if (argCount == 1)
			priority = 255;
		//cout << "Operator " << name_ << " created with priority " << priority_ << " and associativity " << (associativity_ == Associativity::Left ? "Left" : "Right") << endl;
	}

	int getPriority() const override { return priority; }
	int getArgCount() const { return argCount; }
	OperatorType getOperatorType() { return func; }
	Associativity getAssociativity() const { return associativity; }

	//Number* ExecNum(Number* op1, Number* op2)
	//{

	//}

	shared_ptr<Operand> Execute(shared_ptr<Operand> a, shared_ptr<Operand> b = 0) const {
		//cout << "EXEC ";printLex(a.get());printLex(b.get());
		if (a == 0)
			throw runtime_error("No arguments");
		auto numA = dynamic_pointer_cast<Number>(a);
		if(argCount == 2)
		{
			if (b == 0)
				throw runtime_error("Not enough arguments");
			auto numB = dynamic_pointer_cast<Number>(b);

			if (func == OperatorType::Add)
			{
				return make_shared<Number>(numA->getValue() + numB->getValue());
			}
			if (func == OperatorType::Sub)
			{
				return make_shared<Number>(numA->getValue() - numB->getValue());
			}
			if (func == OperatorType::Mul)
			{
				return make_shared<Number>(numA->getValue() * numB->getValue());
			}
		}

		

		return nullptr;
	}
};

void printLex(shared_ptr<Lexeme> l)
{
	//cout << l<<"("<<l.use_count()<<")" << "_";
	if (l == nullptr) { cout << "NULLLEX";return; }
	LexemeType lt = l->getType();
	if (lt == LexemeType::Undef)
		cout << "[?]";
	else if (lt == LexemeType::Operand)
	{
		auto od = dynamic_pointer_cast<Operand>(l);
		if (auto nu = dynamic_pointer_cast<Number> (od))
			cout << "{" << nu->getValue() << "}";
	}
	else if (lt == LexemeType::Operator)
	{
		auto op = dynamic_pointer_cast<Operator>(l);
		cout << "[OP" << static_cast<int>(op->getOperatorType()) << "]";
	}
	else if (lt == LexemeType::ParOpen)
	{
		cout << " ( ";
	}
	else if (lt == LexemeType::ParClose)
	{
		cout << " ) ";
	}

	else { cout << "??? "; }
}

////
/////////////////////////////////////////
///


class LexBase
{
	unordered_map<string, shared_ptr<Lexeme>> map;
	//TableMANAGER!!!!
	TableManager* tableman;

public:
	LexBase() = default;

	~LexBase() {
		//for (auto& pair : map) {
		//	delete pair.second;
		//}
	}

	void addNum(string name, double value)
	{
		auto it = map.find(name);
		if (it == map.end())
		{
			
			map.emplace(name, make_shared<Number>(value));
			//cout << "ADDED VAR :" << nvar->getName() << ":" << endl;
			return;
		}
		throw "ADD existing lexeme";
	}

	void addPoly(string name)
	{
		auto* it = tableman->find(name);
		if (it != nullptr)
		{
			map.emplace(name, make_shared<PolynomialRef>(it));
			
			return;
		}
		throw "cant find Polynom";
	}

	void addOperator(string name, int argCount, int priority,OperatorType optorType, Associativity associativity = Associativity::Left) {
		map.emplace(name, make_shared<Operator>(argCount,priority,optorType, associativity));
		//cout << "ADDED OPERATOR: " << nOp->getName() << " with priority " << nOp->getPriority() << " and associativity " << (associativity == Associativity::Left ? "Left" : "Right") << endl;
	}

	shared_ptr<Lexeme> getLexeme(string name)
	{
		auto it = map.find(name);
		if (it != map.end())
			return it->second;
		return nullptr;
	}

	void addLexeme(string name, LexemeType type, bool defined = false) { //Только для вспомогательных лексем без функционала
		map.emplace(name, make_shared<Lexeme>(type, defined));
		//cout << "ADDED LEX :" << lex.getName() << ":" << endl;
	}

	//vector<pair<string, Lexeme*>> getAllLexemes() {
	//	vector<pair<string, Lexeme*>> lexemes;
	//	for (const auto& pair : map) {
	//		lexemes.emplace_back(pair.first, pair.second);
	//	}
	//	return lexemes;
	//}
};


///////////////////////////////////////


class Postfix
{
	LexBase base;
	string infix;
	vector<shared_ptr<Lexeme>> postfix;
	stack<shared_ptr<Lexeme>> opStack;
	shared_ptr<Operand> result;

public:

	Postfix(bool importBasicOperators = true)
	{

		if (importBasicOperators) {
			base.addOperator("+", 2, 0, OperatorType::Add);
			base.addOperator("-", 2, 0, OperatorType::Sub);
			base.addOperator("*", 2, 1, OperatorType::Mul);
			base.addOperator("/", 2, 1, OperatorType::Div);

			base.addLexeme("(", LexemeType::ParOpen, 1);
			base.addLexeme(")", LexemeType::ParClose, 1);
		}
	}

	void inputInfix(string infix_)
	{
		infix = infix_;
	}


	void parseToPostfix() {

		if (infix.empty())
			throw logic_error("Trying to parse empty infix");

		istringstream iss(infix);
		string token;

		while (iss >> token) {
			shared_ptr<Lexeme> lex = base.getLexeme(token);
			//cout << "Got token " << token << endl;

			if (lex == nullptr) {
				// Если лексема не найдена, добавляем как новую переменную

				if (token.find_first_of("()") != string::npos)
					throw runtime_error("Variable name or number fused with parentheses");

				// Попытаемся сразу определить если это число
				try {
					double num = stod(token);
					if (!isnan(num))
					{
						postfix.push_back(make_shared<Number>(num));
						//cout << "CAN NUM " << num << endl;
					}
					else
					{
						base.addPoly(token);
					}
				}
				catch (exception e) {}


				//cout << "New var " << endl;
			}
			else {
				// Вывод информации для отладки
				//cout << "Found lexeme: " << lex->getName() << " Type: " << lex->getType() << " Priority: " << lex->getPriority() << endl;
				LexemeType type = lex->getType();

				if (type == LexemeType::Operand) {
					// Если лексема — переменная, добавляем её в постфиксное выражение
					postfix.push_back(lex);
				}
				else if (type == LexemeType::Operator) {
					// Если это оператор, обрабатываем приоритеты и ассоциативность
					auto op = dynamic_pointer_cast<Operator>(lex);
					while (!opStack.empty() && opStack.top()->getType() == LexemeType::Operator) {
						auto topOp = dynamic_pointer_cast<Operator>(opStack.top());

						// Проверка на приоритет и ассоциативность
						bool shouldPop;
						if (op->getAssociativity() == Associativity::Left) {
							shouldPop = topOp->getPriority() >= op->getPriority();
						}
						else { // Правоассоциативный оператор
							shouldPop = topOp->getPriority() > op->getPriority();
						}

						if (shouldPop) {
							postfix.push_back(opStack.top());
							opStack.pop();
						}
						else {
							break;
						}
					}
					opStack.push(lex); // Добавляем текущий оператор в стек
				}
				else if (type == LexemeType::ParOpen) {
					// Если открывающая скобка, добавляем её в стек операторов
					opStack.push(lex);
				}
				else if (type == LexemeType::ParClose) {
					// Если закрывающая скобка, выталкиваем операторы в постфикс до открывающей скобки
					while (!opStack.empty() && opStack.top()->getType() != LexemeType::ParOpen) {
						postfix.push_back(opStack.top());
						opStack.pop();
					}
					if (!opStack.empty() && opStack.top()->getType() == LexemeType::ParOpen) {
						opStack.pop(); // Убираем открывающую скобку из стека
					}
				}
			}

			//cout << "It was token " << token << endl;
			//cout << "Stack size: " << opStack.get_size() << endl;
			//cout << "Postfix size: " << postfix.size() << endl;
		}

		// Перемещаем оставшиеся операторы в постфикс
		while (!opStack.empty()) {
			if (opStack.top()->getType() == LexemeType::ParOpen) {
				opStack.pop(); // Удаляем открывающую скобку, если она осталась
			}
			else {
				postfix.push_back(opStack.top());
				//cout << "END Popping " << opStack.top()->getName() << " stack size " << endl;
				opStack.pop();
			}
		}
		//cout << "PARSING END\n";
	}




	string getInfix() { return infix; }
	string GetPostfix()
	{
		if (!postfix.size())
			throw logic_error("Get empty postfix");
		string res;
		for (int i = 0; i < postfix.size(); i++) {
			//cout << "res " << postfix[i]->getName() << endl;
			res += to_string(((int)postfix[i]->getType()))+' ';
		}
		return res;
	}

	void addOperator(string name, int argCount, int priority, OperatorType optorType, Associativity associativity = Associativity::Left) {
		base.addOperator(name, argCount, priority, optorType, associativity);
	}

	//Функционал с неопределёнными переменными и доопределением пока не поддерживатеся
	
	//void setVariable(const string& name, T value) {
	//	Lexeme* lex = base.getLexeme(name);
	//	if (lex && lex->getType() == LexemeType::var) {
	//		dynamic_cast<Variable<T>*>(lex)->define(value);
	//	}
	//	else {
	//		//cout << "Variable not found: " << name << endl;
	//		throw logic_error("Variable not found: " + name);
	//	}
	//}
	//
	//void setVariables(const vector<pair<string, T>>& vars) {
	//	for (const auto& var : vars) {
	//		setVariable(var.first, var.second);
	//	}
	//}
	//
	//vector<string> getUndefinedVars() {
	//	vector<string> undefinedVars;
	//	for (const auto& lexemePair : base.getAllLexemes()) {
	//		Lexeme* lexeme = lexemePair.second;
	//		if (lexeme->getType() == LexemeType::var) {
	//			Variable<T>* var = static_cast<Variable<T>*>(lexeme);
	//			if (!var->isDefined()) {
	//				undefinedVars.push_back(var->getName());
	//			}
	//		}
	//	}
	//	return undefinedVars;
	//}
	//
	//vector<string> getOperatorNames() {
	//	vector<string > ops;
	//	for (const auto& lexemePair : base.getAllLexemes()) {
	//		Lexeme* lexeme = lexemePair.second;
	//		if (lexeme->getType() == LexemeType::op) {
	//
	//			ops.push_back(lexeme->getName());
	//
	//		}
	//	}
	//	return ops;
	//}
	//
	//bool checkForUndefinedVars()
	//{
	//	for (const auto& lexemePair : base.getAllLexemes()) {
	//		Lexeme* lexeme = lexemePair.second;
	//		if (lexeme->getType() == LexemeType::var) {
	//			//Variable<T>* var = static_cast<Variable<T>*>(lexeme);
	//			if (!lexeme->isDefined()) {
	//				//cout << "ud "<< lexeme->getName() << endl;
	//				return true;
	//			}
	//		}
	//	}
	//	return false;
	//}

	shared_ptr<Operand> Calculate()
	{
#ifdef CALCPRINT
		int cc = 0;
		cout << "CALCULATING\n";
#endif
		/*if (checkForUndefinedVars())
			throw logic_error("Calculating when some variables are not defined");*/
		if (!postfix.size())
			throw logic_error("Calculate when no postfix");

		

		stack<shared_ptr<Operand>> calcStack;

		for (auto lexeme : postfix) {
#ifdef CALCPRINT
			printLex(lexeme);
			cout << "\t"; cc++;
#endif
			if (lexeme->getType() == LexemeType::Operand) {
				// Если лексема — переменная, добавляем её значение в стек
				calcStack.push(dynamic_pointer_cast<Operand>(lexeme));
				//cout << "|";printLex(calcStack.top());cout << "|";
			}
			else if (lexeme->getType() == LexemeType::Operator) {
				auto op = dynamic_pointer_cast<Operator>(lexeme);
				int argCount = op->getArgCount();

				if (argCount == 1) {
					// Для унарного оператора извлекаем один операнд
					auto operand = calcStack.top(); calcStack.pop();
					calcStack.push(op->Execute(operand));
				}
				else if (argCount == 2) {
					// Для бинарного оператора извлекаем два операнда
					auto op2 = calcStack.top(); calcStack.pop();
					auto op1 = calcStack.top(); calcStack.pop();

#ifdef CALCPRINT



					cout << endl;
					cc -= 2;
					for (int k = 0;k < cc - 1;k++) { cout << "\t"; }
					cout << "<";
					printLex(op);printLex(op1);printLex(op2);
					cout << ">\t";

#endif // CALCPRINT

					// Выполняем операцию и помещаем результат в стек
					calcStack.push(op->Execute(op1, op2));
				}
				else {
					throw logic_error("Operator arguments count not supported");
				}
			}
		}
		result = calcStack.top();
		//cout << "\nCALC END\n";
		return result;
	}

	double getNumberResult()
	{
		Number* n = dynamic_cast<Number*>(result.get());
		if (!n)
			throw runtime_error("Getting Polynom result as Number");
		return n->getValue();
	}

	~Postfix()
	{
		//cout << "POSTFIX DELETING\n";

	//	for (auto& ptr : postfix) {
	//		ptr.reset();
	//	}
	//	postfix.clear();
	}
};

