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
 
#include "ns3/core-module.h"
#include "ns3/trust-helper.h"

using namespace ns3;


int 
main (int argc, char *argv[])
{
  bool verbose = true;

  CommandLine cmd;
  cmd.AddValue ("verbose", "Tell application to log if true", verbose);

  cmd.Parse (argc,argv);

  /* ... */

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}


