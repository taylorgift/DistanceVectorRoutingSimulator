# DistanceVectorRoutingSimulator
A discrete-time event simulator for distance vector routing.

Summary:
This program is a discrete-time event simulator that simulates distance vector routing for a given network. It is helpful in studying the impact of different factors on the convergence times to optimal routes. This simulator deals with data packets, distance vector packets, and link failure events.

Input file:
The input file for this program is a basic text file containing information representing the topology of the network that is to be simulated. Each network topology file consists of a number of rows, each row represents an edge in the network. There are four entries per row. The first entry is the source node ID (address), the second is the destination node ID, the third entry is the cost of the link between the source and the destination. The last column represents the delay it takes to send and receive packets on that link. Notice that the third entry is the one that will be used to find the shortest paths in the network. For example, a row with these values: 2 12 23 0.3, would mean that there is a link between node 2 and node 12. The link has a cost of 23 and it would take 0.3 seconds for a packet (DV or data) to be sent from one end to the other.

Command line compilation: make
Command line execution: ./a topology1.txt secondsSimulated

Entirety of code written in C++ by Taylor Gift

