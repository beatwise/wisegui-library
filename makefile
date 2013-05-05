
COMPILE = cl.exe
LIB = lib.exe
    
CFLAGS = /O2 /Oi /GL /D "WIN32" /D "NDEBUG" /D "_LIB" /FD /EHsc /MT /Gy \
    /Fo"bin\\" /Fd"bin\vc90.pdb" /W3 /nologo /c /Zi /TP \
    /errorReport:prompt /D "_CRT_SECURE_NO_WARNINGS"  /D "_SECURE_SCL=0"
    
LIBFLAGS = /OUT:"bin\WiseGuiLibrary.lib" /NOLOGO /LTCG
    
SRCS = \
    bitmap_pool.h bitmap_pool.cpp \
    button.h button.cpp \
    checkbox.h checkbox.cpp \
    control.h control.cpp \
    controls.h \
    dropdownlist.h dropdownlist.cpp \
    knob.h knob.cpp \
    label.h label.cpp \
    listeners.h \
    paraminfo.h paraminfo.cpp \
    slider.h slider.cpp \
    statebutton.h statebutton.cpp \
    switch.h switch.cpp \
    view.h view.cpp \
    window.h window.cpp \
    wisegui.h wisegui.cpp \
    
OBJS = \
    bin\bitmap_pool.obj \
    bin\button.obj \
    bin\checkbox.obj \
    bin\control.obj \
    bin\dropdownlist.obj \
    bin\knob.obj \
    bin\label.obj \
    bin\paraminfo.obj \
    bin\slider.obj \
    bin\statebutton.obj \
    bin\switch.obj \
    bin\view.obj \
    bin\window.obj \
    bin\wisegui.obj \

wisegui: $(OBJS) $(SDK_OBJS) $(RES_OBJ)     
    $(LIB) $(LIBFLAGS) $(OBJS) /OUT:bin\WiseGuiLibrary.lib
    
$(OBJS): $(SRCS)
    $(COMPILE) $(CFLAGS) -c $(*B).cpp 

         