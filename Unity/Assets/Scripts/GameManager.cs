using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;
using UnityEngine.Events;

public class GameManager : Singleton<GameManager>
{

    private static GameManager _instance;
    public static GameManager INSTANCE { get { return _instance; } }

    [Serializable]
    public class GameAction : UnityEvent { }
    public GameAction onPlay, onPlayNetwork, onExit;

    [Serializable]
    public class GameActionFlag : UnityEvent<bool> { }
    public GameActionFlag onNetworkConnectionHandled;

    [Serializable]
    public class GameActionMessage : UnityEvent<string> { }
    public GameActionMessage onNetworkRejected;


    public float updateDelay = 0.5f;
    public GameObject playerNetworkPrefab;

    private NetInterface netty;

    private uint id;
    private PlayerReference localPlayer;
    private Coroutine localPlayerUpdates;
    private Dictionary<uint, PlayerReference> networkPlayerMap;

    void Awake()
    {
        this.loadSingleton(this, ref GameManager._instance);
    }

    void Start()
    {

        this.netty = NetInterface.INSTANCE;

        this.localPlayer = null;
        this.localPlayerUpdates = null;
    }
	
    public void Play()
    {
        this.onPlay.Invoke();
    }

    public void PlayNetwork()
    {
        this.onPlayNetwork.Invoke();
    }

    public void Exit()
    {
        this.onExit.Invoke();
    }

    public void setID(uint id)
    {
        this.id = id;
    }

    public uint getID()
    {
        return this.id;
    }

    public void setPlayer(PlayerReference player)
    {
        this.localPlayer = player;
        if (this.localPlayer != null)
        {
            this.localPlayer.setID(this.getID());
        }

        if (this.localPlayerUpdates != null)
        {
            StopCoroutine(this.localPlayerUpdates);
            this.localPlayerUpdates = null;
        }

        if (player != null)
        {
            this.localPlayerUpdates = StartCoroutine(this.sendPositionUpdates());
        }

    }

    public void sendPositionUpdate()
    {
        if (this.netty == null) return;
        this.netty.Dispatch(new EventNetwork.EventUpdatePosition(this.getID(), this.localPlayer.transform.position.x, this.localPlayer.transform.position.y));
    }

    private IEnumerator sendPositionUpdates()
    {
        while (true)
        {
            this.sendPositionUpdate();
            yield return new WaitForSeconds(this.updateDelay);
        }
    }

    public void spawnPlayer(uint id, float posX, float posY)
    {
        if (this.networkPlayerMap.ContainsKey(id))
        {
            return;
        }

        GameObject playerNetworked = Instantiate(this.playerNetworkPrefab);
        PlayerReference player = playerNetworked.GetComponent<PlayerNetwork>();
        this.networkPlayerMap.Add(id, player);

        this.updatePlayer(id, posX, posY);

    }

    public void updatePlayer(uint id, float posX, float posY)
    {
        if (!this.networkPlayerMap.ContainsKey(id))
        {
            this.spawnPlayer(id, posX, posY);
            return;
        }

        PlayerReference player;
        if (this.networkPlayerMap.TryGetValue(id, out player))
        {
            player.updateAt(posX, posY);
        }

    }

}
