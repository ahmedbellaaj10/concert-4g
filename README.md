:PURPOSE
=======
This is an omnetpp simulation. It aims to replicate a simple solution for providing internet connection at a certain crowded event.

STRUCTURE
=========
## USERS ##
Users would be able to choose between 4G data connection and Wifi. The first one should match eachone ISP in order for them to being able to connect.

## NETWORK ##
Wifi and 4G antennas are connected to the header of the network provided by each ISP. As a result it  will be displayed a three levels tree network with the header as the root and each smartphone as a leaf.

## TRAFFIC ##
Each smartphone will scheudle a random size package (from 1000 to 1500 Kbytes, matchign the biggest most common MTU) for its forwarding to the attached antenna. This package will travel through the antena to the header and back to the device with originated it. Delay will be introduced and, if traffic exceeds the channel data rate, it will be put on queue resulting on a bigger delay

## CONCLUSION ##
Few devices using the whole bandwidth will result in slower speeds for eachone.
Many devices useing not so much bandwidth will produce higer delays.

SCALABILITY
=========== 
The network is composed by two instances of a compound module, each one of them containing a whole functional network. A new whole ISP network deploy can be easily added by instantiating another 'IspNetwork' and modifying its default parameters.
