/*
 * File:   InfixToPostfixConverter.h
 * Author: ceyhun
 *
 * Created on April 13, 2011, 8:55 PM
 */

#ifndef INFIXTOPOSTFIXCONVERTER_H
#define	INFIXTOPOSTFIXCONVERTER_H

#include <string>
#include <cstdlib>
#include <cstring>
#include <iostream>

using namespace std;

class InfixToPostfixConverter {

public:

    InfixToPostfixConverter(string);
    inline string getPosfixExpression() { return postfix; }
    string getPosfixExpression(bool*);

private:
    bool success = true;
    string error;
    string regex;
    char c;
    int cc;
    string postfix;

    void E();
    void T();
    void F();
    void G();
    void fatal(const char*);
};


void testInfixToPostfix();

#endif	/* INFIXTOPOSTFIXCONVERTER_H */
