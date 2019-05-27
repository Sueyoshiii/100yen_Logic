#include "Wolf.h"

Wolf::Wolf(std::weak_ptr<MyLib> lib)
{
	this->lib = lib;

}

Wolf::~Wolf()
{
}
