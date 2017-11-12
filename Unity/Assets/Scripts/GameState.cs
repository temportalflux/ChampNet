using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

using ID = System.UInt32;

public class GameState : ScriptableObject
{

    [MenuItem("Assets/Create/Asset/Game State")]
    public static void CreateBrush()
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
    public struct Player
    {

        [HideInInspector]
        public bool editorFoldout;

        /// <summary>
        /// If the player id is from a local player (controlled on this game) or networked (controlled by a peer)
        /// </summary>
        public bool isLocal;

        // Indentification

        [Tooltip("The unique identifier for this specific player")]
        public ID id;

        [Tooltip("The name of this player")]
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

    }

    [HideInInspector]
    public bool editorFoldoutPlayers;

    public Dictionary<ID, Player> players;

    private void OnEnable()
    {
        this.players = new Dictionary<ID, Player>();
    }

    public bool HasPlayer(ref Player info)
    {
        if (this.players.ContainsKey(info.id))
        {
            info = this.players[info.id];
            return true;
        }
        return false;
    }

    public void AddPlayer(Player info)
    {
        this.players.Add(info.id, info);
    }

    public void RemovePlayer(Player info)
    {
        if (this.HasPlayer(ref info))
        {
            this.players.Remove(info.id);
        }
    }

}
