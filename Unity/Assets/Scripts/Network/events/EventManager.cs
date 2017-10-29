using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EventManager : MonoBehaviour {

    private Queue<EventNetwork> events;

    void Start()
    {
        this.events = new Queue<EventNetwork>();
    }

	public void onReceive(int id, string address, byte[] data)
    {
        Debug.Log(data.Length);
        EventNetwork evt = this.createEvent(id);
        
        int lastIndex = 0;
        evt.deserialize(data, ref lastIndex);

        events.Enqueue(evt);
    }

    private EventNetwork createEvent(int id)
    {
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

    private bool PollEvent(out EventNetwork evt)
    {
        evt = null;
        if (this.events.Count > 0)
        {
            evt = this.events.Dequeue();
            return true;
        }
        return false;
    }

    public void ProcessEvents()
    {
        EventNetwork evt;
        while (this.PollEvent(out evt))
        {
            evt.execute();
        }
    }

}
