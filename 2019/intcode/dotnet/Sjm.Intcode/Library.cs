using System;
using System.Runtime.InteropServices;

namespace Sjm.Intcode
{
    public static class Library
    {
        [DllImport("intcode")]
        public static extern int ic_run(IntPtr vm, IntPtr log);
        [DllImport("intcode")]
        public static extern int ic_step(IntPtr vm, IntPtr log);
        [DllImport("intcode")]
        public static extern void ic_load(IntPtr vm, IntPtr f);
        [DllImport("intcode")]
        public static extern void ic_decode(IntPtr vm, int IntPtr f);
        [DllImport("intcode")]
        public static extern IntPtr ic_deref(Icvm)
    }
}
