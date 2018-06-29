Name = st
d = yes

Project = .
BuildTime = $(shell date +%s)

Cxx = g++
Cc = gcc
MyFlags =  -DLINUX 
MyFlags += -DBUILD_TIME=$(BuildTime)

ifeq (yes,$(d))
	CxxFlags = -std=c++11 -w -fmessage-length=0 -g
	Bin = $(Project)/bin/debug
	MyFlags += -DDEBUG
else
	CxxFlags = -std=c++11 -w -fmessage-length=0 -O3
	Bin = $(Project)/bin/release
endif

Cclags = $(CxxFlags)


#find . -iregex '.*\.\(c\|cc\|h\)'
SrcFolder = $(Project)
#SrcFolder += ../../common/proto/Common ../../common/proto/ScoreServer 


SrcFile = $(shell find $(SrcFolder) -iregex '.*\.\(c\|cpp\|cc\)')
HeadFile = $(shell find $(SrcFolder) -iregex '.*\.\(h\|hpp\)')
TMPDIR = $(dir $(HeadFile))
HeadFolder = $(sort $(TMPDIR))

#proto文件生成需要在外层目录
#[root@localhost proto]# protoc ./Common/*.proto --cpp_out=./
Includes = -I/usr/local/include/ -I../../3rdparty/protobuf-2.6.1/include \
-I"../../3rdparty/MySQL Connector C 6.0.2/include"

LibPath =  -L/usr/local/lib/ 
LibPara = -lrt -ldl -lpthread -l:libboost_system.a


Objects:=$(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(patsubst %.cc,%.o,$(SrcFile))))  

Depends = $(Objects:.o=.d)

all: print init $(Name)
	

$(Name):$(Objects)
	$(Cxx) $^ -o $(Bin)/$@ $(LibPath) $(LibPara) 

-include $(Depends)

%.o: %.c
	$(Cc) -c -o $@ $< $(Cclags) $(MyFlags) $(Includes) 
%.o: %.cpp
	$(Cxx) -c -o $@ $< $(CxxFlags) $(MyFlags) $(Includes)	
%.o: %.cc
	$(Cxx) -c -o $@ $< $(CxxFlags) $(MyFlags) $(Includes)

define gen_dep
rm -f $@;\
$(Cc) -MM $(Cclags) $< > $@.$$$$;\
sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@;\
rm -f $@.$$$$
endef  

%.d: %.c 
	$(gen_dep)  
%.d: %.cpp  
	$(gen_dep)  
  
init:
	-@mkdir -p $(Bin)
clean:
	@rm -f $(Objects) $(Depends)  
	@echo delete done.
print:
	@echo  SrcFile: $(SrcFile)
	@echo  Objects: $(Objects)
	@echo  Depends: $(Depends)


 .PHONY: all init print clean
