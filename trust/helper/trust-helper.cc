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


#include "trust-helper.h"
#include "ns3/string.h"
#include "ns3/names.h"
//#include "ns3/trust.h"
//#include "ns3/node.h"
//#include "ns3/ipv4-l3-protocol.h"
//#include "ns3/node-list.h"



namespace ns3 {

/* ... */
TrustHelper::TrustHelper ()
{
	m_trustFactory.SetTypeId ("ns3::Trust");
}

//TrustHelper::~TrustHelper ()
//{
//	if (m_trust)
//	{
//		m_trust->Dispose ();
//		m_trust = 0;
//	}
//}

TrustHelper::~TrustHelper (){}
void TrustHelper::SetTrustAttribute (std::string name, const AttributeValue &value)
{
  m_trustFactory.Set (name, value);
}
		


ApplicationContainer 
TrustHelper::Install (Ptr<Node> node) const
{
	return ApplicationContainer (InstallPriv (node));
}

ApplicationContainer
TrustHelper::Install (std::string nodeName) const
{
  Ptr<Node> node = Names::Find<Node> (nodeName);
  return ApplicationContainer (InstallPriv (node));
}

ApplicationContainer
TrustHelper::Install (NodeContainer c) const
{
  ApplicationContainer apps;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      apps.Add (InstallPriv (*i));
    }

  return apps;
}

Ptr<Trust>
TrustHelper::InstallPriv (Ptr<Node> node) const
{
  Ptr<Trust> trust = m_trustFactory.Create<Trust> ();
  trust->SetNode(node);
  node->AggregateObject (trust);
  //node->AddApplication (app);
  std::cout<<"The Trust pointer is "<<trust<<" for node ID: "<<node->GetId ()<<std::endl;
  
  //Serialization Call

  Simulator::Schedule (Seconds(trust->GetSimulationTime()+1), &Trust::SerializeToAsciiFile, trust);
  Simulator::Schedule (Seconds(1), &Trust::SetNodeId, trust);
  Simulator::Schedule (Seconds(1), &Trust::SetSrcIp, trust);
  Simulator::Schedule (Seconds(1), &Trust::SetSrcMac, trust);
  Simulator::Schedule (Seconds(1), &Trust::RunListener, trust);
  

  return trust;
}




}

