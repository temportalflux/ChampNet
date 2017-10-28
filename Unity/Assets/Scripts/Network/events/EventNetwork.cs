using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EventNetwork
{

    private char id;
    private string address;

    public EventNetwork(char id)
    {
        this.id = id;
    }

    public void setAddress(string address)
    {
        this.address = address;
    }

    public void deserialize(byte[] data, ref int lastIndex)
    {

        this.id = System.BitConverter.ToChar(data, lastIndex);
        lastIndex += sizeof(char);

    }

    public int getSize()
    {
        return sizeof(char); // id
    }

    public void serialize(ref byte[] data, ref int lastIndex)
    {
        this.writeTo(ref data, ref lastIndex, System.BitConverter.GetBytes(this.id));
    }

    private void writeTo(ref byte[] data, ref int lastIndex, byte[] dataObj)
    {
        System.Array.Copy(dataObj, 0, data, lastIndex, dataObj.Length);
        lastIndex += dataObj.Length;
    }

    virtual public void execute()
    {
        ChampNetPlugin.MessageIDs message = (ChampNetPlugin.MessageIDs)this.id;
        Debug.Log("Execute event with id: " + message + "(" + this.id + ")");
    }

    public class EventConnected : EventNetwork
    {

        public EventConnected() : base((char)ChampNetPlugin.MessageIDs.ID_CLIENT_CONNECTION_ACCEPTED)
        {
        }

        override public void execute()
        {
            Debug.Log("Connected");
            NetInterface.INSTANCE.Dispatch(new EventUserJoined());
        }

    }

    public class EventUserJoined : EventNetwork
    {

        public EventUserJoined() : base((char)ChampNetPlugin.MessageIDs.ID_USER_JOINED)
        {
        }

        override public void execute()
        {
            Debug.Log("Connected");
        }

    }

}
