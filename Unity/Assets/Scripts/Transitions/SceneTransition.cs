using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class SceneTransition : MonoBehaviour {

    public string nextScene;

	public void load()
    {
        ManagerTransitions.INSTANCE.triggerLoadAsync(this.nextScene);
    }
    
    public void exit()
    {
        ManagerTransitions.INSTANCE.triggerExit();
    }

}
