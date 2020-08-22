using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Hosting;
using NetMQ;
using NetMQ.Sockets;
using System.Threading;
using System.Collections;

namespace WorkerService1
{
    public class Program
    {
        public static void Main(string[] args)
        {
            //string[] args_backup = args;
            using (var client = new PullSocket())
            {
                client.Bind("tcp://127.0.0.1:5488");
                while (true)
                {
                    string a = client.ReceiveFrameString();
                    if (bool.Parse(a))
                    {
                        Console.WriteLine("START");
                        CancellationTokenSource source = new CancellationTokenSource();
                        CancellationToken token = source.Token;
                        Thread t = new Thread(new ParameterizedThreadStart(ThreadProc));
                        t.Start(source);
                        CreateHostBuilder(args).Build().RunAsync(token);//.Wait();
                        Console.WriteLine("DONE Start");
                    }
                }
            }
            // NetMQConfig.Cleanup();
            // CreateHostBuilder(args).Build().Run();
        }

        public static IHostBuilder CreateHostBuilder(string[] args) =>
            Host.CreateDefaultBuilder(args)
                //.UseWindowsService()
                .ConfigureServices((hostContext, services) =>
                {
                    services.AddHostedService<Worker>();
                });
        public static void ThreadProc(object obj)
        {
            //string[] args = ToStringArray(obj);
            var source = obj as CancellationTokenSource;
            // zmq messaging
            using (var recv = new PullSocket())
            {
                recv.Bind("tcp://127.0.0.1:5489");
                while (true)
                {
                    string temp = recv.ReceiveFrameString();
                    if (!bool.Parse(temp))
                    {
                        Console.WriteLine("STOP");
                        source.Cancel();
                        source.Dispose();
                        //CreateHostBuilder(args).Build().StopAsync().Wait();
                        Console.WriteLine("DONE Stop");
                        break;
                    }
                }
            }
        }
        /// <summary>
        /// Cast object array to string array
        /// from: https://stackoverflow.com/a/10745579/13406850
        /// </summary>
        /// <param name="arg"></param>
        /// <returns>
        /// string[]</returns>
        static string[] ToStringArray(object arg)
        {
            var collection = arg as IEnumerable;
            if (collection != null)
            {
                return collection
                  .Cast<object>()
                  .Select(x => x.ToString())
                  .ToArray();
            }

            if (arg == null)
            {
                return new string[] { };
            }

            return new string[] { arg.ToString() };
        }
    }
    public static class ZMQConst
    {
        public static PushSocket client_notify = new PushSocket("@tcp://127.0.0.1:5487");  // Bind to Port 5487
    }
}
