
public class EventBattleLocalStart : EventBattleResultResponse
{

    public EventBattleLocalStart() : base((byte)ChampNetPlugin.MessageIDs.ID_BATTLE_LOCAL_START)
    {

    }

    public EventBattleLocalStart(uint playerID) : this()
    {
        this.playerID = playerID;
    }
}
