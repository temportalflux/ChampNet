using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;
using UnityEngine.Events;
using UnityEngine.SceneManagement;

/// \defgroup client Client: Cretins
/// @{

/// <summary>
/// The manager for the client game
/// Is a singleton, instatiated in the first scene
/// </summary>
/// <author>Dustin Yost</author>
public class GameManager : Singleton<GameManager>
{

    /// <summary>
    /// The singleton instance
    /// </summary>
    private static GameManager _instance;
    /// <summary>
    /// The public getter for the instance
    /// </summary>
    public static GameManager INSTANCE { get { return _instance; } }

    [Serializable]
    public class GameAction : UnityEvent { }

    [Serializable]
    public class GameActionFlag : UnityEvent<bool> { }
    public GameActionFlag onNetworkConnectionHandled;

    [Serializable]
    public class GameActionMessage : UnityEvent<string> { }
    public GameActionMessage onNetworkRejected;

    /// <summary>
    /// The object to handle transitioning between scenes
    /// </summary>
    public SceneTransition transition;
    
    /// <summary>
    /// The game state
    /// </summary>
    public GameState state;

    /// <summary>
    /// The interface for the network
    /// </summary>
    private NetInterface netty;

    /// <summary>
    /// If the game is active - true when the player enters the world, as opposed to the main menu
    /// </summary>
    private bool inGame;

    /// <summary>
    /// The main camera in the open-world
    /// </summary>
    public MainCamera mainCamera;

    /// <summary>
    /// The scoreboard object - instantiated when the world has loaded
    /// </summary>
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

    /// <summary>
    /// Start the game in a local world
    /// </summary>
    public void Play()
    {
        this.transition.load(
            () => {
                this.inGame = true;
                this.state.isLocalGame = true;
                this.grabScoreBoard();
                this.state.SpawnLocalPlayer();
            }
        );
    }

    /// <summary>
    /// Start the game in a networked world
    /// </summary>
    public void PlayNetwork()
    {
        this.transition.load(
            () => {
                this.inGame = true;
                this.state.isLocalGame = false;
                this.grabScoreBoard();
            }
        );
    }

    /// <summary>
    /// Connect the client with some server
    /// </summary>
    /// <param name="address">The IPv4 address</param>
    /// <param name="port">The address port</param>
    /// <param name="players">the desired player descriptors to connect with</param>
    public void NetworkConnect(string address, int port, ConnectMenu.PlayerDescriptor[] players)
    {
        this.state.playerRequest = players;
        this.netty.Connect(address, port);
    }

    /// <summary>
    /// Exit the current scene
    /// </summary>
    public void Exit()
    {
        this.transition.exit();
    }

    /// <summary>
    /// Get a random player from the current player listings in <see cref="GameState.players"/>
    /// </summary>
    /// <param name="notMe">The player which should not be included</param>
    /// <returns></returns>
    public PlayerReference getRandomPlayer(GameState.Player notMe)
    {
        // Get all players
        List<GameState.Player> players = new List<GameState.Player>(this.state.players.Values);
        // Remove the notMe entry if it exists
        players.RemoveAt(players.FindIndex(new Predicate<GameState.Player>(p => p.playerID == notMe.playerID)));
        // If there are players
        if (players.Count > 0)
        {
            // Get a random player
            int index = UnityEngine.Random.Range(0, players.Count);
            // Return the player reference
            return players[index].objectReference;
        }
        else
        {
            // no valid player
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
            // Update the game state
            this.state.FixedUpdate();
        }
    }

    public void grabScoreBoard()
    {
        this.scoreBoardVar = GameObject.FindGameObjectWithTag("ScoreBoard").GetComponent<ScoreBoard>();
    }
}

/// @} doxygen
