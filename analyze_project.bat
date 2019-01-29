@echo off
cd source/build_utilities/cppclean-master
python -m cppclean.py ../../App.h ../../App.cpp ../../IService.h ../../Main.cpp ../../ServiceLocator.h ../../commands ../../components ../../events ../../game ../../input ../../gl ../../physics ../../rendering ../../resources ../../util
cd ..
cd ..
cd ..