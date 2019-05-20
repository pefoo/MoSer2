set(SOURCES
)
# Maybe this could become a interface library. who knows...
ADD_LIBRARY(Constants STATIC ${SOURCES})
target_link_libraries(Constants SettingsProvider)
