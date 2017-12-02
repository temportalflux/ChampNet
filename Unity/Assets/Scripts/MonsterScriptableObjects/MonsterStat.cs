/* Author: Jake Ruth
 */

using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[Serializable]
[CreateAssetMenu(menuName = "Asset/ Create new Monster", order = 100)]
public class MonsterStat : ScriptableObject
{
    [Header("Monster Stats")]
    public string monsterName;
	public Sprite monsterPicture;
    public List<MonsterType> types;
    [Space]
    //[Header("")] // used to create a space in the inspector to space things apart
    public int maxHp;
    public int attack;
    public int defense;
    public int specialAttack;
    public int specialDefense;
    public int speed;
    [Space]
    //[Header("")] // used to create a space in the inspector to space things apart
    public List<AttackObject> availableAttacks;

    void OnValidate()
    {
        if (types.Count > 2)
        {
            types.RemoveAt(types.Count - 1);
            Debug.LogWarning("Types can only be a max of two");
        }

        if (availableAttacks.Count > 4)
        {
            availableAttacks.RemoveAt(types.Count - 1);
            Debug.LogWarning("available attacks can only be a max of four");
        }
        
    }
}
