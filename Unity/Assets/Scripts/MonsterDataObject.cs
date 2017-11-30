using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[Serializable]
public class MonsterDataObject //: MonoBehaviour
{
    public MonsterStat monsterStat;

    public string GetMonsterName { get { return monsterStat.monsterName;} }
    public List<MonsterType> GetTypes { get { return monsterStat.types; } }
    public int GetMaxHp { get { return monsterStat.maxHp; } }
    public int GetAttack { get { return monsterStat.attack; } }
    public int GetDefense { get { return monsterStat.defense; } }
    public int GetSpecialAttack { get { return monsterStat.specialAttack; } }
    public int GetSpecialDefense { get { return monsterStat.specialDefense; } }
    public int GetSpeed { get { return monsterStat.speed; } }
    public List<AttackObject> GetAvailableAttacks { get { return monsterStat.availableAttacks; } }

    private int _hp;

    public int CurrentHP
    {
        get { return _hp = Mathf.Clamp(_hp, 0, GetMaxHp); }
        set { _hp = Mathf.Clamp(value, 0, GetMaxHp); }
    }

    public void Heal()
    {
        _hp = GetMaxHp;
    }

    MonsterDataObject()
    {
        //_hp = GetMaxHp;
    }

    public int GetMonsterAttackStat(MoveKind physicalOrSpecial)
    {
        if (physicalOrSpecial == MoveKind.PHYSICAL)
            return GetAttack;

        return GetSpecialAttack;
    }

    public int GetMonsterDefenseStat(MoveKind physicalOrSpecial)
    {
        if (physicalOrSpecial == MoveKind.PHYSICAL)
            return GetDefense;

        return GetSpecialDefense;
    }
}
