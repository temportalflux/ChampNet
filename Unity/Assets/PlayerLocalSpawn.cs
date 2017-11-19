using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerLocalSpawn : MonoBehaviour
{

    public GameObject player;

    public void OnKeyInput(InputDevice device, char c)
    {
        switch (c)
        {
            case 'i':
                // this.activate();
                GameManager.INSTANCE.state.SpawnPlayer();
                this.gameObject.SetActive(false);
                break;
            default:
                break;
        }
    }

    public void activate()
    {
        this.player.SetActive(true);
    }

}
