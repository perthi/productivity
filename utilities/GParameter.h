#ifndef GPARAMETER_H
#define GPARAMETER_H

#include <string>

class GParameter
{
public:
	GParameter(std::string name, std::string value) { fName = name, fValue = value; }
	std::string GetName() { return(fName); }
	std::string GetValue() { return(fValue); }
private:
	std::string fName;
	std::string fValue;
};

#endif // !GPARAMETER_H
