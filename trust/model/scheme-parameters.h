#ifndef SCHEME_PARAMETERS_H
#define SCHEME_PARAMETERS_H

#include "ns3/socket.h"
#include <stdint.h>

namespace ns3
{

class SchemeParameters
{
public:
	double m_alpha;  
	double m_beta; 

	//LTMS Parameters
	double b;
	double d;


		

	SchemeParameters ();
	virtual ~SchemeParameters ();

	
	
};

}
#endif 