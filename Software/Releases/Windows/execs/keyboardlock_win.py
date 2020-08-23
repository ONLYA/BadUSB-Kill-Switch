# import logging
from time import sleep
try:
    import pyHook
    import pythoncom
    import win32api
    import win32con
    import ctypes
except Exception as e:
    raise ModuleNotFoundError("Please install pyHook and pypiwin32 (that includes pywin32)")


class blockInput():
    def OnKeyboardEvent(self,event):
        return False

    def OnMouseEvent(self,event):
        return False

    def unblock(self, queue, timeout=0):
        # logging.info(" -- Unblock!")
        while not queue.empty():
            thread_id = queue.get()
        # if self.t.is_alive():
        #     self.t.cancel()
        sleep(timeout)
        try:
            self.hm.UnhookKeyboard()
        except:
            pass
        try:
            self.hm.UnhookMouse()
        except:
            pass
        try:
            win32api.PostThreadMessage(thread_id, win32con.WM_QUIT, 0, 0)
        except:
            pass

    def block(self, queue, keyboard=True, mouse=False):
        # logging.info(" -- Block!")
        thread_id = win32api.GetCurrentThreadId()
        queue.put(thread_id)
        # self.t = threading.Timer(timeout, self.unblock)
        # self.t.start()
        if mouse:
            self.hm.MouseAll = self.OnMouseEvent
            self.hm.HookMouse()
        if keyboard:
            self.hm.KeyAll = self.OnKeyboardEvent
            self.hm.HookKeyboard()
        pythoncom.PumpMessages()

    def __init__(self):
        self.hm = pyHook.HookManager()

# # Example Usage
# if __name__ == '__main__':
#     # logging.basicConfig(level=logging.INFO)
#
#     from queue import Queue
#     import threading
#
#     block = blockInput()
#     queue = Queue()
#     t1 = threading.Thread(target=block.block, args=(queue, ))
#     t2 = threading.Thread(target=block.unblock, args=(queue, 10, ))
#     t1.start()
#     t2.start()
#
#     # The following commands are not blocked by PumpMessages
#     import time
#     t0 = time.time()
#     while time.time() - t0 < 10:
#         time.sleep(1)
#         print(time.time() - t0)
#     # logging.info("Done.")
