########################################
## COMMON MAKEFILE
########################################

########################################
## MAKEFILE FOR UHCW
## AUTHOR: Per Thomas Hille pth@embc.no
## COPYRIGHT: Embedded Consulting A/S
########################################

##########################################################################
## This program is free software: you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program.  If not, see <https://www.gnu.org/licenses/>.
##########################################################################



VERBOSE=0
DEBUG = 0
HW = 1


OBJS=$(notdir $(subst .c,.o, $(SRC)))
OBJSCPP=$(notdir $(subst .cpp,.o, $(SRCCPP)))


-include $(subst .c,.d,$(SRC))
-include $(subst .cpp,.d,$(SRCCPP))

targets_local:=$(PROGRAM) $(LIBNAME_A) $(LIBNAME_SO) 

all: $(targets_local)


$(INSTALLDIRS):
	@if [ ! -b $@ ]; \
		then \
		mkdir -p $@; \
	fi



ifneq "$(MAKECMDGOALS)" "clean"
%.d: %.cpp %.h
	@$(CCLOCAL) -M  $(INCLUDES) $< > $(notdir $@.$$$$); \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g'  < $(notdir $@.$$$$) > $(notdir $@); \
	rm -f $(notdir $@.$$$$)

%.d: %.c %.h
	@$(CCLOCAL) -M  $(INCLUDES) $< > $(notdir $@.$$$$); \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $(notdir $@.$$$$) > $(notdir  $@); \
	rm -f $(notdir $@.$$$$)
endif


#$(LIBNAME_A): compileinfo_dir $(OBJS) $(OBJSCPP) $(SRCCPP) $(SRC) $(INSTALLDIRS) 
$(LIBNAME_A):  $(OBJS) $(OBJSCPP) $(SRCCPP) $(SRC) $(INSTALLDIRS) 
	@$(ARLOCAL) -cr  $(LIBNAME_A) $(OBJS) $(OBJSCPP)
	@ranlib $(LIBNAME_A)	
	@rm -f !  $(LIBLOCAL)/$(LIBNAME_A) 
	@cp -p $(LIBNAME_A) $(LIBLOCAL)


#$(LIBNAME_SO): compileinfo_dir  $(OBJS) $(OBJSCPP) $(INSTALLDIRS)
$(LIBNAME_SO):  $(OBJS) $(OBJSCPP) $(INSTALLDIRS)
	$(CCLOCAL) $(LIBFLAGS)   -o $(LIBNAME_SO) $(OBJS) $(OBJSCPP) 
	@rm -f !  $(LIBLOCAL)/$(LIBNAME_SO) 
	@cp -p $(LIBNAME_SO) $(LIBLOCAL)


#.PHONY: compileinfo_dir
.PHONY: 
compileinfo_dir:  
	@if [ ! -b ../../.compileinfo-$(TARGET) ]; \
		then \
		mkdir  -p ../../.compileinfo-$(TARGET) ; \
	fi




define generate-version-info
	@if [ "$(PROGRAM)" !=  "version-info" ]; then \
		$(VERSIONINFO_EXE) $(PROGRAM) -compileflags_file $(CURDIR)/../..//.compileinfo-$(TARGET)/$(PROGRAM)_flags.txt $(CURDIR); \
		mv GVersion.cpp  tmp.cpp; \
		old=../GVersion.cpp; \
		new=tmp.cpp; \
		if [ -f $${old} ]; then \
			chk1=`cksum $${old} | awk -F" " '{print $$1}'` ; \
			chk2=`cksum $${new} | awk -F" " '{print $$1}'` ; \
			echo "chk1=" $${chk1} > /dev/null; \
			echo "chk2=" $${chk1} > /dev/null; \
			if [ $${chk1} -eq $${chk2} ]; then \
	    			echo "Files are identical !!!!!, doing nothing" > /dev/null  ; \
			else \
				echo "Files are not identical !!!!!!" > /dev/null ; \
				mv $${new} $${old};  \
			fi; \
		else	\
			mv $${new} $${old}; \
		fi; \
		rm -f tmp.cpp; \
	fi;
endef



#$(PROGRAM)::  compileinfo_dir
#	$(call generate-version-info )


$(PROGRAM):: $(OBJS) $(OBJSCPP) $(SRCCPP) $(SRC)
	@echo $(LIBS) > $(CURDIR)/../..//.compileinfo-$(TARGET)/$(PROGRAM)_flags.txt
	@echo 	$(CCLOCAL) $(CPPFLAGS) -o  $(PROGRAM) $(OBJS) $(OBJSCPP) $(LIBS) >> $(CURDIR)/../..//.compileinfo-$(TARGET)/$(PROGRAM)_flags.txt 
	$(CCLOCAL) $(CPPFLAGS) -o  $(PROGRAM) $(OBJS) $(OBJSCPP) $(LIBS) 
	$(MAKE) install 

VPATH=../

../GVersion.cpp :
	$(call generate-version-info )



%.o: %.c  %.h 
	$(CCLOCAL) $(CFLAGS) -c -o $(@F) $< $(INCLUDES)


%.o:  	%.cpp  %.h
	$(CCLOCAL) $(CPPFLAGS) -c -o $(@F) $< $(INCLUDES) 

.PHONY: clean-exe
clean-exe:
ifdef PROGRAM
	-@$(RM) $(PROGRAM)
	-@$(RM) $(BUILDDIR)/$(TARGET)/bin/$(PROGRAM)
endif

.PHONY: clean
clean:
ifdef PROGRAM
	-@$(RM) $(BUILDDIR)/$(TARGET)/bin/$(PROGRAM)
endif
ifdef LIBNAME_A
	-@$(RM) $(BUILDDIR)/$(TARGET)/lib/$(LIBNAME_A)
endif
ifdef LIBNAME_SO
	-@$(RM) $(BUILDDIR)/$(TARGET)/lib/$(LIBNAME_SO)
endif
	-@$(RM) $(PROGRAM)
	-@$(RM) $(LIBNAME_A)
	-@$(RM) $(LIBNAME_SO)
	-@$(RM) *.o
	-@$(RM) *.d

install: $(INSTALLDIRS)
   ifdef PROGRAM
	@cp -f -p $(CURDIR)/$(PROGRAM) $(BINDIR)/$(PROGRAM);
   endif
