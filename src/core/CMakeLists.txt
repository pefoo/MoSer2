set(SOURCES
  appsetup.cpp
)

ADD_LIBRARY(Core STATIC ${SOURCES})
target_link_libraries(Core easyloggingpp utility stdc++fs constants)
