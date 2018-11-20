set(SOURCES
  ./threading/callbacktimer.cpp
)

ADD_LIBRARY(Utility STATIC ${SOURCES})
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(Utility Threads::Threads)

