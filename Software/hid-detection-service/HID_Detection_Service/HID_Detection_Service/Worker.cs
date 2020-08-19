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

/// <summary>
///  TODO Create Thread for zmq messaging
/// </summary>

namespace WorkerService1
{
    static class ZMQ_Thread
    {
        public static void ThreadProc()
        {
            // zmq messaging
            using (var recv = new PullSocket())
            {
                recv.Bind("tcp://127.0.0.1:5487");
                while (true)
                {
                    // DO something
                }
            }
        }
        public static Thread t = new Thread(new ThreadStart(ThreadProc));
    }
    public class Worker : BackgroundService
    {
        static readonly IUsbEventWatcher usbEventWatcher = new UsbEventWatcher();

        private readonly ILogger<Worker> _logger;

        public Worker(ILogger<Worker> logger)
        {
            _logger = logger;
        }

        public override async Task StartAsync(CancellationToken cancellationToken)
        {
            // DO YOUR STUFF HERE
            ZMQ_Thread.t.Start();   // thread start
            var allowed_devices = DeviceList.Local.GetHidDevices().ToArray();
            usbEventWatcher.UsbDeviceAdded += (_, dd) =>
            {
                _logger.LogInformation("Added: " + dd);
                var devices = DeviceList.Local.GetHidDevices().ToArray();
                foreach (HidDevice device in devices)
                {
                    if (allowed_devices.Contains(device))
                    {
                        _logger.LogInformation("It contains");
                    }
                    else
                    {
                        _logger.LogWarning("It is not contained");
                        try
                        {
                            using (var client = new PushSocket())
                            {
                                client.Bind("tcp://127.0.0.1:5487");
                                client.SendFrame("1");
                            }
                        }
                        finally
                        {
                            NetMQConfig.Cleanup();
                        }
                    }
                }
            };
            await base.StartAsync(cancellationToken);
        }

        public override async Task StopAsync(CancellationToken cancellationToken)
        {
            // DO YOUR STUFF HERE
            await base.StopAsync(cancellationToken);
        }

        protected override async Task ExecuteAsync(CancellationToken stoppingToken)
        {
            while (!stoppingToken.IsCancellationRequested)
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
                await Task.Delay(1000, stoppingToken);
            }
        }

        public override void Dispose()
        {
            // DO YOUR STUFF HERE
            ZMQ_Thread.t.Join();    // Join the threads at the end.
        }
    }
}
