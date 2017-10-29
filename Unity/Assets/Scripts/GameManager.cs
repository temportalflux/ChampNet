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
    public GameAction onPlay, onPlayNetwork, onExit, onNetworkConnectionHandled;

    [Serializable]
    public class GameActionMessage : UnityEvent<string> { }
    public GameActionMessage onNetworkRejected;

    void Start()
    {
        this.loadSingleton(this, ref GameManager._instance);
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

    /*
    public void NetworkRejected(string message)
    {
        
    }
    //*/

}
