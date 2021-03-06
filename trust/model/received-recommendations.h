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
 
#ifndef RECEIVED_RECOMMENDATIONS_H
#define RECEIVED_RECOMMENDATIONS_H

#include <stdint.h>

namespace ns3
{

class ReceivedRecommendations
{
private:
	uint32_t m_seq;
	double m_receivedTrust;  
	double m_receivedUncertainty; 
		
public:
	ReceivedRecommendations ();
	virtual ~ReceivedRecommendations ();
	
	uint32_t GetSeq();
	void SetSeq ();
	
	double GetReceivedTrust ();
	void SetReceivedTrust (double);
	
	double GetReceivedUncertainty ();
	void SetReceivedUncertainty (double);
	
	
};

}
#endif 