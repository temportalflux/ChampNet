using UnityEngine;
using System;
using System.Runtime.InteropServices;

namespace ChampNetPlugin
{

    public class Network
    {

        const string IDENTIFIER = "ChampNetPlugin";

        // Create a network to connect with
        [DllImport(IDENTIFIER)]
        public static extern int Create();

        // Destroy the network (must call Create prior) (must call when owning object is destroyed)
        [DllImport(IDENTIFIER)]
        public static extern int Destroy();

        // Start a server with the specified credentials using this object
        [DllImport(IDENTIFIER)]
        public static extern int StartServer(int port, int maxClients);

        // Start a client using this object
        [DllImport(IDENTIFIER)]
        public static extern int StartClient();

        // Connect this CLIENT to some server using the specified credentials
        [DllImport(IDENTIFIER)]
        public static extern int ConnectToServer(string address, int port);

        // Fetch all incoming packets. Must call prior to PollPacket. Must be called after Create and before Destroy. Returns the quantity of packets in the queue after fetch.
        [DllImport(IDENTIFIER)]
        public static extern int FetchPackets();

        // Poll the packet queue. Returns a pointer to the first packet, and removes that packet from the queue. If valid is true, then the packet can be processed, else the packet does not exist (no packets presently in the queue).
        [DllImport(IDENTIFIER)]
        public static extern IntPtr PollPacket(ref bool valid);

        // Returns the packet's address, given some valid packet pointer (Call after PollPacket if valid is true).
        [DllImport(IDENTIFIER)]
        public static extern string GetPacketAddress(IntPtr packetRef, ref uint length);

        // Returns the packet's data, given some valid packet pointer (Call after PollPacket if valid is true).
        [DllImport(IDENTIFIER)]
        public static extern IntPtr GetPacketData(IntPtr packetRef, ref uint length);

        // Frees the memory of some packet, given some valid packet pointer (Call after PollPacket if valid is true).
        [DllImport(IDENTIFIER)]
        public static extern void FreePacket(IntPtr packetRef);

        // WRAPPER METHOD
        // Handles polling the network for packets, and returning the address and data of that packet.
        // Use instead of PollPacket(bool), GetPacketAddress, GetPacketData, and FreePacket
        // Copies out the data from a valid packet, and frees the packet from memory.
        // Returns true if a valid packet was found, else false.
        public static bool PollPacket(out string address, out char[] data)
        {

            // Ensure the out params have some value (invalid at start)
            address = null;
            data = null;

            // Determine if there are valid packets in the buffer.
            bool foundPacket = false;
            IntPtr packetRef = PollPacket(ref foundPacket);

            // If a valid packet has be dequeued, copy out the data.
            if (foundPacket)
            {
                // Get the address
                uint addressLength = 0;
                address = GetPacketAddress(packetRef, ref addressLength);

                // Get the data
                uint dataLength = 0;
                IntPtr ptrData = GetPacketData(packetRef, ref dataLength);

                data = new char[dataLength];
                Marshal.Copy(ptrData, data, 0, (int)dataLength);
                // Data is now possessed by C#
                
                // Free the packet - all data is copied over
                FreePacket(packetRef);
            }

            // Return if a valid packet was found
            return foundPacket;
        }

    }

}
