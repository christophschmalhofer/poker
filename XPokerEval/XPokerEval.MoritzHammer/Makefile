CPPC=gcc
include configure.local
CFLAGS=-g -pg -DNOASSERT
OSNAME=linux



# Source lists
LIBNAME=handeval
SOURCES=handevaluator.c randomgenerator.c carddeck.c handevaluatorjni.c
JAVA_SOURCES=./src
JNISTUBFILE=$(JAVA_SOURCES)/handevaluator/jni/JNIHandEvaluator.java

# Linux GNU C Compiler
ifeq ($(OSNAME), linux)
    # GNU Compiler options needed to build it
    COMMON_FLAGS=-fno-strict-aliasing -fPIC -fno-omit-frame-pointer -I.
    # Options that help find errors
    COMMON_FLAGS+= -W -Wall  -Wno-unused -Wno-parentheses
    CFLAGS=-g -O3 $(COMMON_FLAGS) 
    # Object files needed to create library
    OBJECTS=$(SOURCES:%.c=%.o)
    # Library name and options needed to build it
    LIBRARY=lib$(LIBNAME).so
    LDFLAGS=-Wl,-soname=$(LIBRARY) -static-libgcc -mimpure-text
    # Libraries we are dependent on
    LIBRARIES=-L $(JDK)/jre/lib/$(LIBARCH) -lc
    # Building a shared library
    LINK_SHARED=$(LINK.c) -shared -o $@
endif

# Windows Microsoft C/C++ Optimizing Compiler Version 12
ifeq ($(OSNAME), win32)
    CC=cl
    # Compiler options needed to build it
    COMMON_FLAGS=-Gy -DWIN32
    # Options that help find errors
    COMMON_FLAGS+=-W0 -WX
    ifeq ($(OPT), true)
        CFLAGS= -Ox -Op -Zi $(COMMON_FLAGS) 
    else
        CFLAGS= -Od -Zi $(COMMON_FLAGS) 
    endif
    # Sources need java_crw_demo
    # Object files needed to create library
    OBJECTS=$(SOURCES:%.c=%.obj)
    # Library name and options needed to build it
    LIBRARY=$(LIBNAME).dll
    LDFLAGS=
    # Libraries we are dependent on
    LIBRARIES=$(JDK)/
    # Building a shared library
    LINK_SHARED=link -dll -out:$@
endif

.phony: all clean 

all: $(LIBRARY) 

handevaluatorjni.o: handevaluatorjni.c handevaluator_jni_JNIHandEvaluator.h
	$(CPPC) $(CFLAGS) -c -o handevaluatorjni.o handevaluatorjni.c

handevaluator.o: handevaluator.c handevaluator.h
	$(CPPC) $(CFLAGS) -c -o handevaluator.o handevaluator.c

carddeck.o : carddeck.c carddeck.h
	$(CPPC) $(CFLAGS) -c -o carddeck.o carddeck.c

randomgenerator.o: randomgenerator.c randomgenerator.h
	$(CPPC) $(CFLAGS) -c -o randomgenerator.o randomgenerator.c

jnihead: $(JNISTUBFILE)
	$(JDK)/bin/javah -classpath ./bin handevaluator.jni.JNIHandEvaluator
	
$(LIBRARY): jnihead $(OBJECTS)
	$(LINK_SHARED) $(OBJECTS) $(LIBRARIES) SFMT.eo

clean:
	rm -f *.o $(LIBRARY) $(JNIHEADER) 
	
	