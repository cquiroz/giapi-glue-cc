#include "KvPair.h"
#include <giapi/giapi.h>

namespace giapi {

KvPair::KvPair(const char * name) {
	_name = name;
	_buff = NULL;
}

KvPair::~KvPair() {
}

int KvPair::setValueAsInt(int value) {
	_value = value;
	return status::OK;
}

int KvPair::setValueAsString(const char * value) {

	//store pointer to the previous buffer
	char *oldBuff = _buff;

	//maybe this can be optimized by pre-allocating a buffer, and only
	//making it bigger when needed?
	_buff = new char[strlen(value) + 1];
	strcpy(_buff, value);

	_value = (const char *)_buff;

	//now removes the old buffer, now that the value has been changed
	if (oldBuff != NULL) {
		delete[] oldBuff;
	}

	return status::OK;
}

int KvPair::setValueAsDouble(double value) {
	_value = value;
	return status::OK;
}

int KvPair::getValueAsInt() const {
	return boost::any_cast<int>(_value);
}

const char * KvPair::getValueAsString() const {
	return boost::any_cast<const char *>(_value);
}

double KvPair::getValueAsDouble() const {
	return boost::any_cast<double>(_value);
}

const char * KvPair::getName() const {
	return _name;
}

const std::type_info& KvPair::getType() const {
	return _value.type();
}

std::ostream& operator<< (std::ostream& os, const KvPair& pair) {

	os << "[name = " << pair.getName() << ", value = ";
	const std::type_info& typeInfo = pair.getType();

	if (typeInfo == typeid(int)) {
		os << pair.getValueAsInt();
	}
	if (typeInfo == typeid(const char *)) {
		os << pair.getValueAsString();
	}
	if (typeInfo == typeid(double)) {
		os << pair.getValueAsDouble();
	}
	if (typeInfo == typeid(void)) {
		os << "void";
	}

	os << "]";

	return os;
}

}