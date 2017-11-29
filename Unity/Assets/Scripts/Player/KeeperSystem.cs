using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;

public class KeeperSystem : MonoBehaviour
{
    public string keepername;

    //[CustomList]
    public List<MonsterDataObject> monsters;
    public int wins;
    public int losses;
    [CustomList]
    public int[] myInts;
}

