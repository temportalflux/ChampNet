﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Events;

/**
 * Searches for updates from input controllers and sends to listeners
 */
public class InputResponse : MonoBehaviour {

    // The different types of updates available
    [System.Serializable]
    public enum UpdateEvent
    {
        // Never triggered, do not use unless indicating null
        NONE,
        // When a button is initially pressed
        DOWN,
        // When a button is initially released
        UP,
        // When a button or axis/trigger is being held
        TICK,
    }

    // The listener class for handling all button effects
    [System.Serializable]
    public class Listener<T>
    {
        [Tooltip("The type of update desired (not None)")]
        public UpdateEvent type;

        [Tooltip("The button or axis being pressed")]
        public T key;
    }

    // The listener class for buttons (required for UnityEvents)
    [System.Serializable]
    public class ListenerButton : Listener<MappedButton>
    {

        [System.Serializable]
        public class InputEvent : UnityEvent<UpdateEvent, MappedButton> { }

        [Tooltip("The action to perform when a button is changed")]
        public InputEvent action;

    }

    // The listener class for axes (required for UnityEvents)
    [System.Serializable]
    public class ListenerAxis : Listener<MappedAxis>
    {

        [System.Serializable]
        public class InputEvent : UnityEvent<UpdateEvent, MappedAxis, float> { }

        [Tooltip("The action to perform when a axis is changed")]
        public InputEvent action;

    }

    [Tooltip("The main input manager")]
    public GamepadInput input;

    [Range(1, 4)]
    [Tooltip("The controller number")]
    public int inputId = 1;

    [Tooltip("The list of listeners for buttons")]
    public ListenerButton[] listenerButtons;
    [Tooltip("The list of listeners for axes")]
    public ListenerAxis[] listenerAxes;
    
    // The dictionary mapping of listeners for buttons from Update type -> Button -> Listener action
    private Dictionary<UpdateEvent, Dictionary<MappedButton, List<ListenerButton>>> dictListenerButtons;
    // The dictionary mapping of listeners for axes from Update type -> Button -> Listener action
    private Dictionary<UpdateEvent, Dictionary<MappedAxis, List<ListenerAxis>>> dictListenerAxes;

    private void Start()
    {
        // Create the dictionaries
        this.dictListenerButtons = new Dictionary<UpdateEvent, Dictionary<MappedButton, List<ListenerButton>>>();
        this.dictListenerAxes = new Dictionary<UpdateEvent, Dictionary<MappedAxis, List<ListenerAxis>>>();

        // Populate the dictionaries from the arrays
        this.forSet(this.listenerButtons,
            (ListenerButton listener) =>
            {
                // ensure there is a mapping for the update type
                if (!this.dictListenerButtons.ContainsKey(listener.type))
                    this.dictListenerButtons.Add(listener.type, new Dictionary<MappedButton, List<ListenerButton>>());
                // ensure there is a mapping for the button
                if (!this.dictListenerButtons[listener.type].ContainsKey(listener.key))
                    this.dictListenerButtons[listener.type].Add(listener.key, new List<ListenerButton>());
                // ensure there is a mapping for the action
                if (!this.dictListenerButtons[listener.type][listener.key].Contains(listener))
                    this.dictListenerButtons[listener.type][listener.key].Add(listener);
            }
        );
        this.forSet(this.listenerAxes,
            (ListenerAxis listener) =>
            {
                // ensure there is a mapping for the update type
                if (!this.dictListenerAxes.ContainsKey(listener.type))
                    this.dictListenerAxes.Add(listener.type, new Dictionary<MappedAxis, List<ListenerAxis>>());
                // ensure there is a mapping for the axis
                if (!this.dictListenerAxes[listener.type].ContainsKey(listener.key))
                    this.dictListenerAxes[listener.type].Add(listener.key, new List<ListenerAxis>());
                // ensure there is a mapping for the action
                if (!this.dictListenerAxes[listener.type][listener.key].Contains(listener))
                    this.dictListenerAxes[listener.type][listener.key].Add(listener);
            }
        );
    }

    /**
     * Iterate through any set and do some action
     */
    private void forSet<T>(IEnumerable<T> set, UnityAction<T> iteration) {
        foreach (T item in set)
        {
            iteration.Invoke(item);
        }
    }

    public void Update()
    {
        // Check for all inputs
        MappedInput.inputDevices.ForEach(this.updateInput);
    }

    // Check for updates in some input
    void updateInput(InputDevice device)
    {
        UpdateEvent eventType;
        float value;

        // check all mappings currently being tracked
        foreach (MappedButton mapping in this.dictListenerButtons.Keys)
        {
            eventType = UpdateEvent.NONE;

            // Get the appropriate event
            if (device.GetButtonDown(mapping))
            {
                eventType = UpdateEvent.DOWN;
            }
            else if (device.GetButtonUp(mapping))
            {
                eventType = UpdateEvent.UP;
            }
            else if (device.GetButton(mapping))
            {
                eventType = UpdateEvent.TICK;
            }

            // Send the event to the listeners for said update type and mapping
            if (eventType != UpdateEvent.NONE &&
                this.dictListenerButtons.ContainsKey(eventType) &&
                this.dictListenerButtons[eventType].ContainsKey(mapping))
            {
                this.forSet(this.dictListenerButtons[eventType][mapping],
                    (ListenerButton listener) => { listener.action.Invoke(eventType, mapping); }
                );
            }
        }

        // check all mappings currently being tracked
        foreach (MappedAxis mapping in this.dictListenerAxes.Keys)
        {
            eventType = UpdateEvent.TICK;
            value = device.GetAxis(mapping);

            // Send the event to the listeners for said update type and mapping
            if (eventType != UpdateEvent.NONE &&
               this.dictListenerAxes.ContainsKey(eventType) &&
               this.dictListenerAxes[eventType].ContainsKey(mapping))
            {
                this.forSet(this.dictListenerAxes[eventType][mapping],
                    (ListenerAxis listener) => { listener.action.Invoke(eventType, mapping, value); }
                );
            }
        }

    }

}
