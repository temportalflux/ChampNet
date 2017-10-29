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

    [Serializable]
    public class GameActionFlag : UnityEvent<bool> { }
    public GameActionFlag onNetworkConnectionHandled;

    [Serializable]
    public class GameActionMessage : UnityEvent<string> { }
    public GameActionMessage onNetworkRejected;

    public SceneTransition transition;
    public float updateDelay = 0.5f;
    public GameObject playerPrefab;
    public GameObject playerNetworkPrefab;

    private NetInterface netty;

    private uint id;
    private PlayerReference localPlayer;
    private Coroutine localPlayerUpdates;
    private Dictionary<uint, PlayerReference> networkPlayerMap;

    void Awake()
    {
        this.loadSingleton(this, ref GameManager._instance);
        this.networkPlayerMap = new Dictionary<uint, PlayerReference>();
    }

    void Start()
    {
        this.netty = NetInterface.INSTANCE;

        this.localPlayer = null;
        this.localPlayerUpdates = null;
        
    }
	
    public void Play()
    {
        this.transition.load(
            () => {
                this.createPlayer(false);
            }
        );
    }

    public void PlayNetwork()
    {
        this.transition.load(
            () => {
                this.createPlayer(true);
            }
        );
    }

    public void Exit()
    {
        this.transition.exit();
    }

    public void setID(uint id)
    {
        this.id = id;
    }

    public uint getID()
    {
        return this.id;
    }

    private void createPlayer(bool networked)
    {
       // GameObject playerObj = Instantiate(this.playerPrefab);

        // TODO: THis assumes a lot
       // GameObject.FindGameObjectWithTag("MainCamera").GetComponent<CameraController>().target = playerObj.transform;

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
        EventNetwork evt = this.localPlayer.createUpdateEvent();
        if (evt != null)
        {
            this.netty.getEvents().Dispatch(evt);
        }
    }

    private IEnumerator sendPositionUpdates()
    {
        while (true)
        {
            this.sendPositionUpdate();
            yield return null;// new WaitForSeconds(this.updateDelay);
        }
    }

    public void spawnPlayer(uint id, float posX, float posY)
    {
        if (this.networkPlayerMap.ContainsKey(id))
        {
            return;
        }

        // TODO: Need queue of updates; this can be triggered while scenes are in transition
        // having a queue of updates waiting on (insert indicator that transition has finished) can help solved this

        GameObject playerNetworked = Instantiate(this.playerNetworkPrefab);
        PlayerReference player = playerNetworked.GetComponent<PlayerNetwork>();
        this.networkPlayerMap.Add(id, player);

        this.updatePlayer(id, posX, posY, 0);

    }

    public void updatePlayer(uint id, float posX, float posY, float rotZ)
    {
        if (!this.networkPlayerMap.ContainsKey(id))
        {
            this.spawnPlayer(id, posX, posY);
            return;
        }

        PlayerReference player;
        if (this.networkPlayerMap.TryGetValue(id, out player))
        {
            if (player != null)
            {
                player.updateAt(posX, posY, rotZ);
            }
            else
            {
                this.networkPlayerMap.Remove(id);
            }
        }

    }

    public void Disconnect()
    {
        this.netty.getEvents().Dispatch(new EventNetwork.EventUserLeft(this.getID()));
    }

    public void removePlayer(uint id)
    {
        if (this.networkPlayerMap.ContainsKey(id))
        {
            PlayerReference player;
            if (this.networkPlayerMap.TryGetValue(id, out player))
            {
                if (player != null)
                {
                    Destroy(player.gameObject);
                }
            }
            this.networkPlayerMap.Remove(id);
        }
    }

    public PlayerReference getRandomPlayer()
    {
        List<PlayerReference> players = new List<PlayerReference>(this.networkPlayerMap.Values);
        int index = UnityEngine.Random.Range(0, players.Count);
        return players[index];
    }

}
