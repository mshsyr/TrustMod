/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "trust-helper.h"
#include "ns3/string.h"
#include "ns3/names.h"




namespace ns3 {

/* ... */
TrustHelper::TrustHelper ()
{
	m_trustFactory.SetTypeId ("ns3::Trust");
}



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

