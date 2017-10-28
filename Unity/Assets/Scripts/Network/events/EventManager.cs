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
        EventNetwork evt = this.createEvent(id);

        evt.setAddress(address);
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
            default:
                return new EventNetwork((char)id);
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
