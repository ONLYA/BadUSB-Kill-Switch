from pystray import Icon as icon, Menu as menu, MenuItem as item
from PIL import Image
import tkinter as tk
from tkinter.messagebox import showinfo
import multiprocessing as m
import threading
import sys, platform, os
import subprocess
import zmq
from queue import Queue
from plyer import notification
import time

app_name = 'BadUSB Kill Switch'

if platform.system() == 'Windows':
    if subprocess.run("dotnet --info", shell=False, capture_output=False).returncode == 1:
        raise LookupError("Please install .NET 3.1 or later, or add that into PATH")
    try:
        from keyboardlock_win import blockInput
    except Exception as e:
        raise ImportError(e)
else:
    raise EnvironmentError("This app is for Windows only")

# TODO Check C# Worker Service Dependency
file = "HID_Detection_Service.exe"
if not os.path.exists(file):
    raise ProcessLookupError(file+" NOT found. A worker service process is needed. Exiting...")


def resource_path(relative_path):
    """ Get absolute path to resource, works for dev and for PyInstaller """
    try:
        # PyInstaller creates a temp folder and stores path in _MEIPASS
        base_path = sys._MEIPASS
    except Exception:
        base_path = os.path.abspath(".")

    return os.path.join(base_path, relative_path)


def p1(conn):   # Tkinter Process
    root = tk.Tk()
    root.wm_attributes("-topmost", 1)  # Always on top
    root.withdraw()  # Get rid of root window
    timeout = tk.IntVar(root, value=0, name="timeout")  # The global variable for block timeout
    context = zmq.Context.instance()
    block = blockInput()
    queue = Queue()

    window_height = 230
    window_width = 300
    screen_width = root.winfo_screenwidth()
    screen_height = root.winfo_screenheight()
    x_cordinate = int((screen_width / 2) - (window_width / 2))
    y_cordinate = int((screen_height / 2) - (window_height / 2))

    def emergency():
        top = tk.Toplevel()
        top.geometry("{}x{}+{}+{}".format(window_width, window_height, x_cordinate, y_cordinate))
        top.iconbitmap(resource_path("resources/icon1.ico"))
        top.wm_attributes("-topmost", 1)
        top.title("BadUSB Detected!")

        def cmd():
            threading.Thread(target=block.unblock, args=(queue, timeout.get(),)).start()
            showinfo(message="Now the keyboard is released.")
            top.destroy()
            subprocess.check_call(["taskkill.exe", "/im", file])
            subprocess.Popen("ping 127.0.0.1 -n 5 > nul && {}".format(file), shell=True)
            time.sleep(6)
            socket_send1 = context.socket(zmq.PUSH)
            socket_send1.connect("tcp://127.0.0.1:5488")
            socket_send1.send_string("true")
        msg = tk.Message(top, text="Unexpected HID Intrusion detected.\n\n"
                                   "All keyboard inputs are now BLOCKED.\n\n"
                                   "Re-enable it by clicking the button below!\n\n"
                                   "Please noted that unplug all USB devices before re-enabling keyboard.\n\n\n")
        btn = tk.Button(top, text="Unblock keyboard inputs", command=cmd)
        msg.pack()
        btn.pack()

    def t1():                # The thread to listen to the System tray process
        socket = context.socket(zmq.PUSH)
        socket.bind("inproc://service-transfer")
        while True:
            temp = conn.recv()
            if temp == 1:    # Quit Program
                subprocess.run(["taskkill.exe", "/im", file])
                root.destroy()
                sys.exit()
            elif temp == 3:  # Toggle Service on
                socket.send_pyobj(1)
            elif temp == 4:  # Toggle Service off
                socket.send_pyobj(2)

    def t2():                # The thread to receive zmq messaging from C# worker service ->5487
        socket_recv = context.socket(zmq.PULL)
        socket_recv.connect("tcp://127.0.0.1:5487")
        while True:
            temp = socket_recv.recv_string()
            if temp == "0":    # Acknowledge possible HID Intrusion
                threading.Thread(target=block.block, args=(queue,)).start()
                notification.notify(
                    title='BadUSB Detected',
                    message='Now ALL keyboard inputs are disabled',
                    app_name=app_name,
                    app_icon='resources/icon1.' + ('ico' if platform.system() == 'Windows' else 'png')
                )
                emergency()
            elif temp == "1":  # Acknowledge Background Service ON
                notification.notify(
                    title='Service Started',
                    message='Now all the HID devices other than currently connected ones will be blocked.',
                    app_name=app_name,
                    app_icon='resources/icon1.' + ('ico' if platform.system() == 'Windows' else 'png')
                )
            elif temp == "2":  # Acknowledge Background Service OFF
                notification.notify(
                    title='Service Stopped',
                    message='Now BadUSB Vulnerable.',
                    app_name=app_name,
                    app_icon='resources/icon1.' + ('ico' if platform.system() == 'Windows' else 'png')
                )

    def t3():                # The thread to send zmq messages to C# worker service -> 5488
        socket_send1 = context.socket(zmq.PUSH)
        socket_send1.connect("tcp://127.0.0.1:5488")
        socket_send2 = context.socket(zmq.PUSH)
        socket_send2.connect("tcp://127.0.0.1:5489")
        socket = context.socket(zmq.PULL)
        socket.connect("inproc://service-transfer")
        while True:
            temp = socket.recv_pyobj()
            if temp == 1:
                socket_send1.send_string("true")
            elif temp == 2:
                socket_send2.send_string("false")

    tt1 = threading.Thread(target=t1)
    tt2 = threading.Thread(target=t2)
    tt3 = threading.Thread(target=t3)

    tt1.setDaemon(True)
    tt2.setDaemon(True)
    tt3.setDaemon(True)

    tt1.start()  # Do NOT call join() method
    tt2.start()  # Do NOT call join() method
    tt3.start()  # Do NOT call join() method

    notification.notify(
        title='Start up',
        message='The program is shown in system tray. Please ensure that ALL required HID devices are connected',
        app_name=app_name,
        app_icon='resources/icon1.' + ('ico' if platform.system() == 'Windows' else 'png')
    )

    root.mainloop()


def p2(conn):
    im = Image.open("resources/icon1.ico")
    state = False

    def on_clicked(icon, item):
        nonlocal state
        state = not item.checked
        conn.send(3 if state else 4)

    def on_clicked1(icon, item):
        conn.send(1)
        icon.stop()

    # Update the state in `on_clicked` and return the new state in
    # a `checked` callable
    icon('icon_tray', im, "BadUSB Kill Switch", menu=menu(
        item(
            'Switch Service',
            on_clicked,
            checked=lambda item: state),
        item(
            'Quit Program',
            on_clicked1
        ))).run()


if __name__ == '__main__':
    pc1, cc1 = m.Pipe()
    pp1 = m.Process(target=p1, args=(cc1, ))
    pp2 = m.Process(target=p2, args=(pc1, ))

    pp1.start()
    pp2.start()

    pp1.join()
    pp2.join()
