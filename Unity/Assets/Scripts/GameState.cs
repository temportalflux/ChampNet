using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

using ID = System.UInt32;

public class GameState : ScriptableObject, ISerializing
{

    [MenuItem("Assets/Create/Asset/Game State")]
    public static void Create()
    {
        // Get the path to the selected asset
        string selectedPath = "Assets";
        UnityEngine.Object selectedObj = Selection.activeObject;
        if (selectedObj != null) selectedPath = AssetDatabase.GetAssetPath(selectedObj.GetInstanceID());

        // Create the save panel
        string path = EditorUtility.SaveFilePanelInProject(
            "Save Game State", "New Game State",
            "asset", "Save Game State",
            selectedPath
        );
        // Check if path was invalid
        if (path == "")
            return;

        // Create the brush asset
        AssetDatabase.CreateAsset(ScriptableObject.CreateInstance<GameState>(), path);
    }

    [System.Serializable]
    public struct Player : ISerializing
    {

        public static int SIZE =
            sizeof(ID) // clientID
            + sizeof(ID) // playerID
            + sizeof(ID) // localID
            + sizeof(int) + (SIZE_MAX_NAME * sizeof(char))// name
            + (sizeof(float) * 3) // color
            + (sizeof(float) * 3) // position
            + (sizeof(float) * 3) // velocity
            + (sizeof(float) * 3) // acceleration
            + sizeof(bool) // inBattle
            ;

        public static int SIZE_MAX_NAME = 10;
        
        public ID clientID;

        /// <summary>
        /// If the player id is from a local player (controlled on this game) or networked (controlled by a peer)
        /// </summary>
        public bool isLocal {
            get
            {
                return this.clientID == GameManager.INSTANCE.state.clientID;
            }
        }

        // Indentification

        [Tooltip("The unique identifier for this specific player")]
        public ID playerID;

        [Tooltip("The unique identifier for this player on the local machine")]
        public ID localID;

        [Tooltip("The name of this player with 10 characters max")]
        public string name;

        [Tooltip("The color of the character for the player")]
        public Color color;

        // Physics

        [Tooltip("The current position of the player")]
        public Vector3 position;

        [Tooltip("The velocity of the player")]
        public Vector3 velocity;

        [Tooltip("The acceleration of the player")]
        public Vector3 accelleration;

        // Other

        [Tooltip("If the player is in battle")]
        public bool inBattle;


        // Game Objects
        public PlayerReference objectReference;

        public RenderTexture cameraTexture;

        // ~~~~~ ISerializing

        public int GetSize()
        {
            return SIZE;
        }

        /// <summary>
        /// Serializes data from this event into a byte array
        /// </summary>
        /// <param name="data">The data.</param>
        /// <param name="lastIndex">The last index.</param>
        /// <remarks>
        /// Author: Dustin Yost
        /// </remarks>
        public void Serialize(ref byte[] data, ref int lastIndex)
        {

            // write isLocal
            EventNetwork.WriteTo(ref data, ref lastIndex, System.BitConverter.GetBytes(this.clientID));
            // write player id
            EventNetwork.WriteTo(ref data, ref lastIndex, System.BitConverter.GetBytes(this.playerID));
            // write player id
            EventNetwork.WriteTo(ref data, ref lastIndex, System.BitConverter.GetBytes(this.localID));
            // write name
            char[] characters = this.name.ToCharArray();
            int nameLength = Mathf.Min(characters.Length, SIZE_MAX_NAME);
            EventNetwork.WriteTo(ref data, ref lastIndex, System.BitConverter.GetBytes(nameLength));
            for (int i = 0; i < nameLength; i++)
            {
                EventNetwork.WriteTo(ref data, ref lastIndex, System.BitConverter.GetBytes(characters[i]));
            }
            // write color
            EventNetwork.WriteTo(ref data, ref lastIndex, System.BitConverter.GetBytes(this.color.r));
            EventNetwork.WriteTo(ref data, ref lastIndex, System.BitConverter.GetBytes(this.color.g));
            EventNetwork.WriteTo(ref data, ref lastIndex, System.BitConverter.GetBytes(this.color.b));
            // write position
            EventNetwork.WriteTo(ref data, ref lastIndex, System.BitConverter.GetBytes(this.position.x));
            EventNetwork.WriteTo(ref data, ref lastIndex, System.BitConverter.GetBytes(this.position.y));
            EventNetwork.WriteTo(ref data, ref lastIndex, System.BitConverter.GetBytes(this.position.z));
            // write velocity
            EventNetwork.WriteTo(ref data, ref lastIndex, System.BitConverter.GetBytes(this.velocity.x));
            EventNetwork.WriteTo(ref data, ref lastIndex, System.BitConverter.GetBytes(this.velocity.y));
            EventNetwork.WriteTo(ref data, ref lastIndex, System.BitConverter.GetBytes(this.velocity.z));
            // write acceleration
            EventNetwork.WriteTo(ref data, ref lastIndex, System.BitConverter.GetBytes(this.accelleration.x));
            EventNetwork.WriteTo(ref data, ref lastIndex, System.BitConverter.GetBytes(this.accelleration.y));
            EventNetwork.WriteTo(ref data, ref lastIndex, System.BitConverter.GetBytes(this.accelleration.z));
            // write inBattle
            EventNetwork.WriteTo(ref data, ref lastIndex, System.BitConverter.GetBytes(this.inBattle));

        }

        /// <summary>
        /// Deserializes data from a byte array into this event's data
        /// </summary>
        /// <param name="data">The data.</param>
        /// <param name="lastIndex">The last index.</param>
        /// <remarks>
        /// Author: Dustin Yost
        /// </remarks>
        public void Deserialize(byte[] data, ref int lastIndex)
        {

            // read isLocal
            this.clientID = System.BitConverter.ToUInt32(data, lastIndex); lastIndex += sizeof(System.UInt32);
            // read player id
            this.playerID = System.BitConverter.ToUInt32(data, lastIndex); lastIndex += sizeof(System.UInt32);
            // read local id
            this.localID = System.BitConverter.ToUInt32(data, lastIndex); lastIndex += sizeof(System.UInt32);
            // read name
            int nameLength = System.BitConverter.ToInt32(data, lastIndex); lastIndex += sizeof(System.Int32);
            this.name = "";
            for (int i = 0; i < nameLength; i++)
            {
                this.name += System.BitConverter.ToChar(data, lastIndex);
                lastIndex += sizeof(System.Char);
            }
            // read color
            this.color.r = this.DeserializeFloat(data, ref lastIndex);
            this.color.g = this.DeserializeFloat(data, ref lastIndex);
            this.color.b = this.DeserializeFloat(data, ref lastIndex);
            this.color.a = 1.0f;
            // read position
            this.position.x = this.DeserializeFloat(data, ref lastIndex);
            this.position.y = this.DeserializeFloat(data, ref lastIndex);
            this.position.z = this.DeserializeFloat(data, ref lastIndex);
            // read velocity
            this.velocity.x = this.DeserializeFloat(data, ref lastIndex);
            this.velocity.y = this.DeserializeFloat(data, ref lastIndex);
            this.velocity.z = this.DeserializeFloat(data, ref lastIndex);
            // read accelleration
            this.accelleration.x = this.DeserializeFloat(data, ref lastIndex);
            this.accelleration.y = this.DeserializeFloat(data, ref lastIndex);
            this.accelleration.z = this.DeserializeFloat(data, ref lastIndex);
            // read inBattle
            this.inBattle = System.BitConverter.ToBoolean(data, lastIndex); lastIndex += sizeof(System.Boolean);

        }

        private float DeserializeFloat(byte[] data, ref int lastIndex)
        {
            float ret = System.BitConverter.ToSingle(data, lastIndex); lastIndex += sizeof(System.Single);
            return ret;
        }

        // ~~~~~ Data copy

        public void copyFromGameState(Player info, float deltaTime)
        {
            this.clientID = info.clientID;
            this.playerID = info.playerID;
            this.name = info.name;
            this.color = info.color;
            this.position = info.position;
            this.velocity = info.velocity;
            this.accelleration = info.accelleration;
            this.inBattle = info.inBattle;

            this.integratePhysics(deltaTime);

            if (this.objectReference != null)
            {
                this.objectReference.integrateInfo(info);
            }
        }

        public void integratePhysics(float deltaTime)
        {
            this.velocity += this.accelleration * deltaTime;
            this.position += this.velocity * deltaTime;
        }

    }

    public ConnectMenu.PlayerDescriptor[] playerRequest = null;

    [HideInInspector]
    public bool editorFoldoutPlayers;
    public Dictionary<ID, bool> editorFoldouts;

    public bool isLocalGame; // TODO: Serialize
    public uint clientID;

    public float deltaTime;
    public Dictionary<ID, Player> players;

    private List<Player> playersToAdd;
    private List<Player> playersToRemove;
    private Dictionary<uint, GameState.Player> playersLocal;
    private Dictionary<uint, GameState.Player> playersConnected;
    public Dictionary<uint, Player> localPlayers
    {
        get
        {
            return playersLocal;
        }
    }
    public Dictionary<uint, Player> connectedPlayers
    {
        get
        {
            return playersConnected;
        }
    }

    private void OnEnable()
    {
        this.editorFoldouts = new Dictionary<ID, bool>();
        this.players = new Dictionary<ID, Player>();

        this.playersToAdd = new List<Player>();
        this.playersToRemove = new List<Player>();
        this.playersLocal = new Dictionary<uint, GameState.Player>();
        this.playersConnected = new Dictionary<uint, GameState.Player>();
    }

    public bool HasPlayer(ref Player info)
    {
        if (this.players.ContainsKey(info.playerID))
        {
            info = this.players[info.playerID];
            return true;
        }
        return false;
    }

    public void AddPlayer(Player info)
    {
        // Create the player reference object
        GameManager gm = GameManager.INSTANCE;
        GameObject playerObject = GameManager.Instantiate(
            info.isLocal ? gm.playerPrefab : gm.playerNetworkPrefab,
            GameObject.FindGameObjectWithTag("AllPlayers").transform
        );
        info.objectReference = playerObject.GetComponent<PlayerReference>();

        // check if second player, and tweak accordingly
        if (info.isLocal && info.localID > 0)
        {
            Destroy(playerObject.GetComponent<PlayerReference>());
            Destroy(playerObject.GetComponent<InputResponse>());
            info.objectReference = playerObject.AddComponent<PlayerLocalMultiplayer>();
            info.objectReference.sprite = playerObject.GetComponentInChildren<Animator>().transform;
            foreach (SpriteRenderer r in playerObject.GetComponentsInChildren<SpriteRenderer>())
                if (r.gameObject.name == "Overlay")
                    info.objectReference.overlay = r;
        }

        // Add render texture for player camera
        Camera mainCamera = GameObject.FindGameObjectWithTag("MainCamera").GetComponent<Camera>();
        info.cameraTexture = new RenderTexture(Screen.width, Screen.height, 16, RenderTextureFormat.ARGB32);
        info.cameraTexture.antiAliasing = 2;
        info.cameraTexture.Create();
        playerObject.GetComponentInChildren<Camera>(true).targetTexture = info.cameraTexture;
        GameManager.INSTANCE.StartCoroutine(this.AddPlayerToCamera(info));

        // Set the info for the player script
        info.objectReference.setInfo(info);

        info.objectReference.gameObject.name = info.name;

        // Add player to total list
        this.players.Add(info.playerID, info);
        // Add player to the appropriate sublist
        if (info.isLocal) this.AddPlayerLocal(info); else this.AddPlayerConnected(info);
    }

    private IEnumerator AddPlayerToCamera(Player info)
    {
        while (GameManager.INSTANCE.mainCamera == null) yield return null;
        GameManager.INSTANCE.mainCamera.SetTexture((int)info.localID, info.cameraTexture);
    }

    public void RemovePlayer(Player info)
    {
        if (this.HasPlayer(ref info))
        {
            this.players.Remove(info.playerID);
        }
    }

    public void AddPlayerLocal(Player info)
    {
        this.playersLocal.Add(info.playerID, info);
    }

    public void AddPlayerConnected(Player info)
    {
        this.playersConnected.Add(info.playerID, info);
    }

    public void SpawnLocalMultiplayer()
    {
        NetInterface.INSTANCE.Dispatch(new EventRequestPlayer(this.clientID, (uint)this.playersLocal.Count));
    }

    public void SpawnLocalPlayer()
    {
        Player player = new Player();
        player.clientID = 0;
        player.localID = (uint)this.players.Count;
        player.playerID = player.localID;
        player.position = Vector3.zero;
        player.name = "Player " + (player.localID + 1);
        switch (player.localID)
        {
            case 0: player.color = Colors.PuceRed; break;
            case 1: player.color = Colors.BlizzardBlue; break;
            case 2: player.color = Colors.EmeraldGreen; break;
            default: player.color = Colors.YellowOrange; break;
        }
        this.AddPlayer(player);
    }

    public void SpawnPlayer()
    {
        if (this.isLocalGame) this.SpawnLocalPlayer();
        else this.SpawnLocalMultiplayer();
    }

    // ~~~~~ ISerializing

    public int GetSize()
    {
        // clientID + # of players + amount of space required for players
        return sizeof(uint) + sizeof(int) + (this.players.Count * Player.SIZE);
    }

    /// <summary>
    /// Serializes data from this event into a byte array
    /// </summary>
    /// <param name="data">The data.</param>
    /// <param name="lastIndex">The last index.</param>
    /// <remarks>
    /// Author: Dustin Yost
    /// </remarks>
    public void Serialize(ref byte[] data, ref int lastIndex)
    {
        // Serialize the clientID
        EventNetwork.WriteTo(ref data, ref lastIndex, System.BitConverter.GetBytes(this.clientID));

        // Serialize player count
        EventNetwork.WriteTo(ref data, ref lastIndex, System.BitConverter.GetBytes(this.players.Count));

        // Serialize the player data
        foreach (ID playerID in this.players.Keys)
        {
            // Get the player
            ISerializing player = this.players[playerID];
            // Create the byte array to hold the player
            byte[] dataPlayer = new byte[player.GetSize()];
            int dataPlayerIndex = 0;
            // Serialize the player
            player.Serialize(ref dataPlayer, ref dataPlayerIndex);
            // Write player data to main data
            EventNetwork.WriteTo(ref data, ref lastIndex, dataPlayer);
        }
    }

    /// <summary>
    /// Deserializes data from a byte array into this event's data
    /// </summary>
    /// <param name="data">The data.</param>
    /// <param name="lastIndex">The last index.</param>
    /// <remarks>
    /// Author: Dustin Yost
    /// </remarks>
    public void Deserialize(byte[] data, ref int lastIndex)
    {

        // Deserialize clientID
        int clientID = System.BitConverter.ToInt32(data, lastIndex);
        lastIndex += sizeof(ID);
        // Check for valid clientID (if < 0, then the gamestate was broadcasted)
        if (clientID >= 0)
        {
            this.clientID = (uint)clientID;
        }

        // Deserialize player count
        int playerCount = System.BitConverter.ToInt32(data, lastIndex);
        lastIndex += sizeof(System.Int32);

        List<ID> playersFromData = new List<ID>();
        // Read all players, keeping track of the IDs read
        for (int i = 0; i < playerCount; i++)
        {
            // Deserialize the player data
            Player player = new Player();
            player.objectReference = null;
            player.Deserialize(data, ref lastIndex);

            playersFromData.Add(player.playerID);

            // Check if the ID is already in the map
            if (this.players.ContainsKey(player.playerID))
            {
                this.players[player.playerID].copyFromGameState(player, this.deltaTime);
            }
            // playerID not in map, add
            else
            {
                this.playersToAdd.Add(player);
            }

        }

        // some players have been removed from gamestate data
        if (playersFromData.Count < this.players.Count)
        {
            // find all players no longer in the gamestate data
            foreach (ID playerID in this.players.Keys)
            {
                if (!playersFromData.Contains(playerID))
                {
                    // mark those players for removal from game
                    this.playersToRemove.Add(this.players[playerID]);
                }
            }
        }

    }

    // ~~~~~ Update

    public void FixedUpdate()
    {
        // Remove all players that are set to be removed
        foreach (Player player in this.playersToRemove)
        {
            this.RemovePlayer(player);
        }
        this.playersToRemove.Clear();
        // Add all players that are set to be added
        while (this.playersToAdd.Count > 0)
        {
            Player player = this.playersToAdd[0];
            this.playersToAdd.RemoveAt(0);
            // If updates have been received before the list got updated, there could be overlap
            if (this.players.ContainsKey(player.playerID))
            {
                this.players[player.playerID].copyFromGameState(player, 0);
            }
            else
            {
                this.AddPlayer(player);
            }
        }
    }

}
