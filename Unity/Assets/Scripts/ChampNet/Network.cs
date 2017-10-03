using UnityEngine;
using System.Runtime.InteropServices;

namespace ChampNetPlugin
{

    public class Network
    {

        const string IDENTIFIER = "ChampNetPlugin";

        [DllImport(IDENTIFIER)]
        public static extern int Create();

        [DllImport(IDENTIFIER)]
        public static extern int Destroy();

        [DllImport(IDENTIFIER)]
        public static extern int GetRandom();

    }

}
