using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;
using UnityEngine.Events;
using UnityEngine.SceneManagement;

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
    public MainCamera mainCamera;

    private ScoreBoard scoreBoardVar;

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
                this.state.isLocalGame = true;
                this.state.SpawnLocalPlayer();
            }
        );
    }

    public void PlayNetwork()
    {
        this.transition.load(
            () => {
                this.inGame = true;
                this.state.isLocalGame = false;
            }
        );
    }

    public void NetworkConnect(string address, int port, ConnectMenu.PlayerDescriptor[] players)
    {
        this.state.playerRequest = players;
        this.netty.Connect(address, port);
    }

    public void Exit()
    {
        this.transition.exit();
    }
    
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
                scoreBoardVar.removePlayerOnLeave(playerInfo.playerID);
            }
            this.state.RemovePlayer(playerInfo);
        }
    }

    public PlayerReference getRandomPlayer(GameState.Player notMe)
    {
        List<GameState.Player> players = new List<GameState.Player>(this.state.players.Values);
        // Remove the notMe entry if it exists
        players.RemoveAt(players.FindIndex(new Predicate<GameState.Player>(p => p.playerID == notMe.playerID)));
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

    /// <summary>
    /// update the player's win count
    /// </summary>
    /// <param name="winnerID"> ID of the winner of a battle </param>
    public void updatePlayerWin(uint winnerID)
    {
        GameState.Player player;

        if (this.state.connectedPlayers.TryGetValue(winnerID, out player))
        {
            if (player.objectReference != null)
            {
                player.objectReference.score = player.objectReference.score + 1;
            }
        }
    }

    public void grabScoreBoard()
    {
        this.scoreBoardVar = GameObject.FindGameObjectWithTag("ScoreBoard").GetComponent<ScoreBoard>();
    }
}
