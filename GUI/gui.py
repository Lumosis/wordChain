from tkinter import *
from ctypes import *


window = Tk()
window.title('Word Chain')
window.geometry('500x600')
libc = cdll.LoadLibrary('./libtest.so')


error_msg = StringVar()
# Label
error_msg.set('No error')
error = Label(window,textvariable=error_msg,width=15,height=2,bg="white"\
                ,font=('Red',12))
error.place(x=350,y=25,anchor=NW)
def start():
    argv = './wc'
    error_flag = False;
    error_msg.set('No error')
    if v3.get() == 'w':
        argv = argv + ' -w'
    elif v3.get() == 'c':
        argv = argv + ' -c'
    elif v3.get() == 'n':
        argv = argv + ' -n '
        argv = argv + e3.get()
        if e3.get() == '':
            error_flag = True
            error_msg.set('Lack parameter')

    if var1.get() == 1:
        argv = argv + ' -h '
        argv = argv + e4.get()
        if e4.get() == '':
            error_flag = True
            error_msg.set('Lack parameter')

    if var2.get() == 1:
        argv = argv + ' -t '
        argv = argv + e5.get()
        if e5.get() == '':
            error_flag = True
            error_msg.set('Lack parameter')

    if v1.get() == 'A':
        argv = argv + ' ' + e1.get()
        try:
            open(e1.get(),'r')
        except:
            error_flag = True
            error_msg.set('in_file not exist')
    else:
        with open('./temp_in.txt','w') as f:
            if t1.get('1.0',END) == '':
                error_flag = True
                error_msg.set('Lack input')
            f.write(t1.get('1.0',END))
        argv = argv + ' temp_in.txt'

    if error_flag == False:
        print(len(argv.split(' ')),argv)
        char_p_arr = c_char_p * 100;
        argv = argv.split(' ')
        argv = [x.encode('utf-8') for x in argv]
        i=0
        arr = char_p_arr(*argv)
        libc.test(len(argv),pointer(arr))
    else:
        pass

    if v2.get() == 'A':
        filename = e2.get()
        with open('solution.txt','r') as s:
            with open(filename,'w') as f:
                f.write(s.read())
    elif v2.get() == 'B':
        with open('solution.txt','r') as s:
            t2.delete('1.0',END)
            t2.insert(END,s.read())



# Entry
e1 = Entry(window,show=None,width=35)
e1.place(x=90,y=10,anchor=NW)

e2 = Entry(window,show=None,width=35)
e2.place(x=90,y=320,anchor=NW)

e3 = Entry(window,show=None,width=3)
e3.place(x=440,y=150,anchor=NW)

e4 = Entry(window,show=None,width=3)
e4.place(x=440,y=220,anchor=NW)

e5 = Entry(window,show=None,width=3)
e5.place(x=440,y=245,anchor=NW)

# input选项
v1 = StringVar()
r1 = Radiobutton(window,text="in_file",variable=v1,\
                 value='A')
r2 = Radiobutton(window,text="in_text",variable=v1,\
                 value='B')
r1.place(x=10,y=10,anchor=NW)
r2.place(x=10,y=40,anchor=NW)

# output选项
v2 = StringVar()
r3 = Radiobutton(window,text="out_file",variable=v2,\
                 value='A')
r4 = Radiobutton(window,text="out_text",variable=v2,\
                 value='B')
r3.place(x=10,y=320,anchor=NW)
r4.place(x=10,y=350,anchor=NW)


# 参数选项
v3 = StringVar()
r5 = Radiobutton(window,text=" -w      ",variable=v3,\
                 value='w')
r6 = Radiobutton(window,text=" -c       ",variable=v3,\
                 value='c')
r7 = Radiobutton(window,text=" -n       ",variable=v3,\
                 value='n')
r5.place(x=360,y=100,anchor=NW)
r6.place(x=360,y=125,anchor=NW)
r7.place(x=360,y=150,anchor=NW)

var1 = IntVar()
var2 = IntVar()
r8 = Checkbutton(window,text=' -h      ',variable=var1,\
                 onvalue=1,offvalue=0)
r9 = Checkbutton(window,text=' -t      ',variable=var2,\
                 onvalue=1,offvalue=0)
r8.place(x=360,y=220,anchor=NW)
r9.place(x=360,y=245,anchor=NW)

# 开始按钮
b = Button(window,text='Start',width=15,height=10,command=start)
b.place(x=350,y=400,anchor=NW)

# 输入输出
t1 = Text(window,height=17,width=35)
t1.place(x=90,y=40,anchor=NW)
t2 = Text(window,height=17,width=35)
t2.place(x=90,y=350,anchor=NW)





window.mainloop()