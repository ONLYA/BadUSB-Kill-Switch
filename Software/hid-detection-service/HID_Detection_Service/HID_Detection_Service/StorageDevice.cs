using System;
using System.Collections.Generic;
using System.Text;
using HidSharp;

namespace WorkerService1
{
    class StorageDevice
    {
        private Dictionary<string, List<HidDevice>> deviceDict = new Dictionary<string, List<HidDevice>>();
        private Dictionary<string, int> countDict = new Dictionary<string, int>();
        public StorageDevice() { }
        public void Add(HidDevice device)
        {
            string key = $"{device.VendorID}&{device.ProductID}";
            if (CheckExist(device))
            {
                return;
            }
            else
            {
                if (deviceDict.ContainsKey(key))
                {
                    deviceDict[key].Add(device);
                }
                else
                {
                    deviceDict.Add(key, new List<HidDevice> { device });
                    countDict.Add(key, 1);
                }
            }
        }
        public Boolean CheckExist(HidDevice device)
        {
            string key = $"{device.VendorID}&{device.ProductID}";
            if (deviceDict.ContainsKey(key))
            {
                return deviceDict[key].Contains(device);
            }
            else
            {
                return false;
            }
        }
        public List<HidDevice> GetHidDevices(int vid, int pid)
        {
            string key = $"{vid}&{pid}";
            if (deviceDict.ContainsKey(key))
            {
                return deviceDict[key];
            }
            else
            {
                return new List<HidDevice> { };
            }
        }
        public Boolean AlreadyHasVPID(HidDevice device)
        {
            return deviceDict.ContainsKey($"{device.VendorID}&{device.ProductID}");
        }
        public void Remove(int vid, int pid)
        {
            string key = $"{vid}&{pid}";
            if (deviceDict.ContainsKey(key))
            {
                deviceDict.Remove(key);
                countDict.Remove(key);
            }
            else return;
        }
        public void Count(HidDevice device)
        {
            string key = $"{device.VendorID}&{device.ProductID}";
            if (countDict.ContainsKey(key))
            {
                countDict[key] += 1;
            }
        }
        public int GetCount(HidDevice device)
        {
            string key = $"{device.VendorID}&{device.ProductID}";
            if (countDict.ContainsKey(key))
            {
                return countDict[key];
            }
            return -1;
        }
        public void ClearAll()
        {
            deviceDict.Clear();
        }
    }
}
