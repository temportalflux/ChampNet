using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Runtime.InteropServices;

public class EventNetwork
{
    // https://msdn.microsoft.com/en-us/library/system.runtime.interopservices.unmanagedtype(v=vs.110).aspx

    // The event identifier (ChampNetPlugin::MessageIds)
    private byte id;
    
    public EventNetwork(byte id)
    {
        this.id = id;
    }
    
    /**
     * Returns the size of the packet
     */
    virtual public int getSize()
    {
        return sizeof(byte); // id
    }

    /**
     * Reads data from a byte array into this event's data
     */
    virtual public void deserialize(byte[] data, ref int lastIndex)
    {

        // Read the event identifier
        this.id = data[0];
        lastIndex += sizeof(byte);
        
    }

    /**
     * Writes data from this event into a byte array
     */
    virtual public void serialize(ref byte[] data, ref int lastIndex)
    {

        // Write the event identifier
        data[lastIndex] = (byte)this.id;
        lastIndex += sizeof(byte);

    }

    /**
     * Write some byte array into another byte array at some offset
     */
    private void writeTo(ref byte[] data, ref int offset, byte[] dataObj)
    {
        System.Array.Copy(dataObj, 0, data, offset, dataObj.Length);
        offset += dataObj.Length;
    }

    /**
     * Processes this event to affect the actual environment
     */
    virtual public void execute()
    {
        ChampNetPlugin.MessageIDs message = (ChampNetPlugin.MessageIDs)this.id;
        Debug.Log("Execute event with id: " + message + "(" + this.id + ")");
    }

    /**
     * Event: Notification that the client has been accepted to the server
     */
    public class EventConnected : EventNetwork
    {

        public EventConnected() : base((byte)ChampNetPlugin.MessageIDs.ID_CLIENT_CONNECTION_ACCEPTED)
        {
        }

        /**
         * Processes this event to affect the actual environment
         */
        override public void execute()
        {
            //Debug.Log("Connected (" + this.id + ")");
            GameManager.INSTANCE.onNetworkConnectionHandled.Invoke(true);
            // Tell the server we have connected
            NetInterface.INSTANCE.getEvents().Dispatch(new EventUserJoined());
            GameManager.INSTANCE.PlayNetwork();
        }

    }

    /**
     * Event: Notification that the client has been rejected from the server
     */
    public class EventConnectionRejected : EventNetwork
    {

        public EventConnectionRejected() : base((byte)ChampNetPlugin.MessageIDs.ID_CLIENT_CONNECTION_REJECTED)
        {
        }

        /**
         * Processes this event to affect the actual environment
         */
        override public void execute()
        {
            Debug.Log("Connection Rejected");
            GameManager.INSTANCE.onNetworkConnectionHandled.Invoke(false);
            GameManager.INSTANCE.onNetworkRejected.Invoke("Invalid server");
        }

    }

    public class EventDisconnected : EventNetwork
    {

        public EventDisconnected() : base((byte)ChampNetPlugin.MessageIDs.ID_DISCONNECT)
        {
        }

        /**
         * Processes this event to affect the actual environment
         */
        override public void execute()
        {
            Debug.Log("Server Disconnected");
            GameManager.INSTANCE.Exit();
        }

    }

    /**
     * Event: Notification that some other client has also joined
     */
    public class EventUserJoined : EventNetwork
    {

        public EventUserJoined() : base((byte)ChampNetPlugin.MessageIDs.ID_USER_JOINED)
        {
        }

        override public void execute()
        {
            Debug.Log("Some user has joined");
        }

    }

    public class EventWithID : EventNetwork
    {

        // Some user identifier
        protected uint playerID;

        public EventWithID(byte id) : base(id)
        {}

        override public int getSize()
        {
            return base.getSize() + sizeof(System.UInt32); // super + playerID
        }

        override public void deserialize(byte[] data, ref int lastIndex)
        {
            base.deserialize(data, ref lastIndex);

            // https://msdn.microsoft.com/en-us/library/system.bitconverter(v=vs.110).aspx
            // uint is 4 bytes (c++ uint is 4 bytes)
            this.playerID = System.BitConverter.ToUInt32(data, lastIndex);
            lastIndex += sizeof(System.UInt32);

        }

        override public void serialize(ref byte[] data, ref int lastIndex)
        {
            base.serialize(ref data, ref lastIndex);

            this.writeTo(ref data, ref lastIndex, System.BitConverter.GetBytes(this.playerID));

        }

    }

    /**
     * Event: Notification from server of our user id
     */
    public class EventUserID : EventWithID
    {

        public EventUserID() : base((byte)ChampNetPlugin.MessageIDs.ID_USER_ID)
        {
        }

        override public void execute()
        {
            Debug.Log("Got user id " + this.playerID);
            GameManager.INSTANCE.setID(this.playerID);
        }

    }
    
    public class EventUserLeft : EventWithID
    {

        public EventUserLeft() : base((byte)ChampNetPlugin.MessageIDs.ID_USER_LEFT)
        {
        }

        public EventUserLeft(uint id) : this()
        {
            this.playerID = id;
        }

        override public void execute()
        {
            Debug.Log("User " + this.playerID + " has left");
            GameManager.INSTANCE.removePlayer(this.playerID);
        }

    }

    /**
     * A base event to (de)serialize a (float,float) location
     */
    public class EventWithLocation : EventWithID
    {

        protected float posX, posY;

        public EventWithLocation(byte id) : base(id)
        {
        }

        override public int getSize()
        {
            return base.getSize() + (sizeof(System.Single) * 2); // super + posX + posY
        }

        override public void deserialize(byte[] data, ref int lastIndex)
        {
            base.deserialize(data, ref lastIndex);

            // https://msdn.microsoft.com/en-us/library/system.bitconverter(v=vs.110).aspx
            // float(single) is 4 bytes (c++ float is 4 bytes)
            this.posX = System.BitConverter.ToSingle(data, lastIndex);
            lastIndex += sizeof(System.Single);
            this.posY = System.BitConverter.ToSingle(data, lastIndex);
            lastIndex += sizeof(System.Single);


        }

        override public void serialize(ref byte[] data, ref int lastIndex)
        {
            base.serialize(ref data, ref lastIndex);

            this.writeTo(ref data, ref lastIndex, System.BitConverter.GetBytes(this.posX));
            this.writeTo(ref data, ref lastIndex, System.BitConverter.GetBytes(this.posY));

        }

    }

    /**
     * Event: Notification that some user has spawned at some location
     */
    public class EventUserSpawn : EventWithID
    {
        public EventUserSpawn() : base((byte)ChampNetPlugin.MessageIDs.ID_USER_SPAWN)
        {
        }

        override public void execute()
        {
            //Debug.Log("User " + this.playerID + " spawned at (" + this.posX + " | " + this.posY + ")");
            GameManager.INSTANCE.spawnPlayer(this.playerID, 0, 0);
        }

    }

    /**
     * Some player's character needs to be updated
     */
    public class EventUpdatePosition : EventWithLocation
    {

        private float rotZ;

        public EventUpdatePosition() : base((byte)ChampNetPlugin.MessageIDs.ID_USER_UPDATE_POSITION)
        {
        }

        public EventUpdatePosition(uint playerID, float posX, float posY, float rotZ) : this()
        {
            this.playerID = playerID;
            this.posX = posX;
            this.posY = posY;
            this.rotZ = rotZ;
        }

        override public int getSize()
        {
            return base.getSize() + sizeof(System.Single); // super + rotZ
        }

        override public void deserialize(byte[] data, ref int lastIndex)
        {
            base.deserialize(data, ref lastIndex);

            // https://msdn.microsoft.com/en-us/library/system.bitconverter(v=vs.110).aspx
            // float(single) is 4 bytes (c++ float is 4 bytes)
            this.rotZ = System.BitConverter.ToSingle(data, lastIndex);
            lastIndex += sizeof(System.Single);


        }

        override public void serialize(ref byte[] data, ref int lastIndex)
        {
            base.serialize(ref data, ref lastIndex);

            this.writeTo(ref data, ref lastIndex, System.BitConverter.GetBytes(this.rotZ));

        }

        override public void execute()
        {
            //Debug.Log("User " + this.playerID + " to update location to (" + this.posX + " | " + this.posY + ") with " + this.rotZ);
            GameManager.INSTANCE.updatePlayer(this.playerID, this.posX, this.posY, this.rotZ);
        }

    }

    /**
     * A base event to (de)serialize two playerIDs
     */
    public class EventWithIDTwo : EventWithID
    {

        protected uint playerID_second;

        public EventWithIDTwo(byte id) : base(id) { }

        override public int getSize()
        {
            return base.getSize() + sizeof(System.UInt32); // super + playerID
        }

        override public void deserialize(byte[] data, ref int lastIndex)
        {
            base.deserialize(data, ref lastIndex);

            // https://msdn.microsoft.com/en-us/library/system.bitconverter(v=vs.110).aspx
            // uint is 4 bytes (c++ uint is 4 bytes)
            this.playerID_second = System.BitConverter.ToUInt32(data, lastIndex);
            lastIndex += sizeof(System.UInt32);

        }

        override public void serialize(ref byte[] data, ref int lastIndex)
        {
            base.serialize(ref data, ref lastIndex);

            this.writeTo(ref data, ref lastIndex, System.BitConverter.GetBytes(this.playerID_second));

        }

    }

    /**
     * A base event to wrap two player ids into getters
     */
    public class EventBattle : EventWithIDTwo
    {

        protected uint idSender
        {
            get
            {
                return this.playerID;
            }
            set
            {
                this.playerID = value;
            }
        }

        protected uint idReceiver
        {
            get
            {
                return this.playerID_second;
            }
            set
            {
                this.playerID_second = value;
            }
        }

        public EventBattle(byte id) : base(id) { }

    }

    /**
     * Event: Some player requests battle with another
     */
    public class EventBattleRequest : EventBattle
    {

        public EventBattleRequest() : base((byte)ChampNetPlugin.MessageIDs.ID_BATTLE_REQUEST) { }

        public EventBattleRequest(uint sender, uint receiver) : this()
        {
            this.idSender = sender;
            this.idReceiver = receiver;
        }

        public override void execute()
        {
            // Some user (requester) has asked us (receiver) to battle
            Debug.Log("Received request from battle from " + this.idSender + " (i am " + this.idReceiver + "=" + GameManager.INSTANCE.getID() + ")... auto accepting");
            NetInterface.INSTANCE.getEvents().Dispatch(new EventBattleResponse(this.idReceiver, this.idSender, true));
        }

    }

    /**
     * Event: Some player responds to a battle request from another player
     */
    public class EventBattleResponse : EventBattle
    {

        protected bool accepted;

        public EventBattleResponse() : base((byte)ChampNetPlugin.MessageIDs.ID_BATTLE_RESPONSE) { }

        public EventBattleResponse(uint sender, uint receiver, bool accepted) : this()
        {
            this.idSender = sender;
            this.idReceiver = receiver;
            this.accepted = accepted;
        }

        override public int getSize()
        {
            // https://docs.microsoft.com/en-us/dotnet/csharp/language-reference/keywords/sizeof
            return base.getSize() + sizeof(System.Boolean); // super + accepted
        }

        override public void deserialize(byte[] data, ref int lastIndex)
        {
            base.deserialize(data, ref lastIndex);

            // https://msdn.microsoft.com/en-us/library/system.bitconverter(v=vs.110).aspx
            // bool is 1 bytes (c++ bool is 1 bytes)
            this.accepted = System.BitConverter.ToBoolean(data, lastIndex);
            lastIndex += sizeof(System.Boolean);

        }

        override public void serialize(ref byte[] data, ref int lastIndex)
        {
            base.serialize(ref data, ref lastIndex);

            this.writeTo(ref data, ref lastIndex, System.BitConverter.GetBytes(this.accepted));

        }

        public override void execute()
        {
            // This was sent back to the requester to notify them of the other's result, so receiver and requester are flipped
            Debug.Log("Request from " + this.idReceiver + " was" + (this.accepted ? "" : " not ") + " accepted by " + this.idSender);
        }

    }

    /**
     * Event: A battle has finished and the entire world needs to know about it
     */
    public class EventBattleResult : EventBattle
    {

        protected uint playerIDWinner;

        public EventBattleResult() : base((byte)ChampNetPlugin.MessageIDs.ID_BATTLE_RESULT) { }

        override public int getSize()
        {
            return base.getSize() + sizeof(System.UInt32); // super + playerIDWinner
        }

        override public void deserialize(byte[] data, ref int lastIndex)
        {
            base.deserialize(data, ref lastIndex);

            // https://msdn.microsoft.com/en-us/library/system.bitconverter(v=vs.110).aspx
            // uint is 4 bytes (c++ uint is 4 bytes)
            this.playerIDWinner = System.BitConverter.ToUInt32(data, lastIndex);
            lastIndex += sizeof(System.UInt32);

        }

        override public void serialize(ref byte[] data, ref int lastIndex)
        {
            base.serialize(ref data, ref lastIndex);

            this.writeTo(ref data, ref lastIndex, System.BitConverter.GetBytes(this.playerIDWinner));

        }

        public override void execute()
        {
            Debug.Log("Battle between " + this.idSender + " and " + this.idReceiver + " was won by " + this.playerIDWinner);
        }

    }

}
