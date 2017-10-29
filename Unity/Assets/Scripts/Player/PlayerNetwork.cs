using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerNetwork : PlayerReference
{

    public override EventNetwork createUpdateEvent()
    {
        return new EventNetwork.EventUpdatePosition(
            this.getID(),
            this.transform.position.x,
            this.transform.position.y,
            this.sprite.rotation.eulerAngles.z
        );
    }

}
