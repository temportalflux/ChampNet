using System.Collections;
using System.Collections.Generic;
using System.Text;
using ChampNetPlugin;
using UnityEngine;

using Netty = ChampNetPlugin.Network;

[RequireComponent(typeof(EventManager))]
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

    private EventManager events;
    private string serverAddress = "127.0.0.1";
    private int serverPort = 425;

    private void Start()
    {
        NetInterface.loadSingleton(this);
        this.events = this.GetComponent<EventManager>();

        Debug.Log("Creating network");
        Netty.Create();
        //Netty.SetDebugCallback();
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
        Netty.ConnectToServer(this.serverAddress, this.serverPort);
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
            //Debug.Log("Receiving " + id);
            this.events.onReceive(id, address, data);
        }
        this.events.ProcessEvents();
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

    public void Dispatch(EventNetwork evt)
    {
        this.Dispatch(evt, this.serverAddress, this.serverPort);
    }

    public void Dispatch(EventNetwork evt, string address, int port)
    {
        byte[] data = new byte[evt.getSize()];
        int lastIndex = 0;
        evt.serialize(ref data, ref lastIndex);
        Netty.SendByteArray(address, port, data, data.Length);
    }

}
