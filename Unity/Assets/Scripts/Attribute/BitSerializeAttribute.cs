/*
Names and ID: Christopher Brennan: 1028443, Dustin Yost: 0984932, Jacob Ruth: 0890406
Course Info: EGP-405-01 
Project Name: Project 3: Synchronized Networking
Due: 11/22/17
Certificate of Authenticity (standard practice): “We certify that this work is entirely our own.  
The assessor of this project may reproduce this project and provide copies to other academic staff, 
and/or communicate a copy of this project to a plagiarism-checking service, which may retain a copy of the project on its database.”
*/
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
using UnityEngineInternal;
using UnityEditorInternal;
using System;
using System.Reflection;
using System.Linq;

/// <summary>
/// Handles (de)serializing of specific fields.
/// Valid types include: bool, byte, char, (u)short, (u)int, (u)long, float, double, ISerializing, and any composite arrays of the former.
/// If the <see cref="MonoBehaviour"/> being serialized is ISerializing, the ISerializing methods are treated as additions to all fields marked with BitSerialize
/// </summary>
[AttributeUsage(AttributeTargets.Field)]
public class BitSerializeAttribute : Attribute
{

    /// <summary>
    /// A dictionary of primitive types to their primitive sizes
    /// </summary>
    private static Dictionary<Type, Int32> primitiveSizes = new Dictionary<Type, int>()
    {
        {typeof(Boolean), sizeof(Boolean)}, // bool
        {typeof(Byte), 1}, // byte
        {typeof(Char), sizeof(Char)}, // char
        {typeof(Int16), sizeof(Int16)}, // short
        {typeof(Int32), sizeof(Int32)}, // int
        {typeof(Int64), sizeof(Int64)}, // long
        {typeof(UInt16), sizeof(UInt16)}, // ushort
        {typeof(UInt32), sizeof(UInt32)}, // uint
        {typeof(UInt64), sizeof(UInt64)}, // ulong
        {typeof(Single), sizeof(Single)}, // float
        {typeof(Double), sizeof(Double)}, // double
        {typeof(Color), sizeof(Single)*3}, // color
    };

    /// <summary>
    /// Serialize a monobehavior to a byte array, using BitSerialize and ISerializing
    /// </summary>
    /// <param name="mono">The monobehavior to serialize</param>
    /// <returns>A byte array of data formatted in the order of fields and extra data according to ISerializing</returns>
    public static byte[] Serialize<T>(T mono)
    {
        // Find all the bitserializable fields in the object
        uint totalBitSize;
        List<KeyValuePair<FieldInfo, int>> attributeObjects = GetAttributeFields(mono, out totalBitSize);
        int monoSerializingSize = -1;

        // If mono is ISerializing, then it could have extra data to serialize
        if (mono is ISerializing)
        {
            // If there is more data to serialize
            monoSerializingSize = (mono as ISerializing).GetSize();
            if (monoSerializingSize > 0)
            {
                // Add the bytes to the total size
                totalBitSize += (uint)monoSerializingSize;
            }
        }

        // Create the byte array of data
        byte[] data = new byte[totalBitSize];
        int offset = 0;
        // Fill the data with the serialized fields
        for (int fieldIndex = 0; fieldIndex < attributeObjects.Count; fieldIndex++)
        {
            Serialize(ref data, ref offset, attributeObjects[fieldIndex].Key.GetValue(mono));
        }

        // If mono is ISerializing, then if it has additional bytes to serialize
        if (monoSerializingSize > 0)
        {
            // Serialize the remaining bytes
            (mono as ISerializing).Serialize(ref data, ref offset);
        }

        return data;
    }

    /// <summary>
    /// Gets all BitSerialize attribute fields in a monobehavior, calculating the size of each along the way
    /// </summary>
    /// <param name="mono">The MonoBehaviour with BitSerialize fields</param>
    /// <param name="totalBitSize">The total bytes of the BitSerialize fields</param>
    /// <returns></returns>
    private static List<KeyValuePair<FieldInfo, int>> GetAttributeFields<T>(T mono, out uint totalBitSize)
    {
        List<KeyValuePair<FieldInfo, int>> attributes = new List<KeyValuePair<FieldInfo, int>>();
        totalBitSize = 0;

        // Get the type of the object
        Type monoType = mono.GetType();

        // Retreive the fields from the mono instance, sorted by declaration order
        FieldInfo[] objectFields = monoType.GetFields(
            BindingFlags.Instance | BindingFlags.Public // | BindingFlags.NonPublic
        );//.OrderBy(f => f.MetadataToken).ToArray();
        List<FieldInfo> objectFieldList = new List<FieldInfo>(objectFields);
        objectFieldList.Sort(new Comparison<FieldInfo>((a, b) => { return b.MetadataToken - a.MetadataToken; }));
        objectFields = objectFieldList.ToArray();

        // search all fields and find the attribute [BitSerialize]
        for (int i = 0; i < objectFields.Length; i++)
        {
            BitSerializeAttribute attribute = Attribute.GetCustomAttribute(objectFields[i], typeof(BitSerializeAttribute)) as BitSerializeAttribute;

            // if we detect any attribute
            if (attribute != null)
            {
                object fieldObj = objectFields[i].GetValue(mono);

                // confirm fields are in the correct order
                //Debug.Log(objectFields[i].Name);

                // Get the sizeof the object
                int size = BitSerializeAttribute.GetSizeOf(fieldObj);
                // A valid size was found
                if (size >= 0)
                {
                    totalBitSize += (uint)size;
                    attributes.Add(new KeyValuePair<FieldInfo, int>(objectFields[i], size));
                }
                // the size returned was < 0, so it was an invalid attribute
                else
                {
                    Debug.Log("Could not serialize field " + monoType.Name + "#" + objectFields[i].Name + " even though it is marked as bitserialize");
                }
            }
        }

        return attributes;
    }

    /// <summary>
    /// Attempts to determine the size of some object (effectively sizeof(object))
    /// </summary>
    /// <param name="value">The object to size up</param>
    /// <returns>The size of some object, using sizeof for primitive objects, recursive behavior for arrays, and GetSize for ISerializing</returns>
    public static int GetSizeOf(object value)
    {
        if (value == null)
        {
            return 0;
        }
        Type type = value.GetType();
        if (type == typeof(string))
        {
            return sizeof(int) + (value as string).Length * sizeof(char);
        }
        // The passed value is an array of some type T
        else if (type.IsArray)
        {
            // We can cast to ILIst because arrays implement it and we verfied that it is an array in the if statement
            System.Collections.IList fieldArray = (System.Collections.IList)value;
            // The value has no entries, so it has no data
            if (fieldArray.Count <= 0)
            {
                return sizeof(int);
            }
            // the value has entries, so it has a size
            else
            {
                // multiply the size of the inner type by the number of entries (there is at least 1)
                int entrySize = GetSizeOf(fieldArray[0]);
                if (entrySize >= 0)
                {
                    return sizeof(int) + fieldArray.Count * entrySize;
                }
                else
                {
                    return -1;
                }
            }
        }
        // It is not an array, try primitive
        else if (BitSerializeAttribute.primitiveSizes.ContainsKey(type))
        {
            return BitSerializeAttribute.primitiveSizes[type];
        }
        else if (typeof(ISerializing).IsAssignableFrom(type))
        {
            return (value as ISerializing).GetSize();
        }
        // Cannot determine size
        else
        {
            Debug.LogError("Could not determine size of " + type);
            return -1;
        }
    }

    /// <summary>
    /// Serialize a specfic object into a byte array
    /// </summary>
    /// <param name="destination">The byte array of data</param>
    /// <param name="offset">How far into the data to put the objects serialized data</param>
    /// <param name="value">tThe value to serialize</param>
    private static void Serialize(ref byte[] destination, ref int offset, object value)
    {
        if (value == null)
        {
            return;
        }
        Type type = value.GetType();
        // If it is a string
        if (type == typeof(string))
        {
            Serialize(ref destination, ref offset, (value as string).ToCharArray());
        }
        // The passed value is an array of some type T
        else if (type.IsArray)
        {
            // We can cast to ILIst because arrays implement it and we verfied that it is an array in the if statement
            System.Collections.IList fieldArray = (System.Collections.IList)value;

            // Write the size of the array
            CopyTo(ref destination, ref offset, System.BitConverter.GetBytes(fieldArray.Count));

            // Write all values
            for (int i = 0; i < fieldArray.Count; i++)
            {
                Serialize(ref destination, ref offset, fieldArray[i]);
            }
        }
        // It is not an array, try primitive
        else if (BitSerializeAttribute.primitiveSizes.ContainsKey(type))
        {
            CopyTo(ref destination, ref offset, SerializePrimitive(value, type));
        }
        else if (typeof(ISerializing).IsAssignableFrom(type))
        {
            (value as ISerializing).Serialize(ref destination, ref offset);
        }
    }

    /// <summary>
    /// Converts some primitive object into an array of bytes
    /// </summary>
    /// <param name="value">The PRIMITIVE object to convert to bytes via System.BitConverter.GetBytes</param>
    /// <param name="type">The type of the primitive</param>
    /// <returns></returns>
    private static byte[] SerializePrimitive(object value, Type type = null)
    {
        if (type == null) type = value.GetType();
        if (type == typeof(Boolean)) return System.BitConverter.GetBytes((Boolean)value);
        else if (type == typeof(Byte)) return new byte[] { (byte)value };
        else if (type == typeof(Char)) return System.BitConverter.GetBytes((Char)value);
        else if (type == typeof(Int16)) return System.BitConverter.GetBytes((Int16)value);
        else if (type == typeof(Int32)) return System.BitConverter.GetBytes((Int32)value);
        else if (type == typeof(Int64)) return System.BitConverter.GetBytes((Int64)value);
        else if (type == typeof(UInt16)) return System.BitConverter.GetBytes((UInt16)value);
        else if (type == typeof(UInt32)) return System.BitConverter.GetBytes((UInt32)value);
        else if (type == typeof(UInt64)) return System.BitConverter.GetBytes((UInt64)value);
        else if (type == typeof(Single)) return System.BitConverter.GetBytes((Single)value);
        else if (type == typeof(Double)) return System.BitConverter.GetBytes((Double)value);
        else if (type == typeof(Color))
            return SerializePrimitive(((Color)value).r) // serialize red
                .Concat(SerializePrimitive(((Color)value).g)) // serialize green
                .Concat(SerializePrimitive(((Color)value).b)) // serialize blue
                .ToArray();
        else return null;
    }

    /// <summary>
    /// Write some byte array into another byte array at some offset
    /// </summary>
    /// <param name="dest">The data to copy.</param>
    /// <param name="offset">The offset in bytes.</param>
    /// <param name="source">The data object to copy into at the offset.</param>
    /// <remarks>
    /// Author: Dustin Yost
    /// </remarks>
    public static void CopyTo(ref byte[] dest, ref int offset, byte[] source)
    {
        // copy all data from the source to the destination, starting at some offset
        System.Array.Copy(source, 0, dest, offset, source.Length);
        offset += source.Length;
    }

    /// <summary>
    /// Deserialize a byte array into a monobehavior
    /// </summary>
    /// <param name="mono">The monobehavior to set data in</param>
    /// <param name="data">A byte array of data created with Serialize</param>
    public static void Deserialize<T>(T mono, byte[] data)
    {
        // Find all the bitserializable fields in the object
        uint totalBitSize;
        List<KeyValuePair<FieldInfo, int>> attributeObjects = GetAttributeFields(mono, out totalBitSize);

        int offset = 0;

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
            object value;
            Deserialize(data, ref offset, type, out value);
            // Set the new value in the object
            info.SetValue(mono, value);
        }

        // If mono is ISerializing, it might need to deserialize any remaining bytes
        if (mono is ISerializing)
        {
            (mono as ISerializing).Deserialize(data, ref offset);
        }

    }
    
    /// <summary>
    /// Deserialize data from a byte array into a specific type
    /// </summary>
    /// <param name="data">The data to deserialize from</param>
    /// <param name="offset">The offset which the data is at (incremented the appropriate size of the value)</param>
    /// <param name="type">The type of the value to deserialze as</param>
    /// <param name="obj">The object to which the data is deserialized into (as an object of type 'type')</param>
    private static void Deserialize(byte[] data, ref int offset, Type type, out object obj)
    {
        obj = null;
        if (type == null)
        {
            return;
        }
        // The passed value is an array of some type T
        if (type == typeof(string))
        {
            // Get the size of the array
            object arraySizeObj;
            int arraySize = 0;
            Deserialize(data, ref offset, typeof(int), out arraySizeObj);
            arraySize = (int)arraySizeObj;

            string str = "";

            // Iterate over all elements that will be deserializes
            for (int i = 0; i < arraySize; i++)
            {
                object element;
                Deserialize(data, ref offset, typeof(char), out element);
                str += (char)element;
            }

            obj = str;
        }
        else if (type.IsArray)
        {
            // Get the size of the array
            object arraySizeObj;
            int arraySize = 0;
            Deserialize(data, ref offset, typeof(int), out arraySizeObj);
            arraySize = (int)arraySizeObj;

            // The type of the array (T), such that obj = T[]
            Type subtype = type.GetElementType();

            Array arr = System.Array.CreateInstance(subtype, arraySize);

            // Iterate over all elements that will be deserializes
            for (int i = 0; i < arraySize; i++)
            {
                object element;
                Deserialize(data, ref offset, subtype, out element);
                arr.SetValue(element, i);
            }

            obj = arr;
        }
        // It is not an array, try primitive
        else if (BitSerializeAttribute.primitiveSizes.ContainsKey(type))
        {
            obj = DeserializePrimitive(data, ref offset, type);
        }
        else if (typeof(ISerializing).IsAssignableFrom(type))
        {
            // Requires an empty constructor
            obj = Activator.CreateInstance(type, new object[] { });
            (obj as ISerializing).Deserialize(data, ref offset);
        }
    }

    private static object DeserializePrimitive(byte[] data, ref int offset, Type type)
    {
        object obj = null;
        if (type == typeof(Boolean)) { obj = System.BitConverter.ToBoolean(data, offset); offset += sizeof(Boolean); }
        else if (type == typeof(Byte)) { obj = data[offset]; offset += 1; }
        else if (type == typeof(Char)) { obj = System.BitConverter.ToChar(data, offset); offset += sizeof(Char); }
        else if (type == typeof(Int16)) { obj = System.BitConverter.ToInt16(data, offset); offset += sizeof(Int16); }
        else if (type == typeof(Int32)) { obj = System.BitConverter.ToInt32(data, offset); offset += sizeof(Int32); }
        else if (type == typeof(Int64)) { obj = System.BitConverter.ToInt64(data, offset); offset += sizeof(Int64); }
        else if (type == typeof(UInt16)) { obj = System.BitConverter.ToUInt16(data, offset); offset += sizeof(UInt16); }
        else if (type == typeof(UInt32)) { obj = System.BitConverter.ToUInt32(data, offset); offset += sizeof(UInt32); }
        else if (type == typeof(UInt64)) { obj = System.BitConverter.ToUInt64(data, offset); offset += sizeof(UInt64); }
        else if (type == typeof(Single)) { obj = System.BitConverter.ToSingle(data, offset); offset += sizeof(Single); }
        else if (type == typeof(Double)) { obj = System.BitConverter.ToDouble(data, offset); offset += sizeof(Double); }
        else if (type == typeof(Color))
        {
            obj = new Color(
                (Single)DeserializePrimitive(data, ref offset, typeof(Single)), // r
                (Single)DeserializePrimitive(data, ref offset, typeof(Single)), // g
                (Single)DeserializePrimitive(data, ref offset, typeof(Single)) // b
            );
        }
        return obj;
    }

}
