set(SOURCES
  base64.cpp
  gnuplot/gnuplotparameterdict.cpp
  gnuplot/gnuplotwrapper.cpp
)

ADD_LIBRARY(DataProcessorHelper STATIC ${SOURCES})
#target_link_libraries(DataProcessorHelper)
