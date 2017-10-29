using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

/** Author: Dustin Yost
 * Static component class which handles transitioning between scenes
 */
public class ManagerTransitions : MonoBehaviour {

    // The static instance of this class
    private static ManagerTransitions _instance;

    // The static getter for the instance var
    public static ManagerTransitions INSTANCE
    {
        get
        {
            return _instance;
        }
    }
    
    // Mutex for handling if there is a transition in use
    private bool inUse;

    private Coroutine routDisplay, routLoad;

    Transition sceneTransition;

    // variable to store the loading of some scene asynchonously - only non-null while inUse is true
    AsyncOperation sceneLoading;

    bool _allowSceneActivation = false;
    bool allowSceneActivation
    {
        set
        {
            if (this.sceneLoading != null)
                this.sceneLoading.allowSceneActivation = value;
            this._allowSceneActivation = value;
        }
        get
        {
            return this.sceneLoading != null ? this.sceneLoading.allowSceneActivation : this._allowSceneActivation;
        }
    }

    /**
     * Handles caching of the class instance to the static variable
     */
    private void Awake()
    {

        // Check if there is already an instance (this is an error)
        if (ManagerTransitions._instance != null)
        {
            // Tell debugger there was a user error
            Debug.LogError("ManagerTransitions already created... discarding old.");
            // Destroy the old instance
            Destroy(ManagerTransitions._instance);
            ManagerTransitions._instance = null;
        }

        // Cache this instance & mark as not to be destroyed
        _instance = this;
        DontDestroyOnLoad(this);

        // Tell debugger the instance was created
        Debug.Log("Created ManagerTransitions");

    }

    /**
     * Handles on startup caching
     */
    private void Start()
    {

        this.inUse = false;
        this.sceneLoading = null;

    }

    /**
     * Trigger a loading of some scene asynchronously with some transition
     */
    public bool triggerLoadAsync(string nextScene, Transition transition)
    {
        // Check to see if a transition is already occuring
        if (!this.inUse)
        {
            // Mark as in transition
            this.inUse = true;

            // Trigger the coroutines

            // Begin the visual transition
            this.routDisplay = StartCoroutine(this.displayTransition(transition));

            // Begin the level loading
            this.routLoad = StartCoroutine(this.loadScene(nextScene));

            // Add check for merging routines
            StartCoroutine(this.checkDisplayLoad());

            return true;
        }
        return false;
    }

    private IEnumerator displayTransition(Transition transition)
    {
        //Debug.Log("Displaying transition");

        this.sceneTransition = transition;

        // Begin transition display
        this.sceneTransition.forwards();

        // Wait while the transition is running
        while (this.sceneTransition.isAnimating())
        {
            yield return null;
        }

        // Clean the variable
        this.routDisplay = null;
    }

    private IEnumerator loadScene(string nextScene)
    {
        // Tell debugger the scene is loading
        //Debug.Log("Loading next scene");

        if (nextScene != null)
        {
            // Start the scene loading
            this.sceneLoading = SceneManager.LoadSceneAsync(nextScene);
        }

        // Prevent scene loading from finishing
        this.allowSceneActivation = false;

        do
        {
            //Debug.Log((this.sceneLoading.progress * 100) + "% done");
            yield return null;
        } while (!(this.allowSceneActivation && (this.sceneLoading == null || this.sceneLoading.isDone)));

        //Debug.Log((this.sceneLoading.progress * 100) + "% done");

        if (nextScene != null)
        {
            //Debug.Log(nextScene + " is loaded.");
        }
        else
        {
            this.triggerExit();
        }

        this.routLoad = null;
    }

    IEnumerator checkDisplayLoad()
    {
        bool bothHaveFinished = false;

        while (!bothHaveFinished)
        {
            // Display transition has finished
            if (this.routDisplay == null)
            {
                // Allow the scene to finish running
                this.allowSceneActivation = true;

                // Display must finish first, then scene activation can be checked
                if (this.routLoad == null)
                {
                    // the async loading finished after allowing the scene to activate
                    bothHaveFinished = true;
                }

            }

            yield return null;
        }

        // clear the mutex
        this.inUse = false;

        // clean the coroutine
        //this.routMain = null;
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
