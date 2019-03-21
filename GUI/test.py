from tkinter import *  # 导入 Tkinter 库
from ctypes import *

libc = cdll.LoadLibrary('./mylib.so')

print(libc.add(1,3))
