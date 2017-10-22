using System.Collections;
using System.Collections.Generic;
using System.Text;
using ChampNetPlugin;
using UnityEngine;

using Netty = ChampNetPlugin.Network;

public class NetInterface : MonoBehaviour {

    private static NetInterface _instance = null;
    public static NetInterface INSTANCE
    {
        get
        {
            return _instance;
        }
    }

    private static void loadSingleton(NetInterface inst)
    {

        if (_instance != null)
        {
            Destroy(_instance);
            _instance = null;
        }

        _instance = inst;
        DontDestroyOnLoad(_instance);

    }

    private void Start()
    {
        NetInterface.loadSingleton(this);

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

    /// <summary>
    /// Used to test sending a byte array to the server
    /// </summary>
    /// <remarks>
    /// Author: Jake Ruth
    /// </remarks>
    [ContextMenu("Test Sending a byte array to the server")]
    public void SendTestByteArray()
    {
        char id = (char)MessageIDs.ID_USER_JOINED;
        string test = id + "jake was here";
        byte[] byteArray = Encoding.ASCII.GetBytes(test);
        int size = byteArray.Length;
        Netty.SendByteArray("127.0.0.1", 425, byteArray, size);
    }

}
