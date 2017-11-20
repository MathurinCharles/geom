* Needed:
Qt5
Eigen (included)


* Compilation:
qmake && make

* Launch:
./yourAppName




* Modify the app name:
Open main.pro, edit the value of TARGET
Open mainWindow.cpp,
     edit the value of _APPLICATION_NAME
     edit the value of _APPLICATION_EXTENSION (extension of saved/opened files)




* Modify the menu Help/about:
     edit YOUR APPLICATION NAME (VERSION...)
     edit your website (if any), your name and your email (for all students). 




* How to add a new 2D curve type (lets say "Curve2DWonderful")?
Copy curve2DLinear.h in curve2DWonderful.h
edit curve2DWonderful.h:
     replace linear by wonderful everywhere
     modify the "path" function to return your wonderful 2D curve
     Note: you can override any function you want from Curve2D if needed
edit main.pro:
     add curve2DWonderful.h in HEADERS
     Note: if you created an associated .cpp file, you have to add it to SOURCES as well
edit scene.cpp:
     add the line #include "curve2DWonderful.h" at the begining of the file
     add the line addCurveBuilder(new Curve2DWonderfulConstructor()); in the function initCurveBuilders()
recompile and run!




* How to add a new 1D curve type (lets say "Curve1DBlabla")?
Copy curve1DLinear.h in curve1DBlabla.h
edit curve1DBlabla.h:
     replace linear by blabla everywhere
     modify the "path" function to return your new 1D curve
     modify the "evalAt" function to return the evaluation of the curve at one particular position
     Note: you can override any function you want from Curve1D if needed
     Note: you can use "evalAt" inside "path" if you wish 
edit main.pro:
     add curve1DBlabla.h in HEADERS
     Note: if you created an associated .cpp file, you have to add it to SOURCES as well
edit scene.cpp:
     add the line #include "curve1DBlabla.h" at the begining of the file
     add the line initFunctionBuilders(new Curve1DBlablaConstructor()); in the function initFunctionBuilders()
recompile and run!


