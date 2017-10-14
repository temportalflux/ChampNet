using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class ManagerTransitions : MonoBehaviour {

    private static ManagerTransitions _instance;

    public static ManagerTransitions INSTANCE
    {
        get
        {
            return _instance;
        }
    }

    private void Awake()
    {

        if (ManagerTransitions._instance != null)
        {
            Debug.Log("ManagerTransitions already created... discarding old.");
            Destroy(ManagerTransitions._instance);
            ManagerTransitions._instance = null;
        }

        _instance = this;
        DontDestroyOnLoad(this);
        Debug.Log("Created ManagerTransitions");

    }

    public void triggerLoadAsync(string nextScene)
    {
        StartCoroutine(this.loadScene(nextScene));
    }

    private IEnumerator loadScene(string nextScene)
    {
        Debug.Log("Loading next scene");
        AsyncOperation loading = SceneManager.LoadSceneAsync(nextScene);
        while (!loading.isDone)
        {
            Debug.Log((loading.progress * 100) + "% done");
            yield return null;
        }
        Debug.Log((loading.progress * 100) + "% done");
        Debug.Log(nextScene + " is loaded.");
    }

    public void triggerExit()
    {
#if UNITY_EDITOR
        UnityEditor.EditorApplication.isPlaying = false;
#else
        Application.Quit();
#endif
    }

}
