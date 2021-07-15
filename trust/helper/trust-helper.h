/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef TRUST_HELPER_H
#define TRUST_HELPER_H

#include "ns3/trust.h"
#include "ns3/node-container.h"
#include "ns3/object-factory.h"
#include "ns3/application-container.h"
#include <string>

namespace ns3 {

//class AttributeValue;
/* ... */
class TrustHelper
{
public:
	TrustHelper ();
	~TrustHelper ();
	
	void SetTrustAttribute (std::string nl, const AttributeValue &vl);
	
	ApplicationContainer Install (NodeContainer c) const;
	ApplicationContainer Install (Ptr<Node> node) const;
	ApplicationContainer Install (std::string nodeName) const;
	
	

private:

	Ptr<Trust> InstallPriv (Ptr<Node> node) const;
	ObjectFactory m_trustFactory; 

	
};


}

#endif /* TRUST_HELPER_H */

