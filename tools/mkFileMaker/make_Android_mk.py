#!/usr/bin/python
#-*- coding: utf-8 -*-

'''
Created on 2013-5-15

change all cpp in LOCAL_SRC_FILES
'''

import os


#定义 LOG信息到屏幕和txt
def loginfo(message):
    print(message)
    logfile.write('  ')
    logfile.write(str(message))
    logfile.write('\n')

#定义 os walk遍历所有文件
def walk_dir(indir, topdown=True):
    for root, dirs, files in os.walk(indir, topdown):
        #处理lim并拷贝,拷贝其他文件
        for name in files:
            if root.find('.svn') < 0:
                namesuff = os.path.splitext(name)

                if namesuff[1]==".cpp"  or namesuff[1]==".c" :
                    loginfo("name:  " + name)
                    loginfo("root:  " + root)

                    dirlen = len(indir)
                    outname = root[dirlen:] + "/" + name
                    outname = outname.replace('\\', '/')

                    loginfo("outname:  " + outname)

                    fout.write('                   ../../Classes')
                    fout.write(outname)
                    fout.write(' \\')
                    fout.write('\n')

#
def replace():
    dat0 = fin.readlines()

    for i in dat0:
        if i.find('$(CPP_FILES)') >= 0:
            os.chdir('..')
            #path
            trunk = os.getcwd()
            class_path = os.path.join(trunk, "projectHunter/Classes")
            walk_dir(class_path, True)

        else:
            fout.write(i)

#入口
if  __name__ =="__main__":

    logfile = open('log.txt', 'w')

    inFile='Android_sample.mk'
	
    outFile='../projectHunter/proj.android/jni/Android.mk'
    fin = open(inFile, 'r')
    fout = open(outFile, 'w')

    loginfo('--Start')

    replace()

    loginfo('--Success')