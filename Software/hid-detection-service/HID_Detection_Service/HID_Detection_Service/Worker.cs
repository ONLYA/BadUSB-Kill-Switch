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
            //_logger.LogInformation("STARTED!!");
            ZMQConst.client_notify.SendFrame("1");
            var allowed_devices = DeviceList.Local.GetHidDevices().ToArray();
            _VPID.hid_count = allowed_devices.Length;
            handler = (_, dd) =>
            {
                var devices = DeviceList.Local.GetHidDevices().ToArray();
                foreach (HidDevice device in devices)
                {
                    if (allowed_devices.Contains(device))
                    {
                        //_logger.LogInformation("It contains");
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
        }

        public override void Dispose()
        {
            //DO YOUR STUFF HERE
        }
    }
}
