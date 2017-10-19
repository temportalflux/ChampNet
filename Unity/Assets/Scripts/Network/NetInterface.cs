using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using Netty = ChampNetPlugin.Network;

public class NetInterface : MonoBehaviour {

    private void Start()
    {
        Debug.Log("Creating network");
        Netty.Create();
        Netty.SetDebugCallback();
        this.connect();
    }

    void OnDestroy()
    {
        Debug.Log("Destroying network");
        Netty.Destroy();
    }

	public void connect()
    {
        Netty.StartClient();
        Netty.ConnectToServer("127.0.0.1", 425);
    }

    public void disconnect()
    {
        Netty.Disconnect();
    }

    private void FixedUpdate()
    {
        // Get all packets since last update
        Netty.FetchPackets();

        string address;
        byte[] data;
        while (Netty.PollPacket(out address, out data))
        {
            int id = (int)data[0];
            
            switch (id)
            {
                case (int)ChampNetPlugin.MessageIDs.ID_CLIENT_CONNECTION_ACCEPTED:
                    Debug.Log("Connected");
                    break;
                default:
                    break;
            }

        }

    }

}
