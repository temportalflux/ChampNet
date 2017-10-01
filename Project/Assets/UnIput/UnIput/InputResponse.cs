using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Events;

public class InputResponse : MonoBehaviour {

    [System.Serializable]
    public struct GamepadBinding
    {

        [System.Serializable]
        public enum Type
        {
            BUTTON, AXIS, TRIGGER
        }

        [System.Serializable]
        public enum UpdateEvent
        {
            DOWN, UP, TICK
        }

        [Tooltip("The mapping for the binding")]
        public string action;

        public Type type;

        [Tooltip("The button binding")]
        public GamepadButton gamepadButton;

        [Tooltip("The button binding")]
        public MappedButton mappedButton;

        [Tooltip("The button binding")]
        public GamepadAxis gamepadAxis;

        [Tooltip("The button binding")]
        public MappedAxis mappedAxis;

        [Tooltip("The button binding")]
        public GamepadTrigger gamepadTrigger;

        public void Update(GamepadDevice gamepad, System.Action<UpdateEvent, string, float> fireUpdateEvent)
        {
            switch (this.type)
            {
                case Type.BUTTON:
                    if (gamepad.GetButtonDown(this.gamepadButton))
                    {
                        fireUpdateEvent(UpdateEvent.DOWN, this.action, 0f);
                    }
                    if (gamepad.GetButton(this.gamepadButton))
                    {
                        fireUpdateEvent(UpdateEvent.TICK, this.action, 0f);
                    }
                    if (gamepad.GetButtonUp(this.gamepadButton))
                    {
                        fireUpdateEvent(UpdateEvent.UP, this.action, 0f);
                    }
                    break;
                case Type.AXIS:
                    fireUpdateEvent(UpdateEvent.TICK, this.action, gamepad.GetAxis(this.gamepadAxis));
                    break;
                case Type.TRIGGER:
                    fireUpdateEvent(UpdateEvent.TICK, this.action, gamepad.GetTrigger(this.gamepadTrigger));
                    break;
                default:
                    break;
            }
        }

        public void Update(InputDevice gamepad, System.Action<UpdateEvent, string, float> fireUpdateEvent)
        {
            switch (this.type)
            {
                case Type.BUTTON:
                    if (gamepad.GetButtonDown(this.mappedButton))
                    {
                        fireUpdateEvent(UpdateEvent.DOWN, this.action, 0f);
                    }
                    if (gamepad.GetButton(this.mappedButton))
                    {
                        fireUpdateEvent(UpdateEvent.TICK, this.action, 0f);
                    }
                    if (gamepad.GetButtonUp(this.mappedButton))
                    {
                        fireUpdateEvent(UpdateEvent.UP, this.action, 0f);
                    }
                    break;
                case Type.AXIS:
                    fireUpdateEvent(UpdateEvent.TICK, this.action, gamepad.GetAxis(this.mappedAxis));
                    break;
                default:
                    break;
            }
        }

    }

    [System.Serializable]
    public struct EventType
    {
        public GamepadBinding.UpdateEvent type;
        public string action;
    }

    [System.Serializable]
    public struct Listener
    {
        [System.Serializable]
        public class Event : UnityEvent<GamepadBinding.UpdateEvent, string, float> { }

        public EventType[] events;
        public Event action;

        public bool listensTo(GamepadBinding.UpdateEvent type, string action)
        {
            // O(n)
            foreach (EventType evt in this.events)
            {
                if (evt.type == type && evt.action == action)
                {
                    return true;
                }
            }
            return false;
        }


    }

    [Tooltip("The main input manager")]
    public GamepadInput input;

    [Range(1, 4)]
    [Tooltip("The controller number")]
    public int inputId;

    [Tooltip("The action bindings")]
    public GamepadBinding[] bindings;

    public Listener[] listeners;

    public InputResponse()
    {
    }

    public InputResponse(GamepadInput input, int inputId)
    {
        this.input = input;
        this.inputId = inputId;
    }

    public void addBinding(GamepadBinding obj)
    {
        this.bindings = this.add(obj, this.bindings);
    }

    private T[] add<T>(T obj, T[] old)
    {
        List<T> newObjs = new List<T>(old);
        newObjs.Add(obj);
        return newObjs.ToArray();
    }

    public void Update()
    {

        InputDevice active = MappedInput.activeDevice;
        GamepadDevice gamepad = this.getGamepad();
        if (this.bindings.Length > 0)
        {
            foreach (GamepadBinding binding in this.bindings)
            {

                // Update for gamepads
                if (gamepad != null)
                {
                    binding.Update(gamepad, this.onBindingUpdate);
                }

                // Update for the active device
                if (active != null)
                {
                    binding.Update(active, this.onBindingUpdate);
                }

            }
        }

    }

    private GamepadDevice getGamepad()
    {
        return this.inputId > 0 && this.inputId <= input.gamepads.Count ? input.gamepads[this.inputId - 1] : null;
    }

    private void onBindingUpdate(GamepadBinding.UpdateEvent type, string action, float value)
    {
        foreach (Listener listener in this.listeners)
        {
            if (listener.listensTo(type, action))
            {
                listener.action.Invoke(type, action, value);
            }
        }
    }

}
