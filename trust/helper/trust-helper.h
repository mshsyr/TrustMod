/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
//
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation;
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// Author: Muhammad Shadi Hajar  <m.hajar@rgu.ac.uk> <msh.syr@gmail.com>
//

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
	
	
	//Ptr<Trust> Install (Ptr<Node> node);
	//Ptr<Trust> Install (NodeContainer nodes);
	//Ptr<Trust> InstallAll ();
	//Ptr<Trust> GetTrust ();
	//void Serialize ();

private:
	//TrustHelper (const TrustHelper&);
	//TrustHelper& operator= (const TrustHelper&);
	Ptr<Trust> InstallPriv (Ptr<Node> node) const;
	ObjectFactory m_trustFactory; 
	//Ptr<Trust> m_trust;
	
};


}

#endif /* TRUST_HELPER_H */

