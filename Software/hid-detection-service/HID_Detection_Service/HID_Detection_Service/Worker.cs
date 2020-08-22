using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;
using Microsoft.Extensions.Hosting;
using Microsoft.Extensions.Logging;
using HidSharp;
using Usb.Events;
using NetMQ;
using NetMQ.Sockets;
using System.ComponentModel.DataAnnotations;
using Microsoft.VisualBasic;
using System.Reflection;

namespace WorkerService1
{
    public class Worker : BackgroundService
    {
        static readonly IUsbEventWatcher usbEventWatcher = new UsbEventWatcher();

        static readonly VPIDStorage _VPID = new VPIDStorage(DeviceList.Local.GetHidDevices().Count());

        static readonly StorageDevice storageDevice = new StorageDevice();

        public EventHandler<UsbDevice> handler;

        private readonly ILogger<Worker> _logger;

        public Worker(ILogger<Worker> logger)
        {
            _logger = logger;
        }

        public override async Task StartAsync(CancellationToken cancellationToken)
        {
            // DO YOUR STUFF HERE
            _logger.LogInformation("STARTED!!");
            ZMQConst.client_notify.SendFrame("1");
            var allowed_devices = DeviceList.Local.GetHidDevices().ToArray();
            _VPID.hid_count = allowed_devices.Length;
            //_logger.LogInformation("Total " + allowed_devices.Length.ToString() + " HID devices   "+_VPID.VPID.Count.ToString());
            //foreach (HidDevice dev in allowed_devices)
            //{
            //    _logger.LogInformation("VID: " + dev.VendorID.ToString() + " PID: " + dev.ProductID.ToString());
            //}
            handler = (_, dd) =>
            {
                //_logger.LogInformation("Added: " + dd + "    " + allowed_devices.Length.ToString());
                var devices = DeviceList.Local.GetHidDevices().ToArray();
                foreach (HidDevice device in devices)
                {
                    if (allowed_devices.Contains(device))
                    {
                        _logger.LogInformation("It contains");
                    }
                    else
                    {
                        List<HidDevice> a = storageDevice.GetHidDevices(device.VendorID, device.ProductID);
                        List<HidDevice> b = DeviceList.Local.GetHidDevices(vendorID: device.VendorID, productID: device.ProductID).ToList();
                        if (storageDevice.AlreadyHasVPID(device))
                        {
                            if (a.All(item => b.Contains(item)) && b.All(item => a.Contains(item)) && storageDevice.GetCount(device) == b.Count)
                            {
                                ZMQConst.client_notify.SendFrame("0");
                                storageDevice.Remove(device.VendorID, device.ProductID);
                            }
                            if (!storageDevice.CheckExist(device))
                            {
                                storageDevice.Add(device);
                            }
                        }
                        else
                        {
                            ZMQConst.client_notify.SendFrame("0");
                            storageDevice.Add(device);
                        }
                        storageDevice.Count(device);
                        

                        //Dictionary<string, int> temp;
                        //if (_VPID.AddVPID(device.VendorID, device.ProductID, out temp))
                        //{
                        //    _logger.LogWarning("It is not contained {0} {1}", device.VendorID, device.ProductID);
                        //    ZMQConst.client_notify.SendFrame("0");
                        //    if (temp["total_count"] == -1)
                        //    {
                        //        temp["total_count"] = DeviceList.Local.GetHidDevices(productID: device.VendorID, vendorID: device.VendorID).Count();
                        //    }
                        //    break;
                        //}
                        //else if (_VPID.CheckContainVPID(device.VendorID, device.ProductID, out temp))
                        //{
                        //    if (temp["count"] == 0)
                        //    {
                        //        _logger.LogWarning("It is not contained {0} {1}", device.VendorID, device.ProductID);
                        //        ZMQConst.client_notify.SendFrame("0");
                        //        break;
                        //    }
                        //    else if (temp["count"] == temp["total_count"])
                        //    {
                        //        temp["count"] = 0;
                        //        break;
                        //    }
                        //    else
                        //    {
                        //        temp["count"] += 1;
                        //        break;
                        //    }
                        //}
                    }
                }
            };
            usbEventWatcher.UsbDeviceAdded += handler;
            await base.StartAsync(cancellationToken);
        }

        public override async Task StopAsync(CancellationToken cancellationToken)
        {
            // DO YOUR STUFF HERE
            _logger.LogInformation("STOP!!");
            usbEventWatcher.UsbDeviceAdded -= handler;
            _VPID.ClearAll();
            ZMQConst.client_notify.SendFrame("2");
            await base.StopAsync(cancellationToken);
        }

        protected override async Task ExecuteAsync(CancellationToken stoppingToken)
        {
            /*while (!stoppingToken.IsCancellationRequested)
            {
                try
                {
                    //_logger.LogInformation("======== START ========");
                    //var devices = DeviceList.Local.GetHidDevices().ToArray();
                    //foreach (HidDevice device in devices)
                    //{
                    //    _logger.LogInformation(device.DevicePath);
                    //}
                    //_logger.LogInformation("-------- Event Chaged Args --------");

                    //_logger.LogInformation(DeviceListChangedEventArgs.Empty.ToString());
                    //_logger.LogInformation("======== STOP ========");
                }
                catch (Exception e)
                {
                    _logger.LogError(e.InnerException.ToString());
                }
                //_logger.LogInformation("Worker running at: {time}", DateTimeOffset.Now);
                // await Task.Delay(1000, stoppingToken);
            }*/
        }

        public override void Dispose()
        {
            //DO YOUR STUFF HERE
        }
    }
}
