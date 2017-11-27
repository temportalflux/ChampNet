using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

[RequireComponent(typeof(Transition))]
public class SceneTransition : MonoBehaviour {
    
    public string nextScene;

    private Transition transition;

    private void Start()
    {
        this.transition = this.GetComponent<Transition>();
    }

    public void load(ManagerTransitions.OnTransitionFinish onFinish = null)
    {
        ManagerTransitions.INSTANCE.triggerLoadAsync(this.nextScene, this.transition, onFinish);
    }
    
    public void exit(ManagerTransitions.OnTransitionFinish onFinish = null)
    {
        ManagerTransitions.INSTANCE.triggerLoadAsync(null, this.transition, onFinish);
    }

}
