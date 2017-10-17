using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public struct MonsterDataObject //: MonoBehaviour
{
    public MonsterStat monsterStat;

    public string GetName{ get { return monsterStat.monsterName; } }
    public MonsterType[] GetTypes { get { return monsterStat.types; } }
    public int GetMaxHp { get { return monsterStat.maxHp; } }
    public int GetAttack { get { return monsterStat.attack; } }
    public int GetDefense { get { return monsterStat.defense; } }
    public int GetSpecialAttack { get { return monsterStat.specialAttack; } }
    public int GetSpecialDefense { get { return monsterStat.specialDefense; } }
    public int GetSpeed { get { return monsterStat.speed; } }

    private int _hp;

    public int CurrentHp
    {
        get { return _hp = Mathf.Clamp(_hp, 0, GetMaxHp); }
        set { _hp = Mathf.Clamp(value, 0, GetMaxHp); }
    }

    public float GetHpPercentage { get { return (float)_hp / GetMaxHp; } }
}
