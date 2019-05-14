set(SOURCES
)

ADD_LIBRARY(Constants STATIC ${SOURCES})
target_link_libraries(Constants SettingsProvider)
