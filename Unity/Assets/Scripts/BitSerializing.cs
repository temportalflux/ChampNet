using System;
using System.Collections;
using System.Collections.Generic;
using System.Reflection;
using System.Linq;
using UnityEngine;

/// \author Dustin Yost
public class BitSerializing
{

    /// <summary>
    /// The interface to use <see cref="SerializationModule{T}"/> in collections without knowing the type.
    /// </summary>
    public interface ISerializationModule
    {
        /// <summary>
        /// Determine the size of some object - effectively sizeof(object).
        /// </summary>
        /// <param name="obj">The object.</param>
        /// <returns>int</returns>
        int GetSize(object obj);
        /// <summary>
        /// Serialize a specfic object into a byte array. Returns the byte[] with the populated data.
        /// </summary>
        /// <param name="obj">The object to serialize.</param>
        /// <param name="data">The byte array of data.</param>
        /// <param name="start">How far into the data to put the object's serialized data.</param>
        /// <returns>byte[]</returns>
        byte[] Serialize(object obj, byte[] data, int start);
        /// <summary>
        /// Deserialize a byte array into an object.
        /// </summary>
        /// <param name="data">the byte[] of data.</param>
        /// <param name="start">How far into the serialized data the object was put.</param>
        /// <returns>object</returns>
        object Deserialize(byte[] data, int start, Type type);
    }

    /// <summary>
    /// The implementation of <see cref="ISerializationModule"/>, where all the generic "object" fields are objects with the type <see cref="T"/>.
    /// </summary>
    /// <typeparam name="T">The generic type which this struct wraps the (De)Serialization of.</typeparam>
    public class SerializationModule<T> : ISerializationModule
    {
        /// <summary>
        /// A generic, and module settable, version of <see cref="ISerializationModule.GetSize(object)"/>.
        /// </summary>
        public Func<T, int> _GetSize;
        /// <summary>
        /// A generic, and module settable, version of <see cref="ISerializationModule.Serialize(object, byte[], int)"/>.
        /// </summary>
        public Func<T, byte[], int, byte[]> _Serialize;
        /// <summary>
        /// A generic, and module settable, version of <see cref="ISerializationModule.Deserialize(byte[], int)"/>.
        /// </summary>
        public Func<byte[], int, Type, T> _Deserialize;

        /// <summary>
        /// Determine the size of some object - effectively sizeof(object).
        /// </summary>
        /// <param name="obj">The object with the type <see cref="T"/>.</param>
        /// <returns>int</returns>
        public int GetSize(object obj) { return _GetSize((T)obj); }
        /// <summary>
        /// Serialize a specfic object into a byte array. Returns the byte[] with the populated data.
        /// </summary>
        /// <param name="obj">The object to serialize with the type <see cref="T"/>.</param>
        /// <param name="data">The byte array of data.</param>
        /// <param name="start">How far into the data to put the object's serialized data.</param>
        /// <returns>byte[]</returns>
        public byte[] Serialize(object obj, byte[] data, int start) { return _Serialize((T)obj, data, start); }
        /// <summary>
        /// Deserialize a byte array into an object with the type <see cref="T"/>.
        /// </summary>
        /// <param name="data">the byte[] of data.</param>
        /// <param name="start">How far into the serialized data the object was put.</param>
        /// <returns>object with the type <see cref="T"/></returns>
        public object Deserialize(byte[] data, int start, Type type) { return _Deserialize(data, start, type); }
    }

    /// <summary>
    /// A dictionary of (De)Serialization interfaces for handling how any one type should be handled.
    /// </summary>
    private static Dictionary<Type, ISerializationModule> modules = new Dictionary<Type, ISerializationModule>()
    {
        // Byte
        { typeof(Byte), new SerializationModule<Byte> {
            _GetSize = new Func<Byte, int>((Byte valueA) => { return 1; }),
            _Serialize = new Func<Byte, byte[], int, byte[]>((Byte valueB, byte[] data, int start) => {
                data[start] = valueB;
                return data;
            }),
            _Deserialize = new Func<byte[], int, Type, Byte>((byte[] data, int start, Type type) => {
                return data[start];
            }),
        } },
        // Boolean
        { typeof(Boolean), new SerializationModule<Boolean> {
            _GetSize = new Func<Boolean, int>((Boolean valueA) => { return 1; }),
            _Serialize = new Func<Boolean, byte[], int, byte[]>((Boolean valueB, byte[] data, int start) => {
                data[start] = (byte)(valueB ? 1 : 0);
                return data;
            }),
            _Deserialize = new Func<byte[], int, Type, Boolean>((byte[] data, int start, Type type) => {
                return data[start] > 0;
            }),
        } },
        // Char
        { typeof(Char), new SerializationModule<Char> {
            _GetSize = new Func<Char, int>((Char valueA) => { return 1; }),
            _Serialize = new Func<Char, byte[], int, byte[]>((Char valueB, byte[] data, int start) => {
                CopyTo(ref data, start, BitConverter.GetBytes(valueB));
                return data;
            }),
            _Deserialize = new Func<byte[], int, Type, Char>((byte[] data, int start, Type type) => {
                return BitConverter.ToChar(data, start);
            }),
        } },
        // Int16
        { typeof(Int16), new SerializationModule<Int16> {
            _GetSize = new Func<Int16, int>((Int16 valueA) => { return sizeof(Int16); }),
            _Serialize = new Func<Int16, byte[], int, byte[]>((Int16 valueB, byte[] data, int start) => {
                CopyTo(ref data, start, BitConverter.GetBytes(valueB));
                return data;
            }),
            _Deserialize = new Func<byte[], int, Type, Int16>((byte[] data, int start, Type type) => {
                return BitConverter.ToInt16(data, start);
            }),
        } },
        // Int32
        { typeof(Int32), new SerializationModule<Int32> {
            _GetSize = new Func<Int32, int>((Int32 valueA) => { return sizeof(Int32); }),
            _Serialize = new Func<Int32, byte[], int, byte[]>((Int32 valueB, byte[] data, int start) => {
                CopyTo(ref data, start, BitConverter.GetBytes(valueB));
                return data;
            }),
            _Deserialize = new Func<byte[], int, Type, Int32>((byte[] data, int start, Type type) => {
                return BitConverter.ToInt32(data, start);
            }),
        } },
        // Int64
        { typeof(Int64), new SerializationModule<Int64> {
            _GetSize = new Func<Int64, int>((Int64 valueA) => { return sizeof(Int64); }),
            _Serialize = new Func<Int64, byte[], int, byte[]>((Int64 valueB, byte[] data, int start) => {
                CopyTo(ref data, start, BitConverter.GetBytes(valueB));
                return data;
            }),
            _Deserialize = new Func<byte[], int, Type, Int64>((byte[] data, int start, Type type) => {
                return BitConverter.ToInt64(data, start);
            }),
        } },
        // UInt16
        { typeof(UInt16), new SerializationModule<UInt16> {
            _GetSize = new Func<UInt16, int>((UInt16 valueA) => { return sizeof(UInt16); }),
            _Serialize = new Func<UInt16, byte[], int, byte[]>((UInt16 valueB, byte[] data, int start) => {
                CopyTo(ref data, start, BitConverter.GetBytes(valueB));
                return data;
            }),
            _Deserialize = new Func<byte[], int, Type, UInt16>((byte[] data, int start, Type type) => {
                return BitConverter.ToUInt16(data, start);
            }),
        } },
        // UInt32
        { typeof(UInt32), new SerializationModule<UInt32> {
            _GetSize = new Func<UInt32, int>((UInt32 valueA) => { return sizeof(UInt32); }),
            _Serialize = new Func<UInt32, byte[], int, byte[]>((UInt32 valueB, byte[] data, int start) => {
                CopyTo(ref data, start, BitConverter.GetBytes(valueB));
                return data;
            }),
            _Deserialize = new Func<byte[], int, Type, UInt32>((byte[] data, int start, Type type) => {
                return BitConverter.ToUInt32(data, start);
            }),
        } },
        // UInt64
        { typeof(UInt64), new SerializationModule<UInt64> {
            _GetSize = new Func<UInt64, int>((UInt64 valueA) => { return sizeof(UInt64); }),
            _Serialize = new Func<UInt64, byte[], int, byte[]>((UInt64 valueB, byte[] data, int start) => {
                CopyTo(ref data, start, BitConverter.GetBytes(valueB));
                return data;
            }),
            _Deserialize = new Func<byte[], int, Type, UInt64>((byte[] data, int start, Type type) => {
                return BitConverter.ToUInt64(data, start);
            }),
        } },
        // Single
        { typeof(Single), new SerializationModule<Single> {
            _GetSize = new Func<Single, int>((Single valueA) => { return sizeof(Single); }),
            _Serialize = new Func<Single, byte[], int, byte[]>((Single valueB, byte[] data, int start) => {
                CopyTo(ref data, start, BitConverter.GetBytes(valueB));
                return data;
            }),
            _Deserialize = new Func<byte[], int, Type, Single>((byte[] data, int start, Type type) => {
                return BitConverter.ToSingle(data, start);
            }),
        } },
        // Double
        { typeof(Double), new SerializationModule<Double> {
            _GetSize = new Func<Double, int>((Double valueA) => { return sizeof(Double); }),
            _Serialize = new Func<Double, byte[], int, byte[]>((Double valueB, byte[] data, int start) => {
                CopyTo(ref data, start, BitConverter.GetBytes(valueB));
                return data;
            }),
            _Deserialize = new Func<byte[], int, Type, Double>((byte[] data, int start, Type type) => {
                return BitConverter.ToDouble(data, start);
            }),
        } },
        // String
        { typeof(string), new SerializationModule<string> {
            _GetSize = new Func<string, int>((string valueA) => { return sizeof(UInt32) + valueA.Length * sizeof(Char); }),
            _Serialize = new Func<string, byte[], int, byte[]>((string valueB, byte[] data, int start) => {
                SerializeArray<char>(valueB.ToCharArray(), ref data, start);
                return data;
            }),
            _Deserialize = new Func<byte[], int, Type, string>((byte[] data, int start, Type type) => {
                return new string(DeserializeArray<char>(data, start));
            }),
        } },
        // Vector3
        { typeof(Vector3), new SerializationModule<Vector3> {
            _GetSize = new Func<Vector3, int>((Vector3 valueA) => { return 3 * sizeof(Single); }),
            _Serialize = new Func<Vector3, byte[], int, byte[]>((Vector3 valueB, byte[] data, int start) => {
                ISerializationModule module = modules[typeof(Single)];
                module.Serialize(valueB.x, data, start);
                start += sizeof(Single);
                module.Serialize(valueB.y, data, start);
                start += sizeof(Single);
                module.Serialize(valueB.z, data, start);
                return data;
            }),
            _Deserialize = new Func<byte[], int, Type, Vector3>((byte[] data, int start, Type type) => {
                Type single = typeof(Single);
                ISerializationModule module = modules[single];
                float x = (float)modules[typeof(Single)].Deserialize(data, start, single);
                start += sizeof(Single);
                float y = (float)modules[typeof(Single)].Deserialize(data, start, single);
                start += sizeof(Single);
                float z = (float)modules[typeof(Single)].Deserialize(data, start, single);
                return new Vector3(x,y,z);
            }),
        } },
        // Color
        { typeof(Color), new SerializationModule<Color> {
            _GetSize = new Func<Color, int>((Color valueA) => { return 4 * sizeof(Single); }),
            _Serialize = new Func<Color, byte[], int, byte[]>((Color valueB, byte[] data, int start) => {
                ISerializationModule module = modules[typeof(Single)];
                module.Serialize(valueB.r, data, start);
                start += sizeof(Single);
                module.Serialize(valueB.g, data, start);
                start += sizeof(Single);
                module.Serialize(valueB.b, data, start);
                start += sizeof(Single);
                module.Serialize(valueB.a, data, start);
                return data;
            }),
            _Deserialize = new Func<byte[], int, Type, Color>((byte[] data, int start, Type type) => {
                Type single = typeof(Single);
                ISerializationModule module = modules[single];
                float r = (float)modules[typeof(Single)].Deserialize(data, start, single);
                start += sizeof(Single);
                float g = (float)modules[typeof(Single)].Deserialize(data, start, single);
                start += sizeof(Single);
                float b = (float)modules[typeof(Single)].Deserialize(data, start, single);
                start += sizeof(Single);
                float a = (float)modules[typeof(Single)].Deserialize(data, start, single);
                return new Color(r, g, b, a);
            }),
        } },
        // ISerializing
        { typeof(ISerializing), new SerializationModule<ISerializing> {
            _GetSize = new Func<ISerializing, int>((ISerializing valueA) => { return valueA.GetSize(); }),
            _Serialize = new Func<ISerializing, byte[], int, byte[]>((ISerializing valueB, byte[] data, int start) => {
                valueB.Serialize(ref data, ref start);
                return data;
            }),
            _Deserialize = new Func<byte[], int, Type, ISerializing>((byte[] data, int start, Type type) => {
                ISerializing obj = (ISerializing)Activator.CreateInstance(type, new object[] { });
                obj.Deserialize(data, ref start);
                return obj;
            }),
        } },
    };

    /// <summary>
    /// Write some byte array into another byte array at some offset.
    /// </summary>
    /// <param name="dest">The data to copy.</param>
    /// <param name="start">The offset in bytes.</param>
    /// <param name="source">The data object to copy into at the offset.</param>
    private static void CopyTo(ref byte[] dest, int start, byte[] source)
    {
        Array.Copy(source, 0, dest, start, source.Length);
    }

    /// <summary>
    /// Attempts to determine the size of some object (effectively sizeof(object))
    /// </summary>
    /// <param name="value">The object to size up</param>
    /// <returns>The size of some object, using sizeof for primitive objects, recursive behavior for arrays, and GetSize for ISerializing</returns>
    private static int GetSizeOf<T>(T value)
    {
        Type type = value.GetType();
        if (modules.ContainsKey(type))
        {
            return modules[type].GetSize(value);
        }
        return -1;
    }

    /// <summary>
    /// Serialize a specfic object into a byte array
    /// </summary>
    /// <param name="destination">The byte array of data</param>
    /// <param name="start">How far into the data to put the objects serialized data</param>
    /// <param name="value">tThe value to serialize</param>
    private static void Serialize<T>(ref byte[] data, int start, T value)
    {
        Type type = value.GetType();
        if (modules.ContainsKey(type))
        {
            data = modules[type].Serialize(value, data, start);
        }
        else if (type.IsArray)
        {
            SerializeArray(value, ref data, start, type.GetElementType());
        }
    }

    /// <summary>
    /// Deserialize a byte array into a monobehavior
    /// </summary>
    /// <param name="data">A byte array of data created with Serialize</param>
    /// <param name="start"></param>
    /// <returns>the deserialized object</returns>
    private static object Deserialize(byte[] data, int start, Type type)
    {
        if (modules.ContainsKey(type))
        {
            return modules[type].Deserialize(data, start, type);
        }
        else if (type.IsArray)
        {
            return DeserializeArray(data, start, type.GetElementType());
        }
        return null;
    }

    private static void SerializeArray<T>(T[] value, ref byte[] data, int start)
    {
        SerializeArray(value, ref data, start, typeof(T));
    }

    private static T[] DeserializeArray<T>(byte[] data, int start)
    {
        return (T[])DeserializeArray(data, start, typeof(T));
    }

    private static void SerializeArray(object value, ref byte[] data, int start, Type type)
    {
        // We can cast to ILIst because arrays implement it and we verfied that it is an array in the if statement
        System.Collections.IList fieldArray = (System.Collections.IList)value;

        // Write the size of the array
        Serialize(ref data, start, fieldArray.Count);
        start += GetSizeOf(fieldArray.Count);

        // Write all values
        for (int i = 0; i < fieldArray.Count; i++)
        {
            Serialize(ref data, start, fieldArray[i]);
            start += GetSizeOf(fieldArray[i]);
        }
    }

    private static object DeserializeArray(byte[] data, int start, Type type)
    {
        // Get the size of the array
        int size = (int)Deserialize(data, start, typeof(int));
        start += GetSizeOf(size);

        Array arr = System.Array.CreateInstance(type, size);

        // Iterate over all elements that will be deserializes
        for (int i = 0; i < size; i++)
        {
            object element = Deserialize(data, start, type);
            start += GetSizeOf(element);
            arr.SetValue(element, i);
        }

        return arr;
    }

    /// <summary>
    /// Serialize a monobehavior to a byte array, using BitSerialize and ISerializing
    /// </summary>
    /// <param name="mono">The monobehavior to serialize</param>
    /// <returns>A byte array of data formatted in the order of fields and extra data according to ISerializing</returns>
    public static byte[] Serialize<T>(T value)
    {
        // Find all the bitserializable fields in the object
        uint totalBitSize;
        List<KeyValuePair<FieldInfo, int>> attributeObjects = GetAttributeFields(value, out totalBitSize);
        int monoSerializingSize = -1;

        // If mono is ISerializing, then it could have extra data to serialize
        if (value is ISerializing)
        {
            // If there is more data to serialize
            monoSerializingSize = (value as ISerializing).GetSize();
            if (monoSerializingSize > 0)
            {
                // Add the bytes to the total size
                totalBitSize += (uint)monoSerializingSize;
            }
        }

        // Create the byte array of data
        byte[] data = new byte[totalBitSize];
        int start = 0;
        // Fill the data with the serialized fields
        for (int fieldIndex = 0; fieldIndex < attributeObjects.Count; fieldIndex++)
        {
            Serialize(ref data, start, attributeObjects[fieldIndex].Key.GetValue(value));
            // increment the total size of what is being serialized
            start += attributeObjects[fieldIndex].Value;
        }

        // If mono is ISerializing, then if it has additional bytes to serialize
        if (monoSerializingSize > 0)
        {
            // Serialize the remaining bytes
            (value as ISerializing).Serialize(ref data, ref start);
        }

        return data;
    }

    /// <summary>
    /// Deserialize a byte array into a monobehavior
    /// </summary>
    /// <param name="obj">The object to set data in</param>
    /// <param name="data">A byte array of data created with Serialize</param>
    public static void Deserialize<T>(T obj, byte[] data)
    {
        // Find all the bitserializable fields in the object
        uint totalBitSize;
        List<KeyValuePair<FieldInfo, int>> attributeObjects = GetAttributeFields(obj, out totalBitSize);

        int start = 0;

        // Extract the data with the serialized fields
        for (int fieldIndex = 0; fieldIndex < attributeObjects.Count; fieldIndex++)
        {
            // Extract the info and size
            FieldInfo info = attributeObjects[fieldIndex].Key;
            int size = attributeObjects[fieldIndex].Value;
            // Get the current value of the field
            //object fieldValue = info.GetValue(mono);
            Type type = info.FieldType;
            // Deserialize the info
            object value = Deserialize(data, start, type);
            start += attributeObjects[fieldIndex].Value;
            // Set the new value in the object
            info.SetValue(obj, value);
        }

        // If mono is ISerializing, it might need to deserialize any remaining bytes
        if (obj is ISerializing)
        {
            (obj as ISerializing).Deserialize(data, ref start);
        }

    }

    /// <summary>
    /// Gets all BitSerialize attribute fields in a monobehavior, calculating the size of each along the way
    /// </summary>
    /// <param name="value">The MonoBehaviour with BitSerialize fields</param>
    /// <param name="totalBitSize">The total bytes of the BitSerialize fields</param>
    /// <returns></returns>
    private static List<KeyValuePair<FieldInfo, int>> GetAttributeFields<T>(T value, out uint totalBitSize)
    {
        List<KeyValuePair<BitSerializeAttribute, int>> attributes = new List<KeyValuePair<BitSerializeAttribute, int>>();

        // Get the type of the object
        Type monoType = value.GetType();

        // Retreive the fields from the mono instance, sorted by declaration order
        FieldInfo[] objectFields = monoType.GetFields(
            BindingFlags.Instance | BindingFlags.Public // | BindingFlags.NonPublic
        );

        // search all fields and find the attribute [BitSerialize]
        for (int i = 0; i < objectFields.Length; i++)
        {
            BitSerializeAttribute attribute = Attribute.GetCustomAttribute(objectFields[i], typeof(BitSerializeAttribute)) as BitSerializeAttribute;

            // if we detect any attribute
            if (attribute != null)
            {
                attributes.Add(new KeyValuePair<BitSerializeAttribute, int>(attribute, i));
            }
        }

        attributes.Sort(new Comparison<KeyValuePair<BitSerializeAttribute, int>>((a, b) => { return (int)a.Key.order - (int)b.Key.order; }));

        List<KeyValuePair<FieldInfo, int>> fieldsAndSizes = new List<KeyValuePair<FieldInfo, int>>();
        totalBitSize = 0;

        foreach (KeyValuePair<BitSerializeAttribute, int> attributeEntry in attributes)
        {
            object fieldObj = objectFields[attributeEntry.Value].GetValue(value);

            // confirm fields are in the correct order
            //Debug.Log(objectFields[i].Name);

            // Get the sizeof the object
            int size = BitSerializeAttribute.GetSizeOf(fieldObj);
            // A valid size was found
            if (size >= 0)
            {
                totalBitSize += (uint)size;
                fieldsAndSizes.Add(new KeyValuePair<FieldInfo, int>(objectFields[attributeEntry.Value], size));
            }
            // the size returned was < 0, so it was an invalid attribute
            else
            {
                Debug.Log("Could not serialize field " + monoType.Name + "#" + objectFields[attributeEntry.Value].Name + " even though it is marked as bitserialize");
            }
        }

        return fieldsAndSizes;
    }

}
