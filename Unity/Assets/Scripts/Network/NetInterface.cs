using System.Collections;
using System.Collections.Generic;
using System.Text;
using ChampNetPlugin;
using UnityEngine;

using Netty = ChampNetPlugin.Network;

[RequireComponent(typeof(EventManager))]
public class NetInterface : Singleton<NetInterface>
{

    private static NetInterface _instance = null;
    public static NetInterface INSTANCE { get {  return _instance; } }
    

    private EventManager events;
    private string serverAddress = "127.0.0.1";
    private int serverPort = 425;

    void Awake()
    {
        this.loadSingleton(this, ref NetInterface._instance);
    }

    void Start()
    {
        this.events = this.GetComponent<EventManager>();

        Debug.Log("Creating network");
        Netty.Create();
        //Netty.SetDebugCallback();
    }

    void OnDestroy()
    {
        Debug.Log("Destroying network");
        GameManager.INSTANCE.Disconnect();
        Netty.Destroy();
    }

    public EventManager getEvents()
    {
        return this.events;
    }

	public void connect(string address, int port)
    {
        Netty.StartClient();
        this.serverAddress = address;
        this.serverPort = port;
        Netty.ConnectToServer(address, port);
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

    public KeyValuePair<string, int> getServer()
    {
        return new KeyValuePair<string, int>(this.serverAddress, this.serverPort);
    }

}
