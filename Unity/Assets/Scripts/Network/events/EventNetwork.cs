using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Runtime.InteropServices;

/// <summary>
/// A base class for network events
/// https://msdn.microsoft.com/en-us/library/system.runtime.interopservices.unmanagedtype(v=vs.110).aspx
/// </summary>
/// <remarks>
/// Author: Dustin Yost
/// </remarks>
public class EventNetwork
{

    /// <summary>
    /// Creates an event from a packet identifier
    /// </summary>
    /// <param name="id">The identifier.</param>
    /// <returns>Some EventNetwork object</returns>
    /// <remarks>
    /// Author: Dustin Yost
    /// </remarks>
    public static EventNetwork createEvent(int id)
    {
        //Debug.Log("Got event " + id);
        switch (id)
        {
            case (char)ChampNetPlugin.MessageIDs.ID_CLIENT_CONNECTION_ACCEPTED:
                return new EventNetwork.EventConnected();
            case (char)ChampNetPlugin.MessageIDs.ID_CLIENT_CONNECTION_REJECTED:
                return new EventNetwork.EventConnectionRejected();
            case (char)ChampNetPlugin.MessageIDs.ID_DISCONNECT:
                return new EventNetwork.EventDisconnected();
            case (char)ChampNetPlugin.MessageIDs.ID_USER_ID:
                return new EventNetwork.EventUserID();
            case (char)ChampNetPlugin.MessageIDs.ID_USER_LEFT:
                return new EventNetwork.EventUserLeft();
            case (char)ChampNetPlugin.MessageIDs.ID_USER_SPAWN:
                return new EventNetwork.EventUserSpawn();
            case (char)ChampNetPlugin.MessageIDs.ID_USER_UPDATE_POSITION:
                return new EventNetwork.EventUpdatePosition();
            case (char)ChampNetPlugin.MessageIDs.ID_BATTLE_REQUEST:
                return new EventNetwork.EventBattleRequest();
            case (char)ChampNetPlugin.MessageIDs.ID_BATTLE_RESPONSE:
                return new EventNetwork.EventBattleResponse();
            case (char)ChampNetPlugin.MessageIDs.ID_BATTLE_RESULT:
                return new EventNetwork.EventBattleResult();
            default:
                return new EventNetwork((byte)id);
        }
    }

    /// <summary>
    /// The event identifier (ChampNetPlugin::MessageIds)
    /// </summary>
    /// <remarks>
    /// Author: Dustin Yost
    /// </remarks>
    private byte id;
    
    public EventNetwork(byte id)
    {
        this.id = id;
    }

    /// <summary>
    /// Returns the size of the packet (the size for a potential byte[])
    /// </summary>
    /// <returns>the integer length of a byte array to hold this event's data</returns>
    /// <remarks>
    /// Author: Dustin Yost
    /// </remarks>
    virtual public int GetSize()
    {
        return sizeof(byte); // id
    }

    /// <summary>
    /// Deserializes data from a byte array into this event's data
    /// </summary>
    /// <param name="data">The data.</param>
    /// <param name="lastIndex">The last index.</param>
    /// <remarks>
    /// Author: Dustin Yost
    /// </remarks>
    virtual public void Deserialize(byte[] data, ref int lastIndex)
    {

        // Read the event identifier
        this.id = data[0];
        lastIndex += sizeof(byte);
        
    }
    
    /// <summary>
    /// Serializes data from this event into a byte array
    /// </summary>
    /// <param name="data">The data.</param>
    /// <param name="lastIndex">The last index.</param>
    /// <remarks>
    /// Author: Dustin Yost
    /// </remarks>
    virtual public void Serialize(ref byte[] data, ref int lastIndex)
    {

        // Write the event identifier
        data[lastIndex] = (byte)this.id;
        lastIndex += sizeof(byte);

    }
    
    /// <summary>
    /// Write some byte array into another byte array at some offset
    /// </summary>
    /// <param name="dest">The data to copy.</param>
    /// <param name="offset">The offset in bytes.</param>
    /// <param name="source">The data object to copy into at the offset.</param>
    /// <remarks>
    /// Author: Dustin Yost
    /// </remarks>
    private void WriteTo(ref byte[] dest, ref int offset, byte[] source)
    {
        // copy all data from the source to the destination, starting at some offset
        System.Array.Copy(source, 0, dest, offset, source.Length);
        offset += source.Length;
    }

    /// <summary>
    /// Processes this event to affect the actual environment
    /// </summary>
    /// <remarks>
    /// Author: Dustin Yost
    /// </remarks>
    virtual public void Execute()
    {
        ChampNetPlugin.MessageIDs message = (ChampNetPlugin.MessageIDs)this.id;
        //Debug.Log("Execute event with id: " + message + "(" + this.id + ")");
    }
    
    /// <summary>
    /// Event: Notification that the client has been accepted to the server
    /// </summary>
    /// <remarks>
    /// Author: Dustin Yost
    /// </remarks>
    public class EventConnected : EventNetwork
    {

        public EventConnected() : base((byte)ChampNetPlugin.MessageIDs.ID_CLIENT_CONNECTION_ACCEPTED)
        {
        }
        
        /// <summary>
        /// Processes this event to affect the actual environment
        /// </summary>
        /// <remarks>
        /// Author: Dustin Yost
        /// </remarks>
        override public void Execute()
        {
            // Notify the game manger that the connection request has been satisfied
            GameManager.INSTANCE.onNetworkConnectionHandled.Invoke(true);
            // Tell the server we have connected
            NetInterface.INSTANCE.Dispatch(new EventUserJoined());
            // Begin the game in networked mode
            GameManager.INSTANCE.PlayNetwork();
        }

    }
    
    /// <summary>
    /// Event: Notification that the client has been rejected from the server
    /// </summary>
    /// <remarks>
    /// Author: Dustin Yost
    /// </remarks>
    public class EventConnectionRejected : EventNetwork
    {

        public EventConnectionRejected() : base((byte)ChampNetPlugin.MessageIDs.ID_CLIENT_CONNECTION_REJECTED)
        {
        }
        
        /// <summary>
        /// Processes this event to affect the actual environment
        /// </summary>
        /// <remarks>
        /// Author: Dustin Yost
        /// </remarks>
        override public void Execute()
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
        
        /// <summary>
        /// Processes this event to affect the actual environment
        /// </summary>
        /// <remarks>
        /// Author: Dustin Yost
        /// </remarks>
        override public void Execute()
        {
            Debug.Log("Server Disconnected");
            // Exit the game (we have been booted from the server)
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

        /// <summary>
        /// Processes this event to affect the actual environment
        /// </summary>
        /// <remarks>
        /// Author: Dustin Yost
        /// </remarks>
        override public void Execute()
        {
            Debug.Log("Some user has joined");
        }

    }

    public class EventWithID : EventNetwork
    {
        
        /// <summary>
        /// The player identifier
        /// </summary>
        protected uint playerID;

        public EventWithID(byte id) : base(id)
        {}

        /// <summary>
        /// Returns the size of the packet (the size for a potential byte[])
        /// </summary>
        /// <returns>
        /// the integer length of a byte array to hold this event's data
        /// </returns>
        /// <remarks>
        /// Author: Dustin Yost
        /// </remarks>
        override public int GetSize()
        {
            return base.GetSize() + sizeof(System.UInt32); // super + playerID
        }

        /// <summary>
        /// Deserializes data from a byte array into this event's data
        /// </summary>
        /// <param name="data">The data.</param>
        /// <param name="lastIndex">The last index.</param>
        /// <remarks>
        /// Author: Dustin Yost
        /// </remarks>
        override public void Deserialize(byte[] data, ref int lastIndex)
        {
            base.Deserialize(data, ref lastIndex);

            // https://msdn.microsoft.com/en-us/library/system.bitconverter(v=vs.110).aspx
            // uint is 4 bytes (c++ uint is 4 bytes)
            this.playerID = System.BitConverter.ToUInt32(data, lastIndex);
            lastIndex += sizeof(System.UInt32);

        }

        /// <summary>
        /// Serializes data from this event into a byte array
        /// </summary>
        /// <param name="data">The data.</param>
        /// <param name="lastIndex">The last index.</param>
        /// <remarks>
        /// Author: Dustin Yost
        /// </remarks>
        override public void Serialize(ref byte[] data, ref int lastIndex)
        {
            base.Serialize(ref data, ref lastIndex);

            // Write the bytes of the playerID
            this.WriteTo(ref data, ref lastIndex, System.BitConverter.GetBytes(this.playerID));

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

        override public void Execute()
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

        override public void Execute()
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

        override public int GetSize()
        {
            return base.GetSize() + (sizeof(System.Single) * 2); // super + posX + posY
        }

        override public void Deserialize(byte[] data, ref int lastIndex)
        {
            base.Deserialize(data, ref lastIndex);

            // https://msdn.microsoft.com/en-us/library/system.bitconverter(v=vs.110).aspx
            // float(single) is 4 bytes (c++ float is 4 bytes)
            this.posX = System.BitConverter.ToSingle(data, lastIndex);
            lastIndex += sizeof(System.Single);
            this.posY = System.BitConverter.ToSingle(data, lastIndex);
            lastIndex += sizeof(System.Single);


        }

        override public void Serialize(ref byte[] data, ref int lastIndex)
        {
            base.Serialize(ref data, ref lastIndex);

            this.WriteTo(ref data, ref lastIndex, System.BitConverter.GetBytes(this.posX));
            this.WriteTo(ref data, ref lastIndex, System.BitConverter.GetBytes(this.posY));

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

        override public void Execute()
        {
            Debug.Log("User " + this.playerID + " spawned");
            GameManager.INSTANCE.spawnPlayer(this.playerID, 0, 0);
        }

    }

    /**
     * Some player's character needs to be updated
     */
    public class EventUpdatePosition : EventWithLocation
    {

        private float velX;
        private float velY;

        public EventUpdatePosition() : base((byte)ChampNetPlugin.MessageIDs.ID_USER_UPDATE_POSITION)
        {
        }

        public EventUpdatePosition(uint playerID, float posX, float posY, float velX, float velY) : this()
        {
            this.playerID = playerID;
            this.posX = posX;
            this.posY = posY;
            this.velX = velX;
            this.velY = velY;
        }

        override public int GetSize()
        {
            return base.GetSize() + sizeof(System.Single) * 2; // super + rotZ
        }

        override public void Deserialize(byte[] data, ref int lastIndex)
        {
            base.Deserialize(data, ref lastIndex);

            // https://msdn.microsoft.com/en-us/library/system.bitconverter(v=vs.110).aspx
            // float(single) is 4 bytes (c++ float is 4 bytes)
            this.velX = System.BitConverter.ToSingle(data, lastIndex);
            lastIndex += sizeof(System.Single);
            this.velY = System.BitConverter.ToSingle(data, lastIndex);
            lastIndex += sizeof(System.Single);

        }

        override public void Serialize(ref byte[] data, ref int lastIndex)
        {
            base.Serialize(ref data, ref lastIndex);

            this.WriteTo(ref data, ref lastIndex, System.BitConverter.GetBytes(this.velX));
            this.WriteTo(ref data, ref lastIndex, System.BitConverter.GetBytes(this.velY));

        }

        override public void Execute()
        {
            Debug.Log("User " + this.playerID + " to update location to (" + this.posX + " | " + this.posY + ")");
            GameManager.INSTANCE.updatePlayer(this.playerID, this.posX, this.posY, this.velX, this.velY);

        }

    }

    /**
     * A base event to (de)serialize two playerIDs
     */
    public class EventWithIDTwo : EventWithID
    {

        protected uint playerID_second;

        public EventWithIDTwo(byte id) : base(id) { }

        override public int GetSize()
        {
            return base.GetSize() + sizeof(System.UInt32); // super + playerID
        }

        override public void Deserialize(byte[] data, ref int lastIndex)
        {
            base.Deserialize(data, ref lastIndex);

            // https://msdn.microsoft.com/en-us/library/system.bitconverter(v=vs.110).aspx
            // uint is 4 bytes (c++ uint is 4 bytes)
            this.playerID_second = System.BitConverter.ToUInt32(data, lastIndex);
            lastIndex += sizeof(System.UInt32);

        }

        override public void Serialize(ref byte[] data, ref int lastIndex)
        {
            base.Serialize(ref data, ref lastIndex);

            this.WriteTo(ref data, ref lastIndex, System.BitConverter.GetBytes(this.playerID_second));

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

        public override void Execute()
        {
            // Some user (requester) has asked us (receiver) to battle
            Debug.Log("Received request to battle from " + this.idSender + " (i am " + this.idReceiver + "=" + GameManager.INSTANCE.getID() + ")... auto accepting");
            NetInterface.INSTANCE.Dispatch(new EventBattleResponse(this.idReceiver, this.idSender, true));
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

        override public int GetSize()
        {
            // https://docs.microsoft.com/en-us/dotnet/csharp/language-reference/keywords/sizeof
            return base.GetSize() + sizeof(System.Boolean); // super + accepted
        }

        override public void Deserialize(byte[] data, ref int lastIndex)
        {
            base.Deserialize(data, ref lastIndex);

            // https://msdn.microsoft.com/en-us/library/system.bitconverter(v=vs.110).aspx
            // bool is 1 bytes (c++ bool is 1 bytes)
            this.accepted = System.BitConverter.ToBoolean(data, lastIndex);
            lastIndex += sizeof(System.Boolean);

        }

        override public void Serialize(ref byte[] data, ref int lastIndex)
        {
            base.Serialize(ref data, ref lastIndex);

            this.WriteTo(ref data, ref lastIndex, System.BitConverter.GetBytes(this.accepted));

        }

        public override void Execute()
        {
            // This was sent back to the requester to notify them of the other's result, so receiver and requester are flipped
            //Debug.Log("Request from " + this.idReceiver + " was" + (this.accepted ? "" : " not ") + " accepted by " + this.idSender);
        }

    }

    /**
     * Event: A battle has finished and the entire world needs to know about it
     */
    public class EventBattleResult : EventBattle
    {

        protected uint playerIDWinner;

        public EventBattleResult() : base((byte)ChampNetPlugin.MessageIDs.ID_BATTLE_RESULT) { }

        override public int GetSize()
        {
            return base.GetSize() + sizeof(System.UInt32); // super + playerIDWinner
        }

        override public void Deserialize(byte[] data, ref int lastIndex)
        {
            base.Deserialize(data, ref lastIndex);

            // https://msdn.microsoft.com/en-us/library/system.bitconverter(v=vs.110).aspx
            // uint is 4 bytes (c++ uint is 4 bytes)
            this.playerIDWinner = System.BitConverter.ToUInt32(data, lastIndex);
            lastIndex += sizeof(System.UInt32);

        }

        override public void Serialize(ref byte[] data, ref int lastIndex)
        {
            base.Serialize(ref data, ref lastIndex);

            this.WriteTo(ref data, ref lastIndex, System.BitConverter.GetBytes(this.playerIDWinner));

        }

        public override void Execute()
        {
            Debug.Log("Battle between " + this.idSender + " and " + this.idReceiver + " was won by " + this.playerIDWinner);
            GameObject.Find("GameManager").GetComponent<GameManager>().updatePlayerWin(this.playerIDWinner);
        }

    }

}
