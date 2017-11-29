using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MinimapHandler : MonoBehaviour
{
    public Camera MinimapCamera;
    private Transform _localPlayer;
    private Coroutine _lookForPlayerCoroutine;

    void Start ()
    {
        // Find A local player on start
        foreach (KeyValuePair<uint, GameState.Player> pair in GameManager.INSTANCE.state.localPlayers)
        {
            GameState.Player p = pair.Value;
            if (p.isLocal)
            {
                _localPlayer = p.objectReference.transform;
                break;
            }
        }

        // if no player is found, keep looking for one overtime in coroutine
        _lookForPlayerCoroutine = StartCoroutine(LookForLocalPlayer(0.5f));
    }

    private IEnumerator LookForLocalPlayer(float timeBetweenChecks)
    {
        while (_localPlayer == null)
        {
            yield return new WaitForSeconds(timeBetweenChecks);

            foreach (KeyValuePair<uint, GameState.Player> pair in GameManager.INSTANCE.state.localPlayers)
            {
                GameState.Player p = pair.Value;
                if (p.isLocal)
                {
                    _localPlayer = p.objectReference.transform;
                    break;
                }
            }
        }

        _lookForPlayerCoroutine = null;
    }

    void Update ()
    {
        if (_localPlayer != null)
        {
            MinimapCamera.transform.position = _localPlayer.transform.position + Vector3.back * 10;
        }
        else
        {
            if (_lookForPlayerCoroutine == null)
            {
                _lookForPlayerCoroutine = StartCoroutine(LookForLocalPlayer(0.5f));
            }
        }
	}
}
