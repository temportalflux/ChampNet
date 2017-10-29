using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Singleton<T> : MonoBehaviour where T : MonoBehaviour
{

    protected void loadSingleton(T inst, ref T staticRef)
    {

        if (staticRef != null)
        {
            Destroy(staticRef);
            staticRef = null;
        }

        staticRef = inst;
        DontDestroyOnLoad(staticRef);

    }

    private bool inUse = false;

    public bool isInUse()
    {
        return inUse;
    }

    public void Lock() { inUse = true; }
    public void Unlock() { inUse = false; }

}
