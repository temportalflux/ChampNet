using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerReference : MonoBehaviour {

    public float updateDelay = 0.5f;

    private GameManager gm;
    private NetInterface netty;

    void Start()
    {
        this.gm = GameManager.INSTANCE;
        this.netty = NetInterface.INSTANCE;
        this.gm.setPlayer(this);
        StartCoroutine(this.sendPositionUpdates());
    }

    public void sendPositionUpdate()
    {
        if (this.netty == null) return;
        this.netty.Dispatch(new EventNetwork.EventUpdatePosition(this.gm.getID(), this.transform.position.x, this.transform.position.y));
    }

    private IEnumerator sendPositionUpdates()
    {
        while (true)
        {
            this.sendPositionUpdate();
            yield return new WaitForSeconds(this.updateDelay);
        }
    }

}
