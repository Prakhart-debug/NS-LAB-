#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"


using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("FirstScriptExample");

int main (int argc, char *argv[])
{
CommandLine cmd (__FILE__);
cmd.Parse (argc, argv);
Time::SetResolution (Time::NS);
LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
//create node
NodeContainer nodes;
nodes.Create (4);
PointToPointHelper pointToPoint1;
pointToPoint1.SetDeviceAttribute ("DataRate", StringValue ("1000Mbps"));
pointToPoint1.SetChannelAttribute ("Delay", StringValue ("100ms"));
pointToPoint1.SetQueue ("ns3::DropTailQueue","MaxSize", StringValue("3p"));

PointToPointHelper pointToPoint2;
pointToPoint2.SetDeviceAttribute ("DataRate", StringValue ("800Mbps"));
pointToPoint2.SetChannelAttribute ("Delay", StringValue ("300ms"));
pointToPoint2.SetQueue ("ns3::DropTailQueue","MaxSize", StringValue("3p"));

PointToPointHelper pointToPoint3;
pointToPoint3.SetDeviceAttribute("DataRate", StringValue ("1000Mbps"));
pointToPoint3.SetChannelAttribute ("Delay", StringValue("400ms"));
pointToPoint3.SetQueue ("ns3::DropTailQueue","MaxSize", StringValue("3p"));

NetDeviceContainer devices1;
devices1 = pointToPoint1.Install (nodes.Get(0), nodes.Get(2));

NetDeviceContainer devices2;
devices2 = pointToPoint2.Install (nodes.Get(1), nodes.Get(2));

NetDeviceContainer devices3;
devices3 = pointToPoint3.Install (nodes.Get(2), nodes.Get(3));

InternetStackHelper stack;
stack.Install (nodes);
//assign ip address
Ipv4AddressHelper address1;
address1.SetBase ("10.1.1.0", "255.255.255.0");
Ipv4InterfaceContainer interfaces1 = address1.Assign (devices1);
Ipv4AddressHelper address2;
address2.SetBase ("10.1.2.0", "255.255.255.0");
Ipv4InterfaceContainer interfaces2 = address2.Assign (devices2);
Ipv4AddressHelper address3;
address3.SetBase("10.1.3.0", "255.255.255.0");
Ipv4InterfaceContainer interfaces3 = address3.Assign (devices3);

UdpEchoServerHelper echoServer (9);

ApplicationContainer serverApps1 = echoServer.Install (nodes.Get (0));
serverApps1.Start (Seconds (1.0));
serverApps1.Stop (Seconds (10.0));
  
ApplicationContainer serverApps2 = echoServer.Install (nodes.Get (1));
serverApps2.Start (Seconds (1.0));
serverApps2.Stop (Seconds (20.0));

UdpEchoClientHelper echoClient1 (interfaces1.GetAddress (0), 9);
echoClient1.SetAttribute ("MaxPackets", UintegerValue (16));
echoClient1.SetAttribute ("Interval", TimeValue (Seconds (5.0)));
echoClient1.SetAttribute ("PacketSize", UintegerValue (1024));

ApplicationContainer clientApps1 = echoClient1.Install (nodes.Get (3));
clientApps1.Start (Seconds (2.0));
clientApps1.Stop (Seconds (20.0));

UdpEchoClientHelper echoClient2 (interfaces2.GetAddress (0), 9);
echoClient2.SetAttribute ("MaxPackets", UintegerValue (16));
echoClient2.SetAttribute ("Interval", TimeValue (Seconds (5.0)));
echoClient2.SetAttribute ("PacketSize", UintegerValue (1024));

ApplicationContainer clientApps2 = echoClient2.Install (nodes.Get (3));
clientApps2.Start (Seconds (2.0));
clientApps2.Stop (Seconds (20.0));


Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
Simulator::Run ();
Simulator::Destroy ();
return 0;
}
