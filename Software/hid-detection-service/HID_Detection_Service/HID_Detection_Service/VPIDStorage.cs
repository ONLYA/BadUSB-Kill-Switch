using System;
using System.Collections.Generic;

namespace WorkerService1
{
    class VPIDStorage
    {
        public List<Dictionary<string, int>> VPID = new List<Dictionary<string, int>>();
        public int hid_count;
        public VPIDStorage(int count)
        {
            hid_count = count;
        }
        public Boolean AddVPID(int vid, int pid, out Dictionary<string, int> dict)
        {
            Dictionary<string, int> temp;
            if (!CheckContainVPID(vid, pid, out temp))
            {
                var d = new Dictionary<string, int> { { "vid", vid }, { "pid", pid }, { "count", 1 }, { "total_count", -1 } };
                VPID.Add(d);
                dict = d;
                return true;
            }
            dict = null;
            return false;
        }
        public Boolean CheckContainVPID(int vid, int pid, out Dictionary<string, int> dict)
        {
            foreach (Dictionary<string, int> a in VPID)
            {
                if (a["vid"] == vid && a["pid"] == pid)
                {
                    dict = a;
                    return true;
                }
            }
            dict = null;
            return false;
        }
        public void ClearAll()
        {
            VPID.Clear();
        }
    }
}
