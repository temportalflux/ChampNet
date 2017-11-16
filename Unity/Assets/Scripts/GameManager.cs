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

    public GameState state;

    private NetInterface netty;
    private bool inGame;

    void Awake()
    {
        this.inGame = false;
        this.loadSingleton(this, ref GameManager._instance);
    }

    void Start()
    {
        this.netty = NetInterface.INSTANCE;
        
    }
	
    public void Play()
    {
        this.transition.load(
            () => {
                this.inGame = true;
            }
        );
    }

    public void PlayNetwork()
    {
        this.transition.load(
            () => {
                this.inGame = true;
            }
        );
    }

    public void Exit()
    {
        this.transition.exit();
    }

    /*
    public void spawnPlayer(GameState.Player playerInfo)
    {
        if (this.state.HasPlayer(ref playerInfo))
        {
            return;
        }

        // TODO: Need queue of updates; this can be triggered while scenes are in transition
        // having a queue of updates waiting on (insert indicator that transition has finished) can help solved this

        GameObject playerNetworked = Instantiate(this.playerNetworkPrefab);

        PlayerReference player = playerNetworked.GetComponent<PlayerNetwork>();
        player.initInfo(playerInfo);

        this.state.AddPlayer(playerInfo);
        if (playerInfo.isLocal)
        {
            this.state.AddPlayerLocal(playerInfo);
        }
        else
        {
            this.state.AddPlayerConnected(playerInfo);
        }

        this.updatePlayer(playerInfo);

    }

    public void updatePlayer(GameState.Player playerInfo)
    {
        if (!this.state.HasPlayer(ref playerInfo))
        {
            this.spawnPlayer(playerInfo);
            return;
        }

        if (playerInfo.objectReference != null)
        {
            playerInfo.objectReference.updateFromInfo();
        }
        else
        {
            this.state.RemovePlayer(playerInfo);
        }

    }
    //*/

    public void Disconnect()
    {
        this.netty.Dispatch(new EventClientLeft(this.state.clientID));
    }

    public void removePlayer(GameState.Player playerInfo)
    {
        if (this.state.HasPlayer(ref playerInfo))
        {
            if (playerInfo.objectReference != null)
            {
                Destroy(playerInfo.objectReference.gameObject);
            }
            this.state.RemovePlayer(playerInfo);
        }
    }

    public PlayerReference getRandomPlayer()
    {
        List<GameState.Player> players = new List<GameState.Player>(this.state.players.Values);
        if (players.Count > 0)
        {
            int index = UnityEngine.Random.Range(0, players.Count);
            return players[index].objectReference;
        }
        else
        {
            return null;
        }
    }

    /// <summary>
    /// Gets the music source.
    /// </summary>
    /// <returns>The AudioSource component</returns>
    public AudioSource GetMusicSource()
    {
        GameObject source = GameObject.FindGameObjectWithTag("Music");
        if (source != null)
        {
            return source.GetComponent<AudioSource>();
        }
        return null;
    }

    private void FixedUpdate()
    {
        if (this.inGame)
        {
            //GameObject allPlayers = GameObject.FindGameObjectWithTag("AllPlayers");
            this.state.FixedUpdate();
        }
    }

}
