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


ifndef KTS_SHARED
$(LIBNAME_A): $(OBJS) $(OBJSCPP) 
	@$(ARLOCAL) -cr  $(LIBNAME_A) $(OBJS) $(OBJSCPP)
	@ranlib $(LIBNAME_A)	
	@rm -f !  $(LIBLOCAL)/$(LIBNAME_A) 
	@cp -p $(LIBNAME_A) $(LIBLOCAL)


$(LIBNAME_SO): $(OBJS) $(OBJSCPP) 
	@echo hello world  > /dev/null
else
$(LIBNAME_SO): $(OBJS) $(OBJSCPP) $(INSTALLDIRS)
	@$(CCLOCAL) $(LIBFLAGS) -fPIC  -shared  -o $(LIBNAME_SO) $(OBJS) $(OBJSCPP) $(LIBS)
	@rm -f !  $(LIBLOCAL)/$(LIBNAME_SO) 
	@cp -p $(LIBNAME_SO) $(LIBLOCAL)

$(LIBNAME_A): $(OBJS) $(OBJSCPP) 
	@echo hello world  > /dev/null

endif



$(PROGRAM):: $(OBJS) $(OBJSCPP) $(SRCCPP) $(SRC)
	$(CCLOCAL) $(CPPFLAGS) -o  $(PROGRAM) $(OBJS) $(OBJSCPP) $(LIBS) 
	$(MAKE) install 

VPATH=../

../GVersion.cpp :
	$(call generate-version-info )



%.o: %.c  %.h 
	$(CC) $(CFLAGS) -c -o $(@F) $< $(INCLUDES)


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
