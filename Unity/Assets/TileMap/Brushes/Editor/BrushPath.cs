using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Tilemaps;

namespace UnityEditor
{

    public enum TileSide
    {
        CENTER,
        OUTER_NW,
        INNER_NW,
        NORTH,
        OUTER_NE,
        INNER_NE,
        EAST,
        OUTER_SE,
        INNER_SE,
        SOUTH,
        OUTER_SW,
        INNER_SW,
        WEST,

    }

    [System.Serializable]
    public struct PathItem
    {
        public TileSide side;
        public TileBase tile;

        public PathItem(TileSide side)
        {
            this.side = side;
            this.tile = null;
        }

        public PathItem(TileSide side, PathItem[] tiles)
        {
            this.side = side;
            if (tiles != null) this.tile = tiles[(int)side].tile;
            else this.tile = null;
        }

    }

    [CustomGridBrush(false, true, false, "Path Brush")]
    public class BrushPath : GridBrush
    {

        // Create the asset for the brush
        [MenuItem("Assets/Create/Asset/Brush - Path")]
        public static void CreateBrush()
        {
            // Create the save panel
            string path = EditorUtility.SaveFilePanelInProject("Save Brush (Path)", "New Brush (Path)", "asset", "Save Brush (Path)", "Assets");
            // Check if path was invalid
            if (path == "")
                return;
            // Create the brush asset
            AssetDatabase.CreateAsset(ScriptableObject.CreateInstance<BrushPath>(), path);
        }

        public static Vector3Int offset(Vector3Int position, TileSide side, int mult = 1)
        {
            if (side == TileSide.NORTH ||
                side == TileSide.OUTER_NW ||
                side == TileSide.OUTER_NE ||
                side == TileSide.INNER_NW ||
                side == TileSide.INNER_NE
                )
            {
                position += Vector3Int.up * mult;
            }
            if (side == TileSide.SOUTH ||
                side == TileSide.OUTER_SW ||
                side == TileSide.OUTER_SE ||
                side == TileSide.INNER_NW ||
                side == TileSide.INNER_NE
                )
            {
                position += Vector3Int.up * mult * -1;
            }
            if (side == TileSide.EAST ||
                side == TileSide.OUTER_NE ||
                side == TileSide.OUTER_SE ||
                side == TileSide.INNER_NE ||
                side == TileSide.INNER_SE
                )
            {
                position += Vector3Int.right * mult;
            }
            if (side == TileSide.WEST ||
                side == TileSide.OUTER_NW ||
                side == TileSide.OUTER_SW ||
                side == TileSide.INNER_NW ||
                side == TileSide.INNER_SW
                )
            {
                position += Vector3Int.right * mult * -1;
            }
            return position;
        }

        public static PathItem[] newTileList(PathItem[] root = null, bool small = false)
        {
            if (!small)
            {
                return new PathItem[] {
                    new PathItem(TileSide.CENTER, root),
                    new PathItem(TileSide.OUTER_NW, root),
                    new PathItem(TileSide.INNER_NW, root),
                    new PathItem(TileSide.NORTH, root),
                    new PathItem(TileSide.OUTER_NE, root),
                    new PathItem(TileSide.INNER_NE, root),
                    new PathItem(TileSide.EAST, root),
                    new PathItem(TileSide.OUTER_SE, root),
                    new PathItem(TileSide.INNER_SE, root),
                    new PathItem(TileSide.SOUTH, root),
                    new PathItem(TileSide.OUTER_SW, root),
                    new PathItem(TileSide.INNER_SW, root),
                    new PathItem(TileSide.WEST, root)
                };
            }
            else
            {
                return new PathItem[] {
                    new PathItem(TileSide.CENTER, root),
                    new PathItem(TileSide.OUTER_NW, root),
                    new PathItem(TileSide.INNER_NW, null),
                    new PathItem(TileSide.NORTH, root),
                    new PathItem(TileSide.OUTER_NE, root),
                    new PathItem(TileSide.INNER_NE, null),
                    new PathItem(TileSide.EAST, root),
                    new PathItem(TileSide.OUTER_SE, root),
                    new PathItem(TileSide.INNER_SE, null),
                    new PathItem(TileSide.SOUTH, root),
                    new PathItem(TileSide.OUTER_SW, root),
                    new PathItem(TileSide.INNER_SW, null),
                    new PathItem(TileSide.WEST, root)
                };
            }
        }

        public PathItem[] tiles = newTileList();

        public PathItem at(TileSide side, PathItem[] tiles = null)
        {
            if (tiles == null)
                tiles = this.tiles;
            return tiles[(int)side];
        }

        public void set(TileSide side, TileBase tile, ref PathItem[] tiles)
        {
            tiles[(int)side].tile = tile;
        }

        public override void Paint(GridLayout gridLayout, GameObject brushTarget, Vector3Int position)
        {

            if (brushTarget == null)
                return;

            Tilemap tilemap = brushTarget.GetComponent<Tilemap>();
            if (tilemap == null)
                return;

            this.paint(gridLayout, brushTarget, position, tilemap, tilemap.SetTile);

            //base.Paint(gridLayout, brushTarget, position);
        }

        public delegate void DoPaint(Vector3Int pos, TileBase tile);

        public void paint(GridLayout gridLayout, GameObject brushTarget,
            Vector3Int position, Tilemap tilemap, DoPaint doPaint)
        {
            PathItem[] tilesOut = newTileList(this.tiles, true);
            // Cache the center tile to be placed
            TileBase tilesCenter = this.at(TileSide.CENTER).tile;
            TileBase tilesNorth = this.at(TileSide.NORTH).tile;

            Vector3Int pos;
            TileBase localTile;

            // Get the current tile at the position 2 units to the west of the current position
            // this is the predicted center of the last tile area
            // If this tile is indeed a center tile...
            if (tilemap.GetTile(offset(position, TileSide.WEST, 2)) == tilesCenter)
            {
                // Set the tile to be placed to the west of the center to be a center tile
                this.set(TileSide.WEST, tilesCenter, ref tilesOut);
            }

            {
                pos = position;
                localTile = null;
                // Get the tile to the west by 2, and north 1
                /*
                  x   _ _ _
                  - - _ p _
                      _ _ _
                */
                pos = offset(pos, TileSide.WEST, 2);
                pos = offset(pos, TileSide.NORTH, 1);
                localTile = tilemap.GetTile(pos);
                // If this tile is indeed a center tile...
                if (localTile == tilesCenter)
                {
                    // Set the tile to be placed to the west of the center to be a center tile
                    this.set(TileSide.OUTER_NW, tilesCenter, ref tilesOut);
                }
                else if (localTile == tilesNorth)
                {
                    // Set the tile to be placed to the west of the center to be a center tile
                    this.set(TileSide.OUTER_NW, tilesNorth, ref tilesOut);
                }
            }

            foreach (PathItem item in tilesOut)
            {
                if (item.tile != null)
                {
                    doPaint(offset(position, item.side), item.tile);
                }
            }

        }

    }

    [CustomEditor(typeof(BrushPath))]
    public class BrushPathEditor : GridBrushEditor
    {
        private BrushPath theBrush { get { return target as BrushPath; } }
        private GameObject lastBrushTarget;

        public override void OnInspectorGUI()
        {
            // Draw script line
            GUI.enabled = false;
            MonoScript script = EditorGUILayout.ObjectField(
                "Script",
                MonoScript.FromScriptableObject(this.theBrush),
                typeof(MonoScript), false
            ) as MonoScript;
            GUI.enabled = true;

            // Draw each enum line
            for (int i = 0; i < this.theBrush.tiles.Length; i++)
            {
                this.theBrush.tiles[i].tile = (TileBase)EditorGUILayout.ObjectField(this.theBrush.tiles[i].side.ToString(), this.theBrush.tiles[i].tile, typeof(TileBase), false);
            }
            
        }

        public override void PaintPreview(GridLayout gridLayout, GameObject brushTarget, Vector3Int position)
        {
            base.PaintPreview(gridLayout, null, position);

            if (brushTarget == null)
                return;

            Tilemap tilemap = brushTarget.GetComponent<Tilemap>();
            if (tilemap == null)
                return;

            tilemap.SetEditorPreviewTile(position, this.theBrush.at(TileSide.CENTER).tile);

            this.theBrush.paint(gridLayout, brushTarget, position, tilemap, tilemap.SetEditorPreviewTile);

            lastBrushTarget = brushTarget;

        }

        public override void ClearPreview()
        {
            if (lastBrushTarget != null)
            {
                var tilemap = lastBrushTarget.GetComponent<Tilemap>();
                if (tilemap == null)
                    return;

                tilemap.ClearAllEditorPreviewTiles();

                lastBrushTarget = null;
            }
            else
            {
                base.ClearPreview();
            }
        }

    }

}
